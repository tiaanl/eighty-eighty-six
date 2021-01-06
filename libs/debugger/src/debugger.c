#include "debugger/debugger.h"

#include "window_callbacks.h"

#include <GLFW/glfw3.h>
#include <base/platform.h>
#include <cpu/bus.h>
#include <cpu/cpu.h>
#include <glad.h>
#include <stdio.h>
#include <string.h>

#define MEMORY_WIDTH 0x100
#define MEMORY_HEIGHT 0x100

#define ASSERT_GL_NO_ERROR() assert(glGetError() == GL_NO_ERROR)

const char *vertex_shader_source = "#version 330 core\n"
                                   "layout (location = 0) in vec2 in_pos;\n"
                                   "layout (location = 1) in vec2 in_tex_coord;\n"
                                   "out vec2 tex_coord;\n"
                                   "void main() {\n"
                                   "  gl_Position = vec4(in_pos, 0.0, 1.0);\n"
                                   "  tex_coord = in_tex_coord;\n"
                                   "}\n";

const char *fragment_shader_source = "#version 330 core\n"
                                     "in vec2 tex_coord;\n"
                                     "out vec4 out_color;\n"
                                     "uniform sampler2D u_tex;\n"
                                     "void main() {\n"
                                     "  out_color = texture(u_tex, tex_coord);\n"
                                     "}\n";

u32 create_shader(const char *source, i32 type) {
  u32 shader = glCreateShader(type);
  ASSERT_GL_NO_ERROR();
  glShaderSource(shader, 1, &source, 0);
  ASSERT_GL_NO_ERROR();
  glCompileShader(shader);
  ASSERT_GL_NO_ERROR();

  return shader;
}

int debugger_init(struct debugger *debugger) {
  memset(debugger, 0, sizeof(*debugger));

  if (glfwInit() != GLFW_TRUE) {
    debugger_destroy(debugger);
    return -1;
  }

  debugger->window = glfwCreateWindow(MEMORY_WIDTH, MEMORY_HEIGHT, "Debugger - Memory", 0, 0);
  if (!debugger->window) {
    debugger_destroy(debugger);
    return -1;
  }

  glfwSetWindowUserPointer(debugger->window, debugger);
  glfwSetWindowSizeCallback(debugger->window, debugger_window_size_callback);

  glfwMakeContextCurrent(debugger->window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    debugger_destroy(debugger);
    return -1;
  }

  u32 vertex_shader = create_shader(vertex_shader_source, GL_VERTEX_SHADER);
  u32 fragment_shader = create_shader(fragment_shader_source, GL_FRAGMENT_SHADER);

  u32 program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  ASSERT_GL_NO_ERROR();
  glAttachShader(program, fragment_shader);
  ASSERT_GL_NO_ERROR();
  glLinkProgram(program);
  ASSERT_GL_NO_ERROR();

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  i32 success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    return -1;
  }

  debugger->program = program;

  // Create the texture we'll use to render the memory buffer.
  glGenTextures(GL_TEXTURE_2D, &debugger->texture);
  ASSERT_GL_NO_ERROR();
  glBindTexture(GL_TEXTURE_2D, debugger->texture);
  ASSERT_GL_NO_ERROR();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MEMORY_WIDTH, MEMORY_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
               0);
  ASSERT_GL_NO_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  ASSERT_GL_NO_ERROR();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  ASSERT_GL_NO_ERROR();

  glBindTexture(GL_TEXTURE_2D, 0);
  ASSERT_GL_NO_ERROR();

  static f32 vertices[] = {
      -1.0f, -1.0f, 0.0f, 1.0f, //
      1.0f,  -1.0f, 1.0f, 1.0f, //
      1.0f,  1.0f,  1.0f, 0.0f, //
      -1.0f, 1.0f,  0.0f, 0.0f, //
  };

  u32 vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  u32 vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), 0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(f32), (void *)(2 * sizeof(f32)));
  glEnableVertexAttribArray(1);

  debugger->vao = vao;

  return 0;
}

void debugger_destroy(struct debugger *debugger) {
  if (debugger->window) {
    glfwDestroyWindow(debugger->window);
  }

  glfwTerminate();
}

static void debugger_bus_listener_fetch(u32 addr, u8 value, void *context) {
  struct debugger *debugger = context;
  debugger->fetch_time[addr] = 255;
}

static void debugger_bus_listener_store(u32 addr, u8 value, void *context) {
  struct debugger *debugger = context;
  debugger->store_time[addr] = 255;
}

void debugger_map_bus_listener(struct debugger *debugger, struct bus *bus) {
  bus_add_listener(bus, debugger, debugger_bus_listener_fetch, debugger_bus_listener_store);
}

struct timespec timer_start() {
  struct timespec start_time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
  return start_time;
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(struct timespec start_time) {
  struct timespec end_time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
  long diffInNanos =
      (end_time.tv_sec - start_time.tv_sec) * (long)1e9 + (end_time.tv_nsec - start_time.tv_nsec);
  return diffInNanos;
}

int debugger_pre_step(struct debugger *debugger, struct cpu *cpu) {
  debugger->start_time = timer_start();

  glfwPollEvents();
  if (glfwWindowShouldClose(debugger->window)) {
    return 0;
  }

  memset(debugger->fetch_time, 0, sizeof(debugger->fetch_time));
  memset(debugger->store_time, 0, sizeof(debugger->store_time));

  return 1;
}

int debugger_post_step(struct debugger *debugger, struct cpu *cpu) {
  memset(debugger->image, 0, MEMORY_WIDTH * MEMORY_HEIGHT * sizeof(debugger->image[0]));
  debugger->image[0].red = 255;

  for (int i = 0; i < MEMORY_WIDTH * MEMORY_HEIGHT; ++i) {
    u32 flat = flatten_address(segment_offset(cpu->segs[SS], i));
    byte value = bus_fetch_byte(cpu->bus, flat);
    debugger->image[i].red = debugger->fetch_time[i];
    debugger->image[i].green = debugger->store_time[i];
    debugger->image[i].blue = value;
  }

  //  for (unsigned i = 0; i < MEMORY_WIDTH * MEMORY_HEIGHT; ++i) {
  //    if (debugger->fetch_time[i] > 1) {
  //      debugger->fetch_time[i] -= 1;
  //    }
  //  }

  //  for (unsigned i = 0; i < MEMORY_WIDTH * MEMORY_HEIGHT; ++i) {
  //    if (debugger->store_time[i] > 1) {
  //      debugger->store_time[i] -= 1;
  //    }
  //  }

  glBindTexture(GL_TEXTURE_2D, debugger->texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MEMORY_WIDTH, MEMORY_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE,
               debugger->image);

  glUseProgram(debugger->program);
  glBindVertexArray(debugger->vao);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

  long elapsed = timer_end(debugger->start_time);
  double diff = (double)elapsed / 1000.0; // milliseconds
  printf("elapsed: %lf\n", 1000000.0 / diff);

  glfwSwapBuffers(debugger->window);

  return 1;
}

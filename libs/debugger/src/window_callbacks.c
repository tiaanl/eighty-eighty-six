#include <GLFW/glfw3.h>
#include <base/platform.h>
#include <glad.h>

void debugger_window_size_callback(GLFWwindow *window, int width, int height) {
  UNUSED(window);

  glViewport(0, 0, width, height);
}

#include <base/address.h>
#include <cpu/bus.h>
#include <cpu/cpu.h>
#include <cpu/ports.h>
#include <malloc.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// 1Mb of memory
#define DEFAULT_MEMORY_SIZE 0x100000

int kbhit(void) {
  static bool initflag = false;
  static const int STDIN = 0;

  if (!initflag) {
    // Use termios to turn off line buffering
    struct termios term;
    tcgetattr(STDIN, &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(STDIN, TCSANOW, &term);
    setbuf(stdin, NULL);
    initflag = true;
  }

  int nbbytes;
  ioctl(STDIN, FIONREAD, &nbbytes); // 0 is STDIN
  return nbbytes;
}

int main() {
  static struct address reset_vector = {
      .segment = 0xf000,
      .offset = 0xfff0,
  };

  byte *memory = calloc(DEFAULT_MEMORY_SIZE, 1);

  struct bus bus;
  bus_init(&bus, memory, DEFAULT_MEMORY_SIZE);

  struct ports *ports = malloc(sizeof(struct ports));
  ports_init(ports);

  struct cpu cpu;
  cpu_init(&cpu, ports, &bus, reset_vector);

  // const char *bios_file = "/home/tilo/Code/life-16/life.com";
  const char *bios_file = "/home/tilo/Code/Faux86/data/pcxtbios.bin";

  // Load the BIOS at the very end of memory.
  FILE *handle = fopen(bios_file, "rb");
  fseek(handle, 0, SEEK_END);
  long file_size = ftell(handle);
  fseek(handle, 0, SEEK_SET);
  fread(memory + DEFAULT_MEMORY_SIZE - file_size, file_size, 1, handle);
  fclose(handle);

  bool running = true;
  while (running) {
    while (!kbhit()) {
      usleep(100);
    }
    int c = fgetc(stdin);
    switch (c) {
      case 'q':
        running = false;
        break;

      case 's':
        cpu_step(&cpu);
        break;

      default:
        break;
    }
  }

  free(memory);
  free(ports);

  return 0;
}

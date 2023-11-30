#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  bool show_pids = false;
  bool numeric_sort = false;
  // for (int i = 0; i < argc; i++) {
  //   assert(argv[i]);
  //   printf("argv[%d] = %s\n", i, argv[i]);
  // }
  // assert(!argv[argc]);
  int opt;
  while ((opt = getopt(argc, argv, "pnV")) != -1) {
    switch (opt) {
      case 'p':
        show_pids = true;
        break;
      case 'n':
        numeric_sort = true;
        break;
      case 'V':
        puts("pstree Version 0.1.0");
        break;
      default:
        assert(0 && "Unknown option");
        break;
    }
  }

  return 0;
}

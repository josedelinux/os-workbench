/*
我们也把问题分解一下：
得到命令行的参数，根据要求设置标志变量的数值；
得到系统中所有进程的编号 (每个进程都会有唯一的编号) 保存到列表里；
对列表里的每个编号，得到它的的父亲是谁；
在内存中把树建好，按命令行参数要求排序；
把树打印到终端上。
*/
#include <assert.h>
#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

bool show_pids = false;
bool numeric_sort = false;

const char *proc_fs_dirpath = "/proc/";

struct option long_options[] = {{"show-pids", no_argument, 0, 'p'},
                                {"numeric-sort", no_argument, 0, 'n'},
                                {"version", no_argument, 0, 'V'},
                                {0, 0, 0, 0}};

// walk every /proc/{pid} directory to discover all process
void for_dir_in_proc(const char *dirPath) {
  DIR *dir;
  struct dirent *entry;
  struct stat fileStat;

  dir = opendir(dirPath);
  if (dir == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  while ((entry = readdir(dir)) != NULL) {
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/%s", dirPath, entry->d_name);

    if (stat(filePath, &fileStat) < 0) {
      perror("stat");
      continue;
    }

    if (S_ISDIR(fileStat.st_mode)) {
      // Ignore "." and ".." directories
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;

      // Recursively traverse subdirectory
      traverseDirectory(filePath);
    } else {
      // Process file
      printf("File: %s\n", filePath);
    }
  }

  closedir(dir);
}
// read parent id(4th parameter of /proc/pid/stat)
void read_ppid() {}

// build ps tree
void build_tree() {}

void print_tree() {}

int main(int argc, char *argv[]) {
  int option_index = 0;
  int option;
  while ((option = getopt_long(argc, argv, "pnV", long_options,
                               &option_index)) != -1) {
    switch (option) {
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
        fprintf(stderr, "Unknown option: %c\n", optopt);
        exit(EXIT_FAILURE);  // Defined in header <stdlib.h>
        break;
    }
  }

  // 打印处理后的参数
  printf("show_pids: %d\n", show_pids);
  printf("numeric_sort: %d\n", numeric_sort);

  for_dir_in_proc(proc_fs_dirpath);

  return 0;
}

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
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
  pid_t process_id;
  pid_t parent_pid;
  char name[256];
} Process;

bool show_pids = false;
bool numeric_sort = false;

const char *proc_fs_dirpath = "/proc";

struct option long_options[] = {{"show-pids", no_argument, 0, 'p'},
                                {"numeric-sort", no_argument, 0, 'n'},
                                {"version", no_argument, 0, 'V'},
                                {0, 0, 0, 0}};

// get ppid and name by parsing the file
int get_process_info(pid_t pid) {
  // Construct the path to the /proc/[pid]/stat file
  char stat_path[256];
  snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);

  // Open the /proc/[pid]/stat file for reading
  FILE *file = fopen(stat_path, "r");
  if (file == NULL) {
    perror("fopen /proc/[pid]/stat");
    return -1;  // Error opening the file
  }

  // Read the contents of the /proc/[pid]/stat file
  long ppid = -1;  // Default value if not found
  // the asterisk * is used for suppression
  int ret;
  char name[56];
  if ((ret = fscanf(file, "%*d %s %*c %ld", &name, &ppid)) != 1) {
    printf("ret:%d", ret);
    perror("fscanf");
    fclose(file);
    return -1;  // Error reading the file
  }

  // Close the file
  fclose(file);

  return ppid;
}

// walk every /proc/[pid] directory to discover all process
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
    // Calculate the size needed for the formatted string
    size_t size = snprintf(NULL, 0, "%s/%s", dirPath, entry->d_name);

    // Allocate buffer dynamically
    char *filePath = malloc(size + 1);  // +1 for null terminator

    if (filePath == NULL) {
      perror("malloc");
      exit(EXIT_FAILURE);
    }

    // Use snprintf to safely format the string
    snprintf(filePath, size + 1, "%s/%s", dirPath, entry->d_name);

    if (stat(filePath, &fileStat) < 0) {
      perror("stat");
      free(filePath);  // Don't forget to free the memory
      continue;
    }

    if (S_ISDIR(fileStat.st_mode)) {
      // Ignore "." and ".." directories
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        continue;
      }

      printf("Dir: %s\n", entry->d_name);

      // If dir's name is a pid
      char *endptr = NULL;  // it shouble be empty after call
      int val = strtol(entry->d_name, &endptr, 10);
      if (strcmp(endptr, "")) {
        printf("skip non [pid] directory:\tenptr: %s\n", endptr);
        continue;  // it's not what we are looking for
      }

      printf("raw: %s\t parsed: %d\n", entry->d_name, val);

      int ppid = get_process_info(val);

      printf("ppid: %d\n", ppid);

      // if (is_)
    } else {
      // Process file
      // printf("File: %s\n", filePath);
    }

    free(filePath);  // Don't forget to free the memory
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

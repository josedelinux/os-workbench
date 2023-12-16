#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void traverseDirectory(const char *dirPath) {
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
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        continue;

      // Recursively traverse subdirectory
      traverseDirectory(filePath);
    } else {
      // Process file
      printf("File: %s\n", filePath);
    }
    free(filePath);  // Don't forget to free the memory
  }

  closedir(dir);
}

int main() {
  const char *dirPath = ".";
  traverseDirectory(dirPath);

  return 0;
}

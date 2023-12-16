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

int main() {
  const char *dirPath = ".";
  traverseDirectory(dirPath);

  return 0;
}

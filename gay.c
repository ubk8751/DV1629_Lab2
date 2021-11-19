#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
  if(argc != 4) {
    printf("You suck, pls use 3 args\n");
    exit(1);
  }
  int page_count = atoi(argv[1]);
  int page_size = atoi(argv[2]);
  char* file_name = argv[3];
  printf("No of physical pages = %d, page size = %d\nReading memory trace from %s\nYou suck at programming\n", page_count, page_size, file_name);

  // hippedy hoppedy this code is now our property
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(file_name, "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);

  int read_lines = 0;
  int page_faults = 0;

  int queue_size = sizeof(int) * page_count * page_size;
  int queue_pos = 0;
  int* queue = malloc(queue_size);
  int read_number;
  while ((read = getline(&line, &len, fp)) != -1) {
      read_lines++;
      read_number = atoi(line);

      printf("its debug time %d %d\n", read_lines, queue_pos);
      int found = 0;
      for(int i = 0; i < queue_size; i++){
        if(queue[i] == read_number){
          found = 1;
          break;
        }
      }
      if(found == 0){
        queue[queue_pos] = read_number;
        page_faults++;

        queue_pos = (queue_pos + 4) % queue_size;
      }
  }
  fclose(fp);
  if (line)
      free(line);

  printf("Read %d memory references => %d pagefaults\n", read_lines, page_faults);

  return 0;
}

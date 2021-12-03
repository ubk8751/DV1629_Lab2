#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
  if(argc != 4) {
    printf("U suc, pls use 3 args\n");
    exit(1);
  }
  int page_count = atoi(argv[1]);
  int page_size = atoi(argv[2]);
  char* file_name = argv[3];
  printf("No of physical pages = %d, page size = %d\nReading memory trace from %s\n", page_count, page_size, file_name);

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  int read_lines = 0;
  int page_faults = 0;

  int table[page_count];

  for(int i =0; i < page_count; i++){
    table[i] = -1;
  }

  fp = fopen(file_name, "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);

  long read_number;
  int final_addr;
  int temp;
  while ((read = getline(&line, &len, fp)) != -1) {
      read_lines++;
      read_number = atoi(line);

      // Calculate the address of the page in memory
      temp = (int)(read_number/page_size);
      final_addr = temp*page_size;

      // Look if page is in table
      int found = 0;
      for (int i = 0; i < page_count; i++) {
        if (table[(i)] == final_addr) {
          found = 1;
        }
      }
      // If page is in table, remove the first element and insert the new one
      // This is equivalent to simply replacing the first one with the new one and incrementing the index
      // Here we use page_faults as the index itself
      if(found == 0){
        page_faults++;
        table[(page_faults % page_count)] = final_addr;
      }
  }
  fclose(fp);
  if (line)
      free(line);

  printf("Read %d memory references => %d pagefaults\n\n", read_lines, page_faults);

  return 0;
}

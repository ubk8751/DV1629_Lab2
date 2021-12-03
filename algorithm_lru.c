#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct lru_data {
    int last_used;
    int addr;
} lru_data;

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

  // Allocate data for the table
  lru_data *table;
  table = malloc(page_count * sizeof(lru_data));
  for(int i =0; i < page_count; i++){
    table[i].addr = -1;
    table[i].last_used = -1;
  }

  fp = fopen(file_name, "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);

  long read_number;
  int final_addr, temp;
  while ((read = getline(&line, &len, fp)) != -1) {
    read_lines++;
    read_number = atoi(line);

    temp = (int)(read_number/page_size);
    final_addr = temp*page_size;

    // Look for the page in the table
    int found = 0;
    for (int i = 0; i < page_count; i++) {
      if (table[i].addr == final_addr) {
        table[i].last_used = read_lines;
        found = 1;
        break;
      }
    }
    // If the page is in the table
    // Find the page which has been used least recently and replace it
    if(found == 0){
      int value = INT_MAX, final_idx = -1;
      lru_data* data;

      // Loop through all pages and find the one which was least recently used
      for(int idx = 0; idx < page_count; idx++){
        data = table + idx;
        if(data->last_used < value){
          value = data->last_used;
          final_idx = idx;
      }
    }

    // Increment page faults and replace page
    page_faults++;
    table[final_idx].addr = final_addr;
    table[final_idx].last_used = read_lines;
    }
  }
  fclose(fp);
  if (line)
      free(line);

  printf("Read %d memory references => %d pagefaults\n\n", read_lines, page_faults);

  return 0;
}

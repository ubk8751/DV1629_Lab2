#include <limits.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
  int front, rear, size;
  long* queue;
  unsigned capacity;
} Queue;

Queue* create_queue(unsigned capacity){
  Queue *queue = (Queue*)malloc(sizeof(Queue));

  queue->capacity = capacity;
  queue->front = 0;
  queue->size = 0;
  queue->rear = capacity - 1;
  queue->queue = (long*)malloc(capacity * sizeof(long));
  return queue;
}

void enqueue(Queue* queue, long value){
  if(queue->size < queue->capacity){
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->queue[queue->rear] = value;
    queue->size = queue->size + 1;
  }else {
    printf("Could not instert value %lu because the queue is full\n", value);
  }
}

void dequeue(Queue* queue){
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
}

int isInQueue(Queue* queue, long value){
  int found_value = 0;
  int i = queue->front;
  for(int j = 0; j < queue->size; j++) {
    if(queue->queue[(i + j) % queue->capacity] == value){
      found_value = 1;
      break;
    }
  }
  return found_value;
}

int main(int argc, char** argv)
{
  if(argc != 4) {
    printf("You suck, pls use 3 args\n");
    exit(1);
  }
  int page_count = atoi(argv[1]);
  int page_size = atoi(argv[2]);
  char* file_name = argv[3];
  printf("No of physical pages = %d, page size = %d\nReading memory trace from %s\n", page_count, page_size, file_name);

  // hippedy hoppedy this code is now our property
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  int read_lines = 0;
  int page_faults = 0;

  unsigned int queue_size = page_count;
  Queue* queue = create_queue(queue_size);

  fp = fopen(file_name, "r");
  if (fp == NULL)
      exit(EXIT_FAILURE);

  int bit_count_in_page_size = 0;
  int psize = page_size;
  while(psize > 1) { bit_count_in_page_size++; psize /= 2;}

  int bit_count_in_page_table = 0;
  int pt_size = page_count;
  while(pt_size > 1) { bit_count_in_page_table++; pt_size /= 2;}

  int bit_count = bit_count_in_page_size + bit_count_in_page_table;
  int mask = 1;
  for(int i = 0; i <= bit_count; i++)
    mask *= 2;
  mask -= 1;

  long read_number;
  while ((read = getline(&line, &len, fp)) != -1) {
      read_lines++;
      read_number = atoi(line); 


      read_number = (read_number >> (page_size / (8 * page_count))) & 0b1111111111111111111;

      int found = isInQueue(queue, read_number);

      if(found == 0){
        if(queue->size < queue->capacity){
          page_faults++;
          enqueue(queue, read_number);
        } else {
          page_faults++;
          dequeue(queue);
          enqueue(queue, read_number);
        }
      }
  }

  free(queue);
  fclose(fp);
  if (line)
      free(line);

  printf("Read %d memory references => %d pagefaults\n", read_lines, page_faults);

  return 0;
}

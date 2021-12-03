// Check https://www.thecrazyprogrammer.com/2016/11/optimal-page-replacement-algorithm-c.html
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_TRACE_LENGTH 100000

typedef struct {
    int next_use;
    int addr;
} optimal_data;

int main(int argc, char** argv)
{
    if(argc != 4) {
        printf("U suc, pls use 3 args\n");
        exit(1);
    }
    const int page_count = atoi(argv[1]);
    const int page_size = atoi(argv[2]);
    char* file_name = argv[3];
    printf("No of physical pages = %d, page size = %d\nReading memory trace from %s\n", page_count, page_size, file_name);

    // hippedy hoppedy this code is now our property
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file_name, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    long read_number;
    int final_addr;

    long mem_addrs[MEMORY_TRACE_LENGTH];
    int read_lines, i = 0;
    
    // Read in all addresses from file into one big array in memory
    while ((read = getline(&line, &len, fp)) != -1) {
        read_lines++;
        read_number = atoi(line);
        final_addr = ((int)(read_number/page_size)) * page_size;
        mem_addrs[i] = final_addr;

        i++;
    }

    fclose(fp);
    if (line)
        free(line);

    optimal_data* table = malloc(sizeof(optimal_data) * page_count);
    for(i = 0; i < page_count; i++){
        table[i].addr = -1;
        table[i].next_use = 0;
    }

    // For each page
    int page_faults = 0, size = 0;
    for(int i = 0; i < MEMORY_TRACE_LENGTH; i++) {
        final_addr = mem_addrs[i];

        // Search if page is loaded in table
        int found = 0;
        for (int j = 0; j < page_count; j++) {
            if (table[j].addr == final_addr) {
                found = 1; break;
            }
        }

        // If page is not found, this is a page fault
        if(found == 0){
            page_faults++;

            // If page table is not full, add page to table
            if(size < page_count){
                table[size].addr = final_addr;
                size++;
            } // Else replace page with Optimal Page Replacement Algorithm
            else {
                int latest_use = 0, latest_use_index = -1;

                // Loop through all loaded pages
                for(int k = 0; k < page_count; k++){
                    table[k].next_use = -1; // Reset the data to -1

                    // Loop through all addresses in the future
                    for(int j = i+1; j < MEMORY_TRACE_LENGTH; j++){

                        // If address in page is same as address in future, store the time stamp
                        if(table[k].addr == mem_addrs[j]){
                            table[k].next_use = j; break;
                        }
                    }

                    // If page is not to be used, replace it
                    if(table[k].next_use == -1){
                        latest_use_index = k; break;
                    } 
                    // Else find the page which's next use is the latest of all loaded pages
                    else {
                        if(latest_use < table[k].next_use){
                            latest_use = table[k].next_use;
                            latest_use_index = k;
                        }
                    }
                }
                table[latest_use_index].addr = final_addr;
            }
        }
    }

    printf("Read %d memory references => %d pagefaults\n\n", read_lines, page_faults);

    return 0;
}

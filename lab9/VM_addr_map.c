#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAXSTR 1000

int main(int argc, char *argv[])
{
  char line[MAXSTR];
  int *page_table, *mem_map;
  unsigned int log_size, phy_size, page_size, d;
  unsigned int num_pages, num_frames;
  unsigned int offset, logical_addr, physical_addr, page_num, frame_num;

  /* Get the memory characteristics from the input file */
  fgets(line, MAXSTR, stdin);
  if((sscanf(line, "Logical address space size: %d^%d", &d, &log_size)) != 2){
    fprintf(stderr, "Unexpected line 1. Abort.\n");
    exit(-1);
  }
  fgets(line, MAXSTR, stdin);
  if((sscanf(line, "Physical address space size: %d^%d", &d, &phy_size)) != 2){
    fprintf(stderr, "Unexpected line 2. Abort.\n");
    exit(-1);
  }
  fgets(line, MAXSTR, stdin);
  if((sscanf(line, "Page size: %d^%d", &d, &page_size)) != 2){
    fprintf(stderr, "Unexpected line 3. Abort.\n");
    exit(-1);
  }

  //Number of pages and frames
  num_pages = pow(2, log_size) / pow(2, page_size);
  num_frames = pow(2, phy_size) / pow(2, page_size);

  /* Allocate arrays to hold the page table and memory frames map */
  page_table = (int *)malloc(num_pages * sizeof(int));
  mem_map = (int *)malloc(num_frames * sizeof(int));

  /* Initialize page table to indicate that no pages are currently mapped to
     physical memory */
  for (int i = 0; i < num_pages; i++){
    page_table[i] = -1;
  }
  

  /* Initialize memory map table to indicate no valid frames */
  for (int i = 0; i < num_frames; i++){
    mem_map[i] = -1;
  }

  /* Read each accessed address from input file. Map the logical address to
     corresponding physical address */
  fgets(line, MAXSTR, stdin);
  while(!(feof(stdin))){
    sscanf(line, "0x%x", &logical_addr);
    fprintf(stdout, "Logical address: 0x%x\n", logical_addr);
    
    /* Calculate page number and offset from the logical address */
    page_num = logical_addr >> page_size;
    offset = logical_addr & ((1 << page_size) - 1);
      

    /* Form corresponding physical address */
    if (page_table[page_num] == -1){
      fprintf(stdout, "Page Fault!\n");
    } else {
      frame_num = page_table[page_num];
      physical_addr = (frame_num <<  page_size) | offset;
      fprintf(stdout, "Physical address: 0x&x\n", physical_addr);
    }
    

    /* Read next line */
    fgets(line, MAXSTR, stdin);    
  }

  free(page_table);
  free(mem_map);

  return 0;
}

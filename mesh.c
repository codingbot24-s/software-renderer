//
//
// Created by saad on 8/10/26.
//

#include "mesh.h"
#include "vec.h"
#include <stdio.h>
#include <stdlib.h>

int is_line_ending(const char* buffer, int idx, int end_idx) {
  if (buffer[idx] == '\0') return 1;
  if (buffer[idx] == '\n') return 1; 
  if (buffer[idx] == '\r')
  {
    if ((idx + 1 < end_idx) && (buffer[idx + 1]  != '\n'))
    {
      return 1;
    }
    
  }
  
  return 0;
}

int get_line_info(const char* buffer, size_t bufflen, size_t* num_lines) {
  
  int num_of_line = 0;
  for (int i = 0; i < bufflen; i++)
  {
    if (is_line_ending(buffer,i,bufflen))
    {
      // line ended increase the line count
      num_lines++;
    }
     
  }
  
}
// NOTE: always check before using this function that 
// this is not returning null else we will get the crash
mesh* load_mesh(const char* path) {
  FILE* f = fopen(path,"r");
  if (f == NULL) {
    fprintf(stderr,"cant open the file \n");
    return NULL;
  }
  
  if(fseek(f,0,SEEK_END) != 0) {
    fprintf(stderr, "seekin error \n");
    return NULL;
  }
  ssize_t fsize = ftell(f);
  rewind(f);
  
  char* buffer = malloc(fsize + 1);
  if (buffer == NULL)
  {
    fprintf(stderr,"Cant allocate the buffer for reading \n");
    return NULL;
  }

  size_t bytes_read = fread(buffer,1,fsize,f);
  if (bytes_read < fsize)
  {
    fprintf(stderr,"cant read the file \n");
    free(buffer);
    return NULL;
  }
  
  buffer[bytes_read] = '\0';
  printf("readed %s\n",bytes_read);

  // we have readed the file in buffer now we need to extract the 
  // the vertices

}

void delete_mesh() {}


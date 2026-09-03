//
//
// Created by saad on 8/10/26.
//

#include "vec.h"
#include <SDL3/SDL_pixels.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_line_ending(const char *buffer, int idx, int end_idx) {
  if (buffer[idx] == '\0')
    return 1;
  if (buffer[idx] == '\n')
    return 1;
  if (buffer[idx] == '\r') {
    if ((idx + 1 < end_idx) && (buffer[idx + 1] != '\n')) {
      return 1;
    }
  }

  return 0;
}

void get_line_count(const char *buffer, size_t bufflen, size_t *num_lines) {

  for (int i = 0; i < bufflen; i++) {
    if (is_line_ending(buffer, i, bufflen)) {
      (*num_lines)++;
    }
  }
}


int is_space(const char chr) {
	if (chr == ' ' || chr == '\t') {
		return 1;
    } else {
		return 0;
	}
}


int parse_line(char *buffer, int diff) {

    char line_buff[255];
	char* token;
	memcpy(line_buff, buffer, diff);
	token = line_buff;

    if (token[0] == '\0') {
        return 0;
    }
    if (token[0] == '#') {
        return 0;
    }
   if (token[0] == 'v' && is_space(token[1])) {
        int vert_count = 0;
        float x, y, z;
        if (sscanf(token, "v %f %f %f", &x, &y, &z) == 3) {
			vec3 vertex = (vec3){x,y,z};
			append(vertex, vector);
		}
    }
}

// NOTE: always check before using this function that
// this is not returning null else we will get the crash
mesh *load_mesh(const char *path) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    fprintf(stderr, "cant open the file \n");
    return NULL;
  }

  if (fseek(f, 0, SEEK_END) != 0) {
    fprintf(stderr, "seeking error \n");
    return NULL;
  }
  ssize_t fsize = ftell(f);
  rewind(f);

  char *buffer = malloc(fsize + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Cant allocate the buffer for reading \n");
    return NULL;
  }

  size_t bytes_read = fread(buffer, 1, fsize, f);
  if (bytes_read < fsize) {
    fprintf(stderr, "cant read the file \n");
    free(buffer);
    return NULL;
  }

  buffer[bytes_read] = '\0';

  size_t num_lines = 0;
  get_line_count(buffer, strlen(buffer), &num_lines);

  for (int i = 0; i < strlen(buffer); ++i) {
    char *eol = strchr(buffer, '\n');
    if (eol == NULL) {
      break;
    }
    parse_line(buffer, eol - buffer);

  }
}

void delete_mesh() {}

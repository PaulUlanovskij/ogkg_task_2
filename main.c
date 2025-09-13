#include <stdio.h>
#include <string.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#define CANVAS_WIDTH 960
#define CANVAS_HEIGHT 540

int read_entire_file(const char *path, char* read_buffer)
{
  FILE *f = fopen(path, "rb");
  size_t new_count = 0;
  long long m = 0;
  if (f == NULL ||              
    fseek(f, 0, SEEK_END) < 0){
    fclose(f);
    return -1;
  }
  m = _ftelli64(f);
  if (m < 0 ||
    fseek(f, 0, SEEK_SET) < 0) {
    fclose(f);
    return -1;
  }

  if (read_buffer == NULL){
    fclose(f);
    return m;
  }

  fread(read_buffer, m, 1, f);
  if (ferror(f)) {
    fclose(f);
    return -1;
  }

  return m;
}

int main(){
  stbi_flip_vertically_on_write(1);
  int buf_len = read_entire_file("DS3.txt", NULL);
  if (buf_len == -1){
    printf("Failed to read data set\n");
    return -1;
  }
  char* file_buffer = calloc(buf_len + 1, 1);
  buf_len = read_entire_file("DS3.txt", file_buffer);
  if (buf_len == -1){
    printf("Failed to read data set\n");
    return -1;
  }

  char* image = calloc(CANVAS_WIDTH*CANVAS_HEIGHT*3, 1);
   

  int x = 0;
  int y = 0;
  char write_x = 1;

  for(int i = 0; i < buf_len; i++){
    char* pos1 = strchr(file_buffer+i, ' ');
    char* pos2 = strchr(file_buffer+i, '\n');
    char* pos = 0;
    if(pos1 != NULL && pos2 != NULL){
      pos = pos1 < pos2 ? pos1 : pos2;
    }else if(pos1){
      pos = pos1;
    }else{
      pos = pos2;
    }
    if(pos){
      *pos = '\0';
    }
    
    if (write_x){
      x = atoi(file_buffer + i);
      write_x = 0;
    }else{
      y = atoi(file_buffer + i);
      int offset = (y*CANVAS_WIDTH+x)*3;
      image[offset] = (char)255;
      image[offset+1] = (char)255;
      image[offset+2] = (char)255;
      write_x = 1;
    }
    i = pos - file_buffer;
  }
  if(stbi_write_bmp("plot.bmp", CANVAS_WIDTH, CANVAS_HEIGHT, 3, image)==0){
    printf("Failed to write image\n");
  }else{
    printf("Success!\n");
  }
}

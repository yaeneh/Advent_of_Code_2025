#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int count(int* top, int* middle, int* low,char* buf, int n){
  int result = 0;
  if (middle == NULL) return 0;

  if(top == 0 && middle != 0 && low != 0){
    //do topmost line

    for(int i = 0; i <n; i++){
      int neighbours = 0;
      if(i == 0){
        neighbours = middle[i+1] + low[i] + low[i+1];
      }else if(i == n-1){
        neighbours = middle[i-1] + low[i] + low[i-1];
      }else{
        neighbours += middle[i-1] + middle[i+1];
        neighbours += low[i-1] + low[i] + low[i+1];
      }

      if(neighbours < 4 && middle[i]){
        result += 1;
        buf[i] = ' ';
      }else if(middle[i]){
        buf[i]= '@';
      }else{
        buf[i] = ' ';
      }

    }

    printf("once topmost\n");

  }else if(low == 0 && top != 0 && middle != 0){
    //do lowest line

    for(int i = 0; i <n; i++){
      int neighbours = 0;
      if(i == 0){
        neighbours = middle[i+1] + top[i] + top[i+1] ;
      }else if(i == n-1){
        neighbours = middle[i-1] + top[i] + top[i-1] ;
      }else{
        neighbours += top[i-1] + top[i] + top[i+1];
        neighbours += middle[i-1] + middle[i+1];
      }

      if(neighbours < 4 && middle[i]){
        result += 1;
        buf[i] = ' ';
      }else if(middle[i]){
        buf[i]= '@';
      }else{
        buf[i] = ' ';
      }
    }

    printf("once lowest\n");
  }else{
    //regular
    for(int i = 0; i <n; i++){
      int neighbours = 0;
      if(i == 0){
        neighbours = middle[i+1] + top[i] + top[i+1] + low[i] + low[i+1];
      }else if(i == n-1){
        neighbours = middle[i-1] + top[i] + top[i-1] + low[i] + low[i-1];
      }else{
        neighbours += top[i-1] + top[i] + top[i+1];
        neighbours += middle[i-1] + middle[i+1];
        neighbours += low[i-1] + low[i] + low[i+1];
      }

      if(neighbours < 4 && middle[i]){
        result += 1;
        buf[i] = ' ';
      }else if(middle[i]){
        buf[i]= '@';
      }else{
        buf[i] = ' ';
      }

    }

  }

  return result;
}

int main(int argc, char *argv[])
{
  FILE *fptr = fopen("input", "r");
  FILE *readfile = fopen("imediate", "w+");
  FILE *writefile = fopen("imediate2", "w+");
  if(!fptr){ printf("failed to open input file\n"); return 1; }
  if(!readfile || !writefile){ printf("failed to open intermediate files\n"); return 1; }

  char buf[1024];
  long total = 0;
  long removed_this_pass = 0;
  int column = 0;
  int counter = 0;

  do {
    removed_this_pass = 0;

    FILE *current_read = (counter == 0) ? fptr : (counter % 2 == 1 ? writefile : readfile);
    FILE *current_write = (counter % 2 == 1) ? readfile : writefile;

    rewind(current_read);
    rewind(current_write);

    int *top = NULL;
    int *middle = NULL;
    int *low = NULL;
    char *bufwback = calloc(1024, sizeof(char));
    char *prevbuf = NULL;   // holds the written line for the previous middle row

    while(fgets(buf, 1024, current_read)){
      int *newrow = malloc(1024 * sizeof(int));
      column = 0;
      for(int i = 0; i < 1024; i++){
        if(buf[i] == '\n' || buf[i] == '\0'){ column = i; break; }
        newrow[i] = (buf[i] == '@') ? 1 : 0;
      }

      free(top);
      top = middle;
      middle = low;
      low = newrow;

      if(middle != NULL){
        removed_this_pass += count(top, middle, low, bufwback, column);

        // now write the previous middle row (bufwback is its result)
        if(prevbuf != NULL){
          fprintf(current_write, "%s\n", prevbuf);
          free(prevbuf);
        }
        prevbuf = malloc(1024 * sizeof(char));
        memcpy(prevbuf, bufwback, 1024);
        free(bufwback);
        bufwback = calloc(1024, sizeof(char));
      }
    }

    // handle the last row
    removed_this_pass += count(middle, low, NULL, bufwback, column);
    if(prevbuf != NULL){
      fprintf(current_write, "%s\n", prevbuf);
      free(prevbuf);
    }
    fprintf(current_write, "%s\n", bufwback);

    free(top);
    free(middle);
    free(low);
    free(bufwback);

    fflush(current_write);
    total += removed_this_pass;
    counter++;

  } while(removed_this_pass > 0);

  fclose(fptr);
  fclose(readfile);
  fclose(writefile);

  printf("The result is %ld\n", total);
  return 0;
}

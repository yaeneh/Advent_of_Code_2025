#include <stdio.h>
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
        buf[i] = 0;
      }else {
        buf[i]=middle[i];
      }

    }

    printf("once topmost");

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
        buf[i] = 0;
      }else {
        buf[i]=middle[i];
      }
    }

    printf("once lowest");
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
        buf[i] = 0;
      }else {
        buf[i]=middle[i];
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

  if(!fptr){
    printf("failed to open input file\n");
    return 1;
  }

  if(!readfile || !writefile){
    printf("failed to open intermediate files\n");
    return 1;
  }

  char buf[1024];
  long result = 0;
  long oldresult = 0;
  int row = 0;
  int column = 0;

  int counter = 0;

  do{
    oldresult = result;

    // choose input file
    FILE *current_read;
    FILE *current_write;

    if(counter % 2 == 0){
      current_read = readfile;
      current_write = writefile;
    }else{
      current_read = writefile;
      current_write = readfile;
    }

    rewind(current_read);

    int *top = 0;
    int *middle = 0;
    int *low = 0;
    char *bufwback = calloc(1024,  sizeof(char));

    while(fgets(buf, 1024, fptr)){

      int *new = malloc(1024 * sizeof(int));
      for(int i = 0; i < 1024; i++){
        if(buf[i] == '\n'){
          column = i;
          break;
        }

        new[i] = (buf[i] == '@') ? 1 : 0;
      }

      row++;

      // your transformation (unchanged)
      for(int i = 0; i < column && top; i++){
        if(bufwback[i] == 1){
          buf[i] = '@';
        }else{
          buf[i] = ' ';
        }
      }

      buf[column] = '\0';

      fprintf(current_write, "%s\n", buf);

      free(top);
      top = middle;
      middle = low;
      low = new;

      if(middle != 0){
        result += count(top, middle, low, bufwback, column);
      }
    }

    result += count(middle, low, 0, bufwback, column);

    for(int i = 0; i < column && top; i++){
      if(bufwback[i] == 1){
        buf[i] = '@';
      }else{
        buf[i] = ' ';
      }
    }
    free(top);
    free(middle);
    free(low);
    free(bufwback);

    fflush(current_write);

    counter++;

  }while(oldresult != result);

  fclose(fptr);
  fclose(readfile);
  fclose(writefile);

  printf("rows: %d, columns: %d\n", row, column);
  printf("The result is %ld\n", result);

  return 0;
}


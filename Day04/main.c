#include <stdio.h>
#include <stdlib.h>

int count(int* top, int* middle, int* low, int n){
  int result = 0;

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
        middle[i] = 0;
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
        middle[i] = 0;
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

      if(neighbours < 4&& middle[i]){
        result += 1;
        middle[i] = 0;
      }

    }

  }

  return result;
}

int main(int argc, char *argv[])
{
  FILE *fptr = fopen("input", "r");
  FILE *imediateptr = fopen("imediate", "w");

  if(!fptr){
    printf("failed to open file\n");
    return 1;
  }

  if(!imediateptr){
    printf("failed to open file\n");
    return 1;
  }

  char buf[1024];
  long result = 0;
  long oldresult = 0;
  int row = 0;
  int column = 0;
  
  do{
    oldresult = result;
    int *top = 0;
    int *middle = 0;
    int *low = 0;
    
    if(result > 0){
      fptr = imediateptr;
    }

    while(fgets(buf, 1024, fptr)){

      int *new = malloc(1024 * sizeof(int));

      for(long i = 0; i < 1024;i++){
        if (buf[i]== '\n'){
          column = i;
          break;
        }else{
          if(buf[i]== '@'){
            new[i] = 1;
          }else{
            new[i] = 0;
          }
        }
      }
      row++; 

      for(int i = 0; i< column; i++){
        if(top[i] == 1){
          buf[i] = '@';
        }else {
          buf[i] = ' ';
        }
      }
      buf[column] = '\0';
      fprintf(imediateptr, "%s\n", buf);
      free(top);
      top = middle;
      middle = low;
      low = new;

      //printf("top: %p\nmiddle: %p\nlow: %p\n", top, middle, low);

      if(middle != 0){
        result += count(top, middle, low, column);
      }
    }

    result += count(middle, low, 0, column);

    for(int i = 0; i< column; i++){
      if(middle[i] == 1){
        buf[i] = '@';
      }else {
        buf[i] = ' ';
      }
      if(low[i] == 1){
        buf[i] = '@';
      }else {
        buf[i] = ' ';
      }
    }
    free(top);
    free(middle);
    free(low);

  }while(oldresult != result);

  printf("rows: %d, columns: %d\n", row, column);

  printf("The result is %ld\n", result);
  return 0;
}



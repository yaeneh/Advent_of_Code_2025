#include <stdio.h>
#include <stdlib.h>

const int bufsize = 5012;

int main(int argc, char *argv[])
{
  FILE *fptr = fopen("input", "r");

  if(!fptr){
    printf("failed to open file\n");
    return 1;
  }

  char buf[bufsize];
  int row = 0;
  int column = 0;

  if(fgets(buf,bufsize,fptr)){row++;}
  while(buf[column] != '\n' && buf[column] != '\0'){column++;}

  while(fgets(buf, bufsize, fptr)){
    row++;
  }

  printf("rows: %d columns: %d\n", row, column);

  long (*manifold)[column] = calloc(row, sizeof(*manifold));
  char input[row][column];

  fptr = fopen("input", "r");

  if(!fptr){
    printf("failed to open file\n");
    return 1;
  }
  
  int rowiter = 0; 
  while(fgets(buf,bufsize,fptr)){
    int columniter = 0;
    while(buf[columniter] != '\n' && buf[columniter] != '\0'){
      input[rowiter][columniter] = buf[columniter];
      columniter++;
    }
    rowiter++;
  }

  for(int j = 0; j < column; j++){
    if(input[0][j] == 'S'){
      manifold[0][j] = 1;
    }
  } 

  int splitevent = 0;

  for(int i = 1; i < row; i++){
    for(int j = 0; j < column; j++){

      if(input[i][j] == '.'){
        manifold[i][j] += manifold[i-1][j];
      }else if( input[i][j] == '^'){

        if(manifold[i-1][j] > 0 ){
          splitevent++;
          if(j> 0){
            manifold[i][j-1] += manifold[i-1][j];
          }
          if(j < column){
            manifold[i][j+1] += manifold[i-1][j];
          }
          manifold[i][j] = 0;
        }
      }else {
        printf("parsing error %s\n", input[i][j]);
      }
    }
  }
  for(int i = 0; i < row; i++){
    for(int j = 0; j < column; j++){
      printf("%d ", manifold[i][j]);
    }
    printf("\n");
  } 

  printf("The tachyons have been split %d times\n", splitevent);

  long sum = 0;
  for(int j = 0; j < column; j++){
    sum += manifold[row-1][j];
  }

  printf("The tachyons have %ld different ways\n", sum);
  return 0;
}


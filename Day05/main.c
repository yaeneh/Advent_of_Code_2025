#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  FILE *fptr = fopen("input", "r");

  if(!fptr){
    printf("failed to open file\n");
    return 1;
  }

  char buf[1024];
  long result = 0;
  long resultlength = 12;

  long numbers[1024];
  long n = 0;

  while(fgets(buf, 1024, fptr)){
    

    for(long i = 0; i < 1024;i++){
      if (buf[i]== '\n'){
        n = i;
        break;
      } else {
        numbers[i] = buf[i] - '0';
      }
    }
    result += find_joltag(n, numbers, resultlength);
    
  }

  printf("The result is %ld\n", result);
  return 0;
}





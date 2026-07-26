#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

long inputbuf[2][1024];

int main(int argc, char *argv[])
{
  FILE *fptr = fopen("input", "r");
  if(!fptr){
    printf("failed to open file\n");
    return 1;
  }
  static char buf[1000000];

  long solution = 0;

  for(int i = 0; i< 1024; i++){
    inputbuf[0][i] = 0;
    inputbuf[1][i] = 1;
  }
  long linenum = 0;
  while(fgets(buf, 100000, fptr)){
    linenum++;
    long currentvalue = 0;
    long currentcolumn = 0;
    long numcount = 0;              // <-- new
    bool lastseenempty = false;
    for(long i = 0; i < 100000 && buf[i] != '\n' && buf[i] != '\0'; i++){
      if(buf[i] >= '0' && buf[i] <= '9'){
        if(lastseenempty == false){
          currentvalue = currentvalue*10 + (buf[i]-'0');
        } else {
          inputbuf[0][currentcolumn] += currentvalue;
          inputbuf[1][currentcolumn] *= currentvalue;
          currentcolumn++;
          numcount++;          // <-- count each finalized number
          currentvalue = buf[i]-'0';
          lastseenempty = false;
        }
      } else if(buf[i] == '+' || buf[i] == '*'){
        if(buf[i] == '+') solution += inputbuf[0][currentcolumn];
        else               solution += inputbuf[1][currentcolumn];
        currentcolumn++;
        numcount++;
      } else {
        if(currentcolumn == 0 && currentvalue == 0) continue;
        lastseenempty = true;
      }
    }
    // finalize last token on the line (the dead-code fix from before)
    if (currentvalue != 0 || !lastseenempty) {
      inputbuf[0][currentcolumn] += currentvalue;
      inputbuf[1][currentcolumn] *= currentvalue;
      numcount++;
    }
  }
  printf("the result is %ld\n", solution);

  fptr = fopen("input", "r");
  if(!fptr){
    printf("failed to open file\n");
    return 1;
  }

  long numberbuf[100000];
  for (int i = 0; i < 100000; i++){
    numberbuf[i]=0;
  }
  solution = 0;

  while(fgets(buf, 100000, fptr)){

    int i = 0;
    char* lastseenop = 0;
    while(buf[i] != '\n' && buf[i] != '\0'){
      long partsolution= 0;
      if(buf[i] >= '0' && buf[i] <= '9'){

        numberbuf[i] = (numberbuf[i] * 10) + (buf[i]- '0');
      }else if(buf[i] == '+' || buf[i] == '*'){

        if(lastseenop != 0){
          int start = lastseenop - &buf[0];
          long operator = 1;
          if(*lastseenop == '+'){operator = 0;}
          long partsolution = 1;
          if(operator ==0){partsolution = 0;}

          for(int j = start; j < i-1;j++){
            if(operator == 0){
              partsolution = partsolution + numberbuf[j];
            }else{
              partsolution = partsolution * numberbuf[j];
            }
          }

          solution += partsolution;
          lastseenop = &buf[i];

        }else {lastseenop = &buf[i];} 
      }

      i++;
    }

    if(lastseenop != NULL){
      int start = lastseenop - &buf[0];
      long operator = 1;
      if(*lastseenop == '+'){operator = 0;}
      long partsolution = 1;
      if(operator ==0){partsolution = 0;}

      for(int j = start; j < i;j++){
        if(operator == 0){
          partsolution = partsolution + numberbuf[j];
        }else{
          partsolution = partsolution * numberbuf[j];
        }
      }

      solution += partsolution;
    }
  }

  printf("the solution for the second part is %ld", solution);



  return 0;
}

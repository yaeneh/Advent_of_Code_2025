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
    printf("line %ld: numcount=%ld finalcolumn=%ld\n", linenum, numcount, currentcolumn);
}
  printf("the result is %ld\n", solution);
  return 0;
}

#include <stdio.h>
#include <stdlib.h>

struct resultvector {
  long resultvalue;
  long* startnextsegment;
  long* endnextsegment;
};

struct resultvector *findmax(long* start, long* end){ 

  long value = 0;
  long *resultp = start;

  while(start <= end){

    if(*start > value){
      value = *start;
      resultp = start;
    }

    start += 1;

    printf("%d start \n%d end\n" , start, end);
  }

  struct resultvector *result = malloc(sizeof(struct resultvector));
  result -> resultvalue = value;
  result -> startnextsegment = resultp + 1; 
  result -> endnextsegment = end + 1; 
  
  return result;

}

long find_joltag(long n, long nums[1024]){
  
  long amount = 12;
  long *start = nums;
  long *end = nums + (n -amount);
  long sum = 0;

  for(long i = 0; i < amount; i++){
    struct resultvector *res = findmax(start, end);
    start = res -> startnextsegment;
    end = res -> endnextsegment;
    sum *= 10;
    sum += res -> resultvalue;

    free(res);
  }

  return sum;
}

int main(int argc, char *argv[])
{
  FILE *fptr = fopen("input", "r");

  if(!fptr){
    printf("failed to open file\n");
    return 1;
  }

  char buf[1024];
  long result = 0;

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
    result += find_joltag(n, numbers);
    
  }

  printf("The result is %ld\n", result);
  return 0;
}





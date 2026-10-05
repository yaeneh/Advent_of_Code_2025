#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int bufsize = 5012;

struct Points{
  long i;
  long j;
};

struct Node{
  struct Points point;
  struct Node *next;
};

long calculatesize(struct Points *p1, struct Points *p2){

  long di = labs(p1 ->i - p2 -> i) +1;
  long dj = labs(p1 ->j - p2 -> j)+ 1;

  return di*dj;

}

long min(long a, long b){
  return a > b ? b : a;
}

long max(long a, long b){
  return a > b ? a : b;
}

bool checkmid(long i, long j1, struct Node *LinkedList){
  struct Node *p1 = LinkedList;
  struct Node *p2 = p1 -> next;

  long counter = 0;
  long j2 = LONG_MAX;

  long p1i, p1j, p2i, p2j;

  while(p2 != NULL){

    p1i = min(p1 -> point.i, p2 -> point.i);
    p1j = p1 -> point.j;
    p2i = max(p1 -> point.i, p2 -> point.i);
    p2j = p2 -> point.j;

    if(p1j == p2j && p1j > j1 && p1j < j2 && p1i< i && p2i >i){
      counter++;
    }

    p1 = p2;
    p2 = p1-> next;
  }
  p2 = LinkedList;

  p1i = min(p1 -> point.i, p2 -> point.i);
  p1j = p1 -> point.j;
  p2i = max(p1 -> point.i, p2 -> point.i);
  p2j = p2 -> point.j;

  if(p1j == p2j && p1j > j1 && p1j < j2 && p1i< i && p2i >i){
    counter++;
  }
  return (counter%2 == 1);
}

bool checkvertical(long j1, long j2, long i, struct Node *LinkedList){

  struct Node *p1 = LinkedList;
  struct Node *p2 = p1 -> next;
  long p1i, p1j, p2i, p2j;

  while(p2 != NULL){

    p1i = min(p1 -> point.i, p2 -> point.i);
    p1j = p1 -> point.j;
    p2i = max(p1 -> point.i, p2 -> point.i);
    p2j = p2 -> point.j;

    if(p1j == p2j && p1j > j1 && p1j < j2 && p1i< i && p2i >i){
      return true;
    }


    p1 = p2;
    p2 = p1-> next;
  }
  p2 = LinkedList;

  p1i = min(p1 -> point.i, p2 -> point.i);
  p1j = p1 -> point.j;
  p2i = max(p1 -> point.i, p2 -> point.i);
  p2j = p2 -> point.j;

  if(p1j == p2j && p1j > j1 && p1j < j2 && p1i< i && p2i >i){
    return true;
  }
  return false;
}

bool checkhorizontal(long i1, long i2, long j,  struct Node *LinkedList){
  struct Node *p1 = LinkedList;
  struct Node *p2 = p1 -> next;
  long p1i, p1j, p2i, p2j;

  while(p2 != NULL){

    p1i = p1 ->point.i;
    p1j = min(p1 -> point.j, p2 -> point.j);
    p2i = p2 ->point.i;
    p2j = max(p1 -> point.j, p2 -> point.j);

    if(p1i == p2i && p1i < i1 && p1i < i2 && p1j< j && p2j >j){
      return true;
    }


    p1 = p2;
    p2 = p1-> next;
  }
  p2 = LinkedList;

  p1i = p1 ->point.i;
  p1j = min(p1 -> point.j, p2 -> point.j);
  p2i = p2 ->point.i;
  p2j = max(p1 -> point.j, p2 -> point.j);

  if(p1i == p2i && p1i < i1 && p1i < i2 && p1j< j && p2j >j){
    return true;
  }

  return false;
}

bool rectangleisvalid(struct Points *start, struct Points *end, struct Node *LinkedList){

  long topi = min(start->i, end->i);
  long boti = max(start ->i, end->i);
  long leftj = min(start -> j, end ->j);
  long rightj = max(start -> j, end -> j);

  return checkmid((topi + boti)/2, (leftj+rightj)/2, LinkedList) && !checkvertical(leftj, rightj, topi, LinkedList) && !checkvertical(leftj, rightj, boti, LinkedList) &&
  !checkhorizontal(topi, boti, rightj, LinkedList) && !checkhorizontal(topi, boti, leftj, LinkedList);

}

long calculaterectanglep2(struct Node *start, struct Node *end){
  long biggestrectangle = 0;
  struct Node *Linkedliststart = start;

  while(start != NULL){

    for(struct Node *iterator = start; iterator != NULL; iterator = iterator -> next ){

      long currentrectsize = calculatesize(&start -> point, &iterator -> point);
      if(currentrectsize > biggestrectangle && rectangleisvalid(&start -> point, &iterator -> point, Linkedliststart)){biggestrectangle = currentrectsize;}

    }
    start = start -> next;
  }
  
  return biggestrectangle;
}

long calculaterectangle(struct Node *start, struct Node *end){
  long biggestrectangle = 0;

  while(start != NULL){

    for(struct Node *iterator = start; iterator != NULL; iterator = iterator -> next ){

      long currentrectsize = calculatesize(&start -> point, &iterator -> point);
      if(currentrectsize > biggestrectangle){biggestrectangle = currentrectsize;}

    }
    start = start -> next;
  }
  
  return biggestrectangle;
}

int main(int argc, char *argv[])
{

  FILE *fptr = fopen("input", "r");
  if(!fptr){
    printf("file not opend");
    return EXIT_FAILURE;
  }

  char buf[bufsize];
  struct Node *Linkedliststart = NULL;
  struct Node *Linkedlistend = Linkedliststart;
  
  

  while(fgets(buf, bufsize, fptr)){
    
    long sum = 0;
    long pointi;
    long pointj;
    for(int i = 0; i< bufsize; i++){
      
      if(buf[i] >= '0' && buf[i] <= '9'){
        sum = sum*10 + (buf[i] - '0');
      }else if(buf[i] == ','){
        pointi = sum;
        sum = 0;
        continue;
      }else if(buf[i] == '\n' || buf[i] == '\0'){
        if(Linkedliststart == NULL){
          pointj = sum;
          struct Node *temp = calloc(1, sizeof(struct Node));
          temp -> point.i = pointi;
          temp -> point.j = pointj;
          Linkedliststart = temp;
          Linkedlistend = temp;
          break;
        } else{
          pointj = sum;
          struct Node *temp = calloc(1, sizeof(struct Node));
          temp -> point.i = pointi;
          temp -> point.j = pointj;
          Linkedlistend -> next = temp;
          Linkedlistend = Linkedlistend -> next;
          break;
        }
      }

    }
  }

  for(struct Node *start = Linkedliststart; start != NULL; start = start -> next){
    printf("%ld,%ld\n", start-> point.i, start-> point.j);
  }

  long resultp1 = calculaterectangle(Linkedliststart, Linkedlistend);
  printf("The result of p1 is: %ld\n", resultp1);

  long resultp2 = calculaterectanglep2(Linkedliststart, Linkedlistend);

  printf("The result of p2 is: %ld\n", resultp2);

  return EXIT_SUCCESS;
}

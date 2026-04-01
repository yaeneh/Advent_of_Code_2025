#include <stdio.h>
#include <stdlib.h>

struct listelement {
  long start;
  long end;
  struct listelement *next;
};

long listsize(struct listelement *linkedlist){
  long counter = 0;
  while(linkedlist != 0){
    counter++;
    linkedlist = linkedlist -> next;
  }

  return counter;
}


struct listelement *coaleslinkedlist(struct listelement *linkedlist){

  struct listelement *workinglist = linkedlist;
  struct listelement *resultlist = 0; 
  long lastsize = listsize(linkedlist); 

  do{
    lastsize = listsize(resultlist);    
    resultlist = 0;

    while(workinglist != 0){

      long start = workinglist -> start;
      long end = workinglist -> end;
      struct listelement *parent = workinglist;
      struct listelement *tocheck = workinglist -> next;

      while(tocheck != 0){

        if(tocheck -> start < start && tocheck -> end < start){
          //element to check before our working element
          continue;
        }else if(tocheck -> start > end && tocheck -> end > end){
          //element to check after our working element
          continue;
        }else if(tocheck -> start < start && tocheck -> end < end){
          //element to check starts before but ends before working element 
          start = tocheck -> start;
          parent -> next = tocheck -> next;
          free(tocheck);
          tocheck = parent;
          continue;
        }else if(tocheck -> start > start && tocheck -> end > end){
          //element to check starts after but ends after working element
          end = tocheck -> end;
          parent -> next = tocheck -> next;
          free(tocheck);
          tocheck = parent;
          continue;
        }else if(tocheck -> start > start && tocheck -> end < end){
          //element contained in our working element
          parent -> next = tocheck -> next;
          free(tocheck);
          tocheck = parent;
          continue;
        }else if(tocheck -> start < start && tocheck -> end > end){
          //element contains our working element
          start = tocheck -> start;
          end = tocheck -> end;
          parent -> next = tocheck -> next;
          free(tocheck);
          tocheck = parent;
          continue;
        }else {
          printf("You should have never ended up here!!!");
        }

        parent = tocheck;
        tocheck = tocheck -> next;

      }

      struct listelement *newelem = malloc(sizeof(struct listelement));
      newelem -> start = start;
      newelem -> end = end;
      if(resultlist == 0){
        resultlist = newelem;
      }else {
        newelem -> next = resultlist;
        resultlist = newelem;
      } 

      workinglist = workinglist -> next;
    }
  }while(lastsize > listsize(resultlist));

  return resultlist;
}

long countitems(struct listelement *linkedlist){

  long amount = 0;

  while(linkedlist != 0){
    long start = linkedlist -> start;
    long end = linkedlist -> end;
    amount += end - start + 1;
    linkedlist = linkedlist -> next;
  }

  return amount;

}

long totalgooditems(struct listelement *linkedlist){

  long amount = 0; 

  //coales linkedlist
  struct listelement *colaesedlist = coaleslinkedlist(linkedlist);  

  //count items in new list 
  return countitems(colaesedlist);

}

long checkspoilage(struct listelement *linkedlist,long value){
  struct listelement *curelem = linkedlist;
  while(curelem != 0) {
    if(curelem -> start <= value && curelem -> end >= value){
      return 1;
    }
    curelem = curelem -> next; 
  }
  return 0;
}

long main(long argc, char *argv[])
{
  FILE *fptr = fopen("input", "r");

  if(!fptr){
    printf("failed to open file\n");
    return 1;
  }

  char buf[1024];
  long result = 0;

  long secondpart = 0;
  struct listelement *linkedlist = malloc(sizeof(struct listelement));
  struct listelement *startlinkedlist = linkedlist;

  while(fgets(buf, 1024, fptr)){
    //printf("%s", buf);
    if(!secondpart){
      long acc = 0;
      for(long i = 0; i < 1024;i++){
        if (buf[i]== '-'){
          linkedlist -> start = acc;
          acc = 0;
          continue;
        }else if (i==0 && buf[i] == '\n') {
          secondpart = 1;
          linkedlist -> next -> next = 0;
          break;
        }else if(buf[i] == '\n'){
          linkedlist -> end = acc;
          linkedlist -> next = malloc(sizeof(struct listelement)); 
          //printf("start: %ld end: %ld\n", linkedlist -> start, linkedlist -> end);
          linkedlist -> next -> next = linkedlist;
          linkedlist = linkedlist -> next;
          break;
        }else {
          acc *= 10;
          acc += buf[i] - '0';
        } 
      }}
    else {
      long acc = 0;
      for(long i = 0; i < 1024; i++){
        if(buf[i] == '\n'){
          result += checkspoilage(startlinkedlist, acc);
          break;
        }else {
          acc *= 10;
          acc += buf[i] - '0';
        }
      } 
    }
  }

  long r2 = totalgooditems(startlinkedlist);

  printf("The result is %ld\n", result);
  printf("The result for the secondpart is %ld\n", r2);
  return 0;
}





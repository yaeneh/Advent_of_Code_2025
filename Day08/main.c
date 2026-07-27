#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

const int bufsize = 5012;

struct Boxes{
  long x;
  long y;
  long z;
};

long computedistance(struct Boxes *b1, struct Boxes *b2){
  long xd = ((b1 -> x) - (b2 -> x))*((b1 -> x) - (b2 -> x));
  long yd = ((b1 -> y) - (b2 -> y))*((b1 -> y) - (b2 -> y));
  long zd = ((b1 -> z) - (b2 -> z))*((b1 -> z) - (b2 -> z));
  return (sqrt(xd + yd + zd));
}

void dfs(int n, int node, bool adjacencym[n][n], bool visited[]){
  visited[node] = true;
  for(int i = 0; i < n; i++){
    if(adjacencym[node][i] == true && visited[i] == false){
      dfs(n, i, adjacencym, visited);
    }
  }
}

bool isconnected(int n, bool visited[]){
  bool sum = true;
  for(int i = 0; i < n; i++){sum = sum && visited[i];}
  return sum;
}

int main(int argc, char *argv[])
{
  FILE *fptr = fopen("input", "r");

  if(!fptr){
    printf("failed to open file\n");
    return 1;
  }

  char buf[bufsize];
  long elements = 0;

  while(fgets(buf, bufsize, fptr)){
    elements++;
  }

  printf("elements: %ld\n", elements);
  
  struct Boxes * boxesarray = calloc(elements, sizeof(struct Boxes));
  bool *visited = calloc(elements, sizeof *visited);
  double (*distances)[elements] = calloc(elements, sizeof *distances);
  bool (*adjacency)[elements] = calloc(elements, sizeof *adjacency);
  fptr = fopen("input", "r");

  long row = 0;
  while(fgets(buf, bufsize, fptr)){
    
    long value = 0;
    long valuesseen = 0;
    for(int i = 0; i < bufsize; i++){
      
      if(buf[i] >= '0' && buf[i]<= '9'){
        value = value*10 + (buf[i]- '0');
      }else if(buf[i] == ','){
        if(valuesseen == 0){
          boxesarray[row].x = value;
          valuesseen++;
          value = 0;
        }else if(valuesseen == 1){
          boxesarray[row].y = value;
          valuesseen++;
          value = 0;
        }else {
          printf("error found in numberparsing\n");
          return 0;
        } 
      }else if(buf[i] == '\n' || buf[i] == '\0'){
        if(valuesseen != 2){
          printf("error found in end of line parsing\n");
          printf("valueseen: %ld\n", valuesseen);
          return 0;
        }
        boxesarray[row].z = value;
        value = 0;
        valuesseen = 0;
        break;

      }else {
        printf("found not expected input\n");
        return 0;
      }
    }
    row++;
  }
  
  for(int i = 0; i < elements; i++){
    for(int j = 0; j < elements; j++){
      if(i != j){
        distances[i][j] = computedistance(&boxesarray[i], &boxesarray[j]);  
      }
    }
  }

  for(int i = 0; i< elements; i++){
    distances[i][i] = DBL_MAX;
    adjacency[i][i] = false;
  }

  for(int connections = 0; connections < 1000; connections++){
    double smallestd = DBL_MAX;
    int indexi= 0;
    int indexj= 0;
    for(int i = 0; i < elements; i++){
      for(int j = 0; j < elements; j++){
        if(i != j){
         if(distances[i][j] < smallestd){
            smallestd = distances[i][j];
            indexi = i;
            indexj = j;
          } 
        }
      }
    }
    adjacency[indexi][indexj] = true;
    adjacency[indexj][indexi] = true;
    distances[indexi][indexj] = DBL_MAX;
    distances[indexj][indexi] = DBL_MAX;
  } 

  long resultp1 =0; 
  long max1 =0;
  long max2 = 0;
  long max3 = 0;
  long lastseensize = 0;

  for(int i = 0; i< elements; i++){
    if(visited[i] == false){
      dfs(elements, i, adjacency, visited);
      long sum = 0;
      for(int j = 0; j < elements; j++){
        if(visited[j] == true){sum++;}
      }
      int temp = sum - lastseensize;
      if(temp > max1){
        max3 = max2;
        max2 = max1;
        max1 = temp;
      }else if(temp > max2){
        max3 = max2;
        max2 = temp;
      }else if(temp > max3){
        max3 = temp;
      }
      lastseensize = sum;

    }else {
      continue;
    }
  }

  resultp1 = max1*max2*max3;
  printf("max1: %ld\n", max1);
  printf("max2: %ld\n", max2);
  printf("max3: %ld\n", max3);

  printf("result of part1 is: %ld", resultp1);

  for(int i = 0; i < elements; i++){visited[i]= false;}

  for(int i = 0; i < elements; i++){
    for(int j = 0; j < elements; j++){
      if(i != j){
        distances[i][j] = computedistance(&boxesarray[i], &boxesarray[j]);  
      }
    }
  }

  for(int i = 0; i< elements; i++){
    distances[i][i] = DBL_MAX;
    adjacency[i][i] = false;
  }

  int indexi= 0;
  int indexj= 0;
  while(!isconnected(elements, visited)){

    for(int i = 0; i< elements;i++){visited[i]=false;}

    double smallestd = DBL_MAX;
    for(int i = 0; i < elements; i++){
      for(int j = 0; j < elements; j++){
        if(i != j){
         if(distances[i][j] < smallestd){
            smallestd = distances[i][j];
            indexi = i;
            indexj = j;
          } 
        }
      }
    }
    adjacency[indexi][indexj] = true;
    adjacency[indexj][indexi] = true;
    distances[indexi][indexj] = DBL_MAX;
    distances[indexj][indexi] = DBL_MAX;

    dfs(elements, 0, adjacency, visited);
  } 

  long resultp2 = boxesarray[indexi].x * boxesarray[indexj].x; 

  printf("the result for p2 is: %ld\n", resultp2);

}


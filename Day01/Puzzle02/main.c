#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){

    FILE *fptr;
    fptr = fopen("input", "r");

    char buf[1<<10]; 
    
    int max = 100;
    int pos = 50;
    int res = 0;

    while(fgets(buf, 1<<10, fptr)){
        
        int result = atoi(buf + 1);

        if(buf[0] == 'R'){
            
            int crosses = ((pos + result)/max);
            pos = (pos + result)%max;
            
            res += crosses;

//            printf("R%d\n", result);

        }else {

            res += result / max;
            result = result%max;

            if (pos != 0 && result >= pos) {
                res++;
            }
            pos = (pos - result + max) % max;


//            printf("L%d\n", result);

        }

    }

    printf("the code is: %d", res);


    return 1;
}

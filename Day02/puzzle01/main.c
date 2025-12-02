#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

long numbersfound (long a, long b){

    if(b < a) return 0;

    long result = 0;

    long current = a;   

    while (current <= b){

        long temp = current;

        long length = 1;

        while (temp > 9){
            temp /= 10;
            length++;
        }

        //printf("current = %ld, length %ld\n", current, length);

        if(length%2 == 1) {
            current++;
            continue; 
        }

        long power = length/2;

        long decimalvalue = (long)powl(10, power);

        long lower = current%decimalvalue;
        long upper = current/decimalvalue;
        
        printf("upper value: %ld\n", upper);
        printf("lower value: %ld\n", lower);

        if(lower == upper) result += current; 

        current++;
    }

    return result;
}

int main (int argc, char **argv){

    FILE *fptr = fopen("input", "r"); 
    
    if(!fptr){
        printf("failed to open file");
        return 1;
    }

    char buf[1024];
    long result = 0;

    while (fgets(buf, 1024, fptr)) {

        char *token = strtok(buf, ",");
        while (token != NULL) {
            long a;
            long b;
            if(sscanf(token, "%ld-%ld", &a, &b) == 2){
                result += numbersfound(a, b);
                
            } else {
                printf("invalid entry\n");
            }
            token = strtok(NULL,",");
        }
    }


    printf("the result should be %ld\n", result);

    fclose(fptr);

    return 0;
}

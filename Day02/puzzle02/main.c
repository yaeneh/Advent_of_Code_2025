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

        //computing decimal length of int
        while (temp > 9){
            temp /= 10;
            length++;
        }

        temp = current;

        //printf("current = %ld, length %ld\n", current, length);
    
        //bool value array if i divides the lenth
        int divisors[length/2];

        //instantiating divisors array
        for (int i = 0; i < length/2;i++) {

            if(length%(i+1) == 0) {
                divisors[i] = 1;
            }else {
                divisors[i] = 0;
            }
        }

        int sum = 0;

        //for all divisors of a given length check if it is valid 
        for (int i = 0;i < length/2; i++){
            
            if(divisors[i]){
            
            sum = 1;
            long power = i+1;
            long amount = length/(i+1);

            long decimalvalue = (long)powl(10, power);

            long values[amount];
            temp = current;

            //generate all the values
            for(int j = 0; j < amount; j++){
               values[j] = temp%decimalvalue; 
               temp = temp/decimalvalue;
            }

//            for(int j = 0; j < amount; j++){
//
//                printf("value at %d is %ld\n", j, values[j]);
//            }
            
            for(int j = 0; j < amount-1; j++){
                
               sum = (values[j] == values[j+1]) && sum; 

            }

            if (sum ==1) {
                break;
            }

            }
        }

        if(sum == 1) {
            result += current;
        }

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

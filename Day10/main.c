#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define BUFSIZE 1024

typedef struct Node {
    uint64_t value;
    struct Node *next;
}Node; 

uint64_t calculate_part1(Node *LinkedList, uint64_t goalstate ) {

  



}

void parse_nbrackets(char *buf, Node **LinkedList){
  char *start = strchr(buf, '(');
  char *end = strchr(buf, ')');

  //while we still find () brackets
  while(start != NULL){
     
    uint64_t buttonchange = 0;
    char *nrpointer = start + 1;

    //parsing a single () bracket
    char *nrendpointer = NULL;
    long nr = strtol(nrpointer, &nrendpointer, 10);
    do{
      buttonchange = buttonchange | (1<<nr);
      nrpointer = nrendpointer + 1;
      nr = strtol(nrpointer, &nrendpointer, 10);
    } while(nrendpointer != nrpointer);

    //creating a new Linkedlist entry and adding it in the front
    Node *tmp = malloc(sizeof(Node));
    tmp -> value = buttonchange;
    tmp -> next = *LinkedList;
    *LinkedList = tmp;

    //searching the next () brackets
    start = strchr(end+1, '(');
    end = strchr(end+1, ')');
  }
}

void parse_ebrackets(char *buf, uint64_t *result, uint64_t *bits){
  char *start = strchr(buf, '[');
  char *end = strchr(buf, ']');

  char *tmp = start + 1;
  
  while(tmp != end){

    uint64_t towrite = 0x1;
    if(*tmp == '.') {
      towrite = 0x0;
    }

    *result = ((*result) << 1)|towrite;
    tmp++;

  }
 
}

int main(int argc, char *argv[])
{
    FILE *fptr = fopen("input", "r");

    if (!fptr) {
        printf("file not opened\n");
        return EXIT_FAILURE;
    }

    char buf[BUFSIZE];

    // Total minimum presses across all lines
    uint64_t total = 0;

    while (fgets(buf, sizeof(buf), fptr)) {

        // Remove newline
        buf[strcspn(buf, "\n")] = '\0';

        uint64_t goalstate = 0;
        uint64_t bits = 0;

        parse_ebrackets(buf, &goalstate, &bits);

        printf("Goalstate = %b\n", goalstate);
        
        Node *LinkedList = NULL;
        parse_nbrackets(buf, &LinkedList);
        
        Node *tmp = LinkedList;
        while(tmp != NULL){
          printf("Button: %b\n", tmp->value);
          tmp = tmp -> next;
        }
        //parse_sbrackets();
        
        calculate_part1(LinkedList, goalstate);
    /*

        // Number of possible states = 2^size 

        uint32_t possible_states = UINT32_C(1) << size;

        // ========================= Parse () ========================= 

        Node *head = NULL;

        char *p = buf;

        while ((p = strchr(p, '(')) != NULL) {

            end = strchr(p, ')');

            if (!end)
                break;

            Node *new_Node = malloc(sizeof(Node));

            if (new_Node == NULL) {
                perror("malloc");
                fclose(fptr);
                return EXIT_FAILURE;
            }

            uint32_t value = 0;

            char *number_start = p + 1;

            while (number_start < end) {

                char *number_end;

                long number = strtol(
                    number_start,
                    &number_end,
                    10
                );

                if (number_end == number_start) {
                    break;
                }

                printf("number = %ld\n", number);

                value |= UINT32_C(1) << number;

                number_start = number_end;

                // Skip comma
                if (number_start < end &&
                    *number_start == ',') {
                    number_start++;
                }
            }

            new_Node->value = value;
            new_Node->next = head;
            head = new_Node;

            p = end + 1;
        }

        // ========================= Parse {} =========================

        start = strchr(buf, '{');
        end = strchr(buf, '}');

        if (start && end) {
            printf("{}: ");

            for (char *p = start + 1; p < end; p++) {
                printf("%c", *p);
            }

            printf("\n");
        }

        // ========================= Initialize states ========================= 

        uint32_t *states =
            malloc(possible_states * sizeof(uint32_t));

        if (states == NULL) {
            perror("malloc");
            fclose(fptr);
            return EXIT_FAILURE;
        }

        for (uint32_t i = 0; i < possible_states; i++) {
            states[i] = UINT32_MAX;
        }

        // ========================= BFS ========================= Start at state 0. 

        uint32_t result = bfs(
            goal,
            head,
            states,
            ~(0x0)
        );

        if (result == UINT32_MAX) {
            printf("Goal cannot be reached\n");
        } else {
            printf("Minimum presses: %u\n", result);

            // Add this line's minimum to the total
            total += result;
        }

        // ========================= Free states ========================= 
        free(states);

        // ========================= Free linked list ========================= 

        Node *tmp;

        while (head != NULL) {
            tmp = head;
            head = head->next;
            free(tmp);
        }
        */
        printf("LINE: %s\n\n", buf);
        
    }

    fclose(fptr);

    // ========================= Final result ========================= 

    printf("Total minimum presses: %" PRIu64 "\n", total);

    return EXIT_SUCCESS;
}

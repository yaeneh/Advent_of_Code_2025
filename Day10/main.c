#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 1024

typedef struct Node {
  uint64_t value;
  struct Node *next;
} Node;

uint64_t calculate_part1(Node *LinkedList, uint64_t goalstate, uint64_t *states,
                         uint64_t bits) {

  uint64_t currentstate = 0;

  uint64_t length = bits;
  uint64_t *arraylist = calloc(length, sizeof(uint64_t));
  if (arraylist == NULL) {
    return UINT64_MAX;
  }
  uint64_t start = 0;
  uint64_t useduntil = 1;

  for (uint64_t i = 0; i < bits; i++) {
    states[i] = UINT64_MAX;
  }
  states[0] = 0;
  arraylist[0] = 0;

  Node *tmp = LinkedList;

  // reapeat bfs until return if is triggered
  while (start < useduntil) {
    currentstate = arraylist[start++];

    if (currentstate == goalstate) {
      uint64_t result = states[goalstate];
      return result;
    }
    tmp = LinkedList;

    // cycling trough the linkedlist until the end
    while (tmp != NULL) {

      // calulate the next state we receive from currentstate and pushing the
      // button
      uint64_t nextstate = currentstate ^ tmp->value;

      // if the nextstate is unvisited queue it in the list and write the
      // layernumber down
      assert(tmp->value < bits);
      assert(nextstate < bits);
      if (states[nextstate] == UINT64_MAX) {
        states[nextstate] = states[currentstate] + 1;
        arraylist[useduntil++] = nextstate;
      }
      tmp = tmp->next;
    }
  }

  printf("We shouldnt end up here\n");
  return 0;
}

void parse_nbrackets(char *buf, Node **LinkedList) {
  char *start = strchr(buf, '(');
  char *end = strchr(buf, ')');

  // while we still find () brackets
  while (start != NULL) {

    uint64_t buttonchange = 0;
    char *nrpointer = start + 1;

    // parsing a single () bracket
    char *nrendpointer = NULL;
    long nr = strtol(nrpointer, &nrendpointer, 10);
    do {
      buttonchange = buttonchange | (1 << nr);
      nrpointer = nrendpointer + 1;
      nr = strtol(nrpointer, &nrendpointer, 10);
    } while (nrendpointer != nrpointer);

    // creating a new Linkedlist entry and adding it in the front
    Node *tmp = malloc(sizeof(Node));
    tmp->value = buttonchange;
    tmp->next = *LinkedList;
    *LinkedList = tmp;

    // searching the next () brackets
    start = strchr(end + 1, '(');
    end = strchr(end + 1, ')');
  }
}

void parse_ebrackets(char *buf, uint64_t *result, uint64_t *bits) {
  char *start = strchr(buf, '[');
  char *end = strchr(buf, ']');

  for (char *tmp = start + 1; tmp != end; tmp++) {
    if (*tmp == '#') {
      *result |= UINT64_C(1) << *bits;
    }
    (*bits)++;
  }
}

int main(int argc, char *argv[]) {
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

    printf("Goalstate = %lb\n", goalstate);

    Node *LinkedList = NULL;
    parse_nbrackets(buf, &LinkedList);

    Node *tmp = LinkedList;
    while (tmp != NULL) {
      printf("Button: %lb\n", tmp->value);
      tmp = tmp->next;
    }
    // parse_sbrackets();
    int nrofstates = 1 << (bits);
    printf("nr of states possible: %d\n", nrofstates);
    uint64_t *states = malloc(nrofstates * sizeof(uint64_t));

    uint64_t result_p1 =
        calculate_part1(LinkedList, goalstate, states, nrofstates);
    assert(result_p1 >= 1);
    total += result_p1;

    printf("subresult: %lu\n", result_p1);
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

        // ========================= BFS ========================= Start at
       state 0.

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define BUFSIZE 1024

typedef struct Node {
    uint32_t value;
    struct Node *next;
} Node;

uint32_t bfs(uint32_t goal,
             uint32_t state,
             Node *head,
             uint32_t *states,
             uint32_t possible_states)
{
    uint32_t *queue = malloc(possible_states * sizeof(uint32_t));

    if (queue == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    uint32_t front = 0;
    uint32_t back = 0;

    states[state] = 0;
    queue[back++] = state;

    while (front < back) {
        state = queue[front++];

        if (state == goal) {
            uint32_t result = states[state];

            free(queue);

            return result;
        }

        Node *tmp = head;

        while (tmp != NULL) {
            uint32_t new_state = state ^ tmp->value;

            if (states[new_state] == UINT32_MAX) {
                states[new_state] = states[state] + 1;
                queue[back++] = new_state;
            }

            tmp = tmp->next;
        }
    }

    free(queue);

    return UINT32_MAX;
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

        /*
         * =========================
         * Parse []
         * =========================
         */

        char *start = strchr(buf, '[');
        char *end = strchr(buf, ']');

        uint32_t goal = 0;
        uint32_t size = 0;

        if (start && end) {
            printf("[]: ");

            for (char *p = start + 1; p < end; p++) {

                goal <<= 1;

                if (*p == '#') {
                    goal |= 1;
                }

                size++;

                printf("%c", *p);
            }

            printf("\n");
        }

        /*
         * Number of possible states = 2^size
         */

        uint32_t possible_states = UINT32_C(1) << size;

        /*
         * =========================
         * Parse ()
         * =========================
         */

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

        /*
         * =========================
         * Parse {}
         * =========================
         */

        start = strchr(buf, '{');
        end = strchr(buf, '}');

        if (start && end) {
            printf("{}: ");

            for (char *p = start + 1; p < end; p++) {
                printf("%c", *p);
            }

            printf("\n");
        }

        /*
         * =========================
         * Initialize states
         * =========================
         */

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

        /*
         * =========================
         * BFS
         * =========================
         *
         * Start at state 0.
         */

        uint32_t result = bfs(
            goal,
            0,
            head,
            states,
            possible_states
        );

        if (result == UINT32_MAX) {
            printf("Goal cannot be reached\n");
        } else {
            printf("Minimum presses: %u\n", result);

            // Add this line's minimum to the total
            total += result;
        }

        /*
         * =========================
         * Free states
         * =========================
         */

        free(states);

        /*
         * =========================
         * Free linked list
         * =========================
         */

        Node *tmp;

        while (head != NULL) {
            tmp = head;
            head = head->next;
            free(tmp);
        }

        printf("LINE: %s\n\n", buf);
    }

    fclose(fptr);

    /*
     * =========================
     * Final result
     * =========================
     */

    printf("Total minimum presses: %" PRIu64 "\n", total);

    return EXIT_SUCCESS;
}

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

int bufsize = 5012;

struct Points {
    long i;
    long j;
};

struct Node {
    struct Points point;
    struct Node *next;
};


long calculatesize(struct Points *p1, struct Points *p2)
{
    long di = labs(p1->i - p2->i) + 1;
    long dj = labs(p1->j - p2->j) + 1;

    return di * dj;
}


long min(long a, long b)
{
    return a < b ? a : b;
}


long max(long a, long b)
{
    return a > b ? a : b;
}


/*
 * Return true if point (x,y) lies exactly on the line
 * segment between (x1,y1) and (x2,y2).
 */
bool point_on_segment(long x, long y,
                      long x1, long y1,
                      long x2, long y2)
{
    if (x1 == x2) {
        /*
         * Vertical segment.
         */
        return x == x1 &&
               y >= min(y1, y2) &&
               y <= max(y1, y2);
    }

    if (y1 == y2) {
        /*
         * Horizontal segment.
         */
        return y == y1 &&
               x >= min(x1, x2) &&
               x <= max(x1, x2);
    }

    return false;
}


/*
 * Check whether a point is inside the polygon.
 *
 * Points on the polygon boundary are also considered inside.
 */
bool point_inside_or_boundary(long x, long y,
                              struct Node *LinkedList)
{
    bool inside = false;

    struct Node *p = LinkedList;

    while (p != NULL) {

        struct Node *q = p->next;

        if (q == NULL)
            q = LinkedList;

        long x1 = p->point.i;
        long y1 = p->point.j;

        long x2 = q->point.i;
        long y2 = q->point.j;


        /*
         * First handle points lying exactly on the polygon.
         */
        if (point_on_segment(x, y, x1, y1, x2, y2))
            return true;


        /*
         * Ray casting.
         *
         * We cast a ray horizontally to the right.
         */
        if ((y1 > y) != (y2 > y)) {

            long double intersection =
                (long double)x1 +
                (long double)(x2 - x1) *
                (long double)(y - y1) /
                (long double)(y2 - y1);

            if ((long double)x < intersection)
                inside = !inside;
        }

        p = p->next;

        if (p == LinkedList)
            break;
    }

    return inside;
}


/*
 * Check whether a polygon edge passes through the INTERIOR
 * of the candidate rectangle.
 *
 * Touching the rectangle boundary is okay.
 */
bool edge_crosses_rectangle_interior(
    long ex1, long ey1,
    long ex2, long ey2,
    long rx1, long ry1,
    long rx2, long ry2)
{
    /*
     * Horizontal polygon edge.
     */
    if (ey1 == ey2) {

        long y = ey1;

        long edge_left = min(ex1, ex2);
        long edge_right = max(ex1, ex2);

        /*
         * The polygon edge is strictly inside the vertical
         * range of the rectangle.
         */
        if (y > ry1 && y < ry2) {

            /*
             * Does it pass through the interior horizontally?
             */
            if (edge_right > rx1 && edge_left < rx2)
                return true;
        }
    }


    /*
     * Vertical polygon edge.
     */
    else if (ex1 == ex2) {

        long x = ex1;

        long edge_bottom = min(ey1, ey2);
        long edge_top = max(ey1, ey2);

        /*
         * The polygon edge is strictly inside the horizontal
         * range of the rectangle.
         */
        if (x > rx1 && x < rx2) {

            /*
             * Does it pass through the interior vertically?
             */
            if (edge_top > ry1 && edge_bottom < ry2)
                return true;
        }
    }

    return false;
}


/*
 * Determine whether the rectangle defined by two red points
 * is completely inside the red/green polygon.
 */
bool rectangleisvalid(struct Points *start,
                      struct Points *end,
                      struct Node *LinkedList)
{
    long x1 = min(start->i, end->i);
    long x2 = max(start->i, end->i);

    long y1 = min(start->j, end->j);
    long y2 = max(start->j, end->j);


    /*
     * Check all four corners.
     *
     * This is important because the rectangle may be thin,
     * so checking only the center is not sufficient.
     */
    if (!point_inside_or_boundary(x1, y1, LinkedList))
        return false;

    if (!point_inside_or_boundary(x1, y2, LinkedList))
        return false;

    if (!point_inside_or_boundary(x2, y1, LinkedList))
        return false;

    if (!point_inside_or_boundary(x2, y2, LinkedList))
        return false;


    /*
     * Now make sure no polygon edge cuts through the interior
     * of the rectangle.
     */
    struct Node *p = LinkedList;

    while (p != NULL) {

        struct Node *q = p->next;

        if (q == NULL)
            q = LinkedList;

        long ex1 = p->point.i;
        long ey1 = p->point.j;

        long ex2 = q->point.i;
        long ey2 = q->point.j;

        if (edge_crosses_rectangle_interior(
                ex1, ey1,
                ex2, ey2,
                x1, y1,
                x2, y2)) {

            return false;
        }

        p = p->next;

        if (p == LinkedList)
            break;
    }

    return true;
}


/*
 * Part 2.
 *
 * Try every pair of red points.
 */
long calculaterectanglep2(struct Node *start,
                           struct Node *end)
{
    long biggestrectangle = 0;

    struct Node *Linkedliststart = start;

    while (start != NULL) {

        for (struct Node *iterator = start;
             iterator != NULL;
             iterator = iterator->next) {

            long currentrectsize =
                calculatesize(
                    &start->point,
                    &iterator->point
                );

            /*
             * Don't perform the expensive geometry test
             * if this rectangle cannot improve the answer.
             */
            if (currentrectsize <= biggestrectangle)
                continue;

            if (rectangleisvalid(
                    &start->point,
                    &iterator->point,
                    Linkedliststart)) {

                biggestrectangle = currentrectsize;
            }
        }

        start = start->next;
    }

    return biggestrectangle;
}


/*
 * Part 1.
 */
long calculaterectangle(struct Node *start,
                        struct Node *end)
{
    long biggestrectangle = 0;

    while (start != NULL) {

        for (struct Node *iterator = start;
             iterator != NULL;
             iterator = iterator->next) {

            long currentrectsize =
                calculatesize(
                    &start->point,
                    &iterator->point
                );

            if (currentrectsize > biggestrectangle)
                biggestrectangle = currentrectsize;
        }

        start = start->next;
    }

    return biggestrectangle;
}


int main(int argc, char *argv[])
{
    FILE *fptr = fopen("input", "r");

    if (!fptr) {
        printf("file not opened\n");
        return EXIT_FAILURE;
    }

    char buf[bufsize];

    struct Node *Linkedliststart = NULL;
    struct Node *Linkedlistend = NULL;


    /*
     * Read input.
     */
    while (fgets(buf, bufsize, fptr)) {

        long sum = 0;
        long pointi = 0;
        long pointj = 0;

        for (int i = 0; i < bufsize; i++) {

            if (buf[i] >= '0' && buf[i] <= '9') {

                sum = sum * 10 + (buf[i] - '0');

            }
            else if (buf[i] == ',') {

                pointi = sum;
                sum = 0;

            }
            else if (buf[i] == '\n' ||
                     buf[i] == '\0') {

                pointj = sum;

                struct Node *temp =
                    calloc(1, sizeof(struct Node));

                if (temp == NULL) {
                    fclose(fptr);
                    return EXIT_FAILURE;
                }

                temp->point.i = pointi;
                temp->point.j = pointj;
                temp->next = NULL;

                if (Linkedliststart == NULL) {

                    Linkedliststart = temp;
                    Linkedlistend = temp;

                } else {

                    Linkedlistend->next = temp;
                    Linkedlistend = temp;
                }

                break;
            }
        }
    }

    fclose(fptr);


    /*
     * Print points.
     */
    for (struct Node *start = Linkedliststart;
         start != NULL;
         start = start->next) {

        printf("%ld,%ld\n",
               start->point.i,
               start->point.j);
    }


    /*
     * Part 1.
     */
    long resultp1 =
        calculaterectangle(
            Linkedliststart,
            Linkedlistend
        );

    printf("The result of p1 is: %ld\n", resultp1);


    /*
     * Part 2.
     */
    long resultp2 =
        calculaterectanglep2(
            Linkedliststart,
            Linkedlistend
        );

    printf("The result of p2 is: %ld\n", resultp2);


    /*
     * Free linked list.
     */
    struct Node *current = Linkedliststart;

    while (current != NULL) {

        struct Node *next = current->next;

        free(current);

        current = next;
    }

    return EXIT_SUCCESS;
}

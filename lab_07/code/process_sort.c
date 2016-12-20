// filename: process_sort.c
// author: James Rolfe
// updated: 20161018

#include <stdlib.h>
#include <stdio.h>

typedef int (*compar) (const void* a, const void* b);

// arrival time
int ascending_order(const void* a, const void* b)
{
    return (((int *)a)[1] - ((int *)b)[1]);
}

// priorities
int descending_order(const void* a, const void* b)
{
    if((((int *)a)[2] - ((int *)b)[2]) == 0)
    {
        return ((((int *)a)[1] - ((int *)b)[1]));
    }
    else
    {
        return (((int *)b)[2] - ((int *)a)[2]);
    }
}

int main ()
{
    // hardcoded process.txt, {pid, arrival time, priority}
    int p[7][3] = {
                    {5, 10, 3}, 
                    {2, 4, 0},
                    {7, 14, 0},
                    {3, 6, 1},
                    {1, 2, 1},
                    {4, 8, 2},
                    {6, 12, 3}
                  };

    printf("NOT SORTED:\n");
    printf("pid | arrival_time | priority\n");
    for(int i = 0; i < 7; i++)
    {
        if(p[i][1] > 9)
        {
            printf("%d | %d | %d\n", p[i][0], p[i][1], p[i][2]);
        }
        else
        {
            printf("%d |  %d | %d\n", p[i][0], p[i][1], p[i][2]);
        }
    }

    printf("\n"); // aesthetic

    qsort(p, 7, sizeof(int)*3, ascending_order); // sort by arrival time

    printf("SORTED BY ARRIVAL TIME:\n");
    printf("pid | arrival_time | priority\n");
    for(int i = 0; i < 7; i++)
    {
        if(p[i][1] > 9)
        {
            printf("%d | %d | %d\n", p[i][0], p[i][1], p[i][2]);
        }
        else
        {
            printf("%d |  %d | %d\n", p[i][0], p[i][1], p[i][2]);
        }
    }

    printf("\n"); // aesthetic

    qsort(p, 7, sizeof(int)*3, descending_order); // sort by priority

    printf("SORTED BY PRIORITY:\n");
    printf("pid | arrival_time | priority\n");
    for(int i = 0; i < 7; i++)
    {
        if(p[i][1] > 9)
        {
            printf("%d | %d | %d\n", p[i][0], p[i][1], p[i][2]);
        }
        else
        {
            printf("%d |  %d | %d\n", p[i][0], p[i][1], p[i][2]);
        }
    }
}

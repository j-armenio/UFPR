#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int key;
    int value;
} Point;

int compare(const void *p1, const void *p2)
{
    Point *point1 = *(Point **)p1;
    Point *point2 = *(Point **)p2;

    if (point1->key < point2->key)
        return -1;
    else if (point1->key > point2->key)
        return 1;
    else
        return 0;
}

int main()
{
    Point **points = malloc(10 * sizeof(Point *));
    for (int i = 0; i < 10; i++)
    {
        points[i] = malloc(sizeof(Point));
    }

    int j = 10;
    for (int i = 0; i < 10; i++)
    {
        points[i]->key = j;
        points[i]->value = i;
        j--;
    }

    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", points[i]->key);
    }

    printf("--------------------\n");

    int length = 10;

    qsort(points, length, sizeof(Point *), compare);

    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", points[i]->key);
    }

    for (int i = 0; i < 10; i++)
    {
        free(points[i]);
    }
    free(points);

    return 0;
}
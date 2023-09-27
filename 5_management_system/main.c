#include "my_malloc.h"
#include <stdio.h>

int main()
{
    int sum = 0;
    int *arr = (int *)my_malloc(4 * sizeof(int));

    if (!arr)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    for (int i = 0; i < 4; i++)
    {
        printf("Enter integer %d: ", i + 1);
        scanf("%d", arr + i);
        sum += *(arr + i);
    }

    printf("Entered numbers: ");
    for (int i = 0; i < 4; i++)
    {
        printf("%d ", *(arr + i));
    }
    printf("\nSum: %d\n", sum);

    my_free(arr);
    return 0;
}

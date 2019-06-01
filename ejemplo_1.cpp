#include <stdio.h>
#include <omp.h>

struct Compare { int val; int index; };
#pragma omp declare reduction(maximum : struct Compare : omp_out = omp_in.val > omp_out.val ? omp_in : omp_out)

void selectionsort(int* arr, int size)
{
    for (int i = size - 1; i > 0; --i)
    {
        struct Compare max;
        max.val = arr[i];
        max.index = i;
        #pragma omp parallel for reduction(maximum:max)
        for (int j = i - 1; j >= 0; --j)
        {
            if (arr[j] > max.val)
            {
                max.val = arr[j];
                max.index = j;
            }
        }
        int tmp = arr[i];
        arr[i] = max.val;
        arr[max.index] = tmp;
    }
}

int main()
{
        int x[100] = {8,7,9,1,2,5,4,3,0,6,
                    8,7,9,1,2,5,4,3,0,6,
                    8,7,9,1,2,5,4,3,0,6,
                    8,7,9,1,2,5,4,3,0,6,
                    8,7,9,1,2,5,4,3,0,6,
                    8,7,9,1,2,5,4,3,0,6,
                    8,7,9,1,2,5,4,3,0,6,
                    8,7,9,1,2,5,4,3,0,6,
                    8,7,9,1,2,5,4,3,0,6,
                    8,7,9,1,2,5,4,3,0,6};
        selectionsort(x, 10);

        for (int i = 0; i < 100; i++)
                printf("%d\n", x[i]);
        return 0;
}
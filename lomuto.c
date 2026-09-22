#include <stdio.h>
#include <stdlib.h>

// Lomuto median

void swap(long long *a, long long *b)
{
    long long t = *a;
    *a = *b;
    *b = t;
}

int partition(long long *arr, int low, int high)
{
    long long pivot = arr[high];
    int i = low - 1;

    for(int j = low; j < high; j++){
        if(arr[j] <= pivot){
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    // place pivot in correct position
    swap(&arr[i + 1], &arr[high]);

    return i + 1;
}

long long lomuto(long long *arr, int n)
{
    int low = 0;
    int high = n - 1;
    int middle = n / 2;

    while(low <= high){
        int pivot = partition(arr, low, high);

        if(pivot == middle) return arr[pivot];

        if(pivot > middle) high = pivot - 1;

        else low = pivot + 1;
    }

    return arr[middle];
}

int main(void) {

    // using long long for simplicity, sacrificing a bit of efficency
    long long n;

    scanf("%lld", &n);

    long long *arr = malloc(n * sizeof(long long));

    for(int i = 0; i < n; i++){
        scanf("%lld", &arr[i]);
    }

    long long middle = lomuto(arr, n);

    if(n % 2){
        printf("%lld\n", middle);
    }
    else{
        // lomuto already placed arr[n / 2] in its final position, every value before it is <= middle, so the largest one is the other middle value without running lomuto again
        long long lower = arr[0];

        for(int i = 1; i < n / 2; i++){
            if(arr[i] > lower) lower = arr[i];
        }

        printf("%g\n", (lower + middle) / 2.0);
    }

    // ??
    free(arr);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>

// Median of Medians

void swap(long long *a, long long *b)
{
    long long t = *a;
    *a = *b;
    *b = t;
}

// sort small groups of 5
void insertionsort(long long *arr, int n)
{
    for(int i = 1; i < n; i++){
        long long current = arr[i];
        int j = i - 1;

        while(j >= 0 && arr[j] > current){
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = current;
    }
}

// partition around chosen pivot
int partition(long long *arr, int low, int high, long long pivot)
{
    // move pivot to the end
    int pivotindex = low;

    for(int i = low; i <= high; i++){
        if(arr[i] == pivot){
            pivotindex = i;
            break;
        }
    }

    swap(&arr[pivotindex], &arr[high]);

    // lomuto partition
    int i = low - 1;

    for(int j = low; j < high; j++){
        if(arr[j] <= pivot){
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);

    return i + 1;
}

long long m_of_m(long long *arr, int low, int high, int k)
{
    int n = high - low + 1;

    if(n <= 5){
        insertionsort(arr + low, n);
        return arr[k];
    }

    int groups = (n + 4) / 5;

    long long *medians = malloc(groups * sizeof(long long));

    for(int i = 0; i < groups; i++){
        int start = low + i * 5;
        int size = 5;

        // last group can have less than 5
        if(start + size > high + 1) size = high - start + 1;

        insertionsort(arr + start, size);
        // save median of each 
        medians[i] = arr[start + size / 2];
    }

    // recursion find median of medians
    long long pivot = m_of_m(medians, 0, groups - 1, groups / 2);

    free(medians);

    // partition original array around median of medians
    int pivotindex = partition(arr, low, high, pivot);

    if(k == pivotindex) return arr[pivotindex];

    if(k < pivotindex) return m_of_m(arr, low, pivotindex - 1, k);

    return m_of_m(arr, pivotindex + 1, high, k);
}

int main(void) {

    // using long long for simpl
    long long n;

    scanf("%lld", &n);

    long long *arr = malloc(n * sizeof(long long));

    for(int i = 0; i < n; i++){
        scanf("%lld", &arr[i]);
    }

    // n / 2 for position of the median
    long long middle = m_of_m(arr, 0, n - 1, n / 2);

    if(n % 2){
        printf("%lld\n", middle);
    }

    else{
        // m_of_m already placed arr[n / 2] in its final position, everything before it is <= middle, so the largest value on the left side is the lower median without running m_of_m a second time, same as with lomuto previous thing
        long long lower = arr[0];
        
        for(int i = 1; i < n / 2; i++) if(arr[i] > lower) lower = arr[i];

        printf("%g\n", (lower + middle) / 2.0);
    }

    free(arr);

    return 0;
}
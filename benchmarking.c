#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(long long *a, long long *b)
{
    long long t = *a;
    *a = *b;
    *b = t;
}

// Sort and Pick

// to avoid math.h
int log2int(int n)
{
    int result = 0;

    while(n > 1){
        n /= 2;
        result++;
    }

    return result;
}

// Heap Sort part

void maxheap(long long *arr, int n, int i)
{
    int largest = i;

    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // if left child exists and is larger
    if(left < n && arr[left] > arr[largest]) largest = left;

    // if right child exists and is larger
    if(right < n && arr[right] > arr[largest]) largest = right;

    // If one child was larger
    if(largest != i){
        swap(&arr[i], &arr[largest]);

        // rechecked moved down value
        maxheap(arr, n, largest);
    }
}

void heapsort(long long *arr, int n)
{
    // create max heap
    for(int i = n / 2 - 1; i >= 0; i--) maxheap(arr, n, i);

    // move largest element to the end
    for(int i = n - 1; i > 0; i--){
        swap(&arr[0], &arr[i]);

        // arr[i] is now sorted.
        maxheap(arr, i, 0);
    }
}

// Short Insertion fucntion

void insertionsort(long long *arr, int low, int high)
{
    for(int i = low + 1; i <= high; i++){
        long long current = arr[i];
        int j = i - 1;

        // move larger elements one position
        while(j >= low && arr[j] > current){
            arr[j + 1] = arr[j];
            j--;
        }

        // insert current in correct position
        arr[j + 1] = current;
    }
}

// Quick Sort/Lomuto partition

int partition(long long *arr, int low, int high)
{
    // lomuto partition
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

void quicksort(long long *arr, int low, int high)
{
    if(low >= high) return;

    int pivot = partition(arr, low, high);

    quicksort(arr, low, pivot - 1);
    quicksort(arr, pivot + 1, high);
}

// Introsort

// recursion function
void intro(long long *arr, int low, int high, int depth)
{
    int n = high - low + 1;

    if(n <= 1) return;

    if(n <= 32){
        insertionsort(arr, low, high);
        return;
    }

    // if recursion too deep, use heap
    if(depth <= 0){
        heapsort(arr + low, n);
        return;
    }

    // continue quicksort
    int pivot = partition(arr, low, high);

    intro(arr, low, pivot - 1, depth - 1);
    intro(arr, pivot + 1, high, depth - 1);
}

void introsort(long long *arr, int n)
{
    if(n <= 1) return;

    // maximum quicksort recursion depth
    int depth = 2 * log2int(n);

    intro(arr, 0, n - 1, depth);
}

double sortandpick(long long *arr, int n)
{
    introsort(arr, n);

    if(n % 2) return arr[n / 2];

    return (arr[n / 2 - 1] + arr[n / 2]) / 2.0;
}

// Lomuto median

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

double lomutomedian(long long *arr, int n)
{
    long long middle = lomuto(arr, n);

    if(n % 2){
        return middle;
    }

    else{
        // lomuto already placed arr[n / 2] in its final position, every value before it is <= middle, so the largest one is the other middle value without running lomuto again
        long long lower = arr[0];

        for(int i = 1; i < n / 2; i++){
            if(arr[i] > lower) lower = arr[i];
        }

        return (lower + middle) / 2.0;
    }
}

// Median of Medians

// sort small groups of 5
void insertionsort_mom(long long *arr, int n)
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
int partition_mom(long long *arr, int low, int high, long long pivot)
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
        insertionsort_mom(arr + low, n);
        return arr[k];
    }

    int groups = (n + 4) / 5;

    long long *medians = malloc(groups * sizeof(long long));

    for(int i = 0; i < groups; i++){
        int start = low + i * 5;
        int size = 5;

        // last group can have less than 5
        if(start + size > high + 1) size = high - start + 1;

        insertionsort_mom(arr + start, size);

        // save median of each
        medians[i] = arr[start + size / 2];
    }

    // recursion find median of medians
    long long pivot = m_of_m(medians, 0, groups - 1, groups / 2);

    free(medians);

    // partition original array around median of medians
    int pivotindex = partition_mom(arr, low, high, pivot);

    if(k == pivotindex) return arr[pivotindex];

    if(k < pivotindex) return m_of_m(arr, low, pivotindex - 1, k);

    return m_of_m(arr, pivotindex + 1, high, k);
}

double medianofmedians(long long *arr, int n)
{
    // n / 2 for position of the median
    long long middle = m_of_m(arr, 0, n - 1, n / 2);

    if(n % 2){
        return middle;
    }

    else{
        // m_of_m already placed arr[n / 2] in its final position, everything before it is <= middle, so the largest value on the left side is the lower median without running m_of_m a second time, same as with lomuto previous thing
        long long lower = arr[0];

        for(int i = 1; i < n / 2; i++) if(arr[i] > lower) lower = arr[i];

        return (lower + middle) / 2.0;
    }
}

int main(void) {

    FILE *file = fopen("tests/test.txt", "r");

    if(file == NULL){
        return 1;
    }

    int n = 0;
    long long value;

    while(fscanf(file, "%lld", &value) == 1){
        n++;
    }

    rewind(file);

    long long *arr = malloc(n * sizeof(long long));

    for(int i = 0; i < n; i++){
        fscanf(file, "%lld", &arr[i]);
    }

    fclose(file);

    long long *sortarr = malloc(n * sizeof(long long));
    long long *lomutoarr = malloc(n * sizeof(long long));
    long long *momarr = malloc(n * sizeof(long long));

    for(int i = 0; i < n; i++){
        sortarr[i] = arr[i];
        lomutoarr[i] = arr[i];
        momarr[i] = arr[i];
    }

    clock_t start = clock();

    double sortmedian = sortandpick(sortarr, n);

    clock_t end = clock();

    double sorttime = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();

    double lomutomed = lomutomedian(lomutoarr, n);

    end = clock();

    double lomutotime = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();

    double mommedian = medianofmedians(momarr, n);

    end = clock();

    double momtime = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Sort and Pick\n");
    printf("Median: %g\n", sortmedian);
    printf("Time: %.6f seconds\n\n", sorttime);

    printf("Lomuto\n");
    printf("Median: %g\n", lomutomed);
    printf("Time: %.6f seconds\n\n", lomutotime);

    printf("Median of Medians\n");
    printf("Median: %g\n", mommedian);
    printf("Time: %.6f seconds\n", momtime);

    free(arr);
    free(sortarr);
    free(lomutoarr);
    free(momarr);

    return 0;
}
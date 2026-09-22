#include <stdio.h>
#include <stdlib.h>

// to avoid math.h
int log2(int n)
{
    int result = 0;

    while(n > 1){
        n /= 2;
        result++;
    }

    return result;
}

// Heap Sort part


void swap(long long *a, long long *b)
{
    long long t = *a;
    *a = *b;
    *b = t;
}


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
    int depth = 2 * (int)log2(n);

    intro(arr, 0, n - 1, depth);
}

int main(void) {
    
    // using long long for simplicity, sacrificing a bit of efficency

    long long n;
    scanf("%lld", &n);

    long long *arr = malloc(n * sizeof(long long));

    for(int i = 0; i < n; i++){
        scanf("%lld", &arr[i]);
    }

    introsort(arr, n);

    for(int i = 0; i < n; i++){
        printf("%lld\n", arr[i]);
    }

    // ??
    free(arr);

    return 0;
}
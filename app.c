#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "raylib.h"

// avoid Windows / raylib name conflicts
#define Rectangle WinRectangle
#define CloseWindow WinCloseWindow
#define ShowCursor WinShowCursor
#define LoadImage WinLoadImage
#define DrawText WinDrawText
#define DrawTextEx WinDrawTextEx
#define PlaySound WinPlaySound

#include <windows.h>
#include <commdlg.h>

#undef Rectangle
#undef CloseWindow
#undef ShowCursor
#undef LoadImage
#undef DrawText
#undef DrawTextEx
#undef PlaySound

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

// DISCLAIMER. This part was actually worked alongside an AI, to debug the OS calls

int openfile(char *filename)
{
    OPENFILENAME ofn = {0};

    filename[0] = '\0';

    ofn.lStructSize = sizeof(ofn);

    ofn.lpstrFile = filename;

    ofn.nMaxFile = MAX_PATH;

    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";

    ofn.nFilterIndex = 1;

    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    if(!GetOpenFileName(&ofn)){
        return 0;
    }

    return 1;
}

int loadfile(char *filename, long long **arr)
{
    FILE *file = fopen(filename, "r");

    if(file == NULL){
        return 0;
    }

    int n = 0;

    long long value;

    while(fscanf(file, "%lld", &value) == 1){
        n++;
    }

    rewind(file);

    *arr = malloc(n * sizeof(long long));

    for(int i = 0; i < n; i++){
        fscanf(file, "%lld", &(*arr)[i]);
    }

    fclose(file);

    return n;
}

int main(void)
{
    InitWindow(1000, 600, "Median Calculator");

    SetTargetFPS(60);

    Color background = {245, 238, 255, 255};
    Color pink = {255, 190, 210, 255};
    Color blue = {190, 220, 255, 255};
    Color green = {195, 240, 210, 255};
    Color text = {65, 60, 75, 255};

    Rectangle button = {400, 130, 200, 55};

    char filename[MAX_PATH] = "";

    char shownfile[MAX_PATH] = "";

    int loaded = 0;

    double sortmedian = 0;
    double lomutomed = 0;
    double mommedian = 0;

    double sorttime = 0;
    double lomutotime = 0;
    double momtime = 0;

    while(!WindowShouldClose()){

        Vector2 mouse = GetMousePosition();

        if(CheckCollisionPointRec(mouse, button) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){

            if(openfile(filename)){

                long long *arr = NULL;

                int n = loadfile(filename, &arr);

                if(n > 0){

                    long long *sortarr = malloc(n * sizeof(long long));

                    long long *lomutoarr = malloc(n * sizeof(long long));

                    long long *momarr = malloc(n * sizeof(long long));

                    for(int i = 0; i < n; i++){
                        sortarr[i] = arr[i];
                        lomutoarr[i] = arr[i];
                        momarr[i] = arr[i];
                    }

                    clock_t start = clock();

                    sortmedian = sortandpick(sortarr, n);

                    clock_t end = clock();

                    sorttime = (double)(end - start) / CLOCKS_PER_SEC;

                    start = clock();

                    lomutomed = lomutomedian(lomutoarr, n);

                    end = clock();

                    lomutotime = (double)(end - start) / CLOCKS_PER_SEC;

                    start = clock();

                    mommedian = medianofmedians(momarr, n);

                    end = clock();

                    momtime = (double)(end - start) / CLOCKS_PER_SEC;

                    char *name = strrchr(filename, '\\');

                    if(name != NULL){
                        strcpy(shownfile, name + 1);
                    }

                    else{
                        strcpy(shownfile, filename);
                    }

                    loaded = 1;

                    free(arr);
                    free(sortarr);
                    free(lomutoarr);
                    free(momarr);
                }
            }
        }

        BeginDrawing();

        ClearBackground(background);

        DrawText(
            "Median Calculator",
            500 - MeasureText("Median Calculator", 36) / 2,
            50,
            36,
            text
        );

        DrawRectangleRounded(button, 0.2f, 10, pink);

        DrawText(
            "Open TXT",
            button.x + button.width / 2 - MeasureText("Open TXT", 22) / 2,
            button.y + 16,
            22,
            text
        );

        if(loaded){

            DrawText(
                shownfile,
                500 - MeasureText(shownfile, 18) / 2,
                205,
                18,
                text
            );

            DrawRectangleRounded((Rectangle){70, 280, 260, 200}, 0.08f, 10, pink);
            DrawRectangleRounded((Rectangle){370, 280, 260, 200}, 0.08f, 10, blue);
            DrawRectangleRounded((Rectangle){670, 280, 260, 200}, 0.08f, 10, green);

            DrawText(
                "Sort and Pick",
                200 - MeasureText("Sort and Pick", 22) / 2,
                310,
                22,
                text
            );

            DrawText(
                "Lomuto",
                500 - MeasureText("Lomuto", 22) / 2,
                310,
                22,
                text
            );

            DrawText(
                "Median of Medians",
                800 - MeasureText("Median of Medians", 22) / 2,
                310,
                22,
                text
            );

            char result[100];

            sprintf(result, "Median: %g", sortmedian);

            DrawText(
                result,
                200 - MeasureText(result, 18) / 2,
                375,
                18,
                text
            );

            sprintf(result, "Time: %.6f s", sorttime);

            DrawText(
                result,
                200 - MeasureText(result, 18) / 2,
                420,
                18,
                text
            );

            sprintf(result, "Median: %g", lomutomed);

            DrawText(
                result,
                500 - MeasureText(result, 18) / 2,
                375,
                18,
                text
            );

            sprintf(result, "Time: %.6f s", lomutotime);

            DrawText(
                result,
                500 - MeasureText(result, 18) / 2,
                420,
                18,
                text
            );

            sprintf(result, "Median: %g", mommedian);

            DrawText(
                result,
                800 - MeasureText(result, 18) / 2,
                375,
                18,
                text
            );

            sprintf(result, "Time: %.6f s", momtime);

            DrawText(
                result,
                800 - MeasureText(result, 18) / 2,
                420,
                18,
                text
            );
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <commdlg.h>

// DISCLAIMER. This part was actually worked alongside an AI, to debug the OS calls

int main(void)
{
    // Stores the complete path of the file selected by the user.
    char filename[MAX_PATH] = "";

    // Windows structure used to configure the File Explorer selection window.
    // {0} initializes all of its values to zero before configuration.
    OPENFILENAME ofn = {0};

    // Tell Windows the size of the structure being used.
    ofn.lStructSize = sizeof(ofn);

    // Give Windows the variable where the selected file path will be stored.
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;

    // Show .txt files by default, while still allowing all file types.
    ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";
    ofn.nFilterIndex = 1;

    // Only allow the user to select files and paths that actually exist.
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    // Open the Windows file selector.
    // If a file is selected, its complete path is stored in filename.
    // If the window is cancelled, the program ends.
    if(!GetOpenFileName(&ofn)){
        return 1;
    }

    // Open the selected file in read mode.
    FILE *file = fopen(filename, "r");

    // fopen returns NULL if the file could not be opened.
    if(file == NULL){
        return 1;
    }

    int n = 0;
    long long value;

    // First pass through the file:
    // Count how many numbers it contains to know the required array size.
    while(fscanf(file, "%lld", &value) == 1){
        n++;
    }

    // The previous loop leaves the file at the end.
    // Move back to the beginning before reading the actual values.
    rewind(file);

    // Allocate exactly enough memory to store every number in the file.
    long long *arr = malloc(n * sizeof(long long));

    // Second pass through the file:
    // Read each number and store it in the allocated array.
    for(int i = 0; i < n; i++){
        fscanf(file, "%lld", &arr[i]);
    }

    // The file is no longer needed after all values have been loaded.
    fclose(file);

    // Print every number stored in the array.
    for(int i = 0; i < n; i++){
        printf("%lld\n", arr[i]);
    }

    // Release the memory allocated for the array.
    free(arr);

    return 0;
}
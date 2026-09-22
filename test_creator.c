#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    int n;

    scanf("%d", &n);

    FILE *file = fopen("tests/test.txt", "w");

    srand(time(NULL));

    for(int i = 0; i < n; i++){
        long long number = ((long long)rand() << 32) | rand();
        fprintf(file, "%lld\n", number);
    }

    fclose(file);

    return 0;
}
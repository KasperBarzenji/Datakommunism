#include <stdio.h>

int main() {

    int arr[] = {1,2,3,4,5,6};


    for (int i = 0; i < 6; i++) {
        printf("%d ", arr[i]);
    }

    arr[3] = NULL;

    for (int i = 0; i < 6; i++) {
        printf("%d ", arr[i]);
    }

    return 0;   
}
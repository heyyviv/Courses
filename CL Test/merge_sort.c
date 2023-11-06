#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 100

int arr[MAX];
int temp[MAX];

// Structure to pass data to the thread function
struct ThreadData {
    int low;
    int high;
};

void merge(int low, int mid, int high) {
    int i = low, j = mid + 1, k = low;
    
    while (i <= mid && j <= high) {
        if (arr[i] < arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = arr[i++];
    }

    while (j <= high) {
        temp[k++] = arr[j++];
    }

    for (i = low; i <= high; i++) {
        arr[i] = temp[i];
    }
}

void* mergeSort(void* arg) {
    struct ThreadData *data = (struct ThreadData*)arg;
    int low = data->low;
    int high = data->high;

    if (low < high) {
        int mid = (low + high) / 2;

        // Create two threads to sort the left and right halves
        struct ThreadData leftData, rightData;
        leftData.low = low;
        leftData.high = mid;
        rightData.low = mid + 1;
        rightData.high = high;

        pthread_t leftThread, rightThread;
        pthread_create(&leftThread, NULL, mergeSort, &leftData);
        pthread_create(&rightThread, NULL, mergeSort, &rightData);

        // Wait for both threads to complete
        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);

        // Merge the sorted halves
        merge(low, mid, high);
    }

    pthread_exit(NULL);
}

int main() {
    int n;
    printf("Enter the number of elements: ");
    scanf("%d", &n);

    printf("Enter the elements: ");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    struct ThreadData data;
    data.low = 0;
    data.high = n - 1;

    pthread_t mainThread;
    pthread_create(&mainThread, NULL, mergeSort, &data);
    pthread_join(mainThread, NULL);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}

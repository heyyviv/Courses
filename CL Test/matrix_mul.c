#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 3 // Number of rows in the first matrix
#define M 3 // Number of columns in the first matrix (and rows in the second matrix)
#define P 3 // Number of columns in the second matrix

int A[N][M]; // First matrix
int B[M][P]; // Second matrix
int C[N][P]; // Result matrix

// Structure to pass row and column indices to the thread function
struct ThreadData {
    int row;
    int col;
};

// Function to multiply a specific row of A by a specific column of B
void *multiply(void *arg) {
    struct ThreadData *data = (struct ThreadData *)arg;
    int row = data->row;
    int col = data->col;

    C[row][col] = 0;
    for (int i = 0; i < M; i++) {
        C[row][col] += A[row][i] * B[i][col];
    }

    free(data); // Free memory allocated for the ThreadData structure
    pthread_exit(NULL);
}

int main() {
    // Read matrices A and B from files
    FILE *fileA = fopen("matrixA.txt", "r");
    FILE *fileB = fopen("matrixB.txt", "r");

    if (fileA == NULL || fileB == NULL) {
        perror("Error opening input files");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (fscanf(fileA, "%d", &A[i][j]) != 1) {
                perror("Error reading from matrixA.txt");
                return 1;
            }
        }
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            if (fscanf(fileB, "%d", &B[i][j]) != 1) {
                perror("Error reading from matrixB.txt");
                return 1;
            }
        }
    }

    fclose(fileA);
    fclose(fileB);

    pthread_t threads[N][P];

    // Create threads to perform matrix multiplication
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            struct ThreadData *data = (struct ThreadData *)malloc(sizeof(struct ThreadData));
            data->row = i;
            data->col = j;
            pthread_create(&threads[i][j], NULL, multiply, data);
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    // Print the result matrix C
    printf("Result matrix C:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    return 0;
}

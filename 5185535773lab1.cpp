#include <time.h>
#include <iostream.h>
#include <fstream.h>

using namespace std;

// ----------------
// Matrix functions
// ----------------

// Allocate memory and initialize arrays to construct an n x n matrix
int** newMatrix(int n) {
    int** M = new int*[n];
    for (int i=0; i<n; i++) {
        M[i] = new int[i];
        for (int j=0; j<n; j++) {
            M[i][j] = new int[j];
        }
    }
    return M;
}

// Free up previously allocated memory for next n x n matrix to process
void clearMatrix(int** M, int n) {
    if (M==nullptr) {
        return;
    }
    else {
        for (int i=0; i<n; i++) {
            delete[] M[i];
        }
    }
    delete[] M;
}

// Pad matrices A and B with zeros if they aren't already 2^n x 2^n
int** padMatrix(int** srcM, int n, int newSize) {
    int** M = newMatrix(newSize);
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            M[i][j] = srcM[i][j];
        }
    }
    return M;
}

// Truncate final matrix product C by removing zero-padded columns and rows
int** truncateMatrix(int** srcM, int n, int origSize) {
    int** M = newMatrix(origSize);
    for (int i=0; i<origSize; i++) {
        for (int j=0; j<origSize; j++) {
            M[i][j] = srcM[i][j];
        }
    }
    return M;
}

// Partition n x n matrix into four n/2 x n/2 submatrices of equal size
void partitionMatrix(int** M, int half, int** M11, int** M12, int** M21, int** M22) {
    for (int i=0; i<half; i++) {
        for (int j=0; j<half; j++) {
            M11[i][j] = M[i][j];
            M12[i][j] = M[i][j + half];
            M21[i][j] = M[i + half][j];
            M22[i][j] = M[i + half][j + half];
        }
    }
}

// Combine the four n/2 x n/2 submatrices to produce the final matrix C
int** combineMatrix(int half, int** C11, int** C12, int** C21, int** C22) {
    int** C = newMatrix(half*2)
    for (int i=0; i<half; i++) {
        for (int j=0; j<half; j++) {
            C[i][j] = C11[i][j];
            C[i][j + half] = C12[i][j];
            C[i + half][j] = C21[i][j];
            C[i + half][j + half] = C22[i][j];
        }
    }
    return C;
}

// ----------------------------
// Method 1: Divide and Conquer
// ----------------------------


int main() {
    

    return 0;
}
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

// Add n x n matrices
int** addMatrices(int** A, int** B, int n) {
    int** C = newMatrix(n);
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

// Subtract n x n matrices
int** subMatrices(int** A, int** B, int n) {
    int** C = newMatrix(n);
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}


// ----------------------------
// Method 1: Divide and Conquer
// ----------------------------
int** matrixMultDC(int** A, int** B, int n) {
    // Corner case: 1 x 1 matrix
    if (n == 1) {
        int** C = newMatrix(1);
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    bool padded = false;
    int origSize = n;
    // int** A_padded = A;
    // int** B_padded = B;

    // If n is odd, pad matrices with an extra row and column
    if (n % 2 != 0) {
        // int** A_padded = padMatrix(A, n, n);
        // int** B_padded = padMatrix(B, n, n);
        A = padMatrix(A, n, n+1);
        B = padMatrix(B, n, n+1);
        n += 1;
        padded = true;
    }

    // Partition matrices A and B into four equal submatrices
    int halfSize = n/2;
    int** A11 = newMatrix(halfSize);
    int** A12 = newMatrix(halfSize);
    int** A21 = newMatrix(halfSize);
    int** A22 = newMatrix(halfSize);
    partitionMatrix(A, halfSize, A11, A12, A21, A22);

    int** B11 = newMatrix(halfSize);
    int** B12 = newMatrix(halfSize);
    int** B21 = newMatrix(halfSize);
    int** B22 = newMatrix(halfSize);
    partitionMatrix(B, halfSize, B11, B12, B21, B22);

    // Compute submatrices of C
    int** A11B11 = matrixMultiplyDC(A11, B11, halfSize);
    int** A12B21 = matrixMultiplyDC(A12, B21, halfSize);
    int** A11B12 = matrixMultiplyDC(A11, B12, halfSize);
    int** A12B22 = matrixMultiplyDC(A12, B22, halfSize);
    int** A21B11 = matrixMultiplyDC(A21, B11, halfSize);
    int** A22B21 = matrixMultiplyDC(A22, B21, halfSize);
    int** A21B12 = matrixMultiplyDC(A21, B12, halfSize);
    int** A22B22 = matrixMultiplyDC(A22, B22, halfSize);
    
    int** C11 = addMatrices(A11B11, A12B21, halfSize);
    int** C12 = addMatrices(A11B12, A12B22, halfSize);
    int** C21 = addMatrices(A21B11, A22B21, halfSize);
    int** C22 = addMatrices(A21B12, A22B22, halfSize);

    // Combine submatrices into final result
    int** C = combineMatrix(halfSize, C11, C12, C21, C22);

    // Clear allocated memory of matrices used within the function
    if (padded) {
        clearMatrix(A, n);
        clearMatrix(B, n);
        int** truncC = truncateMatrix(C, n, origSize);
        clearMatrix(C, n);
        return truncC;
    }
    else {
        clearMatrix(A11, n); clearMatrix(A12, n); clearMatrix(A21, n); clearMatrix(A22, n);
        clearMatrix(B11, n); clearMatrix(B12, n); clearMatrix(B21, n); clearMatrix(B22, n);
        clearMatrix(C11, n); clearMatrix(C12, n); clearMatrix(C21, n); clearMatrix(C22, n);
        clearMatrix(A11B11, halfSize); clearMatrix(A12B21, halfSize);
        clearMatrix(A11B12, halfSize); clearMatrix(A12B22, halfSize);
        clearMatrix(A21B11, halfSize); clearMatrix(A22B21, halfSize);
        clearMatrix(A21B12, halfSize); clearMatrix(A22B22, halfSize);
    }

    return C;
}


// ---------------------------
// Method 2: Strassen's Method
// ---------------------------
int** matrixMultStrassen(int** A, int** B, int n) {
    // Corner case: 1 x 1 matrix
    if (n == 1) {
        int** C = newMatrix(1);
        C[0][0] = A[0][0] * B[0][0];
        return C;
    }

    bool padded = false;
    int origSize = n;
    // int** A_padded = A;
    // int** B_padded = B;

    // If n is odd, pad matrices with an extra row and column
    if (n % 2 != 0) {
        // int** A_padded = padMatrix(A, n, n);
        // int** B_padded = padMatrix(B, n, n);
        A = padMatrix(A, n, n+1);
        B = padMatrix(B, n, n+1);
        n += 1;
        padded = true;
    }

    // Partition matrices A and B into four equal submatrices
    int halfSize = n/2;
    int** A11 = newMatrix(halfSize);
    int** A12 = newMatrix(halfSize);
    int** A21 = newMatrix(halfSize);
    int** A22 = newMatrix(halfSize);
    partitionMatrix(A, halfSize, A11, A12, A21, A22);

    int** B11 = newMatrix(halfSize);
    int** B12 = newMatrix(halfSize);
    int** B21 = newMatrix(halfSize);
    int** B22 = newMatrix(halfSize);
    partitionMatrix(B, halfSize, B11, B12, B21, B22);

    // M1 = (A11 + A22)(B11 + B22)
    int** term1 = addMatrices(A11, A22, halfSize);
    int** term2 = addMatrices(B11, B22, halfSize);
    int** M1 = matrixMultStrassen(term1, term2, halfSize);
    clearMatrix(term1, halfSize);
    clearMatrix(term2, halfSize);

    // M2 = (A21 + A22)B11
    term1 = addMatrices(A21, A22, halfSize);
    int** M2 = matrixMultStrassen(term1, B11, halfSize);
    clearMatrix(term1, halfSize);

    // M3 = A11(B12 - B22)
    term2 = subMatrices(B12, B22, halfSize);
    int** M3 = matrixMultStrassen(A11, term2, halfSize);
    clearMatrix(term2, halfSize);

    // M4 = A22(B21 - B11)
    term2 = subMatrices(B21, B11, halfSize);
    int** M4 = matrixMultStrassen(A22, term2, halfSize);
    clearMatrix(term2, halfSize);

    // M5 = (A11 + A12)B22
    term1 = addMatrices(A11, A12, halfSize);
    int** M5 = matrixMultStrassen(term1, B22, halfSize);
    clearMatrix(term1, halfSize);

    // M6 = (A21 - A11)(B11 + B12)
    term1 = subMatrices(A21, A11, halfSize);
    term2 = addMatrices(B11, B12, halfSize);
    int** M6 = matrixMultStrassen(term1, term2, halfSize);
    clearMatrix(term1, halfSize);
    clearMatrix(term2, halfSize);

    // M7 = (A12 - A22)(B21 + B22)
    term1 = subMatrices(A12, A22, halfSize);
    term2 = addMatrices(B21, B22, halfSize);
    int** M7 = matrixMultStrassen(term1, term2, halfSize);
    clearMatrix(term1, halfSize);
    clearMatrix(term2, halfSize);

    // C11 = M1 + M4 - M5 + M7
    term1 = addMatrices(M1, M4, halfSize);
    term2 = addMatrices(M5, M7, halfSize);
    int** C11 = subMatrices(term1, term2, halfSize);
    clearMatrix(term1, halfSize);
    clearMatrix(term2, halfSize);

    // C12 = M3 + M5
    int** C12 = addMatrices(M3, M5, halfSize);

    // C21 = M2 + M4
    int** C21 = addMatrices(M2, M4, halfSize);

    // C22 = M1 - M2 + M3 + M6
    term1 = subMatrices(M1, M2, halfSize);
    term2 = addMatrices(M3, M6, halfSize);
    int** C22 = addMatrices(term1, term2, halfSize);
    clearMatrix(term1, halfSize);
    clearMatrix(term2, halfSize);

    // Combine submatrices into final result
    int** C = combineMatrix(halfSize, C11, C12, C21, C22);

    // Clear allocated memory of matrices used within the function
    if (padded) {
        clearMatrix(A, n);
        clearMatrix(B, n);
        int** truncC = truncateMatrix(C, n, origSize);
        clearMatrix(C, n);
        return truncC;
    }
    else {
        clearMatrix(A11, n); clearMatrix(A12, n); clearMatrix(A21, n); clearMatrix(A22, n);
        clearMatrix(B11, n); clearMatrix(B12, n); clearMatrix(B21, n); clearMatrix(B22, n);
        clearMatrix(C11, n); clearMatrix(C12, n); clearMatrix(C21, n); clearMatrix(C22, n);
    }

    return C;
} 


int main() {
    

    return 0;
}
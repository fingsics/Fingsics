#include "../include/Helpers.h"

string getObjectPairId(pair<Object*, Object*> objectPair) {
    string firstKey = objectPair.first->getId() < objectPair.second->getId() ? objectPair.first->getId() : objectPair.second->getId();
    string secondKey = objectPair.first->getId() < objectPair.second->getId() ? objectPair.second->getId() : objectPair.first->getId();
    return firstKey + "-" + secondKey;
}

double** prepareForEscalerization(Matrix LHS, Point RHS) {
        double* row0 = new double[4];
        double* row1 = new double[4];
        double* row2 = new double[4];
        double** ret = new double* [3];
        row0[0] = LHS[0][0];
        row0[1] = LHS[0][1];
        row0[2] = LHS[0][2];
        row0[3] = RHS[0];
        row1[0] = LHS[1][0];
        row1[1] = LHS[1][1];
        row1[2] = LHS[1][2];
        row1[3] = RHS[1];
        row2[0] = LHS[2][0];
        row2[1] = LHS[2][1];
        row2[2] = LHS[2][2];
        row2[3] = RHS[2];
        ret[0] = row0;
        ret[1] = row1;
        ret[2] = row2;
        return ret;
}

// Works only if the system has a SINGLE solution
double* solveLinearSystem(Matrix LHS, Point RHS) {
    double** A = prepareForEscalerization(LHS, RHS);
    int h = 0;
    int k = 0;
    int m = 3;
    int n = 4;

    // Gaussian elimination
    while (h < m && k < n) {
        // Find the k-th pivot
        int i_max = h;
        int val_max = abs(A[h][k]);
        for (int g = h + 1; g < m; g++) {
            if (abs(A[g][k]) > val_max) {
                i_max = g;
                val_max = abs(A[g][k]);
            }
        }
        if (A[i_max][k] == 0) {
            k++; // No pivot in this column, pass to next column
        } else {
            // Swap rows
            double* temp = A[h];
            A[h] = A[i_max];
            A[i_max] = temp;
            // Do for all rows below pivot
            for (int i = h + 1; i < m; i++) {
                double f = A[i][k] / A[h][k];
                A[i][k] = 0; // Fill with zeros the lower part of pivot column
                for (int j = k + 1; j < n; j++) A[i][j] -= A[h][j] * f; // Do for all remaining elements in current row
            }
            h++; k++;
        }
    }

    // Back substitution
    for (int i = m - 1; i >= 0; i--) {
        for (int j = n - 2; j > i; j--) {
            A[i][n - 1] -= A[i][j] * A[j][n - 1];
        }
        A[i][n - 1] /= A[i][i];
    }

    double* ret = new double[3];
    ret[0] = A[0][3];
    ret[1] = A[1][3];
    ret[2] = A[2][3];
    return ret;
}
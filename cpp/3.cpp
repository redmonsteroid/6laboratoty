#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;
void LU(vector<vector<float>> LEFT, vector<float> RIGHT) {

    vector<vector<float>> L(4, vector<float>(4, 0));
    vector<vector<float>> U(4, vector<float>(4, 0));


    for (int i = 0; i < 4; i++) {
        for (int j = i; j < 4; j++) {
            U[i][j] = LEFT[i][j];
            for (int k = 0; k < i; k++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }

        for (int j = i + 1; j < 4; j++) {
            L[j][i] = LEFT[j][i];
            for (int k = 0; k < i; k++) {
                L[j][i] -= L[j][k] * U[k][i];
            }
            L[j][i] /= U[i][i];
        }
    }

    cout << "\nL - матрица ====================================\n";
    for (auto i : L) {
        for (auto j : i) {
            cout << j << '\t';
        }
        cout << endl;
    }

    cout << "\nU - матрица ====================================\n";
    for (auto i : U) {
        for (auto j : i) {
            cout << j << '\t';
        }
        cout << endl;
    }
    vector<float> y(4, 0);
    for (int i = 0; i < 4; i++) {
        y[i] = RIGHT[i];
        for (int j = 0; j < i; j++) {
            y[i] -= L[i][j] * y[j];
        }
    }

    vector<float> x(4, 0);
    for (int i = 3; i >= 0; i--) {
        x[i] = y[i];
        for (int j = i + 1; j < 4; j++) {
            x[i] -= U[i][j] * x[j];
        }
        x[i] /= U[i][i];
    }

    cout << endl;
    int count = 1;
    for (auto i : x) {
        cout << "x" << count << " = " << i << endl;
        count++;
    }
}
void gaussSeidel(vector<vector<float>>& A, vector<float>& b, vector<float>& x) {
    const int maxIterations = 99;
    const float epsilon = 0.0001;
    int n = A.size();

    for (int k = 0; k < maxIterations; k++) {
        vector<float> x_old = x;

        for (int i = 0; i < n; i++) {
            float sum = 0;
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    sum += A[i][j] * x[j];
                }
            }
            x[i] = (b[i] - sum) / A[i][i];
        }

        cout << "Iteration " << k + 1 << ": ";
        for (float xi : x) {
            cout << xi << " ";
        }
        cout << endl;

        bool convergence = false;
        for (int i = 0; i < n; ++i) {
            if (fabs(x[i] - x_old[i]) < epsilon) {
                convergence = true;
                break;
            }
        }

        if (convergence) {
            cout << "Сошлось!\n";
            return;
        }
    }
    cout << "Не сошлось!\n";
}

int main() {
    system("chcp 65001");
    vector<vector<float>> LEFT = {
        {1.08, -0.04, 0.21, -18},
        {0.25, -1.23, 0.22, -0.09},
        {-0.21, 0.22, 0.8, -0.13},
        {0.15, -1.31, 0.06, -1.16}
    };
    vector<float> RIGHT = { -1.24, -1.16, 2.56, 1.08 };

    vector<float> f;
    for (int i = 0; i < 4; i++) {
        f.push_back(RIGHT[i] / LEFT[i][0]);
    }

    vector<vector<float>> C(4, vector<float>(4, 0));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i != j) {
                C[i][j] = -(LEFT[i][j] / LEFT[i][i]); // Заполняем матрицу C
            }
        }
    }
    cout << "МАТРИЦА C-------------------------\n";
    for (auto i : C) {
        for (auto j : i) {
            cout << setprecision(3) <<  j << "\t\t";
        }
        cout << endl;
    }
    cout << "\nМАТРИЦА F-------------------------\n";
    for (auto i : f) {
        cout << i << endl;
    }
    vector<float> x_k = { 0, 0, 0, 0 };
    gaussSeidel(LEFT, RIGHT, x_k);
    LU(LEFT, RIGHT);
}

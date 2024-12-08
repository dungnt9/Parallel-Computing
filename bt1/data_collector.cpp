#include <iostream>
#include <omp.h>
#include <vector>
#include <fstream>
#include <windows.h>

using namespace std;

// Hàm tạo ma trận ngẫu nhiên
void randomMatrix(vector<vector<double>>& matrix, int n) {
    matrix.resize(n, vector<double>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 100;
        }
    }
}

// Hàm tạo vector ngẫu nhiên
void randomVector(vector<double>& vec, int n) {
    vec.resize(n);
    for (int i = 0; i < n; i++) {
        vec[i] = rand() % 100;
    }
}

// Hàm đo thời gian chạy tuần tự
double measureSequential(const vector<vector<double>>& matrix, const vector<double>& vector_1, int n) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    vector<double> result(n, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector_1[j];
        }
    }

    QueryPerformanceCounter(&end);
    return static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
}

// Hàm đo thời gian chạy song song
double measureParallel(const vector<vector<double>>& matrix, const vector<double>& vector_1, int n) {
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    vector<double> result(n, 0);
#pragma omp parallel for num_threads(4)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector_1[j];
        }
    }

    QueryPerformanceCounter(&end);
    return static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
}

int main() {
    ofstream file("runtime_data.csv");
    file << "MatrixSize,Sequential,Parallel\n";

    for (int n = 1000; n <= 10000; n += 1000) {
        vector<vector<double>> matrix;
        vector<double> vector_1;

        randomMatrix(matrix, n);
        randomVector(vector_1, n);

        double sequentialTime = measureSequential(matrix, vector_1, n);
        double parallelTime = measureParallel(matrix, vector_1, n);

        file << n << "," << sequentialTime << "," << parallelTime << "\n";
        cout << "Matrix size: " << n << ", Sequential: " << sequentialTime
            << "s, Parallel: " << parallelTime << "s\n";
    }

    file.close();
    cout << "Data written to runtime_data.csv\n";
    return 0;
}

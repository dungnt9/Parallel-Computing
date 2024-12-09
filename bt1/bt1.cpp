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
            matrix[i][j] = rand() % 100; // Giới hạn giá trị trong khoảng [0, 99]
        }
    }
}

// Hàm tạo vector ngẫu nhiên
void randomVector(vector<double>& vec, int n) {
    vec.resize(n);
    for (int i = 0; i < n; i++) {
        vec[i] = rand() % 100; // Giới hạn giá trị trong khoảng [0, 99]
    }
}

// Hàm in ma trận ra console
void printMatrix(const vector<vector<double>>& matrix, int n) {
    cout << "Matrix (" << n << "x" << n << "):" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

// Hàm in vector ra console
void printVector(const vector<double>& vec, int n) {
    cout << "Vector (" << n << "): ";
    for (int i = 0; i < n; i++) {
        cout << vec[i] << " ";
    }
    cout << endl;
}

// Hàm nhân ma trận với vector song song
vector<double> multiplyParallel(const vector<vector<double>>& matrix, const vector<double>& vector_1, int n) {
    vector<double> result(n);
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

#pragma omp parallel for num_threads(8) // Điều chỉnh số luồng
    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector_1[j];
        }
    }

    QueryPerformanceCounter(&end);
    double elapsed_time = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    cout << "Parallel runtime: " << elapsed_time << " seconds" << endl;
    return result;
}

// Hàm nhân ma trận với vector tuần tự
vector<double> multiply(const vector<vector<double>>& matrix, const vector<double>& vector_1, int n) {
    vector<double> result(n);
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    for (int i = 0; i < n; i++) {
        result[i] = 0;
        for (int j = 0; j < n; j++) {
            result[i] += matrix[i][j] * vector_1[j];
        }
    }

    QueryPerformanceCounter(&end);
    double elapsed_time = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    cout << "Sequential runtime: " << elapsed_time << " seconds" << endl;
    cout << endl;
    return result;
}

int main() {
    vector<vector<double>> matrix;
    vector<double> vector_1;
    int n;

    cout << "Enter n: ";
    cin >> n;

    randomMatrix(matrix, n);
    randomVector(vector_1, n);

    // Hiển thị ma trận và vector
    printMatrix(matrix, n);
    printVector(vector_1, n);

    // Tính toán kết quả
    cout << endl;
    vector<double> result1 = multiplyParallel(matrix, vector_1, n);
    vector<double> result2 = multiply(matrix, vector_1, n);

    // Hiển thị kết quả
    cout << endl;
    cout << "Result (Parallel): ";
    printVector(result1, n);

    cout << "Result (Sequential): ";
    printVector(result2, n);

    return 0;
}

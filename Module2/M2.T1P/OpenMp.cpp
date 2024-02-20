// Student Name: Gaganveer Singh
// Student ID: 2210994783
// SIT315 - Module 2 - Task 1
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <omp.h> // Include OpenMP header

using namespace std;
using namespace std::chrono;

const int N = 300;

int main()
{
    cout << "Filling Matrixes with random values of size: " << N << endl;

    int A[N][N]; // matrix A
    int B[N][N]; // matrix B
    int C[N][N]; // matrix C

    srand(time(0)); 

    // filling all the 2d arrays with random values between 1 to 100 
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            A[i][j] = rand() % 100;
            B[i][j] = rand() % 100;
        }
    }

    auto start = high_resolution_clock::now(); // starting time for before multiplication

    // Multiply matrix A and B and store the result in matrix C using OpenMP parallelization
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto end = high_resolution_clock::now(); // stopping time after multiplication
    
    // calculating duration of multiplication
    auto duration = duration_cast<microseconds>(end - start);
    cout << "Time taken for multiplication: " << duration.count()/1E6 << " seconds" << endl;

    return 0;
}

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <omp.h>
using namespace std;
using namespace std::chrono;

void randomVector(int vector[], int size)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = rand() % 100;
    }
}

int main()
{
    unsigned long size = 100000000;
    srand(time(0));
    int *v1, *v2, *v3;
    v1 = (int *)malloc(size * sizeof(int));
    v2 = (int *)malloc(size * sizeof(int));
    v3 = (int *)malloc(size * sizeof(int));
    randomVector(v1, size);
    randomVector(v2, size);

    auto start = high_resolution_clock::now();

#pragma omp parallel default(none) shared(v1, v2) private(v3, size)
    {
#pragma omp for
        for (int i = 0; i < size; i++)
        {
            v3[i] = v1[i] + v2[i];
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Size used: " << size << endl;
    cout << "Time taken by function in microseconds :" << duration.count() << " microseconds" << endl;
    cout << "Time taken by function in seconds :" << duration.count() / 1E6 << " seconds" << endl;

    return 0;
}
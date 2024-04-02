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

    int total = 0; // Shared variable to hold the total sum

#pragma omp parallel default(none) shared(v1, v2, v3, size) private(total)
    {
        int partial_sum = 0; // Private variable to hold partial sum for each thread

#pragma omp for
        for (int i = 0; i < size; i++)
        {
            v3[i] = v1[i] + v2[i];
            partial_sum += v3[i]; // Compute partial sum for each thread
        }

// Critical section to update the total sum
#pragma omp critical
        {
            total += partial_sum;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    
    cout << "Size used: " << size << endl;
    cout << "Time taken by function in microseconds :" << duration.count() << " microseconds" << endl;
    cout << "Time taken by function in seconds :" << duration.count() / 1E6 << " seconds" << endl;
    cout << "Total sum of all elements in v3: " << total << endl;

    return 0;
}

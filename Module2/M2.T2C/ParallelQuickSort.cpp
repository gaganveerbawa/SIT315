// Student Name: Gaganveer Singh
// Student Roll No: 2210994783

#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int ARRAY_SIZE = 1000000;

void randomArrayFill(int array[], int ARRAY_SIZE)
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = rand() % 100;
    }
}

// function for placing smaller elments before pivot and higher elements after pivot
int partition(int array[], int start, int end, int ARRAY_SIZE)
{
    int pivot = array[end]; // considering pivot as last element
    int partitionindex = start;

    for (int i = start; i < end; i++)
    {
        if (array[i] < pivot) // swapping if current element smaller than pivot
        {
            swap(array[i], array[partitionindex]);
            partitionindex++; // incrementing the index to another place
        }
    }
    swap(array[end], array[partitionindex]); // swapping pivot to the last element left such that larger and smaller are or different sides
    return partitionindex;
}

// function for recursion
void quick_sort(int array[], int start, int end, int ARRAY_SIZE)
{
    if (start < end)
    {
        int pidx = partition(array, start, end, ARRAY_SIZE); // index of pivot after partition
        quick_sort(array, start, pidx - 1, ARRAY_SIZE);      // recursion on smaller elements than pivot
        quick_sort(array, pidx + 1, end, ARRAY_SIZE);        // recursion on maxIndex elements than pivot
    }
}

int main()
{
    int *array = new int[ARRAY_SIZE];

    cout << "Size of array used: " << ARRAY_SIZE << endl;

    srand(time(0));
    randomArrayFill(array, ARRAY_SIZE);

    auto start = std::chrono::high_resolution_clock::now(); // starting time for before multiplication

    quick_sort(array, 0, ARRAY_SIZE - 1, ARRAY_SIZE);

    auto end = std::chrono::high_resolution_clock::now(); // stoping time after multiplication

    // calculating duration of multiplication
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout << "Time taken for multiplication in ms: " << duration.count() << " microseconds" << endl;
    cout << "Time taken for sorting in Seconds: " << duration.count() /1E6 << " seconds" << endl;
    return 0;
}

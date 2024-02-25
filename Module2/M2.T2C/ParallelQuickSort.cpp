// Student Name: Gaganveer Singh
// Student Roll No: 2210994783

#include <iostream>
#include <cstdlib>
#include <chrono>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

const int ARRAY_SIZE = 1000000;
const int NUM_THREADS = 2;

struct ThreadData {
    int* array;
    int start;
    int end;
};

void randomArrayFill(int array[], int ARRAY_SIZE)
{
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = rand() % 100;
    }
}

int partition(int array[], int start, int end)
{
    int pivot = array[end];
    int partitionindex = start;
    for (int i = start; i < end; i++)
    {
        if (array[i] < pivot)
        {
            swap(array[i], array[partitionindex]);
            partitionindex++;
        }
    }
    swap(array[end], array[partitionindex]);
    return partitionindex;
}

void* quick_sort_thread(void* arg)
{
    ThreadData* data = (ThreadData*)arg;
    int* array = data->array;
    int start = data->start;
    int end = data->end;
    if (start < end)
    {
        int pidx = partition(array, start, end);
        quick_sort_thread(new ThreadData{array, start, pidx - 1});
        quick_sort_thread(new ThreadData{array, pidx + 1, end});
    }
    return NULL;
}

void quick_sort(int array[], int start, int end)
{
    pthread_t threads[NUM_THREADS];
    ThreadData* data = new ThreadData[NUM_THREADS];
    int chunk_ARRAY_SIZE = (end - start + 1) / NUM_THREADS;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        data[i] = {array, start + i * chunk_ARRAY_SIZE, (i == NUM_THREADS - 1) ? end : (start + (i + 1) * chunk_ARRAY_SIZE - 1)};
        pthread_create(&threads[i], NULL, quick_sort_thread, &data[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    delete[] data;
}

int main()
{
    int* array = new int[ARRAY_SIZE];

    cout << "Size of array used: " << ARRAY_SIZE << endl;
    cout << "Threads used: " << NUM_THREADS << endl;

    srand(time(0));
    randomArrayFill(array, ARRAY_SIZE);

    auto start = high_resolution_clock::now();

    quick_sort(array, 0, ARRAY_SIZE - 1);

    auto end = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(end - start);
    cout << "Time taken for sorting in ms: " << duration.count() << " microseconds" << endl;
    cout << "Time taken for sorting in Seconds: " << duration.count() /1E6 << " seconds" << endl;

    delete[] array;

    return 0;
}

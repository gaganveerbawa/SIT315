#include <chrono>
#include <cstdlib>
#include <iostream>
#include <pthread.h>

using namespace std::chrono;
using namespace std;

#define MAX_THREAD 4

struct ThreadData {
    int *v1;
    int *v2;
    int *v3;
    int size;
    int threadId;
};

void randomVector(int vector[], int size)
{
    for (int i = 0; i < size; i++)
    {
        vector[i] = rand() % 100;
    }
}

void* addVector(void* arg)
{
    ThreadData* data = (ThreadData*)arg;
    int start = (data->threadId * data->size) / MAX_THREAD;
    int end = ((data->threadId + 1) * data->size) / MAX_THREAD;

    for (int i = start; i < end; i++)
    {
        data->v3[i] = data->v1[i] + data->v2[i];
    }

    pthread_exit(NULL);
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

    pthread_t threads[MAX_THREAD];
    ThreadData threadData[MAX_THREAD];

    auto start = high_resolution_clock::now();

    for (int i = 0; i < MAX_THREAD; i++) 
    {
        threadData[i].v1 = v1;
        threadData[i].v2 = v2;
        threadData[i].v3 = v3;
        threadData[i].size = size;
        threadData[i].threadId = i;

        pthread_create(&threads[i], NULL, addVector, (void*)&threadData[i]);
    }

    for (int i = 0; i < MAX_THREAD; i++) 
    {
        pthread_join(threads[i], NULL);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function :" << duration.count() << " microseconds" << endl;

    return 0;
}

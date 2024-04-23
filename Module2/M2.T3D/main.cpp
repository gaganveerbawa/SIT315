#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <chrono>

using namespace std;
mutex dataMutex;

int numProducerThreads = 4;
int numConsumerThreads = 4;
int topN = 3;

int producerCount = 0;
int consumerCount = 0;
int totalRows = 0;

condition_variable producerCV, consumerCV;

vector<int> indexVector;
vector<int> trafficLightVector;
vector<int> carCountVector;
vector<string> timeStampVector;

struct TrafficData
{
    int index;
    string timeStamp;
    int trafficLightID;
    int numberOfCars;
};

TrafficData trafficArray[5] = {{0, "", 1, 0}, {0, "", 2, 0}, {0, "", 3, 0}, {0, "", 4, 0}, {0, "", 5, 0}};

queue<TrafficData> trafficDataQueue;
bool compare(TrafficData first, TrafficData second)
{
    return (first.numberOfCars > second.numberOfCars);
}

void produceData()
{
    while (producerCount < totalRows)
    {
        unique_lock<mutex> lock(dataMutex);

        if (producerCount < totalRows)
        {
            trafficDataQueue.push(TrafficData{indexVector[producerCount], timeStampVector[producerCount], trafficLightVector[producerCount], carCountVector[producerCount]}); // Push into queue
            consumerCV.notify_all();
            producerCount++;
        }
        else
        {
            producerCV.wait(lock, [] { return producerCount < totalRows; }); // If count is greater than the number of rows in the dataset, wait
        }

        lock.unlock();
        this_thread::sleep_for(chrono::seconds(rand() % 3));
    }
}
TrafficData signal;
void consumeData()
{
    while (consumerCount < totalRows)
    {
        unique_lock<mutex> lock(dataMutex); // Lock until processing

        if (!trafficDataQueue.empty())
        {
            signal = trafficDataQueue.front();

            if (signal.trafficLightID == 1)
                trafficArray[0].numberOfCars += signal.numberOfCars;
            if (signal.trafficLightID == 2)
                trafficArray[1].numberOfCars += signal.numberOfCars;
            if (signal.trafficLightID == 3)
                trafficArray[2].numberOfCars += signal.numberOfCars;
            if (signal.trafficLightID == 4)
                trafficArray[3].numberOfCars += signal.numberOfCars;
            if (signal.trafficLightID == 5)
                trafficArray[4].numberOfCars += signal.numberOfCars;

            trafficDataQueue.pop();
            producerCV.notify_all();
            consumerCount++;
        }
        else
        {
            consumerCV.wait(lock, []
                            { return !trafficDataQueue.empty(); });
        }

        if (!trafficDataQueue.empty() && consumerCount % 10 == 0)
        {
            sort(trafficArray, trafficArray + 5, compare);
            cout << endl;
            cout << "Hour: " << signal.timeStamp << endl;
            cout << "==============================" << endl;
            for (int i = 0; i < topN; ++i)
            {
                cout << "Traffic Light ID: " << trafficArray[i].trafficLightID << ", Cars Passed: " << trafficArray[i].numberOfCars << endl;
            }
            cout << "==============================" << endl
                 << endl;
        }

        lock.unlock();
        this_thread::sleep_for(chrono::seconds(rand() % 3));
    }
}

int main()
{
    auto start = chrono::steady_clock::now(); // Record the start time

    ifstream file;

    file.open("data.txt");

    if (file.is_open())
    {
        string line;
        getline(file, line);

        while (!file.eof())
        {
            string index, timeStamp, trafficLightID, numberOfCars;
            getline(file, index, ',');
            indexVector.push_back(stoi(index));

            getline(file, timeStamp, ',');
            timeStampVector.push_back(timeStamp);

            getline(file, trafficLightID, ',');
            trafficLightVector.push_back(stoi(trafficLightID));

            getline(file, numberOfCars, '\n');
            carCountVector.push_back(stoi(numberOfCars));

            totalRows += 1;
        }
        file.close();
    }
    else
    {
        cout << "Could not open file, try again.";
    }

    thread producers[numProducerThreads];
    thread consumers[numConsumerThreads];

    for (int i = 0; i < numProducerThreads; i++)
        producers[i] = thread(produceData);
    for (int i = 0; i < numConsumerThreads; i++)
        consumers[i] = thread(consumeData);

    for (int i = 0; i < numProducerThreads; i++)
        producers[i].join();
    for (int i = 0; i < numConsumerThreads; i++)
        consumers[i].join();
    auto end = chrono::steady_clock::now(); // Record the end time

    // Calculate the duration and print it
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl;

    return 0;
}

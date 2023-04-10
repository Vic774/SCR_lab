/* Do not change this lines */
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
#include <chrono>
#include <mutex>
#include <stdint.h>
#include <thread>
#include <iomanip>

constexpr int64_t N = 4*1024;
int64_t matrix[N][N] = {};

const int NUMBER_OF_THREADS = 4;


/* Do not change this function */
void fillMatrixWithDummyData()
{
    // uzupełnienie macierzy danymi testowymi
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            matrix[i][j] = i * N + j;
        }
    }
}

/* Do not change this function */
int64_t calculateMatrixSumSingleThread(){
    int64_t sum = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int64_t element = matrix[i][j];
            sum += pow(element, 0.25);
        }
    }
    return sum;
}

/* Implement this function */
int64_t calculateMatrixSumMultiThread(){
    int64_t sum = 0;
    // Your code...

    std::mutex mtx;
    std::vector<std::thread> collection;

    auto partialSumCalculations = [&mtx, &sum](int part_number)
    {
        int idx_begin = part_number * N/NUMBER_OF_THREADS;
        int idx_end = (part_number + 1) * N/NUMBER_OF_THREADS;

        int64_t tempSum = 0;

        for (int i = idx_begin; i < idx_end; ++i)
        {
            for (int j = 0; j < N; ++j)
            {
                int64_t element = matrix[i][j];
                tempSum += pow(element, 0.25);
            }
        }

        mtx.lock();
        sum += tempSum;
        mtx.unlock();
    };

    for (int i = 0; i < NUMBER_OF_THREADS; ++i)
    {
        collection.emplace_back(std::thread(partialSumCalculations, i));
    }

    for (auto& watek : collection)
    {
        watek.join();
    }


    return sum;
}

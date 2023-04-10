#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>
#include <set>
#include <memory>
#include <atomic>

using namespace std;

void foo() {
    for (int i = 0; i < 5; i++) {
        std::cout << "hello foo" << std::endl;
    }
}

void bar(int x, float y) {
    for (int i = 0; i < 5; i++) {
        std::cout << "hello bar " << x << " " << y << std::endl;
    }
}

void thread_id()
{
    cout << this_thread::get_id() << endl;
}

bool isPrime(uint64_t n)
{
    if (n <= 1)
        return false;

    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

void find_prime_from(uint64_t x, set<uint64_t>& results, mutex& mtx)
{
    x = x-1;
    while (x > 1)
    {
        if(isPrime(x))
        {
            mtx.lock();
            results.insert(x);
            mtx.unlock();

            return;
        }
        x = x-1;
    }

}
void task1()
{
    vector<thread> collection;
    int threads_number = 100;

    collection.reserve(threads_number);

    for (int i = 0; i < threads_number; i++)
    {
        collection.emplace_back(thread_id);
    }

    for (int i = 0; i < threads_number; i++)
    {
        collection[i].join();
    }
}

void task2()
{
    vector<uint64_t> value_series;
    set<uint64_t> prime_numbers;
    vector<thread> collection;
    int threads_number = 6;

    auto mtx = make_shared<mutex>();


    for (int i = 2; i < 63; i++)
    {
        value_series.push_back(pow(2,i));
    }

    while (!value_series.empty())
    {
        for (int i = 0; i < threads_number; i++)
        {
            collection.emplace_back(find_prime_from, value_series.front(), ref(prime_numbers), ref(*mtx));
            value_series.erase(value_series.begin());

            if(value_series.empty())
            {
                break;
            }
        }

        for (auto & i : collection)
        {
            i.join();
        }

        collection.clear();
    }

    // Print the results
    std::cout << "Wyniki: ";
    for (uint64_t result : prime_numbers) {
        std::cout << result << " ";
    }
    std::cout << std::endl;
}

void task3()
{
    int threads_number = 1000;
    int a = 0;

    vector<thread> collection;

    collection.reserve(threads_number);

    for (int i = 0; i < threads_number; i++)
    {
        collection.emplace_back([&a]()
        {
            this_thread::sleep_for(chrono::milliseconds(10));
            a += 100;
        });
    }

    for (int i = 0; i < threads_number; i++)
    {
        collection[i].join();
    }

    cout<< a << endl;

}

void task4()
{
    int threads_number = 1000;
    int a = 0;
    auto mtx = make_shared<mutex>();
    vector<thread> collection;

    collection.reserve(threads_number);

    for (int i = 0; i < threads_number; i++)
    {
        collection.emplace_back([&a, &mtx]()
                                {
                                    this_thread::sleep_for(chrono::milliseconds(10));
                                    mtx->lock();
                                    a += 100;
                                    mtx->unlock();

                                });
    }

    for (int i = 0; i < threads_number; i++)
    {
        collection[i].join();
    }

    cout<< a << endl;

}

void task5()
{
    int threads_number = 1000;
    atomic<int> a = 0;

    vector<thread> collection;

    collection.reserve(threads_number);

    for (int i = 0; i < threads_number; i++)
    {
        collection.emplace_back([&a]()
                                {
                                    this_thread::sleep_for(chrono::milliseconds(10));
                                    a += 100;
                                });
    }

    for (int i = 0; i < threads_number; i++)
    {
        collection[i].join();
    }

    cout<< a << endl;

}

int main(int argc, const char * argv[]) {

// checks for available threads
//    cout << thread::hardware_concurrency() << endl;

    task5();

    return 0;
}

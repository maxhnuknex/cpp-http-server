#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>

using task = std::function<void ()>;

class ThreadPool{
    std::mutex mtx;
    std::vector<std::thread> workers;
    std::condition_variable cv;
    std::queue<std::function<void()>> tasks;
    bool stop;

    void workerLoop();
public:
    ThreadPool(int count);
    ~ThreadPool();

    void addTask(task handleClient);
};
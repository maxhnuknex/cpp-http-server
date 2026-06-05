#include "../../include/TCPserver/ThreadPool.h"

ThreadPool::ThreadPool (int count)
{
    for(int i=0;i<count;i++)
    {
        stop = false;
        for(int i = 0;i<count;i++)
        {
            workers.emplace_back(&ThreadPool::workerLoop, this);
        }
    }
}

void ThreadPool::workerLoop()
{
    while (true)
    {
        std::function<void ()> task;
        {   
            std::unique_lock lock(mtx);
            cv.wait(lock, [this]{
                return !tasks.empty() || stop;
            });
            
            if(tasks.empty() && stop)return;

            if(!tasks.empty())
            {
                task  = tasks.front();
                tasks.pop();
            }
        }
        task();

    }
    
}

void ThreadPool::addTask(task handleClient)
{
    {std::lock_guard<std::mutex> lock(mtx);
    tasks.push(handleClient);}
    cv.notify_one();
}

ThreadPool::~ThreadPool()
{
    {std::lock_guard<std::mutex> lock(mtx);
    stop = true;}
    cv.notify_all();

    for(std::thread& worker: workers)
    {
        if(worker.joinable())
        {
            worker.join();
        }
    }

}
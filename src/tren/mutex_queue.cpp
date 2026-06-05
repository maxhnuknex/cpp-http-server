#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

using namespace std;

mutex mtx;
condition_variable cv;
queue<int> task;
bool finishing = false;

void printt()
{
    while(true)
    {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [] {
            return !task.empty() || finishing;
        });

        if(!task.empty())
        {
            int temp = task.front();
            task.pop();

            lock.unlock();
            cout <<temp<< ' ';
        }
        else if(finishing)
        {
            break;
        }
    }
}
void inputt()
{
    for (int i=0;i<5;i++)
    {
        this_thread::sleep_for(chrono::milliseconds(300));
        {
            lock_guard<mutex> lock(mtx);
            task.push(i);
        }
        
        cv.notify_one();
    }   
    {lock_guard<mutex> lock(mtx);
    finishing = true;}
    cv.notify_one();

}
int main()
{
    thread th1(inputt);
    thread th2(printt);

    th1.join();
    th2.join();
    
    return 0;
}
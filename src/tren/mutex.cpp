#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex mtx;
void printt(char ch)
{

    cout << "A"<<endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
    mtx.lock();
    for(int i = 0;i<5;i++)
    {
        for (int j= 0;j<5;j++)
        {
            cout<<ch;
            this_thread::sleep_for(chrono::milliseconds(20));     
        }
        cout<<endl;
    }
    mtx.unlock();
    cout<<"B"<<endl;
    this_thread::sleep_for(chrono::milliseconds(1000));
}

int main ()
{
    thread th1(printt, '#');
    thread th2(printt, '*');
   
    th1.join();
    th2.join();

    return 0;
}
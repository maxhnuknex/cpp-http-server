#include <iostream>
#include <thread>
#include <chrono>

using namespace std;


void dowork()
{
    for (int i=0;i<5;i++)
        {
            cout<<this_thread::get_id()<<'\t' << i<<"dowork"<<endl;
            this_thread::sleep_for(chrono::milliseconds(1000));
        }
}


int main ()
{
    thread th(dowork);
    thread th2(dowork);


    for (int i=0;i<5;i++)
    {
        cout<<this_thread::get_id()<<'\t' << i<<"main"<<endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    th.join();
    th2.join();
    return 0;
}


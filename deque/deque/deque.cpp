#include <iostream>
#include <thread>
#include <deque>
#include <chrono>
#include <shared_mutex>
#include <atomic>

using namespace std;

#define MAX_SIZE 10

shared_mutex queue_lock;
atomic<bool> transmitter_done(false); // Sinaliza quando o transmissor terminar

void Transmitter(deque<int> &queue)
{
    for (int data = 0; data < 20; data++)
    {
        unique_lock<shared_mutex> lock(queue_lock);

        if (queue.size() >= MAX_SIZE)
            queue.pop_front();

        queue.push_back(data);
        cout << "Enviado: " << data << endl;
        lock.unlock();

        this_thread::sleep_for(chrono::milliseconds(100));
    }
    transmitter_done = true;
}

void Receiver(deque<int> &queue)
{
    while (true)
    {
        unique_lock<shared_mutex> lock(queue_lock);

        if (queue.empty() && transmitter_done) 
            break;
      
        if(!queue.empty())
        {
            int data = queue.front();
            queue.pop_front();
            lock.unlock();
            cout << "Recebido: " << data << endl;
        }         

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main()
{
    deque<int> queue;

    thread tx_thread(Transmitter, ref(queue));
    thread rx_thread(Receiver, ref(queue));

    tx_thread.join();
    rx_thread.join();

    cout << "Fim do programa." << endl;
    return 0;
}
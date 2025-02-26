#include <iostream>
#include <thread>
#include "SafeQueue.h"

using namespace std;

atomic<bool> transmitter_done(false); // Sinaliza quando o transmissor terminar

void Transmitter(SafeQueue<int> &queue)
{
    for (int data = 0; data < 20; data++)
    {
        queue.push(data);
        cout << "Enviado: " << data << endl;

        this_thread::sleep_for(chrono::milliseconds(100));
    }
    transmitter_done = true;
}

void Receiver(SafeQueue<int> &queue)
{
    while (true)
    {
        if (queue.empty() && transmitter_done) 
            break;
      
        if(!queue.empty())
        {
            int data = queue.pop();
            cout << "Recebido: " << data << endl;
        }         

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main()
{
    SafeQueue<int> queue(10);

    thread tx_thread(Transmitter, ref(queue));
    thread rx_thread(Receiver, ref(queue));

    tx_thread.join();
    rx_thread.join();

    cout << "Fim do programa." << endl;
    return 0;
}
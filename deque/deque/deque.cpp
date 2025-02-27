#include <iostream>
#include <thread>
#include "SafeQueue.h"

using namespace std;

atomic<bool> transmitter_done(false); // Sinaliza quando o transmissor terminar

void Transmitter(SafeQueue<float> &queue)
{
    for (float data = 0; data < 20; data+=0.5)
    {
        queue.push(data);
        cout << "Enviado: " << data << endl;

        this_thread::sleep_for(chrono::milliseconds(100));
    }
    transmitter_done = true;
}

void Receiver(SafeQueue<float> &queue)
{
    while (true)
    {
        if (queue.empty() && transmitter_done) 
            break;
      
        if(!queue.empty())
        {
            float data = queue.pop();
            cout << "Recebido: " << data << endl;
        }         

        this_thread::sleep_for(chrono::milliseconds(500));
    }
}

int main()
{
    SafeQueue<float> queue(10);

    thread tx_thread(Transmitter, ref(queue));
    thread rx_thread(Receiver, ref(queue));

    tx_thread.join();
    rx_thread.join();

    cout << "Fim do programa." << endl;
    return 0;
}
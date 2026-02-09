#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <pthread.h>
#include <cmath>
#include <thread>
#include <time.h>
#include <iomanip>
#include <mutex>
using namespace std;
pthread_mutex_t mutex1;
int x = 0;
int producer1 = 0;
int producer2 = 0;
int producer3 = 0;
int batteryNum = 0;
int propellerNum = 0;
int aircraftNum = 0;
bool item = false;
bool battery[4] = { false };
bool propeller[4] = { false };
bool aircraft[4] = { false };
bool took[4] = { false };
void* thread1_func(void* args);
void* thread2_func(void* args);
void* thread3_func(void* args);

void* thread1_func(void* args) // thread
{
    if (took[3] || took[2]) // took by other threads
    {
        pthread_exit(NULL);
    }
    if (x == 1)
    { // exit if aircraft
        pthread_exit(NULL);
    }
    if (x == 0 && battery[1] == false)
    {
        pthread_mutex_lock(&mutex1);
        took[1] = true;
        item = false;
        battery[1] = true;
        cout << "Producer 1(aircraft):get battery" << endl;
        sleep(1);
    }
    else if (x == 2 && propeller[1] == false)
    {
        pthread_mutex_lock(&mutex1);
        took[1] = true;
        item = false;
        propeller[1] = true;
        cout << "Producer 1(aircraft):get propeller" << endl;
        sleep(1);
    }
    if (battery[1] == true && propeller[1] == true)
    {
        producer1 += 1;
        battery[1] = false;
        propeller[1] = false;
        cout << "Producer 1 (aircraft): OK," << producer1 << " drone(s)" << endl;
        sleep(1);
    }

    pthread_mutex_unlock(&mutex1);

    took[1] = false;
    pthread_exit(NULL);
}

void* thread2_func(void* args) // thread
{
    if (took[1] || took[3]) // took by other threads
    {
        pthread_exit(NULL);
    }
    if (x == 0 && battery[2] == false)
    {
        pthread_mutex_lock(&mutex1);
        item = false;
        battery[2] = true;
        took[2] = true;
        cout << "Producer 2:get battery" << endl;
        sleep(1);
    }

    else if (x == 1 && aircraft[2] == false)
    {
        pthread_mutex_lock(&mutex1);
        took[2] = true;
        item = false;
        aircraft[2] = true;

        cout << "Producer 2:get aircraft" << endl;
        sleep(1);
    }
    else if (x == 2 && propeller[2] == false)
    {
        pthread_mutex_lock(&mutex1);
        took[2] = true;
        item = false;
        propeller[2] = true;

        cout << "Producer 2:get propeller" << endl;
        sleep(1);
    }
    if (battery[2] == true && propeller[2] == true && aircraft[2] == true)
    {
        producer2 += 1;
        battery[2] = false;
        propeller[2] = false;
        aircraft[2] = false;
        cout << "Producer 2: OK," << producer2 << " drone(s)" << endl;
        sleep(1);
    }

    pthread_mutex_unlock(&mutex1);

    took[2] = false;
    pthread_exit(NULL);
}
void* thread3_func(void* args) // thread
{
    if (took[1] || took[2]) // took by other threads
    {
        pthread_exit(NULL);
    }
    if (x == 0 && battery[3] == false)
    {
        pthread_mutex_lock(&mutex1);
        took[3] = true;
        item = false;
        battery[3] = true;
        cout << "Producer 3:get battery" << endl;
        sleep(1);
    }

    else if (x == 1 && aircraft[3] == false)
    {
        pthread_mutex_lock(&mutex1);
        took[3] = true;
        item = false;
        aircraft[3] = true;
        cout << "Producer 3:get aircraft" << endl;
        sleep(1);
    }
    else if (x == 2 && propeller[3] == false)
    {
        pthread_mutex_lock(&mutex1);
        took[3] = true;
        item = false;
        propeller[3] = true;
        cout << "Producer 3:get propeller" << endl;
        sleep(1);
    }
    if (battery[3] == true && propeller[3] == true && aircraft[3] == true)
    {
        producer3 += 1;
        battery[3] = false;
        propeller[3] = false;
        aircraft[3] = false;
        cout << "Producer 3: OK," << producer3 << " drone(s)" << endl;
        sleep(1);
    }

    pthread_mutex_unlock(&mutex1);

    took[3] = false;
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    int advance, RandomNum;
    advance = stoi(argv[1], nullptr, 0);
    RandomNum = stoi(argv[2], nullptr, 0);
    aircraft[1] = true;
    pthread_t thread1, thread2, thread3; // creating thread
    while (producer1 + producer2 + producer3 < 50 && item == false)
    {
        srand(RandomNum);
        x = rand() % 3; // x=0 battery,x=1 aircraft,x=2 propeller
        if (battery[1] == true && battery[2] == true && battery[3] == true)
        {
            while (x == 0)
            { // til is not 0
                RandomNum += 1;
                srand(RandomNum);
                x = rand() % 3; // x=0 battery,x=1 aircraft,x=2 propeller
            }
        }
        if (aircraft[1] == true && aircraft[2] == true && aircraft[3] == true)
        {
            while (x == 1)
            { // til is not 1
                RandomNum += 1;
                srand(RandomNum);
                x = rand() % 3; // x=0 battery,x=1 aircraft,x=2 propeller
            }
        }
        if (propeller[1] == true && propeller[2] == true && propeller[3] == true)
        {
            while (x == 2)
            { // til is not 2
                RandomNum += 1;
                srand(RandomNum);
                x = rand() % 3; // x=0 battery,x=1 aircraft,x=2 propeller
            }
        }
        if ((battery[1] == true && battery[2] == true && battery[3] == true) && (aircraft[1] == true && aircraft[2] == true && aircraft[3] == true))
        { // two part repeat
            x = 2;
        }
        else if ((battery[1] == true && battery[2] == true && battery[3] == true) && (propeller[1] == true && propeller[2] == true && propeller[3] == true))
        {
            x = 1;
        }
        else if ((propeller[1] == true && propeller[2] == true && propeller[3] == true) && (aircraft[1] == true && aircraft[2] == true && aircraft[3] == true))
        {
            x = 0;
        }
        item = true;
        if (x == 0)
        {
            cout << endl;
            cout << "Dispatcher:battery" << endl;
            batteryNum++;
        }
        else if (x == 1)
        {
            cout << endl;
            cout << "Dispatcher:aircraft" << endl;
            aircraftNum++;
        }
        else if (x == 2)
        {
            cout << endl;
            cout << "Dispatcher:propeller" << endl;
            propellerNum++;
        }

        pthread_mutex_init(&mutex1, 0);
        pthread_create(&thread1, NULL, thread1_func, NULL);
        pthread_create(&thread2, NULL, thread2_func, NULL);
        pthread_create(&thread3, NULL, thread3_func, NULL);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
        pthread_join(thread3, NULL);
        pthread_mutex_destroy(&mutex1);
        RandomNum += 1;
    }
    cout << endl;
    cout << "battery used:" << batteryNum << endl;
    cout << "aircraft used:" << aircraftNum << endl;
    cout << "propeller used:" << propellerNum << endl;
    cout << endl;
    if (producer1 > producer2 && producer1 > producer3)
    {
        if (producer2 > producer3)
        {
            cout << "Producer1: " << producer1 << " drones" << endl;
            cout << "Producer2: " << producer2 << " drones" << endl;
            cout << "Producer3: " << producer3 << " drones" << endl;
        }
        else
        {
            cout << "Producer1: " << producer1 << " drones" << endl;
            cout << "Producer3: " << producer3 << " drones" << endl;
            cout << "Producer2: " << producer2 << " drones" << endl;
        }
    }
    else if (producer2 > producer1 && producer2 > producer3)
    {
        if (producer1 > producer3)
        {
            cout << "Producer2 total produced: " << producer2 << " drones" << endl;
            cout << "Producer1 total produced: " << producer1 << " drones" << endl;
            cout << "Producer3 total produced: " << producer3 << " drones" << endl;
        }
        else
        {
            cout << "Producer2 total produced: " << producer2 << " drones" << endl;
            cout << "Producer3 total produced: " << producer3 << " drones" << endl;
            cout << "Producer1 total produced: " << producer1 << " drones" << endl;
        }
    }
    else if (producer3 > producer1 && producer3 > producer2)
    {
        if (producer1 > producer2)
        {
            cout << "Producer3 total produced: " << producer3 << " drones" << endl;
            cout << "Producer1 total produced: " << producer1 << " drones" << endl;
            cout << "Producer2 total produced: " << producer2 << " drones" << endl;
        }
        else
        {
            cout << "Producer3 total produced: " << producer3 << " drones" << endl;
            cout << "Producer2 total produced: " << producer2 << " drones" << endl;
            cout << "Producer1 total produced: " << producer1 << " drones" << endl;
        }
    }
}

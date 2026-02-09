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
using namespace std;
void *thread_func(void *args);

int NonRepeat = 0;
int TermFreq[51][100] = {};
int count = 0;
int MaxDoc = 0;
double HighestAvg = 0;

double cosine_similarity(int *A, int *B, unsigned int Vector_Length) // Cosine similarity coefficient
{
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0;
    for (unsigned int i = 0u; i < Vector_Length; ++i)
    {
        dot += A[i] * B[i];
        denom_a += A[i] * A[i];
        denom_b += B[i] * B[i];
    }
    return dot / (sqrt(denom_a) * sqrt(denom_b));
}
void *thread_func(void *args) // thread
{
    clock_t t;
    t = clock();
    long ith;
    ith = (long)args;
    double avg = 0;
    cout << "[Main thread]: create TID:" << this_thread::get_id() << ",DocID:" << setfill('0') << setw(4) << ith << endl;
    cout << "[TID=" << this_thread::get_id() << "] DocID:" << setfill('0') << setw(4) << ith << " [";
    for (int i = 0; i < NonRepeat - 1; i++)
    {
        cout << TermFreq[ith][i] << ",";
    }
    cout << TermFreq[ith][NonRepeat - 1] << "]" << endl;
    for (int i = 1; i <= count; i++)
    {
        if (i == ith)
        {
            continue;
        }

        cout << "[TID=" << this_thread::get_id() << "] "
             << "cosine(" << setfill('0') << setw(4) << ith << "," << setfill('0') << setw(4) << i << ")=" << cosine_similarity(TermFreq[ith], TermFreq[i], NonRepeat) << endl;
        avg += cosine_similarity(TermFreq[ith], TermFreq[i], NonRepeat);
    }
    avg = avg / (count - 1);
    if (avg > HighestAvg)
    {
        HighestAvg = avg;
        MaxDoc = ith;
    }
    cout << "[TID=" << this_thread::get_id() << "] "
         << "Avg_cosine:" << avg << endl;
    t = clock() - t;
    cout << "[TID=" << this_thread::get_id() << "] "
         << "CPU time:" << ((float)t) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
    pthread_exit(0);
}

int main(int argc, char **argv)
{
    clock_t t;
    t = clock();
    ifstream file; // text processing
    string filename;
    cin >> filename;
    file.open(filename.c_str(), ios::in);
    if (!file.is_open())
    {
        cout << "File error" << endl;
    }
    string word;
    string term[51][100];
    string vocabularys[100] = {};
    int vocabulary = 0;
    int maxlength = 0;
    while (file >> word)
    {
        bool equal = false;
        char arr[4];
        strcpy(arr, word.c_str());
        if (isdigit(arr[0]))
        {
            vocabulary = 1;
            count++;
            continue;
        }
        term[count][vocabulary] = word;
        vocabulary++;
        for (int i = 0; i < NonRepeat; i++)
        {
            if (word == vocabularys[i])
            {
                equal = true;
                break;
            }
        }
        if (!equal)
        {
            vocabularys[NonRepeat] = word;
            NonRepeat++;
        }

        if (vocabulary > maxlength)
        {
            maxlength = vocabulary;
        }
    }
    file.close(); // done

    for (int i = 1; i <= count; i++) // frequency
    {
        for (int j = 1; j < maxlength; j++)
        {
            for (int z = 0; z < NonRepeat; z++)
            {
                if (term[i][j] == vocabularys[z])
                {
                    TermFreq[i][z] += 1;
                }
            }
        }

    } // freq end

    pthread_t thread[count];          // creating threads
    for (long i = 1; i <= count; i++) // i th thread
    {

        int number = i - 1;

        cout << endl;
        pthread_create(&thread[number], NULL, thread_func, (void *)i);
        sleep(1);
    }

    for (int i = 0; i < count; ++i)
    {
        pthread_join(thread[i], NULL);
    }
    cout << "[Main thread] KeyDocID:" << setfill('0') << setw(4) << MaxDoc << " Highest Average Cosine:" << HighestAvg << endl;
    t = clock() - t;
    cout << "[Main thread] CPU time:" << ((float)t) / CLOCKS_PER_SEC * 1000 << "ms" << endl;
}

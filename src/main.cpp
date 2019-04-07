/**
 * main.cpp
 *
 * entrypoint
 *
 **/
#include "Code.hpp"
#include "Log.hpp"
#include "Signal.hpp"
#include <iostream>
#include <thread>

using namespace std;

int main(int argc, char** argv) {
    Log::MSG("Started up SDR");

    Code code;
    Signal signal;

    signal.Read();

    signal.HistFront();

    double corrRes[32][1023];
    double maxRes[32];
    int bestDoppler[32];
    std::thread corrThreads[8];


    // Exhaustive search testing
    for (unsigned int i = 0; i < 8; i++) {
        corrThreads[i] = std::thread(
            [&](unsigned int tid, unsigned int blockSize) {
                for (unsigned int i = tid * blockSize;
                     i < tid * blockSize + blockSize; i++) {
                    unsigned int prn = i;
                    cout << i + 1 << endl;
                    double max = 0.0;
                    // Search code last
                    for (int j = 0; j < 1023; j++) {
                        // Search doppler first, only on the initial code phase
                        for (int k = -14000; k < 14000; k += 500) {
                            corrRes[prn][j] =
                                signal.CrossCorrelate(code, prn, j, (double)k);
                            if (corrRes[prn][j] > max) {
                                max = corrRes[prn][j];
                                maxRes[prn] = max;
                                bestDoppler[prn] = k;
                            }
                        }
                    }
                }
            },
            i, 4);
    }

    for (int i = 0; i < 8; i++) {
        corrThreads[i].join();
    }
    for (int i = 0; i < 32; i++) {
        cout << i + 1 << " ";
        cout << maxRes[i] << " doppler " << bestDoppler[i] << endl;
    }

    return 0;
}
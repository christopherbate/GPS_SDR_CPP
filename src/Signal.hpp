/**
 * Signal.hpp
 *
 * A modular signal interface
 * Contains implementations for "signal stream"
 * Implements direct data manipulation on signal, including
 * putting it in the right format, reading/saving, managing
 * buffers, etc.
 *
 * Signal processing functions are seperate, they operate
 * on the signal data type to perform implementations of functions
 * such as correlation, convolution, filtering, etc.
 *
 */

#include <cstdint>
#include <fstream>
#include <memory>
#include <queue>
#include <vector>
#include "Code.hpp"
#include <complex>

class Signal {
  private:
    std::queue<std::unique_ptr<std::vector<std::complex<int8_t>>>> m_dataBuffers;
    std::vector<int8_t> m_bitmap;
    std::ifstream m_infile;

    unsigned int m_carrierFreq;
    unsigned int m_ifFreq;
    unsigned int m_sampFreq; /* Samples per second*/
    double m_timeSample;     /* Time per Sample */

  public:
    Signal();
    ~Signal();

    double CrossCorrelate( Code &code, int codeIdx, int phaseIdx, double doppler );

    void Convolve( Code &code );

    void Read();    

    void HistFront();
};

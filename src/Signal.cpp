#include "Signal.hpp"
#include "Log.hpp"
#include <cmath>
#include <iostream>
#include <iterator>
#include <string>
#include <unordered_map>

#define PI 3.14159265

using namespace std::complex_literals;

Signal::Signal(/* args */) {
    std::string filename = "./data/if_sige.data";

    m_infile.open(filename, std::ios::binary);

    if (!m_infile.is_open()) {
        throw std::runtime_error("failed to open signal file");
    }

    m_carrierFreq = 4092000;
    m_ifFreq = 4092000;
    m_sampFreq = 16368000;

    m_bitmap = {-1, 1, -3, 3};
}

Signal::~Signal() { m_infile.close(); }

void Signal::Read() {
    const unsigned int blockSize = (m_sampFreq / 1000) * sizeof(int8_t);
    std::vector<int8_t> readBuffer(blockSize);

    std::unique_ptr<std::vector<std::complex<int8_t>>> buffer(
        new std::vector<std::complex<int8_t>>(blockSize));

    m_infile.read((char*)readBuffer.data(), blockSize);
    Log::MSG(std::to_string(m_infile.gcount()));
    for (unsigned int i = 0; i < m_infile.gcount(); i++) {
        unsigned int idx = (unsigned int)readBuffer[i];
        (*buffer)[i] = std::complex<int8_t>(m_bitmap[idx], 0);
    }
    m_dataBuffers.push(std::move(buffer));
}

double Signal::CrossCorrelate(Code& code, int codeIdx, int phaseIdx,
                              double doppler) {
    if (m_dataBuffers.size() == 0) {
        throw std::runtime_error("No signal for correlation");
    }

    // unsigned int phaseIdxAccum = 0;
    // unsigned int phaseAccumPerSample = 0;
    double chipPerSample = code.chipFreq / m_sampFreq;
    double phaseIdxAccum = (double)phaseIdx;
    double accum = 0.0;
    double sigNorm = 0.0; // L1 norm of the signal
    double freq = (double)m_ifFreq + doppler;

    for (unsigned int i = 0; i < m_dataBuffers.front()->size(); i++) {
        // Calclate code phase and then incremenet.        
        unsigned int phaseIdx =
            ((unsigned int)std::round(phaseIdxAccum)) % code.codeLength;

        phaseIdxAccum += chipPerSample;

        // Calculate time
        double timeIdx = i/(double)m_sampFreq;

        // Calculate carrier
        std::complex<double> carrier(std::cos(2.0 * PI * timeIdx * freq),
                                     std::sin(2.0 * PI * timeIdx * freq));
        
        // Perform carrier wipeoff
        std::complex<double> wipeOff = carrier * std::complex<double>(m_dataBuffers.front()->data()[i].real(),m_dataBuffers.front()->data()[i].imag());

        // std::cout << wipeOff << std::endl;

        double valueR = wipeOff.real()*code.code[phaseIdx][codeIdx];

        accum += (valueR);
        sigNorm += abs(valueR);
    }

    return pow(accum,2)/pow(sigNorm,2);
}

void Signal::Convolve(Code& code) {}

void Signal::HistFront() {
    unsigned int counts[4] = {0, 0, 0, 0};

    if (m_dataBuffers.size() > 0) {
        for (unsigned int i = 0; i < m_dataBuffers.front()->size(); i++) {
            switch ((int)m_dataBuffers.front()->at(i).real()) {
            case -3:
                counts[0]++;
                break;
            case -1:
                counts[1]++;
                break;
            case 1:
                counts[2]++;
                break;
            case 3:
                counts[3]++;
                break;
            default:
                std::cout << int(m_dataBuffers.front()->at(i).real())
                          << std::endl;
                break;
            }
        }
    }

    std::cout << "Front buffer counts: \n";
    std::cout << "-3: " << (double)counts[0] / m_dataBuffers.front()->size()
              << std::endl;
    std::cout << "-1: " << (double)counts[1] / m_dataBuffers.front()->size()
              << std::endl;
    std::cout << " 1: " << (double)counts[2] / m_dataBuffers.front()->size()
              << std::endl;
    std::cout << " 3: " << (double)counts[3] / m_dataBuffers.front()->size()
              << std::endl;
    std::cout << std::endl;
}
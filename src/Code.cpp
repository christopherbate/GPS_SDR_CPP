#include "Code.hpp"
#include <fstream>
#include <iostream>
#include <string>

#define G1_OUT_MASK 0x001
#define G2_FB_MASK 0x197
#define G1_FB_MASK 0x081

using namespace std;

Code::Code(/* args */) {
    // Read in phases.
    ifstream phases("./data/phase.txt", ifstream::in);
    if (!phases.is_open()) {
        throw std::runtime_error("Failed to open phase.txt");
    }

    for (int prn = 0; prn < 32; prn++) {
        string input;
        int g1 = 0x3FF;
        int g2 = 0x3FF;

        // Get the G2 output registers.
        phases >> input;
        int g2_p1 = 0x01 << (10 - std::stoi(input));
        phases >> input;
        int g2_p2 = 0x01 << (10 - std::stoi(input));

        std::cout << "PRN: " << prn << std::endl;

        for (int i = 0; i < 1023; i++) {
            // Get current output.
            code[i][prn] = (__builtin_parity(g1 & G1_OUT_MASK) +
                            __builtin_parity((g2 & g2_p1) | (g2 & g2_p2))) %
                           2;
            if (code[i][prn] == 0) {
                code[i][prn] = -1;
            }

            // Feedbacks
            int fbG1 = __builtin_parity(g1 & G1_FB_MASK);
            int fbG2 = __builtin_parity(g2 & G2_FB_MASK);

            // Shift the registers
            g1 = g1 >> 1;
            g2 = g2 >> 1;
            g1 = g1 | fbG1 << 9;
            g2 = g2 | fbG2 << 9;

            std::cout << (int)code[i][prn];
        }
        cout << std::endl;
    }
    phases.close();

    chipFreq = 1.023e6;
    codeLength = 1023;
}

Code::~Code() {}
#ifndef CODE_H_
#define CODE_H_

#include <cstdint>

class Code
{
public:
    /* data */
    int8_t code[1023][32];
    double chipFreq;
    unsigned int codeLength;
public:
    Code();
    ~Code();
};




#endif // !CODE_H_
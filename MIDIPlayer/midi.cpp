#include "midi.h"
using namespace std;

int Np(int n)
{
    double tmp = 440.0 / 32.0 * pow(2, ((n - 9.0) / 12.0));
    return 2000000 / (16 * tmp);
}


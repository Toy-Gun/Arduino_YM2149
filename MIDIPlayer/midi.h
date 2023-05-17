#ifndef _MIDI_H_
#define _MIDI_H_

#include <MidiFile.h>
#include <Options.h>
#include <math.h>

using namespace std;

int Np(int n);
struct myMIDI
{
    int time = 0;
    char f = 0;
    char on = 0;
};

#endif /* _MIDI_H_ */

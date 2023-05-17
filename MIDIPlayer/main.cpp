#include "midi.h"
#include <iostream>
#include <chrono>
#include <CppLinuxSerial/SerialPort.hpp>
#include <unistd.h>
#include <iomanip>
#include <queue>
#include <vector>

using namespace mn::CppLinuxSerial;
using namespace std;
using namespace smf;

SerialPort serialPort("/dev/ttyUSB0", 115200);

vector<queue<myMIDI>> v;
queue<myMIDI> trk;

long long int this_time = 0;

int main(int argc, char **argv)
{
  for (int i = 0; i < 6; i++)
  {
    v.push_back(trk);
  }
  serialPort.SetTimeout(-1);
  serialPort.Open();

  Options options;
  options.process(argc, argv);
  MidiFile midifile;
  if (options.getArgCount() > 0)
    midifile.read(options.getArg(1));
  else
    midifile.read(cin);
  midifile.joinTracks();
  // midifile.getTrackCount() will now return "1", but original
  // track assignments can be seen in .track field of MidiEvent.
  MidiEvent *mev;
  int deltatick;
  for (int event = 0; event < midifile[0].size(); event++)
  {
    mev = &midifile[0][event];
    if (event == 0)
      deltatick = mev->tick;
    else
      deltatick = mev->tick - midifile[0][event - 1].tick;
    if (((*mev)[0] == 0x90 | (*mev)[0] == 0x80) & mev->track < 6)
    {
      myMIDI midi;
      midi.time = mev->tick;
      midi.f = (char)(*mev)[1];
      midi.on = (*mev)[0] == 0x80 ? '0' : '1';
      cout << midi.time << ": " << midi.on << " " << mev->track << " " << (*mev)[1] << "\n";
      v[mev->track].push(midi);
    }
  }
  cout << endl;

  while (!(v[0].empty() & v[1].empty() & v[2].empty() & v[3].empty() & v[4].empty() & v[5].empty()))
  {
    usleep(5000);
    this_time++;
    for (int i = 0; i < 6; i++)
    {
      while (!(v[i].empty()) & v[i].front().time < this_time)
      {
        cout << i << ": " << v[i].front().f << "\n";
        string wr(1, '~');
        string trk(1, (char)i);
        string on(1, v[i].front().on);
        string f(1, v[i].front().f);

        serialPort.Write(wr);
        serialPort.Write(trk);
        serialPort.Write(f);
        serialPort.Write(on);
        v[i].pop();
      }
    }
    // cout << this_time << "\n";
  }
  serialPort.Close();
  return 0;
}

// 24 - 120 C2 - C10
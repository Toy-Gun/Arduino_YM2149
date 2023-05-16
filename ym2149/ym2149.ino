#include "YM2149.h"

byte incomingByte;
byte note;
int noteDown = LOW;
int state = 0;
int channel = 0;

YM2149Class Ym;

void setup()
{
    Serial.begin(115200);

    Ym.begin();
    Ym.setPortIO(1, 1);

    Ym.setPin(0, 1);
    delay(100);
    Ym.setPin(0, 0);
    delay(100);

    Ym.mute();
}

void loop()
{
    if (Serial.available() > 0)
    {
        incomingByte = Serial.read();
        switch (state)
        {
        case 0:
            if ((incomingByte & 0xF0) == 0x90)
            {
                noteDown = HIGH;
                state = 1;
                channel = incomingByte & 0xF;
            }
            if ((incomingByte & 0xF0) == 0x80)
            {
                noteDown = LOW;
                state = 1;
                channel = incomingByte & 0xF;
            }
            break;

        case 1:
            if (incomingByte < 128)
            {
                note = incomingByte;
                state = 2;
            }
            else
            {
                state = 0;
            }
            break;

        case 2:
            if (incomingByte < 128) 
            {
                playNote(channel, noteDown, note, incomingByte);
            } 
            state = 0;
            break;
        }
    }
}

void playNote(byte channel, byte down, byte note, byte velocity)
{
    Ym.setNote(channel, note);

    if(velocity == 0)
        down = LOW;

    if(down == HIGH)
    {
        Ym.setPin(0, 1);
        Ym.setVolume(channel, 15);
    }
    else
    {
        Ym.setPin(0, 0);
        Ym.setVolume(channel, 0);
    }
}
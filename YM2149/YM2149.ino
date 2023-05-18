#include "YM2149.h"
#include "Samples.h"

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

    // Ym.setEnv(0, 1);
    // Ym.setEnvShape(1, 1, 1, 0);
    // Ym.setNote(0, 20);
    // for (int i = 10; i < 150; i += 10)
    // {
    //     Ym.setFreq(4, i);
    //     delay(500);
    // }

    // Ym.setEnv(0, 0);
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

    if (velocity == 0)
        down = LOW;

    if (down == HIGH)
    {
        Ym.setPin(0, 1);
        // EnvShape(0, 0);
        Ym.setVolume(channel, (byte)(15 * ((float)velocity / 127.0f)));
        //  Ym.setNoise(channel, 1);
    }
    else
    {
        Ym.setPin(0, 0);
        Ym.setVolume(channel, 0);
    }
    return;
}

void EnvShape(byte channel, byte value)
{
    Ym.setVolume(channel, Downlifter[value]);
    delay(10);
    if (noteDown && value < 15)
    {
        EnvShape(0, value + 1);
    }
    return;
}
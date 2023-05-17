#ifndef YM2149_h
#define YM2149_h

#include "Arduino.h"

class YM2149Class
{
    public:
        void begin();
        void write(uint8_t address, uint8_t value);
        void setPin(uint8_t pin, bool value);
        void setPort(bool port, uint8_t value);
        void setPortIO(bool portA, bool portB);

        void setNote(uint8_t synth, float value);
        void setTone(uint8_t synth, uint32_t value);
        void setVolume(uint8_t synth, uint8_t value);
        void setNoise(uint8_t synth, uint8_t value);
        void setEnv(uint8_t synth, uint8_t value);
        void mute();

    private:
        uint8_t mixerValue;
        uint8_t levelValue[3];

        uint8_t portAValue;
        uint8_t portBValue;

        static const uint8_t REG_A_FREQ = 0x00;
        static const uint8_t REG_B_FREQ = 0x02;
        static const uint8_t REG_C_FREQ = 0x04;
        static const uint8_t REG_NOISE_FREQ = 0x06;
        static const uint8_t REG_MIXER = 0x07;
        static const uint8_t REG_A_LEVEL = 0x08;
        static const uint8_t REG_B_LEVEL = 0x09;
        static const uint8_t REG_C_LEVEL = 0x0A;
        static const uint8_t REG_ENV_FREQ = 0x0B;
        static const uint8_t REG_ENV_SHAPE = 0x0D;
        static const uint8_t REG_DATAPORT_A = 0x0E;
        static const uint8_t REG_DATAPORT_B = 0x0F;
};

typedef YM2149Class YM2149;

#endif
#include "YM2149.h"

void YM2149Class::begin()
{
    for (int i = 4; i <= 13; i++)
        pinMode(i, OUTPUT);

    levelValue[0] = 0;
    levelValue[1] = 0;
    levelValue[2] = 0;

    mixerValue = B00111000;

    delay(10);
}

void YM2149Class::write(uint8_t address, uint8_t value)
{
    PORTD = (PORTD & B11001111) | B00110000;

    PORTB = address;
    PORTD = (PORTD & B00111111) | (address & B11000000);

    delayMicroseconds(1);

    PORTD &= B11001111;

    PORTB = value;
    PORTD = (PORTD & B00111111) | (value & B11000000);

    PORTD = (PORTD & B11001111) | B00010000;

    delayMicroseconds(1);

    PORTD &= B11001111;
}

void YM2149Class::setPortIO(bool portA, bool portB)
{
    uint8_t portSettings = ((uint8_t)portB) << 7 | ((uint8_t)portA) << 6;

    mixerValue = (mixerValue & B00111111) | portSettings;
    write(REG_MIXER, mixerValue);
}

void YM2149Class::setPin(uint8_t pin, bool value)
{
    uint8_t mask = 1;
    pin &= 0xF;

    if (pin & 0x8)
    {
        pin -= 8;
        portBValue = (portBValue ^ (mask <<= pin)) | (value <<= pin);
        write(REG_DATAPORT_B, portBValue);
    }
    else
    {
        portAValue = (portAValue ^ (mask <<= pin)) | (value <<= pin);
        write(REG_DATAPORT_A, portAValue);
    }
}

void YM2149Class::setPort(bool port, uint8_t value)
{
    if (port)
    {
        write(REG_DATAPORT_B, value);
    }
    else
    {
        write(REG_DATAPORT_A, value);
    }
}

void YM2149Class::setTone(uint8_t voice, uint32_t value)
{
    switch (voice)
    {
    case 0:
        write(REG_A_FREQ, (value & 0xFF));
        write(REG_A_FREQ + 1, (value >> 8) & 0xFF);
        break;
    case 1:
        write(REG_B_FREQ, (value & 0xFF));
        write(REG_B_FREQ + 1, (value >> 8) & 0xFF);
        break;
    case 2:
        write(REG_C_FREQ, (value & 0xFF));
        write(REG_C_FREQ + 1, (value >> 8) & 0xFF);
        break;
    case 3:
        write(REG_NOISE_FREQ, (value & 0x1F));
        break;
    case 4:
        value >>= 4;
        write(REG_ENV_FREQ, (value & 0xFF));
        write(REG_ENV_FREQ + 1, (value >> 8) & 0xFF);
        break;
    }
}

void YM2149Class::setNote(uint8_t voice, float value)
{
    uint16_t f;

    if (voice != 3)
    {
        f = (uint16_t)((2000000.0f / ((pow(2, ((((float)value) - 69) / 12)) * 440.0f))) / 16);
    }
    else
    {
        f = 31 - (value / 4);
    }

    setTone(voice, f);
}

void YM2149Class::setVolume(uint8_t voice, uint8_t value)
{
    if (voice > 2)
        return;

    uint8_t *state = &levelValue[voice];

    value &= 0b00001111;
    *(state) = (*(state)&0b00010000) | value;

    write(REG_A_LEVEL + voice, *(state));
}

void YM2149Class::setNoise(uint8_t voice, uint8_t value)
{
    if (voice > 2)
        return;

    switch (value)
    {
    case 1:
        value = 0b00000001;
        break;
    case 2:
        value = 0b00000000;
        break;

    default:
        value = 0b00001000;
        break;
    }

    value <<= voice;

    switch (voice)
    {
    case 0:
        mixerValue = (mixerValue & 0b11110110) | value;
        break;
    case 1:
        mixerValue = (mixerValue & 0b11101101) | value;
        break;
    default:
        mixerValue = (mixerValue & 0b11011011) | value;
        break;
    }
    write(REG_MIXER, mixerValue);
}

void YM2149Class::setEnv(uint8_t voice, uint8_t value)
{
    if (voice > 2)
        return;
    uint8_t *state = &levelValue[voice];

    value &= 0b00000001;
    value <<= 4;
    *(state) = (*(state)&0b00001111) | value;

    write(REG_A_LEVEL + voice, *(state));
}

void YM2149Class::mute()
{
    write(REG_A_LEVEL, 0);
    write(REG_B_LEVEL, 0);
    write(REG_C_LEVEL, 0);
    levelValue[0] = 0;
    levelValue[1] = 0;
    levelValue[2] = 0;
    mixerValue = 0b11111000;
}

/*
    0,0,0,0 : \_______
    0,1,0,0 : /_______
    1,0,0,0 : \\\\\\\\
    1,0,0,1 : \_______
    1,0,1,0 : \/\/\/\/
    1,0,1,1 : \```````
    1,1,0,0 : ////////
    1,1,0,1 : /```````
    1,1,1,0 : /\/\/\/\
    1,1,1,1 : /_______
*/
void YM2149Class::setEnvShape(uint8_t continuous, uint8_t attack, uint8_t alt, uint8_t hold)
{
    continuous &= 0b00000001;
    attack &= 0b00000001;
    alt &= 0b00000001;
    hold &= 0b00000001;
    write(REG_ENV_SHAPE, (continuous << 3) | (attack << 2) | (alt << 1) | (hold));
}

void YM2149Class::setFreq(uint8_t voice, uint32_t freq)
{
    uint16_t d = 16;
    if (voice == 4)
    {
        d = 256;
    }
    uint16_t f = (uint16_t)((2000000.0f / freq) / d);
    setTone(voice, f);
}
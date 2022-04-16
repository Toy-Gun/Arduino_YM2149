#line 1 "/home/wangjq/文档/ymplayer/ym2149.h"
#define DATA_READ (0x01 << 5)
#define DATA_WRITE (0x02 << 5)
#define ADDRESS_MODE (0x03 << 5)

#define clockPin 2
#define latchPin 3
#define dataPin 4

#define masterClk 2000000

const int Note[128] = {15289, 14430, 13620, 12856, 12134, 11453, 10810, 10204, 9631, 9090, 8580, 8099, 7644, 7215, 6810, 6428, 6067, 5726, 5405, 5102, 4815, 4545, 4290, 4049, 3822, 3607, 3405, 3214, 3033, 2863, 2702, 2551, 2407, 2272, 2145, 2024, 1911, 1803, 1702, 1607, 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012, 955, 901, 851, 803, 758, 715, 675, 637, 601, 568, 536, 506, 477, 450, 425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 238, 225, 212, 200, 189, 178, 168, 159, 150, 142, 134, 126, 119, 112, 106, 100, 94, 89, 84, 79, 75, 71, 67, 63, 59, 56, 53, 50, 47, 44, 42, 39, 37, 35, 33, 31, 29, 28, 26, 25, 23, 22, 21, 19, 18, 17, 16, 15, 14, 14, 13, 12, 11, 11, 10, 9};

int mix = 0xf8;

void flash(char data)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, data);
  digitalWrite(latchPin, HIGH);
}

void set_bus_ctl(void)
{
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  DDRD |= 0xff;
}

void set_address(char addr)
{
  PORTD = (PORTD & 0x9f) | ADDRESS_MODE;
  flash(addr);
  PORTD = (PORTD & 0x9f); /*INACTIVE*/
}

void set_data(char data)
{
  flash(data);
  PORTD = (PORTD & 0x9f) | DATA_WRITE;
  PORTD = (PORTD & 0x9f); /*INACTIVE*/
}

void send_data(char addr, char data)
{
  set_address(addr);
  set_data(data);
}

void play(int f, int channel)
{
  send_data(channel * 2, f & 0xff);
  send_data(channel * 2 + 1, f >> 8);
}

void panano(int f, int channel, int time, int ctn)
{
  play(f, channel);
  delay(time);
  if (ctn == 0)
  {
    play(0, channel);
  }
}
// MSB (PB5) is connected to BDIR
// LSB (PB4) is connected to BC1
// +5V is connected to BC2
#define DATA_READ_1 (0x01 << 2)
#define DATA_WRITE_1 (0x02 << 2)
#define ADDRESS_MODE_1 (0x03 << 2)
#define DATA_READ_2 (0x01 << 4)
#define DATA_WRITE_2 (0x02 << 4)
#define ADDRESS_MODE_2 (0x03 << 4)

#define clockPin_1 5
#define latchPin_1 6
#define dataPin_1 7

#define clockPin_2 2
#define latchPin_2 3
#define dataPin_2 4

#define led_clockPin 14
#define led_latchPin 15
#define led_dataPin 16

#define masterClk 2000000

const int Np[128] = {15289, 14430, 13620, 12856, 12134, 11453, 10810, 10204, 9631, 9090, 8580, 8099, 7644, 7215, 6810, 6428, 6067, 5726, 5405, 5102, 4815, 4545, 4290, 4049, 3822, 3607, 3405, 3214, 3033, 2863, 2702, 2551, 2407, 2272, 2145, 2024, 1911, 1803, 1702, 1607, 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012, 955, 901, 851, 803, 758, 715, 675, 637, 601, 568, 536, 506, 477, 450, 425, 401, 379, 357, 337, 318, 300, 284, 268, 253, 238, 225, 212, 200, 189, 178, 168, 159, 150, 142, 134, 126, 119, 112, 106, 100, 94, 89, 84, 79, 75, 71, 67, 63, 59, 56, 53, 50, 47, 44, 42, 39, 37, 35, 33, 31, 29, 28, 26, 25, 23, 22, 21, 19, 18, 17, 16, 15, 14, 14, 13, 12, 11, 11, 10, 9};

int led = 0;

void led_flash(int n, bool on)
{
  int tmp = 0x01 << n;
  if (on == true)
  {
    led = led | tmp;
  }
  else
  {
    led = led & (tmp ^ 0xff);
  }
  digitalWrite(led_latchPin, LOW);
  shiftOut(led_dataPin, led_clockPin, MSBFIRST, led);
  digitalWrite(led_latchPin, HIGH);
}

void flash_1(char data)
{
  digitalWrite(latchPin_1, LOW);
  shiftOut(dataPin_1, clockPin_1, MSBFIRST, data);
  digitalWrite(latchPin_1, HIGH);
}

void flash_2(char data)
{
  digitalWrite(latchPin_2, LOW);
  shiftOut(dataPin_2, clockPin_2, MSBFIRST, data);
  digitalWrite(latchPin_2, HIGH);
}

void ym_set_bus_ctl(void)
{
  pinMode(latchPin_1, OUTPUT);
  pinMode(clockPin_1, OUTPUT);
  pinMode(dataPin_1, OUTPUT);
  pinMode(latchPin_2, OUTPUT);
  pinMode(clockPin_2, OUTPUT);
  pinMode(dataPin_2, OUTPUT);
  pinMode(led_latchPin, OUTPUT);
  pinMode(led_clockPin, OUTPUT);
  pinMode(led_dataPin, OUTPUT);
  DDRB |= 0xff;
}

void set_address_1(char addr)
{
  PORTB = (PORTB & 0xf3) | ADDRESS_MODE_1;
  flash_1(addr);
  // tAS = 300ns = 4.8 clock cycles
  PORTB = (PORTB & 0xf3) /*INACTIVE*/;
  // tAH = 80ns  = 1.3 clock cycles
}
void set_address_2(char addr)
{
  PORTB = (PORTB & 0xcf) | ADDRESS_MODE_2;
  flash_2(addr);
  // tAS = 300ns = 4.8 clock cycles
  PORTB = (PORTB & 0xcf) /*INACTIVE*/;
  // tAH = 80ns  = 1.3 clock cycles
}

void set_data_1(char data)
{
  flash_1(data);
  PORTB = (PORTB & 0xf3) | DATA_WRITE_1;
  // 300ns < tDW < 10us = 4.8 clock cycles
  PORTB = (PORTB & 0xf3) /*INACTIVE*/; // To fit tDW max
  // tDH = 80ns = 1.3 clock cycles
}
void set_data_2(char data)
{
  flash_2(data);
  PORTB = (PORTB & 0xcf) | DATA_WRITE_2;
  // 300ns < tDW < 10us = 4.8 clock cycles
  PORTB = (PORTB & 0xcf) /*INACTIVE*/; // To fit tDW max
  // tDH = 80ns = 1.3 clock cycles
}

void ym_send_data_1(char addr, char data)
{
  set_address_1(addr);
  set_data_1(data);
}

void ym_send_data_2(char addr, char data)
{
  set_address_2(addr);
  set_data_2(data);
}

void pinano_1(int f, int channel, bool on)
{
  channel = channel * 2;

  if (on == true)
  {
    led_flash(channel / 2, true);
    ym_send_data_1(channel, f & 0xff);
    ym_send_data_1(channel + 1, f >> 8);
  }
  else
  {
    led_flash(channel / 2, false);
    ym_send_data_1(channel, 0);
    ym_send_data_1(channel + 1, 0);
  }
}

void pinano_2(int f, int channel, bool on)
{
  channel = channel * 2;

  if (on == true)
  {
    led_flash(channel / 2 + 3, true);
    ym_send_data_2(channel, f & 0xff);
    ym_send_data_2(channel + 1, f >> 8);
  }
  else
  {
    led_flash(channel / 2 + 3, false);
    ym_send_data_2(channel, 0);
    ym_send_data_2(channel + 1, 0);
  }
}

void EnvelopeShape(int type, float time)
{
  float fE = 1 / time;
  int EP = masterClk / (256 * fE);
  ym_send_data_1(0x0b, EP & 0xff);
  ym_send_data_1(0x0c, (EP >> 8) & 0xff);
  int shape;
  switch (type)
  {
  case 1:
    shape = 0x08;
    break;
  case 2:
    shape = 0x09;
    break;
  case 3:
    shape = 0x0a;
    break;
  case 4:
    shape = 0x0b;
    break;
  case 5:
    shape = 0x0c;
    break;
  case 6:
    shape = 0x0d;
    break;
  case 7:
    shape = 0x0e;
    break;
  case 8:
    shape = 0x0f;
    break;
  default:
    shape = 0x08;
    break;
  }
  ym_send_data_1(0x0d, shape & 0x0f);
}

void drum()
{
  ym_send_data_1(7, 0xf0);
  ym_send_data_1(8, 0x1f);
  ym_send_data_1(9, 0x1f);

  ym_send_data_1(7, 0xf8);
  pinano_1(Np[24], 1, 1);
  EnvelopeShape(2, 0.3);
  delay(1000);
  ym_send_data_1(7, 0xf0);
  pinano_1(Np[24], 1, 0);
  EnvelopeShape(2, 0.3);
  delay(500);

  ym_send_data_1(7, 0xf8);
  pinano_1(Np[24], 1, 1);
  EnvelopeShape(2, 0.3);
  delay(500);
  ym_send_data_1(7, 0xf8);
  pinano_1(Np[24], 1, 1);
  EnvelopeShape(2, 0.3);
  delay(1000);
  ym_send_data_1(7, 0xf0);
  pinano_1(Np[24], 1, 0);
  EnvelopeShape(2, 0.3);
  delay(1000);
}

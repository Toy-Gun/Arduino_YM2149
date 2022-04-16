#include "ym2149.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "lcd.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); //配置LCD地址及行列

Position pos;

void setup()
{
  lcd.init();      //初始化LCD
  lcd.backlight(); //打开背光
  set_bus_ctl();
  for (int i = 0; i < 16; i++)
  {
    send_data(i, 0);
  }
  send_data(7, mix);
  send_data(8, 0x0f);
  send_data(9, 0x0f);
  send_data(10, 0x0f);

  display(lcd, pos);
}

void loop()
{
  panano(Note[64], 0, 300, 1);
  panano(Note[63], 0, 300, 1);

  panano(Note[64], 0, 300, 1);
  panano(Note[63], 0, 300, 1);
  panano(Note[64], 0, 300, 1);
  panano(Note[59], 0, 300, 1);
  panano(Note[62], 0, 300, 1);
  panano(Note[60], 0, 300, 1);

  panano(Note[57], 0, 900, 0);
  delay(300);
  panano(Note[48], 0, 300, 1);
  panano(Note[52], 0, 300, 1);
  panano(Note[57], 0, 300, 1);

  panano(Note[59], 0, 600, 0);
  delay(300);
  panano(Note[52], 0, 300, 1);
  panano(Note[56], 0, 300, 1);
  panano(Note[59], 0, 300, 1);

  panano(Note[60], 0, 900, 0);
  delay(300);
  panano(Note[52], 0, 300, 1);
  panano(Note[64], 0, 300, 1);
  panano(Note[63], 0, 300, 1);

  panano(Note[64], 0, 300, 1);
  panano(Note[63], 0, 300, 1);
  panano(Note[64], 0, 300, 1);
  panano(Note[59], 0, 300, 1);
  panano(Note[62], 0, 300, 1);
  panano(Note[60], 0, 300, 1);

  panano(Note[57], 0, 900, 0);
  delay(300);
  panano(Note[48], 0, 300, 1);
  panano(Note[52], 0, 300, 1);
  panano(Note[57], 0, 300, 1);

  panano(Note[59], 0, 600, 0);
  delay(300);
  panano(Note[52], 0, 300, 1);
  panano(Note[60], 0, 300, 1);
  panano(Note[59], 0, 300, 1);

  panano(Note[57], 0, 900, 0);
  delay(600);
}
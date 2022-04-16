#include <Arduino.h>
#line 1 "/home/wangjq/文档/ym2149/ym2149.ino"
#include "ym2149.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Notes
// Np = 2e6 / (16 * Fn)
// 24 - 120 C2 - C10
LiquidCrystal_I2C lcd(0x27, 16, 2); //配置LCD地址及行列

unsigned int i;

#line 12 "/home/wangjq/文档/ym2149/ym2149.ino"
void setup();
#line 44 "/home/wangjq/文档/ym2149/ym2149.ino"
void loop();
#line 12 "/home/wangjq/文档/ym2149/ym2149.ino"
void setup()
{
  lcd.init();      //初始化LCD
  lcd.backlight(); //打开背光
  Serial.begin(115200);

  ym_set_bus_ctl();

  // reset registers
  for (i = 0; i < 16; i++)
  {
    ym_send_data_1(i, 0);
    ym_send_data_2(i, 0);
    led_flash(i % 8, false);
  }

  // ym_send_data(7, 0xf8); // Only output clear sound
  ym_send_data_1(7, 0xf8);
  ym_send_data_1(8, 0x0f);
  ym_send_data_1(9, 0x0f);
  ym_send_data_1(10, 0x0f);

  ym_send_data_2(7, 0xf8);
  ym_send_data_2(8, 0x0f);
  ym_send_data_2(9, 0x0f);
  ym_send_data_2(10, 0x0f);

  pinMode(13, HIGH);
  lcd.setCursor(0, 0);     //设置显示位置
  lcd.print("PLAYING..."); //显示字符数据
}
int f = 0, c, trk, n, on;
void loop()
{
  if (Serial.available())
  {
    c = Serial.read();
    Serial.print(c);
    if (c == '~')
    {
      trk = Serial.read();
      Serial.print(trk);

      n = Serial.read();
      Serial.print(n);

      on = Serial.read();
      Serial.print(on);

      if (trk < 3)
      {
        pinano_1(Np[n % 127], trk, on % 2);
      }
      else
      {
        pinano_2(Np[n % 127], trk % 3, on % 2);
      }
    }
  }
}


#line 1 "/home/wangjq/文档/ymplayer/lcd.h"
#include <LiquidCrystal_I2C.h>

struct Position
{
    int x = 0;
    int y = 0;
};

void display(LiquidCrystal_I2C lcd, Position pos)
{
    lcd.setCursor(pos.x, pos.y);     //设置显示位置
    lcd.print("|C#4|D 4|E#4");       //显示字符数据
    lcd.setCursor(pos.x, pos.y + 1); //设置显示位置
    lcd.print("|120| 96|120");       //显示字符数据
}
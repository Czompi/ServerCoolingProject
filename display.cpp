#include<LiquidCrystal.h>
#include <Arduino.h>
#include"display.h"
#include"chars.h"
#include"config.h"
Display::Display() {
    init();
}

int Display::printTemperature(int row, int col, byte name) {
    lcd.setCursor(col, row);
    lcd.write(name);
    lcd.setCursor(col, row);
    lcd.write(DEGREE);
    lcd.print("C");
    return col + 1;
}
void Display::init() {
    lcd.createChar((int)DEGREE, degChar);
    lcd.createChar((int)RPM, rpmChar);
    lcd.createChar((int)INTAKE, intakeChar);
    lcd.createChar((int)EXHAUST, exhaustChar);
    lcd.createChar((int)AMBIENT, ambientChar);
}
void Display::setup() {
    lcd.begin(16,2);
    lcd.home();
    int _int = printTemperature(0,0,INTAKE);
    int _exh = printTemperature(0,1,EXHAUST);
    int _amb = printTemperature(_int+1,0,EXHAUST);
    lcd.setCursor(12,1);
    lcd.print("v1.0");
}

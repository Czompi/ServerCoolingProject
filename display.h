#include <LiquidCrystal.h>
#include <Arduino.h>

class Display {
    public:
        Display();
        
        void setup();
        void init();
        int printTemperature(int row, int col, byte name);
    private:
};

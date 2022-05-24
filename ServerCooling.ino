#include <dht11.h>
#include <Arduino.h>
#include <LiquidCrystal.h>
#include"display.h"
#include"config.h"

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// LCD control
Display display(lcd);

dht11 DHT11;
const word PWM_FREQ_HZ = 25000; //Adjust this value to adjust the frequency
const word TCNT1_TOP = 16000000/(2*PWM_FREQ_HZ);

int half_revolutions = 0;

void setup() {
  // configure LED PWM functionalitites
  ledcSetup(PWM_CHNL, PWM_FREQ_HZ, 8);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(PWM_PIN, PWM_CHNL);
  pinMode(TAC_PIN, INPUT); //set RPM pin to digital input
  half_revolutions = 0;
  
  display.setup();
  
  Serial.begin(9600);
}
TaskHandle_t FanControlTask;
void loop() {
  internalLogic();
  //xTaskCreatePinnedToCore(fanControlTask, "FanControlTask",    5000,      NULL,    2,    &FanControlTask,    0);
  delay(1000);
}
float temp_ei_delta =0.0f;
float temp_ea_delta = 0.0f;
void fanControlTask(void*) {
  float duty = 0.0f;
  if(temp_ei_delta >= 5) duty +=.25f;
  if(temp_ei_delta >= 10) duty +=.25f;
  if(temp_ei_delta >= 15) duty +=.25f;
  if(temp_ei_delta >= 25) duty +=.25f;
}
void internalLogic() {
  float temp_ambient = getTemp(TMP_0);
  float temp_intake = getTemp(TMP_1);
  float temp_exhaust = getTemp(TMP_2);
  
  temp_ei_delta = temp_exhaust-temp_intake;
  temp_ea_delta = temp_exhaust-temp_ambient;

  float duty = 0.0f;
  if(temp_ei_delta >= 5) duty +=.25f;
  if(temp_ei_delta >= 10) duty +=.25f;
  if(temp_ei_delta >= 15) duty +=.25f;
  if(temp_ei_delta >= 20) duty +=.25f;
  float fine_duty = (temp_ei_delta > 20 ? 20: temp_ei_delta) / 20;
  if(temp_ei_delta < 5) fine_duty = 0.0f;
  int max_rpm = 2200;
  int pwm_value = setFanSpeed(fine_duty);
  int fan_speed = getFanSpeed(max_rpm);

  // #region Debug
  Serial.print("Ambient: ");
  Serial.print(temp_ambient, 2);
  Serial.print(" °C; Intake: ");
  Serial.print(temp_intake, 2);
  Serial.print(" °C; Exhaust: ");
  Serial.print(temp_exhaust, 2);
  Serial.println(" °C");

  Serial.print("Fan speed: ");
  Serial.print(fan_speed);
  Serial.println(" rpm");
  Serial.println(temp_ei_delta);
  Serial.println(duty);
  // #endregion debug
}

int setFanSpeed(float duty) {
  if(duty > 1.0f) duty = 1.0f;
  int pwm_value = (int)(255 * duty);
  ledcWrite(PWM_CHNL, pwm_value);
  return pwm_value;
}

int getFanSpeed(int max_rpm) {
  int rpm = 0;
  sei(); //enable intterupts
  attachInterrupt(digitalPinToInterrupt(TAC_PIN), fan_rpm, RISING); //record pulses as they rise
  delay(10000);
  detachInterrupt(digitalPinToInterrupt(TAC_PIN));
  cli(); //disable intterupts
  
  rpm = (half_revolutions/2)*6; 

  half_revolutions = 0;
  return rpm;
}
void fan_rpm()
{
   ++half_revolutions; //increment before returning value 
 }
float getTemp(int pin) {
  int chk = DHT11.read(pin);
  return (float)DHT11.temperature;
}

/*
 * rgb.h
 *
 *  Created on: 17-Jan-2017
 *      Author: abhi
 */

#ifndef ARDUINORGBLED_H_
#define ARDUINORGBLED_H_

#include<inttypes.h>
#include <math.h>
#include <Arduino.h>
#include <ArduinoList.h>

#define PWM_MAX 1024

extern "C" {
#include "user_interface.h"
}

/**
 * These should be moved to Animator
 */
typedef enum {

	RGB_DROWSY,
  RGB_NONE

}RGB_MODE;


typedef enum{
	RGB_VIOLET,
	RGB_INDIGO,
	RGB_BLUE,
	RGB_GREEN,
	RGB_YELLOW,
	RGB_ORANGE,
	RGB_RED,
	RGB_WHITE
}RGB_COLOR;

class ArduinoRGBLed {
public:

  static ArduinoList<ArduinoRGBLed *> _rgbLeds;
	/*
	 * Desired color and mode
	 */
	int red;
	int blue;
	int green;
	RGB_MODE mode;
	RGB_COLOR color;

	float brightness;
	float brightness_change_per_step;

	int drowsy_period_ms;
	/*
	 * Actual values currently existing in PWM
	 */

	int red_pwm; float red_increment_per_step;
	int blue_pwm; float blue_increment_per_step;
	int green_pwm; float green_increment_per_step;
	int state_transition_time_ms;

	int transition_step;
/*
 * 	Hardware variables
 */
	uint8_t red_pin;
	uint8_t green_pin;
	uint8_t blue_pin;

	//HardwareTimer *timer;
  static os_timer_t* fsmTimer;
	uint32_t fsm_period_ms;


	ArduinoRGBLed(uint8_t red_p,uint8_t green_p,uint8_t blue_p,uint32_t fsm_ms=20);
	void setMode(RGB_MODE mode) { this->mode = mode; }
	void setBrightness(float b) { brightness = b; }

	void setColor(uint8_t red, uint8_t green, uint8_t blue,int transit_time_ms);
	void setColor(RGB_COLOR color,int transit_time = 10000);

	RGB_COLOR getColor() { return color; }

  static void timerCallback(void *arg);

private:
	void fsm(void);             /** Called by timer callback **/
	int getTransitionSteps();

};


//void timer_handler();



/*HardwareTimer* timer_setup(ulong period_us){
	HardwareTimer *timer = new HardwareTimer(1);
	timer->pause();
	timer->setPeriod(period_us);
	timer->setChannel1Mode(TIMER_OUTPUT_COMPARE);
	timer->setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
	timer->attachCompare1Interrupt(timer_handler);
	timer->resume();
	return timer;
}*/

/*void timer_handler(){

}*/


#endif /* ARDUINORGBLED_H_ */

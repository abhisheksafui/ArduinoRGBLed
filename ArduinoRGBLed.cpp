/*
 * rgb.cpp
 *
 *  Created on: 22-Jan-2017
 *      Author: abhi
 */
#include "RGBLed.h"


/*
 * Constructor:
 *
 * Parameters: pins for leds
 */

RGB::RGB(uint8_t red_p,uint8_t green_p,uint8_t blue_p,uint32_t fsm_ms){
		red_pin = red_p;
		green_pin = green_p;
		blue_pin = blue_p;

		red_pwm = green_pwm = blue_pwm = 0;

		red = blue = green = 0;
		mode = RGB_CONSTANT;
		/*
		 * init pins
		 */
		//pinMode(red_pin,PWM);
		//pinMode(green_pin,PWM);
		//pinMode(blue_pin,PWM);

		analogWrite(red_pin,0);
		analogWrite(green_pin,0);
		analogWrite(blue_pin,0);

		/*
		 * init fsm_period
		 */
		fsm_period_ms = fsm_ms;

		/*
		 * set default mode
		 */
		mode = RGB_DROWSY;
		/*
		 * Drowsy ness period
		 */
		drowsy_period_ms = 2000;

}
void RGB::setColor(uint8_t red, uint8_t green, uint8_t blue,int transit_time_ms){
/*
 * set final color
 */
	this->red = red;
	this->blue = blue;
	this->green = green;
	/*
	 * set the transition time for step calculation
	 * steps = desired_transiion_time/fsm call period
	 */
	if(transit_time_ms <= fsm_period_ms){
		state_transition_time_ms = fsm_period_ms;
	}else{
		state_transition_time_ms = transit_time_ms;
	}

	/*
	 * set the increment per step
	 */
	//if(mode==RGB_TRANSITION){
			red_increment_per_step = ((float)(red-red_pwm))/getTransitionSteps();
			blue_increment_per_step = ((float)(blue-blue_pwm))/getTransitionSteps();
			green_increment_per_step = ((float)(green-green_pwm))/getTransitionSteps();
			transition_step = 1;
	//}

	Serial.print(red_increment_per_step ); Serial.print(" <-red ");
	Serial.print(green_increment_per_step); Serial.print(" <-green ");
	Serial.print(blue_increment_per_step); Serial.println(" <-blue ");

	Serial.print(red_pwm ); Serial.print(" <-red ");
	Serial.print(green_pwm); Serial.print(" <-green ");
	Serial.print(blue_pwm); Serial.println(" <-blue ");

}

void RGB::setColor(RGB_COLOR cl,int transit_time_ms){
/*
 * set final color
 */
	color = cl;
	switch(cl){
	case RGB_RED:
		red = 0xff;
		green = 0x00;
		blue = 0x00;
		break;
	case RGB_GREEN:
		red = 0x00;
		green = 0xff;
		blue =0x00;
		break;
	case RGB_BLUE:
		red = 0x00;
		green = 0x00;
		blue = 0xff;
		break;
	case RGB_WHITE:
		red = 0xff;
		green = 0xff;
		blue = 0xff;
		break;
	case RGB_YELLOW:
		red = 0xff;
		green = 0xff;
		blue = 0x00;
		break;
	case RGB_VIOLET:
		red = 0xEE;
		green = 0x82;
		blue = 0xEE;
		break;
	case RGB_INDIGO:
		red = 0x4b;
		green = 0x00;
		blue = 0x82;
		break;

	}
	/*
	 * set the transition time for step calculation
	 * steps = desired_transiion_time/fsm call period
	 */
	if(transit_time_ms <= fsm_period_ms){
		state_transition_time_ms = fsm_period_ms;
	}else{
		state_transition_time_ms = transit_time_ms;
	}

	/*
	 * set the increment per step
	 */
	//if(mode==RGB_TRANSITION){
			red_increment_per_step = ((float)(red-red_pwm))/getTransitionSteps();
			blue_increment_per_step = ((float)(blue-blue_pwm))/getTransitionSteps();
			green_increment_per_step = ((float)(green-green_pwm))/getTransitionSteps();
			transition_step = 1;
	//}

	Serial.print(red_increment_per_step ); Serial.print(" <-red ");
	Serial.print(green_increment_per_step); Serial.print(" <-green ");
	Serial.print(blue_increment_per_step); Serial.println(" <-blue ");

	Serial.print(red_pwm ); Serial.print(" <-red ");
	Serial.print(green_pwm); Serial.print(" <-green ");
	Serial.print(blue_pwm); Serial.println(" <-blue ");

}

int RGB::getTransitionSteps(){
	return (state_transition_time_ms/fsm_period_ms);
}

void RGB::fsm(){
	int tmp_r, tmp_g, tmp_b;

	if(mode == RGB_DROWSY){
		/*
		 *  This mode increases or decreases brightness of led periodically
		 */
		brightness += brightness_change_per_step;
		if(brightness >= 1){
			brightness_change_per_step = -1.0/(drowsy_period_ms/fsm_period_ms);
			brightness = 1.0;
		}else if(brightness <= 0){
			brightness_change_per_step = 1.0/(drowsy_period_ms/fsm_period_ms)*0.3;
			brightness = 0.0;
		}


		Serial.print("\nbrightness");Serial.println(brightness);
	}
/*
 * transition_step is set to 1 on every color change request and is set to zero by fsm
 * on color change completion (either in one step or multiple steps)
 */
	if(transition_step)
	{
		/*
		 * Calculate pwm for this step.
		 *
		 * If you did not request transition then number of transition steps is 1
		 * so current value (red_pwm) will become same as requested value (red) in a single step
		 *
		 */
		tmp_r = red_pwm + round(transition_step * red_increment_per_step);
		tmp_b = blue_pwm + round(transition_step * blue_increment_per_step);
		tmp_g = green_pwm + round(transition_step * green_increment_per_step);
		if(transition_step == getTransitionSteps())
		{
			/*
			 * This is the final step of the transition. so update red_pwm, which will serve as
			 * LAST State for next transition
			 */
			transition_step = 0;
			red_increment_per_step = 0;
			green_increment_per_step = 0;
			blue_increment_per_step = 0;

			red_pwm = red;
			green_pwm = green;
			blue_pwm = blue;
		}else{
			transition_step++;
		}

	}else{

		/*
		 * Currently transition is not going on as transition time is over. So Now red_pwm is same as red
		 */
		tmp_r = red_pwm;
		tmp_b = blue_pwm;
		tmp_g = green_pwm;
	}

	/*
	 * The pwm value for this step has been calculated by now.
	 * Set the pwm for this step
	 */
	analogWrite(red_pin,(tmp_r/255.0)*PWM_MAX * brightness);
	analogWrite(blue_pin,(tmp_b/255.0)*PWM_MAX * brightness);
	analogWrite(green_pin,(tmp_g/255.0)*PWM_MAX * brightness);



}




#include "APM.h"

namespace APM {
	void init() {
		/*
		//Init PWM Timer 1
		pinMode(11,OUTPUT); //     (PB5/OC1A)
		pinMode(12,OUTPUT); //OUT2 (PB6/OC1B)
		pinMode(13,OUTPUT); //OUT3 (PB7/OC1C)
	
		//Remember the registers not declared here remains zero by default... 
		TCCR1A =((1<<WGM11)|(1<<COM1A1)|(1<<COM1B1)|(1<<COM1C1)); //Please read page 131 of DataSheet, we are changing the registers settings of WGM11,COM1B1,COM1A1 to 1 thats all... 
		TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11); //Prescaler set to 8, that give us a resolution of 0.5us, read page 134 of data sheet
		OCR1A = 3000; //PB5, none
		OCR1B = 3000; //PB6, OUT2
		OCR1C = 3000; //PB7  OUT3
		ICR1 = 40000; //50hz freq...Datasheet says  (system_freq/prescaler)/target frequency. So (16000000hz/8)/50hz=40000,
	
		// Init PWM Timer 3
		pinMode(2,OUTPUT); //OUT7 (PE4/OC3B)
		pinMode(3,OUTPUT); //OUT6 (PE5/OC3C)
		pinMode(4,OUTPUT); //     (PE3/OC3A)
		TCCR3A =((1<<WGM31)|(1<<COM3A1)|(1<<COM3B1)|(1<<COM3C1));
		TCCR3B = (1<<WGM33)|(1<<WGM32)|(1<<CS31); 
		OCR3A = 3000; //PE3, NONE
		OCR3B = 3000; //PE4, OUT7
		OCR3C = 3000; //PE5, OUT6
		ICR3 = 40000; //50hz freq
		*/
	
		// Init PWM Timer 5
		pinMode(44,OUTPUT);  //OUT1 (PL5/OC5C)
		pinMode(45,OUTPUT);  //OUT0 (PL4/OC5B)
		pinMode(46,OUTPUT);  //     (PL3/OC5A)
	
		TCCR5A =((1<<WGM51)|(1<<COM5A1)|(1<<COM5B1)|(1<<COM5C1)); 
		TCCR5B = (1<<WGM53)|(1<<WGM52)|(1<<CS51);
		OCR5A = 3000; //PL3, 
		OCR5B = 3000; //PL4, OUT0
		OCR5C = 3000; //PL5, OUT1
		ICR5 = 40000; //50hz freq
	
		// Init PPM input and PWM Timer 4
		pinMode(49, INPUT);  // ICP4 pin (PL0) (PPM input)
		/*pinMode(7,OUTPUT);   //OUT5 (PH4/OC4B)
		pinMode(8,OUTPUT);   //OUT4 (PH5/OC4C)
	
		TCCR4A =((1<<WGM40)|(1<<WGM41)|(1<<COM4C1)|(1<<COM4B1)|(1<<COM4A1)); */ 
		//Prescaler set to 8, that give us a resolution of 0.5us
		// Input Capture rising edge
		TCCR4B = ((1<<WGM43)|(1<<WGM42)|(1<<CS41)|(1<<ICES4));
	
		OCR4A = 40000; ///50hz freq.
		OCR4B = 3000; //PH4, OUT5
		OCR4C = 3000; //PH5, OUT4
	
		//TCCR4B |=(1<<ICES4); //Changing edge detector (rising edge). 
		//TCCR4B &=(~(1<<ICES4)); //Changing edge detector. (falling edge)
		TIMSK4 |= (1<<ICIE4); // Enable Input Capture interrupt. Timer interrupt mask
	}
		
	void outputPWM(uint8_t channel, int16_t pwm) {
		pwm=constrain(pwm,1000,2000);
		pwm<<=1;   // pwm*2;

		switch(channel)
		{
			case 0:  OCR5B=pwm; break;  //ch0
			case 1:  OCR5C=pwm; break;  //ch1
			case 2:  OCR1B=pwm; break;  //ch2
			case 3:  OCR1C=pwm; break;  //ch3
			case 4:  OCR4C=pwm; break;  //ch4
			case 5:  OCR4B=pwm; break;  //ch5
			case 6:  OCR3C=pwm; break;  //ch6
			case 7:  OCR3B=pwm; break;  //ch7
			case 8:  OCR5A=pwm; break;  //ch8,  PL3
			case 9:  OCR1A=pwm; break;  //ch9,  PB5
			case 10: OCR3A=pwm; break;  //ch10, PE3
		} 
	}
	
	int16_t getPWM(uint8_t channel) {
		switch(channel)
		{
			case 0:  return OCR5B/2;  //ch0
			case 1:  return OCR5C/2;  //ch1
			case 2:  return OCR1B/2;  //ch2
			case 3:  return OCR1C/2;  //ch3
			case 4:  return OCR4C/2;  //ch4
			case 5:  return OCR4B/2;  //ch5
			case 6:  return OCR3C/2;  //ch6
			case 7:  return OCR3B/2;  //ch7
			case 8:  return OCR5A/2;  //ch8,  PL3
			case 9:  return OCR1A/2;  //ch9,  PB5
			case 10: return OCR3A/2;  //ch10, PE3
		} 
		return 0;
	}
}
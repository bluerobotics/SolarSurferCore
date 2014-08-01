#include <avr/interrupt.h>
#include "APM.h"

// Variable definition for Input Capture interrupt
volatile uint16_t ICR5_old;
volatile uint8_t PPM_Counter=0;
volatile uint16_t PWM_RAW[8] = {2400,2400,2400,2400,2400,2400,2400,2400};
volatile uint8_t radio_status=0;

//////////////////////////////////////////////////////
//   Input Capture Interrupt ICP4 => PPM signal read
//////////////////////////////////////////////////////
ISR(TIMER5_CAPT_vect) {
	static const uint8_t NUM_CHANNELS = 8;
  uint16_t Pulse;
  uint16_t Pulse_Width;
  
  Pulse=ICR5;
  if (Pulse<ICR5_old) {    // Take care of the overflow of Timer4 (TOP=40000)
    Pulse_Width=(Pulse + 40000)-ICR5_old;  //Calculating pulse 
  } else {
    Pulse_Width=Pulse-ICR5_old;            //Calculating pulse 
  }
  if (Pulse_Width>8000) {   // SYNC pulse?
    PPM_Counter=0;
    radio_status = 1;
  } else {
    if (PPM_Counter < NUM_CHANNELS) {         // Valid pulse channel?
      PWM_RAW[PPM_Counter++]=Pulse_Width;     // Saving pulse.

      if (PPM_Counter >= NUM_CHANNELS) {
        radio_status = 1;
      }
    }

  }
  ICR5_old = Pulse;
}

namespace APM {
	void init() {
		
		// Timer 1 for Ch 1 and Ch 2
		pinMode(12,OUTPUT);
		pinMode(11,OUTPUT);

		// WGM: 1 1 1 0. Clear Timer on Compare, TOP is ICR1.
    // CS11: prescale by 8 => 0.5us tick
    TCCR1A = ((1<<WGM11)|(1<<COM1A1)|(1<<COM1B1));
    TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
    ICR1 = 40000; // 0.5us tick => 50hz freq
    OCR1A = 0xFFFF; // Ch 2, Init OCR registers to nil output signal
    OCR1B = 0xFFFF; // Ch 1

    cli();
		
		// Set up PPM capture and Ch 10, 11
		pinMode(48,INPUT);		
		pinMode(45,OUTPUT);
		pinMode(44,OUTPUT);
		
		TCNT5 = 0;
		TIFR5 = 0;
		
		TCCR5A = _BV(WGM50) | _BV(WGM51);
		TCCR5B = _BV(WGM53) | _BV(WGM52) | _BV(CS51) | _BV(ICES5);
		OCR5A = 40000-1;
		OCR5B = 0xFFFF;
		OCR5C = 0xFFFF;
		
		TIMSK5 |= _BV(ICIE5);

		sei();
	}
		
	void outputPWM(uint8_t channel, int16_t pwm) {
		pwm=constrain(pwm,1000,2000);
		pwm<<=1;   // pwm*2;

		switch(channel)
		{
			case 0:  OCR1B=pwm; break;  //ch1
			case 1:  OCR1A=pwm; break;  //ch2
		} 
	}
	
	int16_t getPWM(uint8_t channel) {
		switch(channel)
		{
			case 0:  return OCR1B/2;  //ch1
			case 1:  return OCR1A/2;  //ch2
		} 
		return 0;
	}
	
	uint16_t inputCh(uint8_t channel)	{
			uint16_t result;

			// we need to stop interrupts to be sure we get a correct 16 bit value
			cli();
			result = PWM_RAW[channel];
			sei();

			// Because timer runs at 0.5us we need to do value/2
			result >>= 1;

			// Limit values to a valid range
			result = constrain(result,1000,2000);
			radio_status = 0; // Radio channel read
			return result;
	}
	
	uint8_t getRadioInputState() {
		return radio_status;
	}

	float getVCC() {
	  // Read 1.1V reference against AVcc
	  // set the reference to Vcc and the measurement to the internal 1.1V reference
	  ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
	  ADCSRB &= ~_BV(MUX5);
	  
	  delay(2); // Wait for Vref to settle
	  ADCSRA |= _BV(ADSC); // Start conversion
	  while (bit_is_set(ADCSRA,ADSC)); // measuring
	 
	  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
	  uint8_t high = ADCH; // unlocks both
	 
	  long result = (high<<8) | low;
	 
	  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
	  return result/1000.0f; // Vcc in millivolts
	}

	float getVoltage() {
		static const uint8_t voltagePin = A13;
		static const float voltageDivider = 0.099;
		static const float tau = 4.0;
		static bool initialized = false;
		static float voltage;

		static uint32_t timer;

		float dt = (millis()-timer)/1000.0f;
		timer = millis();

		float alpha = (dt/(tau+dt));

		float vcc = getVCC();

		if ( !initialized ) {
			initialized = true;
			voltage = analogRead(voltagePin)*vcc/1023.0/voltageDivider;
		}

		voltage = voltage*(1-alpha)+analogRead(voltagePin)*vcc/1023.0/voltageDivider*alpha;

		return voltage;
	}

	float getCorrectedVoltage() {
		static const float R_supply = 0.11; // ohms

	  // Correct the voltage measurement for voltage drop due to current draw
	  return getVoltage() + getCurrent()*R_supply;
	}

	float getCurrent() {
		static const uint8_t currentPin = A12;
		static const float ampsPerVolt = 18.0018;
		static const float tau = 4.0;
		static bool initialized = false;
		static float current;

		static uint32_t timer;

		float dt = (millis()-timer)/1000.0f;
		timer = millis();

		float alpha = (dt/(tau+dt));

		float vcc = getVCC();

		if ( !initialized ) {
			initialized = true;
			current = analogRead(currentPin)*vcc/1023.0*ampsPerVolt;
		}

		current = current*(1-alpha)+analogRead(currentPin)*vcc/1023.0*ampsPerVolt*alpha;

		return current;
	}

	float getPower() {
		return getVoltage()*getCurrent();
	}
}
/*
 * Wendy_Rev5.c
 *
 * Updated: 5/13/2015 11:18:05 AM
 * Author: Matthew F. Borowski 
 * Copyright:  2015 Magurobotics LLC 
 */ 

#	define F_CPU 2000000UL //2MHz Unsigned Long

// *** INCLUDES ***
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <inttypes.h>

// *** DEFINITIONS ***
#	define BIT_SET(REGISTER,PIN) ((REGISTER) |= (1<<(PIN)))
#	define BIT_CLEAR(REGISTER,PIN) ((REGISTER) &= ~(1<<(PIN)))
#	define BIT_FLIP(REGISTER,PIN) ((REGISTER) ^= (1<<(PIN)))
#	define BIT_CHECK(REGISTER,PIN) ((REGISTER) & (1<<(PIN)))

typedef struct LEDs{
	bool LED1 : 1;
	bool LED2 : 1;
	bool LED3 : 1;
	};
// *** FUNCTIONS ***

// Put the Microcontroller to Sleep in Power Down Mode
static inline void _sleep(void){
	BIT_SET(MCUCR, SE); // Sleep Enable
	BIT_SET(MCUCR, SM1); // Idle sleep mode 10 (Power Down)
	BIT_CLEAR(MCUCR, SM0); // Idle sleep mode 10 
}

static inline void _powerSaveMode_ON(void){
	BIT_CLEAR(PORTA, PA7); // Disable Power Supply
	BIT_CLEAR(ADCSRA, ADEN); // ADC Enable bit OFF
}

static inline void _ready_H2O(void){
	BIT_SET(PORTA, PA1); // Close Water Sensing Circuit
	BIT_CLEAR(PORTB, PB4); // Open Charge on Electrode A
	BIT_CLEAR(PORTB, PB5); // Open Charge on Electrode B
}

static inline void _ready_Charge(void){
	BIT_CLEAR(PORTA, PA1); // Open Water Sensing Circuit
	BIT_CLEAR(PORTB, PB4); // Open Charge on Electrode A
	BIT_CLEAR(PORTB, PB5); // Open Charge on Electrode B
}

static inline void _displayLEDs(LED1, LED2, LED3){
	if (LED1 == 1){
	BIT_SET(PORTA, PA4);}
	else{BIT_CLEAR(PORTA, PA4);}
	
	if (LED2 == 1){
	BIT_SET(PORTA, PA5);}
	else{BIT_CLEAR(PORTA, PA5);}
	
	if (LED3 == 1){
	BIT_SET(PORTA, PA6);}
	else{BIT_CLEAR(PORTA, PA6);}
}



int main(void){
	// ********** Initialization ********** //
	
	/*
 * Pin Configuration
 * -----------------
 * PA0 -- ADC0 (V_battery)
 * PA1 -- Output (Ready_H2O)
 * PA2 -- INT1 (PowerIn_ElectrodeB)
 * PA3 -- AREF (+5V from Power Supply)
 * PA4 -- OUTPUT (LED_1_EN)
 * PA5 -- OUTPUT (LED_2_EN)
 * PA6 -- OUTPUT (LED_3_EN)
 * PA7 -- OUTPUT (PS_EN)
 * PB0 -- PCINT8 (In_H2O)
 * PB1 -- OUTPUT (Battery_Connected_To_Load)
 * PB2 -- NOT USED
 * PB3 -- OUTPUT (Motor_EN)
 * PB4 -- OUTPUT (Charge_ElectrodeA)
 * PB5 -- OUTPUT (Charge_ElectrodeB)
 * PB6 -- INT0 (PowerIn_ElectrodeA)
 * PB7 -- OUTPUT (RESET) <-- Active Low
 */
	
	// Set I/O configuration
	DDRA = 0b11110010;	// PORT A I/O Config -- 1 = OUTPUT, 0 = INPUT
	PORTA = (0b10000010);  // 1 on Output enables output, 1 on Input enables Pull-up
	DDRB = 0b10111010;	// PORT B I/O Config -- 1 = OUTPUT, 0 = INPUT
	PORTB = (0b10000000); // 1 on Output enables output, 1 on Input enables Pull-up

	// ********** Main Loop ********** //
	while(1){
		_sleep();
		_powerSaveMode_ON();
		
		if bit_is_set(PORTB, PB1){
			_ready_H2O(); // Device is ready for water if the UnderVoltageInterrupt is closed
		}
		else if bit_is_clear(PORTB, PB1){
			_ready_Charge(); // Device needs charger if the UnderVoltageInterrupt is open
		}
	}
}
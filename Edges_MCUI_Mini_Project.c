#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//=============================================================
//MCUCR is for choosing edge style
//GICR is for module interrupt enable
unsigned char seconds = 0, minutes = 0, hours = 0;
unsigned char mode = 1;  // 1 = Count-up, 0 = Countdown
unsigned char operation = 1;  // 1 = Running, 0 = Paused
//=============================================================
void Data_Registers_Config(void);
void Display_Time(void);
void settings(void);
//==============================================================
void External_Interrupts_Init() {
	// INT0 (Reset) - Falling edge
	MCUCR |= (1 << ISC01);
	GICR |= (1 << INT0);

	// INT1 (Pause) - Rising edge
	MCUCR |= (1 << ISC11) | (1 << ISC10);
	GICR |= (1 << INT1);

	// INT2 (Resume) -Default Falling edge
	GICR |= (1 << INT2);

	sei();  // Enable global interrupts
}
void Timer1_INIT(void) {  //counting

	TCCR1B |= (1 << WGM12);  //CTC mode enable
	TCCR1B |= (1 << CS12) | (1 << CS10);  //1024 prescalar
	OCR1A = 15624;  //from calculating the formula
	TIMSK |= (1 << OCIE1A);  //module int. enable
	sei();

}
//=============================================================
ISR(INT0_vect) {
//reset
	seconds = 0;
	minutes = 0;
	hours = 0;
}
ISR(INT1_vect) {
//pause
	TCCR1B &= ~(1 << CS12) & ~(1 << CS11) & ~(1 << CS10);  // Stop the timer
	operation = 0;

}
ISR(INT2_vect) {
//resume
	TCCR1B |= (1 << CS12) | (1 << CS10); // Resume the timer
	operation = 1;
}
ISR(TIMER1_COMPA_vect) {

	if (operation) {  // Only update time if not paused
		PORTD &= ~(1 << PD0);  // Turn off buzzer
		if (mode == 1) {  // Count-up mode
			seconds++;
			if (seconds == 60) {
				seconds = 0;
				minutes++;
				if (minutes == 60) {
					minutes = 0;
					hours++;
					if (hours == 24) {
						hours = 0;
					}
				}
			}
		} else {  // Countdown mode
			if (seconds == 0) {
				if (minutes == 0) {
					if (hours == 0) {
						// Countdown finished, trigger buzzer
						PORTD |= (1 << PD0);  // Activate buzzer
						operation = 0;  // Pause the timer
					} else {
						hours--;
						minutes = 59;
						seconds = 59;
					}
				} else {
					minutes--;
					seconds = 59;
				}
			} else {
				seconds--;
			}
		}
	}
}
//=============================================================
int main(void) {

	Data_Registers_Config();
	Timer1_INIT();
	External_Interrupts_Init();

	while (1) {

		Display_Time();
		settings();

	}
}
//=============================================================
void settings(void) {
	// Mode Toggle (PB7)
	if (!(PINB & (1 << 7))) {
		operation = 1;
		mode = !mode;  // Toggle mode
		if (mode == 1) {
			PORTD |= (1 << PD4);  // Turn on Count-up LED
			PORTD &= ~(1 << PD5);  // Turn off Countdown LED
		} else {
			PORTD |= (1 << PD5);  // Turn on Countdown LED
			PORTD &= ~(1 << PD4);  // Turn off Count-up LED
		}
		while (!(PINB & (1 << 7))) {  // Wait for button release
			Display_Time();
		}
	}

	// Hours Increment (PB1)
	if (!(PINB & (1 << 1))) {
		hours++;
		if (hours == 99 && minutes == 59 && seconds == 59) {
			seconds = 0;
			minutes = 0;
			hours = 0;
		}
		while (!(PINB & (1 << 1))) {  // Wait for button release
			Display_Time();
		}
	}

	// Hours Decrement (PB0)
	if (!(PINB & (1 << 0))) {
		if (hours == 0) {
			hours = 0;  // Prevent underflow
		} else {
			hours--;
		}
		while (!(PINB & (1 << 0))) {  // Wait for button release
			Display_Time();
		}
	}

	// Minutes Increment (PB4)
	if (!(PINB & (1 << 4))) {
		minutes++;
		if (minutes == 60) {
			minutes = 0;
		}
		while (!(PINB & (1 << 4))) {  // Wait for button release
			Display_Time();
		}
	}

	// Minutes Decrement (PB3)
	if (!(PINB & (1 << 3))) {
		if (minutes == 0) {
			minutes = 0;  // Prevent underflow
		} else {
			minutes--;
		}
		while (!(PINB & (1 << 3))) {  // Wait for button release
			Display_Time();
		}
	}

	// Seconds Increment (PB6)
	if (!(PINB & (1 << 6))) {
		seconds++;
		if (seconds == 60) {
			seconds = 0;
		}
		while (!(PINB & (1 << 6))) {  // Wait for button release
			Display_Time();
		}
	}

	// Seconds Decrement (PB5)
	if (!(PINB & (1 << 5))) {
		if (seconds == 0) {
			seconds = 0;  // Prevent underflow
		} else {
			seconds--;
		}
		while (!(PINB & (1 << 5))) {  // Wait for button release
			Display_Time();
		}
	}
}
void Display_Time(void) {
	uint8_t digits[6];
	digits[0] = hours / 10;
	digits[1] = hours % 10;
	digits[2] = minutes / 10;
	digits[3] = minutes % 10;
	digits[4] = seconds / 10;
	digits[5] = seconds % 10;

	for (uint8_t i = 0; i < 6; i++) {
		PORTA = (1 << i);  // Enable the current display
		PORTC = digits[i];  // Send the digit to the 7447 decoder
		_delay_ms(2);  // Small delay for multiplexing
	}

}
void Data_Registers_Config(void) {
	//push button and LEDs initialization
	DDRA |= 0x3F;  // Set PORTA[5:0] as output for enabling displays
	PORTA &= ~0x3F;  // Disable all displays initially (active low)

	DDRC |= 0x0F; // Set PORTC[3:0] as output for sending digits to the 7447 decoder
	PORTC = 0x00;  // Clear PORTC initially

	PORTB |= 0xFF;  //PUSH BUTTONS pull up INITIALIZATION

	DDRD |= 0x31; //set pin 0,4,5 output
	PORTD |= 0x0C; //set pin 2,3 for pullup config.
	PORTD |= (1 << 4);

}

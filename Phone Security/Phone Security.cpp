
#define F_CPU 4000000UL  // 4 MHz


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>



#define INP_PORT PORTB
#define INP_PORTDDR DDRB
#define INP_PORTPIN PINB

#define ALM_PORT PORTC
#define ALM_PORTPIN PINC
#define ALM_PORTDDR DDRC

#define LED_PORT PORTD
#define LED_PORTDDR DDRD
#define LED_PORTPIN PIND
#define  BLINK_PERIOD 100

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

uint8_t locked=0;
uint8_t status=0;
uint8_t oldstatus=0xff;
uint8_t alarmMode=0;			//

void Blink_On(int b){			//Blink and then turn on
	
	for(int i=0;i<3;i++){
		BIT_CLEAR(LED_PORT,b);
		_delay_ms(BLINK_PERIOD);
		
		BIT_SET(LED_PORT,b);
		_delay_ms(BLINK_PERIOD);
	}
}

int main(void)
{
	unsigned int i=0;
	INP_PORTDDR=0x0;			//as input
	LED_PORTDDR=0xff;			//as output
	ALM_PORTDDR=0x1;			//as output on PIN0
	

	INP_PORT=0xff;			//enable pull-up resistor
	LED_PORT=0;				//Turn off all LEDs
	ALM_PORT=0x0;

	BIT_SET(ALM_PORT,1);		//Enable pull-up resistor
	
/*
	// setup interrupt 	
		OCR1A = 3906;								//for 1 seconds around 1 seconds
		TCCR1B |= (1 << WGM12);						// Mode 4, CTC on OCR1A
		TIMSK |= (1 << OCIE1A);						//Set interrupt on compare match
		TCCR1B |= (1 << CS12) | (1 << CS10);		// set prescaler to 1024 and start the timer
		sei();
	// enable interrupts
*/

    while(1)
	
    {
		if (!BIT_CHECK(ALM_PORTPIN ,1)){			// Check whether the button is pressed
			if(!locked){
				for(i=0;i<4;i++){
					if( !BIT_CHECK(INP_PORTPIN,i)) {
						Blink_On(i);
						BIT_CLEAR(status,i);
					}else{
						BIT_SET(status,i);
					}
				}
			}else{
				if(((INP_PORTPIN & 0x0f) != status) && (!alarmMode)){		//4 bits only
					BIT_SET(ALM_PORT,0);			//produce the alarm until unlocked
					alarmMode=1;
				}
			}
			locked=1;
		}else{
			if(oldstatus!=INP_PORTPIN){
				for(i=0;i<4;i++){
					if( !BIT_CHECK(INP_PORTPIN,i)){
						if (BIT_CHECK(oldstatus,i))   {
							Blink_On(i);
							BIT_CLEAR(oldstatus,i);
						}
					}else{
						BIT_CLEAR(LED_PORT,i);
						BIT_SET(oldstatus,i);
					}
				}
			}
			BIT_CLEAR(ALM_PORT,0);			//clear the alarm
			locked=0;
			alarmMode=0;
			
		}
    }
}
	
	
ISR (TIMER1_COMPA_vect)
{
	if(BIT_CHECK(ALM_PORTPIN,0)){
		BIT_CLEAR(ALM_PORT,0);
	}else{
		BIT_SET(ALM_PORT,0);
	}
	// action to be done every 1s
}


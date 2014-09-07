


#define F_CPU 4000000UL  // 1 MHz


#include <avr/io.h>
#include <util/delay.h>
#include <avr/sfr_defs.h>


#define INP_PORT PORTB
#define INP_PORTDDR DDRB
#define INP_PORTPIN PINB

#define ALM_PORT PORTC
#define ALM_PORTPIN PC0
#define ALM_PORTDDR DDRC

#define LED_PORT PORTD
#define LED_PORTDDR DDRD
#define  BLINK_PERIOD 100

#define BIT_SET(a,b) ((a) |= (1<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1<<(b)))
#define BIT_CHECK(a,b) ((a) & (1<<(b)))

#define DATABIT_SET(a,b) ((a) |= (1<<(b)))


unsigned int status=0;

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
	

	INP_PORT=0xff;			//pull up resistor to high
	LED_PORT=0;				//Turn off all LEDs
	ALM_PORT=0;
	
	//status=INP_PORTPIN;
	
	//status=1;

    while(1)
	
    {
		
		for(i=0;i<4;i++){
			if( BIT_CHECK(INP_PORTPIN,i) && (!BIT_CHECK(status,i))) {
				Blink_On(i);
				DATABIT_SET(status,i);
			}			
		}
		if((INP_PORTPIN & 0x0f) !=status){		//4 bits only
			BIT_SET(ALM_PORT,0);
		}else{
			BIT_CLEAR(ALM_PORT,0);
		}
        //TODO:: Please write your application code 
    }
}
	

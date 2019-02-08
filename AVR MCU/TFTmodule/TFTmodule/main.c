#include <avr/io.h>
#include "st7735.h"
#include <avr/pgmspace.h>



#define MOSI 5
#define SCK 7
#define SS 0 //SS for pin1, can add device later
#define DC 1 //A0 pin for TFT
#define RESET 7 //Reset pin on TFT
#define LED 0// LED pin

void delayT0()
{
	TCNT0 = 186;
	TCCR0 = 0x01;
	while ((TIFR&0x01) ==0);
	TCCR0 =0;
	TIFR =0x01 ;
}

void delayT1()
{
	TCNT1 = 0xC180;
	TCCR1A = 0x00;
	TCCR1B = 0x01;
	while ((TIFR &0x04 )==0);
	TCCR1B = 0;
	TIFR =0x04 ;
}

uint8_t xfer(uint8_t data)
{
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	
	return SPDR;
}

void TFTSPI()
{
	DDRB |=(1<<SS);
	PORTB &= ~(1<<SS); //SS pin is low
	DDRB |=(1<<MOSI)|(1<<SCK); //MOSI pin and CLK pin as output
	SPCR |=(1<<SPE)|(1<<MSTR); //Running at master mode, 2Mhz, clkpol 0, clkphs 0
	
	
}

void writeCMD(uint8_t cmd)
{
	
	PORTD&= ~(1<<DC); //pin 1 is low
	xfer(cmd);
	PORTD|= (1<<DC); //pin 1 is high
	
}

void writeData (uint8_t b)
{
	
	xfer(b);
};

void HardwareReset()
{
	
	
	PORTD &=~(1<<7);
	delayT1();
	PORTD |=(1<<7);
	delayT1();
	
	
}

void InitDisplay()
{
	
	HardwareReset();
	writeCMD(SLPOUT);
	delayT1();
	
	writeCMD(COLMOD);
	writeData(0x05);
	writeCMD(DISPON);
	
	
	
}

int main ()
{
	
	DDRD |=(1<<LED)|(1<<DC); 
	DDRD|= (1<<RESET);
	PORTD |=(1<<RESET);
	
	TFTSPI();
	InitDisplay();
	PORTD |= (1<<LED);
	
	
	
	
}
	
	
	
	
	
	

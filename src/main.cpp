#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


char channel = 0;
char entero = 0;
char decima = 0;
int valoradc = 0;
int voltaje = 0;

void select_channel(char channel) {
  if (channel <= 15) {
      ADMUX &= 0xF0;
      ADMUX |= channel;
  }
}
ISR(INT0_vect) {
      channel++;
      if (channel > 1) {
          channel = 0;
      }
      select_channel(channel);
  }

void config_INT0() {
  EIMSK |= (1 << INT0);
  EICRA |= (1 << ISC01);
  DDRD &= ~(0x04);
  PORTD |= 0x04;
}

ISR(ADC_vect) {

  valoradc = ADC;
  voltaje = ((long)valoradc * 50+511) / 1023;
  entero = voltaje / 10;
  decima = voltaje % 10;
  ADCSRA |= (1 << ADSC);

}

int main (void) {
    ADMUX |= (1 << REFS0);
    ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADIE);
   
    select_channel(0);
    config_INT0();
    sei();

    DDRD |= 0xF0;
    DDRB |= 0x03;
    ADCSRA |= (1 << ADSC);

    while (1) {

        PORTB &= ~0x03;
        PORTB |= 0x01;
        PORTD &= ~0xF0;
        PORTD |= (decima << 4);
        _delay_ms(2);

        PORTB &= ~0x03;
        PORTB |= 0x02;
        PORTD &= ~0xF0;
        PORTD |= (entero << 4);
        _delay_ms(2);
    }
}

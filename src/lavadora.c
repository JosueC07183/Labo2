#include <avr/io.h> 
#include <util/delay.h> 
#include <avr/interrupt.h> 

int main(void) 
{ 
  DDRB = 0b00011100; // Configuracion del puerto. Se habilita B2-B4.
  DDRD = 0b00000000; // Configuracion del puerto. D2-D4 como entrada.
  PORTB = 0b00000000; // Estado inicial del puerto: /B7..B0
  PORTD = 0b00011100; // Habilitar pull-up en D2-D4.

  sei();
  GIMSK = 0b00101000; // Interrupciones del Grupo 0 (B_pins) activadas.
  PCMSK = 0b00011100; // Se habilita D2, D3, y D4 para interrupciones.

  // Aquí está el core de TODO
  while (1) { 
    if (PIND & (1 << PD2)) // Verificar el estado de D2 (interruptor)
    {
        PORTB |= (1 << PB4); // Encender el LED (B4)
    }
    else
    {
        PORTB &= ~(1 << PB4); // Apagar el LED (B4)
    }
    _delay_ms(250);

    if (PIND & (1 << PD3)) // Verificar el estado de D3 (interruptor)
    {
        PORTB |= (1 << PB3); // Encender el LED (B3)
    }
    else
    {
        PORTB &= ~(1 << PB3); // Apagar el LED (B3)
    }
    _delay_ms(250);

    if (PIND & (1 << PD4)) // Verificar el estado de D3 (interruptor)
    {
        PORTB |= (1 << PB2); // Encender el LED (B3)
    }
    else
    {
        PORTB &= ~(1 << PB2); // Apagar el LED (B3)
    }
    _delay_ms(250);

  } 
}

ISR(PCINT0_vect) // D2
{ 
    PORTD |= (1 << PD2); // Configurar D2 a 1 (pull-up activado)
    _delay_ms(500);
}


ISR(PCINT1_vect) // 
{ 
    PORTD &= ~(1 << PD2); // Configurar D2 a 0 (pull-up desactivado)
    _delay_ms(500);
}

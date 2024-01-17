/**
 * @file lavadora.c
 * @authors Kevin Campos Campos, Josué Salmerón Córdoba
 * @brief En este código si simula el comportamiento de una lavadora por medio de máquina de estados.
 * @version 0.1
 * @date 2024-01-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
/**
 * @brief Inicialmente, se realizan 3 interrupciones para definir la carga que se le pondrá a la lavadora, 
la primera tiene que ver con la carga BAJA, como está conectada al pin D2, entonces dentro de ISR se tiene: INT0_vect, luego un condicional para interactuar
con el display de 7 segmentos. La segunda le corresponde a la carga MEDIA, que se encuentra conectada al pin D3, por lo que dentro de ISR va INT1_vect esto
según la hoja del fabricante, después se presenta un condicional con la misma lógica que el anterior. Después para la carga ALTA se coloca en el pin A0, que
dentro del ISR se tiene PCINT1_vect para que la interrupción tenga sentido, lo que sigue es un condicional igual a los anteriores.
 * @param BAJA: bandera para la carga baja
 * @param MEDIA: bandera para la carga media
 * @param ALTA: botón de la carga alta.
 * @param BUTTON_1: botón de la carga baja.
 * @param BUTTON_2: botón de la carga media.
 * @param BUTTON_3: botón de la carga alta.
 * @param DISPLAY_1: etiqueta del display.
 * 
 */
volatile uint8_t BAJA = 0;
volatile uint8_t MEDIA = 0;
volatile uint8_t ALTA = 0;
volatile uint8_t BUTTON_1 = 0;
volatile uint8_t BUTTON_2 = 0;
volatile uint8_t BUTTON_3 = 0;
volatile uint8_t DISPLAY_1 = 0; // Display 1

// Carga BAJA
ISR(INT0_vect){
    if(DISPLAY_1==0){
        DISPLAY_1 = 1;
        _delay_ms(50);
        BAJA=0;
        BUTTON_1=1;
        
    }
}
// CARGA MEDIA
ISR(INT1_vect){
    if(DISPLAY_1==0){
        DISPLAY_1 = 1;
        _delay_ms(50);
        MEDIA=0;
        BUTTON_2=1;
    }
}
// Carga ALTA
ISR(PCINT1_vect){
    if(DISPLAY_1==0){
        DISPLAY_1 = 1;
        _delay_ms(50);
        ALTA=0;
        BUTTON_3=1;
    }
}

/**
 * @brief En esta parte del main se establecen cuales puertos son salidas y entradas, tal como lo es el caso para todos los puertos/pines B, dado que se definen
 * como DDRB=0xFF, mientras que los pines D son entradas. Lo cual tiene sentido porque en los pines B están conectados los displays, mientras que en los pines
 * D y un pin de A están las interrupciones. Luego se habilitan los pull-up de los resistores en los pines PD2 y PD3. Ahora, con base a las interrupciones de los
 * pines a usar es necesario escribir las macros respectivas para  habilitar, de lo contrario el circuito no responderá adecuadamente si esto no se hace. Por último,
 * se habilitan todas estas interrupciones con sei().
 * Ahora, dentro del ciclo while debe existir lo necesario para que el circuito responda según lo esperado, en ese sentido, aquí se hace un if y dentro de éste un case
 * para darle sentido a lo que está conectada en el display, se debe tener en cuenta que lo siguiente, un display de 7 S se compone de la letra a-g, entonces, se toman 
 * 0 y 1 para definir los números según lo construído en el esquemático. Al ser 7S, el pin B7 no se usa para definirlos, por eso es que siempre es 0. Después, se tienen
 * unos condicionales para demostrar que los displays funcionan correctamente según la condición que se le presente, mayor o igual a 9, 8 o 7.
 * @return int 
 */
int main() {
    DDRB = 0xFF;
    DDRD = 0x00;
    DDRA &= ~PA0;
    PORTD |= (1 << PD2) | (1 << PD3);
    PORTA |= PA0;
    GIMSK |= (1 << INT0) | (1 << INT1) | (1 << PCIE1);
    MCUCR |= (1 << ISC00) | (1 << ISC01);
    PCMSK1 |= (1<<PCINT8);
    sei();

    while (1) {
        // El pin B7 NO se usa.
        if(DISPLAY_1){
            switch(BAJA|MEDIA|ALTA)
            {   
                case 0:
                PORTB = 0b01111110;
                break;
                case 1:
                PORTB = 0b00110000;
                break;
                case 2:
                PORTB = 0b01101101;
                break;
                case 3:
                PORTB = 0b01111001;
                break;
                case 4:
                PORTB = 0b00110011;
                break;
                case 5:
                PORTB = 0b01011011;
                break;
                case 6:
                PORTB = 0b01011111;
                break;
                case 7:
                PORTB = 0b01110000;
                break;
                case 8:
                PORTB = 0b01111111;
                break;
                case 9:
                PORTB = 0b01110011;
                break;
            }
            // Estados de BAJA
            if(BUTTON_1){
                if (BAJA<9)
                {
                    BAJA++;
                }
                else{

                    PORTB=0b01111110;
                    BAJA = 0;
                    DISPLAY_1 = 0;
                }
                
            }
            // Estados de MEDIA
            else if(BUTTON_2){
                if(MEDIA < 9){
                    MEDIA++;
                }
                else{
                    PORTB=0b01111110;
                    MEDIA = 0;
                    DISPLAY_1 = 0;
                }
            }
            // ESTADOS DE ALTA
            else if(BUTTON_3){
                if(ALTA <= 9){
                    ALTA++;
                }
                else{
                    PORTB=0b11111110;
                    ALTA = 0;
                    DISPLAY_1 = 0;
                    _delay_ms(1000);
                    PORTB = 0b01111110;
                }
            }
            else{
                PORTB=0b01111110;
                DISPLAY_1 = 0;
            }        
            _delay_ms(1000);
        }
    }
}



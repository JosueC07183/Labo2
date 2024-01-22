/**
 * @file laboratorio2.c
 * @authors Kevin Campos Campos, Josué Salmerón Córdoba
 * @brief En este código si simula el comportamiento de una lavadora por medio de máquina de estados.
 * @version 0.1
 * @date 2024-01-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// Duraciones de cargas inciales
#define baja    9
#define media   19
#define alta    27

// Variable que guarda el valor inicial del tiempo
volatile int tiempo_inicio = 0;
volatile int segundos = 0;
volatile int pausa = 0;
volatile uint8_t unidades;
volatile uint8_t decenas;

void estados();

// Funcion para encender leds de carga
void led_carga() {
    if (tiempo_inicio == alta) {
        PORTD |= (1 << PD4);
        estados();
    }
    else if (tiempo_inicio == media) {
        PORTD |= (1 << PD5);
        estados();
    }
    else if (tiempo_inicio == baja) {
        PORTD |= (1 << PD6);
        estados();
    }
}

// Funcion para apagar leds basado en criterios
void led_off() {
    // Apagar leds de carga
    if (tiempo_inicio == 0) {
        PORTD = (PORTD & 0x0F); // Limpiar pines de LEDs
    }
    if (segundos == tiempo_inicio) {
        PORTA = (PORTA | 0x4) & 0x4; // Apagar LEDs de estado
    }
}

// Funcion encargada de encender los LEDs dependiendo del estado en el que se encuentre
void estados() {
    switch (tiempo_inicio) {
        // Carga baja
        case baja:
            if (segundos == 0) {
                // Suministro, 00
                PORTA &= ~(1 << PA2); // Encender leds de estado suministro
            }
            else if (1 <= segundos && segundos < 4) {
                // Lavar, 01
                PORTA |= (1 << PA0);
            }
            else if (4 < segundos && segundos < 7) {
                // Enjuagar, 10
                PORTA &= ~(1 << PA0);
                PORTA |= (1 << PA1);
            }
            else if (7 <= segundos && segundos < tiempo_inicio) {
                // Enjuagar, 11
                PORTA |= (1 << PA0);
            }
            led_off(); // Apagar todo
        break;
        
        // Carga media
        case media:
            if (segundos == 0) {
                // Suministro, 00
                PORTA &= ~(1 << PA2); // Encender leds de estado suministro
            }
            else if (2 <= segundos && segundos < 8) {
                // Lavar, 01
                PORTA |= (1 << PA0);
            }
            else if (8 <= segundos && segundos < 13) {
                // Enjuagar, 10
                PORTA &= ~(1 << PA0);
                PORTA |= (1 << PA1);
            }
            else if (13 <= segundos && segundos < tiempo_inicio) {
                // Enjuagar, 11
                PORTA |= (1 << PA0);
            }
            led_off(); // Apagar todo
        break;

        // Carga alta
        case alta:
            if (segundos == 0) {
                // Suministro, 00
                PORTA &= ~(1 << PA2); // Encender leds de estado suministro
            }
            else if (3 <= segundos && segundos < 13) {
                // Lavar, 01
                PORTA |= (1 << PA0);
            }
            else if (14 <= segundos && segundos < 19) {
                // Enjuagar, 10
                PORTA &= ~(1 << PA0);
                PORTA |= (1 << PA1);
            }
            else if (19 <= segundos && segundos < tiempo_inicio) {
                // Enjuagar, 11
                PORTA |= (1 << PA0);
            }
            led_off(); // Apagar todo
        break;
    }
} 

// Funcion encargada de mostrar el conteo en los displays
void display(volatile int tiempo) {
    tiempo = tiempo - segundos;
    if (tiempo_inicio != 0) {
        // Separa unidades del numero
        unidades = tiempo % 10;
        // Separa decenas del numero
        decenas = tiempo / 10;

        PORTB = (PORTB & 0xF0) | (unidades); // Tomar los 4 bits menos significativos del bus y mostrar las unidades
        PORTB = (PORTB & 0x0F) | decenas << 4; // Tomar los 4 bits mas significativos del bus y mostrar las decenas
    }
    else {
        // Mostrar ceros
        PORTB &= ~(1 << PORTB);
    }
    // Condicion de reset
    if ((unidades == 0) & (decenas == 0)) {
        tiempo = 0;
        segundos = 0;
        tiempo_inicio = 0;
    }
}

// Inicializar pines
void init() {
    ////////////// Salidas, se ocupan 4, 1 para el selector y los otros 3 para mostrar numeros del 0-9
    DDRB = 0xff; // Todos los pines del puerto B como salidas
    DDRD = 0x70; // D6, D5, D4 como salidas y D0, D1, D2, D3 como entradas
    DDRA = 0x07; // A0, A1, A2 como salidas
    
    PORTA |= (1 << PA2); // Iniciar PA2 en alto (LEDs de estado apagados)

    GIMSK |= (1 << PCIE2); // Habilitar interrupciones por PD1/PCINT12, baja (PCIE2 cubre del 17-11)
    GIMSK |= (1 << INT0); // Habilitar interrupciones por PD2, media
    GIMSK |= (1 << INT1); // Habilitar interrupciones por PD3, alta

    ///////////////////Temporizadores
// Timer/Counter0 para refrescar displays
    // Configuración del Timer/Counter0
    TCCR0A |= (1 << WGM01);  // Poner bit modo CTC

    // Valor de comparación del timer (con esto se obtiene 0.003 segundos (approx))
    OCR0A = 15;

    TCCR0B |= (1 << CS02);  // Preescaler de 256, pagina 86

    // Configuración de la interrupción
    TIMSK |= (1 << OCIE0A);  // Interrupt enable para comparacion (se da la interrupcion cuando se da un match)


// Timer/Counter1 para interrupciones cada segundo
    // Configuración del Timer/Counter1 (ocupo este porque no alcanza con 8 bits). No ocupo habilitar una interrupcion para este timer
    TCCR0A |= (1 << WGM12);  // Poner bit modo CTC, pag 113

    // Valor de comparación del timer (con esto se obtiene 1 segundos (approx))
    OCR1A = 3906; // Con esto obtendo 1 segundo

    TCCR1B |= (1 << CS12);  // Preescaler de 256, pagina 114

    TIMSK |= (1 << OCIE1A);  // Interrupt enable

    sei();
}

/////////////////////// Interrupciones de switches
// Interrupcion generada por PD1/PCINT12, baja
ISR(PCINT2_vect) {
    tiempo_inicio = baja;
    led_carga();
}

// Interrupcion generada por INT0/PD2, media
ISR(INT0_vect) {
    tiempo_inicio = media;
    led_carga();
}

// Interrupcion generada por INT1/PD3, alta
ISR(INT1_vect) {
    tiempo_inicio = alta;
    led_carga();
}

/////////////////////// Interrupciones de timers
// Esta interrupcion se usa para refresacar los displays cada 0.003 segundos
ISR(TIMER0_COMPA_vect) {
    display(tiempo_inicio);
}

// Entra aqui cada 1 segundos
ISR(TIMER1_COMPA_vect) {
    if (tiempo_inicio == 0)
    {
        segundos = 0;
    }
    else {
        segundos++; // Llevar cuenta de cada segundo que pasa
    }
    led_off(); // Revisar si hay que apagar los leds cada segundo que pasa
    estados(); // Revisar por cambios de los estados cada segundo
}

int main(void) {
    // Inicializar
    init();

    while (1) {
        // No hacer nada, solo interrupciones
    }
    return 0;
}


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

/**
 * @brief Aquí se definen las cargas iniciales que se le podrán añadir a la lavadora, 9, 19 y 27 para baja, media y alta respectivamente. Se tiene el prototipo de la máquina de estados.
 * @param tiempo_inicio: esto es una variable inicial del tiempo.
 * @param segundos: esta variable se encarga de revisar la cantidad de tiempo que queda en el display, se coloca en el intervalo de tiempo y valora si se debe pasar al siguiente estado.
 * @param unidades: es la unidad del posible dígito de dos números que se vaya a formar.
 * @param decenas: muestra las decenas en el display.
 */


// Duraciones de cargas inciales
#define baja    9
#define media   16
#define alta    21

// Variable que guarda el valor inicial del tiempo
volatile int tiempo_inicio = 0;
volatile int segundos = 0;
volatile uint8_t unidades;
volatile uint8_t decenas;

void estados();

/**
 * @brief Esta función se encarga de mostrar los LEDs que se encienden cuando se presiona cualquiera de los 3 botones.
 * @param PD4: corresponde al LED de la carga alta.
 * @param PD5: corresponde al LED de la carga media.
 * @param PD6: corresponde al LED de la carga baja.
 */

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

/**
 * @brief Esta función apaga todos los LEDs una vez que se termine el lavado de una carga.
 */
void led_off() {
    // Apagar leds de carga
    if (tiempo_inicio == 0) {
        PORTD = (PORTD & 0x0F); // Limpiar pines de LEDs
    }
    if (segundos == tiempo_inicio) {
        PORTA = (PORTA | 0x4) & 0x4; // Apagar LEDs de estado
    }
}

/**
 * @brief Esta función es la máquina de estados que se encargá de intercambiar los estados de la lavadora una vez que se encuentre operando,
 * es decir, pasará de suministro, lavar, enjuagar, centrifugar, una vez que termine el contador, se hace un llamada a la función led_off() para
 * apagar todos los LEDs.
 * @param PA2: poner en bajo este pin hace que la simulación no inicie con el led de suministro encendido.
 * @param PAO: LED del demultiplexor.
 * @param PA1: LED del demultiplexor.
 * @param segundos: tiempo asginado para cada carga.
 * 
 */

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
            else if (4 <= segundos && segundos < 6) {
                // Enjuagar, 10
                PORTA &= ~(1 << PA0);
                PORTA |= (1 << PA1);
            }
            else if (6 <= segundos && segundos < tiempo_inicio) {
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
            else if (2 <= segundos && segundos < 7) {
                // Lavar, 01
                PORTA |= (1 << PA0);
            }
            else if (7 <= segundos && segundos < 11) {
                // Enjuagar, 10
                PORTA &= ~(1 << PA0);
                PORTA |= (1 << PA1);
            }
            else if (11 <= segundos && segundos < tiempo_inicio) {
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
            else if (3 <= segundos && segundos < 10) {
                // Lavar, 01
                PORTA |= (1 << PA0);
            }
            else if (10 <= segundos && segundos < 15) {
                // Enjuagar, 10
                PORTA &= ~(1 << PA0);
                PORTA |= (1 << PA1);
            }
            else if (15 <= segundos && segundos < tiempo_inicio) {
                // Enjuagar, 11
                PORTA |= (1 << PA0);
            }
            led_off(); // Apagar todo
        break;
    }
} 

/**
 * @brief Esta función es la encargada de mostrar el conteo en los displays, que por medio de un condicional separa las unidades y decenas de un número. Luego, por medio de un pipe se toman los 4
 * bits LSB para colocar las unidades, y otro (con shift) para las decenas. Luego, se hace un reinicio de las variables, las cuales quedan en cero.
 * @param tiempo: variable que se muestra en los displays.
 * @param segundos: variable que se encarga en el cambio de estado.
 */
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

/**
 * @brief Esta es la parte más importante del circuito, la configuración de los pines para clasificarlos como salidas, entradas, habilitación de interrupciones por medio de máscaras
 * Y la parte de temporizadores donde se usan los registros vistos en clase para poder hacer uso de los timers, CTC, así con sus respectivas interrupciones, lo que permite refrescar los
 * displays correctamente. 
 * 
 */
void init() {
    ////////////// Salidas, se ocupan 4, 1 para el selector y los otros 3 para mostrar numeros del 0-9
    DDRB = 0xff; // Todos los pines del puerto B como salidas
    DDRD = 0x70; // D6, D5, D4 como salidas y D0, D1, D2, D3 como entradas
    DDRA = 0x07; // A0, A1, A2 como salidas
    
    PORTA |= (1 << PA2); // Iniciar PA2 en alto (LEDs de estado apagados)

    // Esto se usa para aplicar una mascara para habilitar interrupcion solo por un pin a la vez porque se pueden
    // habilitar interrupciones por puerto
    PCMSK2 |= (1 << PCINT12); // PD1, baja

    // REV, FALTA METER EL DE PAUSE
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
/**
 * @brief Construct a new ISR object, acá se resumen las interrupciones usadas para este trabajo, ellas corresponden a cada botón y hacen llamado
 * a una función para que se ejecute en las declaraciones void, y así se cumple con el requisito de que sean simples y claras. También están las
 * interrupciones de los timers, esto para refrescar los displays cada 0.003 segundos.
 * 
 */
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

/**
 * @brief Main de todo el código, donde basta con hacer un llamado de la configuración inicial y así ver el funcionamiento de la lavadora.
 * @return int 
 */
int main(void) {
    // Inicializar
    init();

    while (1) {
        // No hacer nada, solo interrupciones
    }
    return 0;
}

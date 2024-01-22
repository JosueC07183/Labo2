### Universidad de Costa Rica
#### IE0624 - Laboratorio de Microcontroladores
##### Laboratorio 2: GPIOs, Timers y FSM

- Kevin Campos Campos, B91519
- Josue Salmerón Cordoba, C07183

III Ciclo 2023

---
## Descripción
Este es el segundo laboratorio del curso IE0624, se simula el comportamiento de una lavadoa automática por medio de interrupciones y el uso de una máquina de estados, el comportamiento de los LEDs, indicará el estado actual de la lavadora, que con ayuda de dos displays es posible saber cuando ésta ha terminado su labor.
## Puntos importantes
1. Ingresar en el directorio `src` y realizar lo siguiente.
   1. Abrir una terminal y escribir en la terminal la palabra `make`. Hecho esto sucederán dos eventos, primero se genera un archivo .hex, y el segundo es que se abrirá el .html con la documentación respectiva de Doxygen.
   2. Con el .hex listo, abrir el archivo *laboratorio2.simu* con SimulIDE y hacer click derecho sobre el MCU, cargar el firmware y seleccionar el .hex generado con el makefile.
   3. Hecho esto, ya es posible presionar un cualquier botón para asignar una carga, ya sea baja, media o alta, así, la lavadora empezará a trabajar y mostrará el tiempo restante que va quedando en la etapa de lavado.
   4. Por último, para limpiar el directorio `src` basta con escribir `make clean` en la terminal.
2. En el directorio `informe` encontrará **todos** los archivos para generar el informe/reporte de este trabajo, que por defecto tiene el nombre: *main.pdf*.
3. **Nota: Al abrir el simulador con el circuito se mostrará un comportamiento extraño que pareciera ser que es una cuestión más con el simulador.** 
Ya que al abrir el circuito se muestra el LED de suministro encendido, lo cual no debería ser así.
![App Screenshot](https://github.com/JosueC07183/Labo2/blob/main/informe/Carpeta%20tex/Imagenes/weird_1.png)

No obstante, para solucionar esto hay que cargar el firmware y apagar y encender el circuito, tal como se muestra a continuación.
![App Screenshot](https://github.com/JosueC07183/Labo2/blob/main/informe/Carpeta%20tex/Imagenes/sol_weird1.png)
Ya hecho esto es posible solucionar este problema y realizar la simulación.
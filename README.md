# StarTracker 
by InnovacionesDoMa, 📍 Chile 🇨🇱

| StarTracker  | Front |
| ------------- | ------------- |
|![alt text](https://github.com/CDonosoK/StarTracker/blob/master/Images/StarTracker.png) |	![alt text](https://github.com/CDonosoK/StarTracker/blob/master/Images/StarTracker%20Front.png)|

| Lateral |	Logo|
| ------------- | ------------- |
|![alt text](https://github.com/CDonosoK/StarTracker/blob/master/Images/StarTracker%20Lateral.png)| ![alt text](https://github.com/CDonosoK/StarTracker/blob/master/Images/StarTracker%20Logo.PNG)

StarTracker es un localizador de estrellas de bajo costo, el cual busca apuntar con un láser a la estrella que se haya seleciconado dentro de la base de datos de este mismo, independiente de la hora, el lugar y el día en que se encuentre el observador.

## BASE DE DATOS

| Estrella| Ascensión | Declinación|
| ----- | ---- | ---- |
| Mercurio| 19h50m19s | -19°3'29'' |
| Venus| 18h59m56s | -16°11'15'' |
| Marte|  18h46m45s | -23°34'51''|
| Jupiter|  22h42m54s | -9°11'22''|
| Saturno| 21h13m49s | -16°55'7'' |
| Urano|  2h34m42s | +14°45'6''|
| Neptuno|  23h30m3s | -4°27'11''|
| Sirio|  6h45m9s | -16°42'58''|
| Betelgeuse|  5h55m10s | +7°24'26''|
| Aldebarán|  4h35m55s | 16°30'33'' |
| Alfa-Centauri| 14h39m37s | -60°50'2'' |
| Antares|  16h29m24s | -26°25'55''|
| Capella|  5h16m41s | +45°59'53''|

## CONEXIONES
### LCD

|LCD | ESP32 |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| SCK | G14 |
| SDA | G13 |

### GPS

| GPS  | ESP32 |
| --- | --- |
| VCC | 5V |
| GND | GND |
| TX | G33 |
| RX | G32 |

### SERVOMOTOR VERTICAL

| MOTOR1 | ESP32 |
| --- | --- |
| VCC | 5V |
| GND | GND |
| SEÑAL | G16 |

### SERVOMOTOR HORIZONTAL
| MOTOR2 | ESP32 |
| --- | --- |
| VCC | 5V |
| GND | GND |
| SEÑAL | G17 |

### BOTÓN

| BOTON | ESP32 |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| SEÑAL | G26 |

### POTENCIOMETRO

| POTENCIOMETRO | ESP32 |
| --- | --- |
| VCC | 3.3V |
| GND | GND |
| SEÑAL | G35 |

## COMO FUNCIONA

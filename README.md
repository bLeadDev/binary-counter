# Binary counter
This is a binary counter which shuts down the LEDs with a flashy effect. All LEDs get turned on, shutdown, and then count up to the actual number. With every click on the button the counter gets increased by 1. Overruns at sizeof(datatype).
While the lights work, inputs do not get read. 
The counter value is saved to emulated eeprom on increase and loaded at startup. 
Serial debugging can be turned on/off with preprocessor directive. 
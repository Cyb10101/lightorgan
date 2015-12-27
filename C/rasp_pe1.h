// Emulation of orginal file
#ifndef _RASP_PE1_
#include <inttypes.h>
#include <unistd.h>

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

int hwLED[8];

int openHardwarePE1(void) {
	printf("[DEBUG] openHardwarePE1()\n");
	return 0;
}
int closeHardwarePE1(void) {
	printf("[DEBUG] closeHardwarePE1()\n");
	return 0;
}

void writeDigitalOutputPE1(int pin, int on) {
	printf("[DEBUG] writeDigitalOutputPE1(pin = %d, on = %d)\n", pin, on);
	hwLED[pin] = on;
}
int readDigitalInputPE1(int pin) {
	printf("[DEBUG] writeDigitalOutputPE1(pin = %d) = %d)\n", pin, hwLED[pin]);
	return hwLED[pin];
}

void delayMilliseconds(int milliSeconds) {
	usleep((unsigned int)milliSeconds);
}

#endif

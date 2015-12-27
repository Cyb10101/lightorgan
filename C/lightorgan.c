// Orginal file or emulation file
//#include <rasp_pe1.h>
#include "rasp_pe1.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

/**
 * Get Linux uptime in seconds since system boot
 * 
 * @return int
 */
long int getLinuxUptime() {
	struct sysinfo si;
	if(sysinfo(&si) == 0) {
		return si.uptime;
	} else {
		return -1L;
	}
}

/**
 * Get character on keypress
 * 
 * @return char
 */
char getCharacter() {
      system("stty raw");    // Raw input - wait for only a single keystroke
      system("stty -echo");  // Echo off
      char c = getchar();
      system("stty cooked"); // Cooked input - reset
      system("stty echo");   // Echo on - Reset
      return c;
}

/**
 * Simple user input with default value for int
 * 
 * @return int
 */
int getUserInputInt(char * title, int defaultValue) {
	int value;
	printf("%s", title);
	scanf("%d", &value);
	return (value > 0 ? value : defaultValue);
}

/**
 * Simple user input with default value for float
 * 
 * @return float
 */
float getUserInputFloat(char * title, float defaultValue) {
	float value;
	printf("%s", title);
	scanf("%f", &value);
	return (value > 0 ? value : defaultValue);
}

/**
 * Set all leds on|off
 * 
 * @param int value
 * @return void
 */
void setAllLeds(int value) {
	int i;
	
	for(i = 0; i < 8; i++) {
		writeDigitalOutputPE1(i, value);
	}
}

/**
 * Set all leds by decimal value
 * 
 * @param int value
 * @return void
 */
void setLedsFromDecimal(int value) {
	if (value < 0) {
		value = 0;
	} else if (value > 255) {
		value = 255;
	}

	writeDigitalOutputPE1(7, (value & 128 ? 1 : 0));
	writeDigitalOutputPE1(6, (value & 64 ? 1 : 0));
	writeDigitalOutputPE1(5, (value & 32 ? 1 : 0));
	writeDigitalOutputPE1(4, (value & 16 ? 1 : 0));
	writeDigitalOutputPE1(3, (value & 8 ? 1 : 0));
	writeDigitalOutputPE1(2, (value & 4 ? 1 : 0));
	writeDigitalOutputPE1(1, (value & 2 ? 1 : 0));
	writeDigitalOutputPE1(0, (value & 1 ? 1 : 0));
}

/**
 * Leds running from left to right
 * 
 * @return void
 */
void runLedsFromLeftToRight() {
	setAllLeds(0);

	int i, LED;
	int repeat = getUserInputInt("Number of repeat (int) [3]: ", 3);
	float pause = getUserInputFloat("Pause in seconds (float) [0.5]: ", 0.5);

	for (i = 0; i < repeat; i++) {
		for (LED = 0; LED < 8; LED++) {
			writeDigitalOutputPE1((LED == 0 ? 7 : LED - 1), 0);
			writeDigitalOutputPE1(LED, 1);
			delayMilliseconds((int)(pause * 1000));
		}
	}

	setAllLeds(0);
}

/**
 * Leds running from right to left
 * 
 * @return void
 */
void runLedsFromRightToLeft() {
	setAllLeds(0);

	int i, LED;
	int repeat = getUserInputInt("Number of repeat (int) [3]: ", 3);
	float pause = getUserInputFloat("Pause in seconds (float) [0.5]: ", 0.5);

	for (i = 0; i < repeat; i++) {
		for (LED = 7; LED >= 0; LED--) {
			writeDigitalOutputPE1((LED == 7 ? 0 : LED + 1), 0);
			writeDigitalOutputPE1(LED, 1);
			delayMilliseconds((int)(pause * 1000));
		}
	}

	setAllLeds(0);
}

/**
 * Leds running from right to left and backwards
 * 
 * @return void
 */
void runLedsFromLeftToRightToLeft() {
	setAllLeds(0);

	int i, LED = 0;
	int repeat = getUserInputInt("Number of repeat (int) [3]: ", 3);
	float pause = getUserInputFloat("Pause in seconds (float) [0.3]: ", 0.3);

	writeDigitalOutputPE1(LED, 1);
	delayMilliseconds((int)(pause * 1000));

	for (i = 0; i < repeat; i++) {
		for (LED = 1; LED < 8; LED++) {
			writeDigitalOutputPE1((LED == 0 ? 7 : LED - 1), 0);
			writeDigitalOutputPE1(LED, 1);
			delayMilliseconds((int)(pause * 1000));
		}
		for (LED = 6; LED >= 0; LED--) {
			writeDigitalOutputPE1((LED == 7 ? 0 : LED + 1), 0);
			writeDigitalOutputPE1(LED, 1);
			delayMilliseconds((int)(pause * 1000));
		}
	}

	setAllLeds(0);
}

/**
 * Leds running from decimal 0 to 255
 * 
 * @return void
 */
void runLedsFromZeroToMax() {
	setAllLeds(0);

	int i;
	float pause = getUserInputFloat("Pause in seconds (float) [0.05]: ", 0.05);

	for (i = 0; i < 256; i++) {
		setLedsFromDecimal(i);
		delayMilliseconds((int)(pause * 1000));
	}

	setAllLeds(0);
}

/**
 * Toggle Leds on|off
 * 
 * @return void
 */
void runToggleLeds() {
	setAllLeds(0);
	printf("Toggle lights with keys [0-7]\n");
	printf("Press [ESC] key to exit.\n");

	char cIn;
	while ((cIn = getCharacter()) != 27) {
		//printf("Pressed Character: %c | Ascii value: %d\n", cIn, (int)cIn);
		int ia = cIn - '0';
		if(ia >= 0 && ia <= 7) {
			writeDigitalOutputPE1(ia, !readDigitalInputPE1(ia));
		}
	}

	setAllLeds(0);
}

/**
 * Main
 * 
 * @return int
 */
int main() {
	int choose;

	if (openHardwarePE1() > 0) {
		printf("Error opening hardware\n");
		return 1; 
	}

	do {
		// TODO: Clear screen
		printf("\n");
		printf(" Light Organ (Licht Orgel)                                            @Cyb10101\n");
		printf("--------------------------------------------------------------------------------\n");
		printf(" [1] Toggle leds\n");
		printf(" [2] From left to right\n");
		printf(" [3] From right to left\n");
		printf(" [4] From right to left and backwards\n");
		printf(" [5] From decimal 0 to 255\n");
		printf("\n");
		printf(" [0] Quit\n");
		printf("--------------------------------------------------------------------------------\n");
		printf(" Choose: ");
		scanf("%d", &choose);
		printf("\n");

		switch(choose) {
			case 1: runToggleLeds(); break;
			case 2: runLedsFromLeftToRight(); break;
			case 3: runLedsFromRightToLeft(); break;
			case 4: runLedsFromLeftToRightToLeft(); break;
			case 5: runLedsFromZeroToMax(); break;

			default:
				if(choose != 0) {
					printf("\nFalsche Eingabe!\n\n");
				}
		}

	} while(choose > 0);

	closeHardwarePE1();
	return 0;
}

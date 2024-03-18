// Written by: Rivoo Bagchi BGCRIV001
// Date created: 18/03/2024
// Programmed in: STM32CubeIDE
// Target: STM32F051
// Description:

// DEFINES AND INCLUDES
#define STM32F051
#include "stm32f0xx.h"
#include "lcd_stm32f0.h"
#define HIGH 1
#define LOW 0

// GLOBAL VARIABLES
enum motion{back=-1, stop, forward, right, left};
struct robot_status{
	enum motion movement;
	_Bool ir_left;
	_Bool ir_right;
} robot_state;


// GLOBAL CONSTANTS
char str[16];
char status_str[30];

// FUNCTION DECLARATIONS
void main(void);
void display(void);

// MAIN FUNCTION
void main(void)
{
	// Power on phase
	// Robot_state initialised
	init_LCD();
	robot_state.movement = stop;
	robot_state.ir_left = LOW;
	robot_state.ir_right = LOW;
	display();

	while(1){

	}
}

// FUNCTION DEFINITIONS

void display(void) {
	sprintf(str,"%d", robot_state.movement);
	lcd_putstring("str");
	lcd_command(LINE_TWO);
	sprintf(status_str, "IR0:%d-IR1:%d", robot_state.ir_left, robot_state.ir_right);
	lcd_putstring(status_str);
}

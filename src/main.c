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
void init_GPIOC(void);
void display(void);
void sensor_polling(void);

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
	lcd_putstring(str);
	lcd_command(LINE_TWO);
	sprintf(status_str, "IR0:%d-IR1:%d", robot_state.ir_left, robot_state.ir_right);
	lcd_putstring(status_str);
}

void init_GPIOA(void){
	RCC -> AHBENR |= RCC_AHBENR_GPIOBEN;
	GPIOA -> MODER |= GPIO_MODER_MODER12_0;
	GPIOA -> MODER |= GPIO_MODER_MODER13_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR12_1;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPDR13_1;
}

void sensor_polling(void){
	robot_state.ir_left = GPIOA -> IDR & GPIO_IDR_0;
	robot_state.ir_right = GPIOA -> IDR & GPIO_IDR_1;
}



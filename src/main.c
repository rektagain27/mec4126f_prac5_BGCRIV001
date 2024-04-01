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
void init_SW3(void);
void EXTI3_1_IRQHandler(void);

// MAIN FUNCTION
void main(void)
{
	// Power on phase
	// Robot_state initialised
	init_LCD();
	display();
	robot_state.movement = stop;
	robot_state.ir_left = LOW;
	robot_state.ir_right = LOW;

	while(1){
		sensor_polling();
		delay(100000);
		lcd_command(CLEAR);
		display();
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

void init_SW3(void){
RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
GPIOA->MODER&= ~GPIO_MODER_MODER3;
GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_0;
}

void init_external_interrupts(){
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;
	EXTI-> IMR |= EXTI_IMR_MR3;
	EXTI-> FTSR |= EXTI_FTSR_TR3;
	NVIC_EnableIRQ(EXTI2_3_IRQn);

}

void EXTI2_3_IRQHandler(void){
	// 0.01 second delay
	delay(10000);
	if ( (robot_state.movement == stop)&&(GPIOA -> IDR & GPIO_IDR_3)){
		robot_state.movement = forward;
		GPIOB -> ODR |= (GPIO_ODR_2|GPIO_ODR_6);
		display();
	} else if ((robot_state.movement == forward)&&(GPIOA -> IDR & GPIO_IDR_3) ){
		robot_state.movement = stop;
		GPIOB -> ODR &= ~(GPIO_ODR_2|GPIO_ODR_6);
		display();
	}

	EXTI -> PR |= EXTI_PR_PR3;
}


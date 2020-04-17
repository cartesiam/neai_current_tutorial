/**
*******************************************************************************
* @file   main.cpp
* @brief  Main program body
*******************************************************************************
* Main program for collecting data and testing NanoEdge AI solution
*
* Compiler Flags
* -DNEAI_MODE : test mode with NanoEdge AI library
*
* @note   if no compiler flag then data logging mode by default
*******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
#include "NanoEdgeAI.h"

/* Objects -------------------------------------------------------------------*/
AnalogIn clamp(A0);
Serial pc(USBTX, USBRX);

/* Defines -------------------------------------------------------------------*/
#define SAMPLE_SIZE 1024
#define BAUD_RATE 115200
#define LEARN_NUMBER 100
#define DATARATE_HZ 2048 /* Hz */ 

/* Buffer with current values ------------------------------------------------*/
float clamp_values[SAMPLE_SIZE] = {0};

/* Functions prototypes ------------------------------------------------------*/
void fill_clamp_buffer(void);
void print_clamp_buffer(void);

/* BEGIN CODE-----------------------------------------------------------------*/
/**
  * @brief  The application entry point
  *
  * @retval int
  */
int main()
{
	pc.baud(BAUD_RATE);
#ifdef NEAI_MODE		
	NanoEdgeAI_initialize();

	for (uint16_t i = 0; i < LEARN_NUMBER; i++) {
		fill_clamp_buffer();
		NanoEdgeAI_learn(clamp_values);
		pc.printf("learn = %d\n", i);
	}
#endif
	
	while(1) {
		fill_clamp_buffer();

#ifdef NEAI_MODE		
		uint8_t score = NanoEdgeAI_detect(clamp_values);
		pc.printf("score = %d\n", score);
#else
		print_clamp_buffer();
#endif
		wait_ms(500);
	}
}

/* Functions definition ------------------------------------------------------*/
void fill_clamp_buffer()
{
	float datarate = ( 1.0 / DATARATE_HZ) * 1000; /* datarate from Hz to ms */
	for (uint16_t i = 0; i < SAMPLE_SIZE; i++) {
		clamp_values[i] = clamp.read();
		wait_ms(datarate); /* represent 2kHz */
	}
}

void print_clamp_buffer()
{
	for (uint16_t i = 0; i < SAMPLE_SIZE; i++) {
		pc.printf("%.4f ", clamp_values[i]);
	}
	pc.printf("\n");
}
/* END CODE-------------------------------------------------------------------*/						 

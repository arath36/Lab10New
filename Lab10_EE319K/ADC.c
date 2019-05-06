// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0
// Last Modified: 11/6/2018
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// Initializes AD0 and ADC1 for PE4/5 and PD0/1
void ADC_Init(void){ 
	
	// Initialize ADC0 for Port 
	volatile int delay;
	SYSCTL_RCGCADC_R |= 0x00000001; // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // 1) activate clock for Port E
  delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTE_DIR_R &= ~0x30;      // 3) make PE4 PE5 input
  GPIO_PORTE_AFSEL_R |= 0x30;     // 4) enable alternate function on PE4 PE5
  GPIO_PORTE_DEN_R &= ~0x30;      // 5) disable digital I/O on PE4 PE5
                                  // 5a) configure PE4 as ?? (skip this line because PCTL is for digital only)
  GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0xFF00FFFF;
  GPIO_PORTE_AMSEL_R |= 0x30;     // 6) enable analog functionality on PE4 PE5
  ADC0_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC0_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC0_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC0_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 2
  ADC0_EMUX_R &= ~0x0F00;         // 11) seq2 is software trigger
  ADC0_SSMUX2_R = 0x0089;         // 12) set channels for SS2
  ADC0_SSCTL2_R = 0x0060;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
  ADC0_IM_R &= ~0x0004;           // 14) disable SS2 interrupts
  ADC0_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 2
	
	// Initialize ADC1 for PD0 and PD1
		SYSCTL_RCGCADC_R |= 0x00000002; // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= 0x08; // 1) activate clock for Port D
  delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTD_DIR_R &= ~0x03;      // 3) make PD1 PD0 input
  GPIO_PORTD_AFSEL_R |= 0x03;     // 4) enable alternate function on PD0 & PD1
  GPIO_PORTD_DEN_R &= ~0x03;      // 5) disable digital I/O on PD0 and PD1
                                  // 5a) configure PE4 as ?? (skip this line because PCTL is for digital only)
  GPIO_PORTD_PCTL_R = GPIO_PORTD_PCTL_R&0xFF00FFFF;
  GPIO_PORTD_AMSEL_R |= 0x03;     // 6) enable analog functionality on PD0 and PD1
  ADC1_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC1_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC1_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC1_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 2
  ADC1_EMUX_R &= ~0x0F00;         // 11) seq2 is software trigger
  ADC1_SSMUX2_R = 0x0067;         // 12) set channels for SS2
  ADC1_SSCTL2_R = 0x0060;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
  ADC1_IM_R &= ~0x0004;           // 14) disable SS2 interrupts
  ADC1_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 2
}

// Samples PE4 and PE5

void ADC_In89(uint32_t data[2]){ 
	
	
  ADC0_PSSI_R = 0x0004;            // 1) initiate SS2
  while((ADC0_RIS_R&0x04)==0){};   // 2) wait for conversion done
  data[1] = ADC0_SSFIFO2_R&0xFFF;  // 3A) read first result
  data[0] = ADC0_SSFIFO2_R&0xFFF;  // 3B) read second result
  ADC0_ISC_R = 0x0004;             // 4) acknowledge completion
}


// Samples PD0 and PD1

void ADC_In67(uint32_t data[2]){ 
	
	
  ADC1_PSSI_R = 0x0004;            // 1) initiate SS2
  while((ADC1_RIS_R&0x04)==0){};   // 2) wait for conversion done
  data[1] = ADC1_SSFIFO2_R&0xFFF;  // 3A) read first result
  data[0] = ADC1_SSFIFO2_R&0xFFF;  // 3B) read second result
  ADC1_ISC_R = 0x0004;             // 4) acknowledge completion
}

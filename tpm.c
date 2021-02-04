#include "MKL05Z4.h"
#include "tpm.h"
static uint8_t tpm0Enabled = 0;
//void TPM0_Init_PWM(void) {
//		
//  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;	// Enable TPM0 mask in SCGC6 register
//	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(3);  // Choose MCGFLLCLK clock source

//	PORTB->PCR[11] = PORT_PCR_MUX(2);		// TPM0_CH0

//	TPM0->SC |= (TPM_SC_PS(7) | TPM_SC_CMOD(1)); 	// Set prescaler to 128, select the internal input clock source

//	TPM0->MOD = 65; 										// Set modulo value to 65, the value chosen so we get period = ~0,2ms
//	
//	TPM0->SC &= ~TPM_SC_CPWMS_MASK; 		/* up counting */
//	TPM0->CONTROLS[0].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK); /* clear Output on match, set Output on reload */ 

//	//TPM0->CONTROLS[0].CnV = 0; 				// For CH0 -> according to the documentation this is reset value, so it is not necesserily
//}
void TPM0_Init_PWM(void) {
		
  SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;		// ToDo 3.1.1: Enable TPM0 mask in SCGC6 register
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);  // ToDo 3.1.1: Choose MCGFLLCLK clock source

	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; // ToDo 3.1.2: Connect port B to clock
	PORTB->PCR[9] = PORT_PCR_MUX(2);		// ToDo 3.1.2: Set multiplekser to TPM0 for PTB9, get channel number (page 148 of the Reference Manual)

	TPM0->SC |= TPM_SC_PS(7);  					// ToDo 3.1.3: Set prescaler to 128
	TPM0->SC |= TPM_SC_CMOD(1);					// ToDo 3.1.4: For TMP0, select the internal input clock source

	TPM0->MOD = 97; 										// ToDo 3.1.5: Set modulo value to maximum value from slider
	
// ToDo 3.1.6: Connect correct channel from TPM0 for "Edge-aligned PWM Low-true pulses" mode
	TPM0->SC &= ~TPM_SC_CPWMS_MASK; 		/* up counting */
	TPM0->CONTROLS[2].CnSC |= (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK); /* set Output on match, clear Output on reload */ 
  TPM0->CONTROLS[2].CnSC &= ~(TPM_CnSC_MSA_MASK | TPM_CnSC_ELSB_MASK);
	TPM0->CONTROLS[2].CnV = 50; 				// ToDo 3.1.7: Set starting value to 50
	tpm0Enabled = 1;  /* set local flag */
}

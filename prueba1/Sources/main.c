#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */

#pragma DATA_SEG _DATA_ZEROPAGE
#pragma DATA_SEG DEFAULT

int control=0;
byte ano=1;
void main(void) {

  
  /* include your code here */
  SOPT1  = 0x23;                          // Disable COP,RSTO, enable STOP,BKGD,RESET
  SOPT2  = 0x00;                          // SPI1 on PTB2/3/4/5, IIC1 on PTA2/3
  SPMSC1 = 0x00;                          // Disable LVD
  SPMSC2 = 0x00;                          // Disable power-down modes
  SPMSC3 = 0x00;                          // Disable LVWIE, low trip points
  SCGC1  = 0xFF;                          // Enable bus clock to peripherals
  SCGC2  = 0xFF;  

  PTAPE = 0xFF;                           // Enable PORT A Internal Pullups
  PTBPE = 0xFF;                           // Enable PORT B Internal Pullups
  PTCPE = 0xFF;                           // Enable PORT C Internal Pullups
  PTDPE = 0xFF;                           // Enable PORT D Internal Pullups
  PTEPE = 0xFF;                           // Enable PORT E Internal Pullups
  PTFPE = 0xFF;                           // Enable PORT F Internal Pullups
  PTGPE = 0x0F;                           // Enable PTG0/1/2/3 Internal Pullups
  PTHPE = 0xC3;                           // Enable PTH0/1/6/7 Internal Pullups
 
  PTEDD = 0xFF;
  PTCDD = 0xFF;
  PTCD = 0xFF;
  
  EnableInterrupts; /* enable interrupts */
  
  
  TPM1MODH = 0x3E;
  TPM1MODL = 0x80;
  TPM1SC = 0b01101111;
  TPM3MODH = 0x7D;
  TPM3MODL = 0x00;
  TPM3C0SC = 0x24;
  TPM3C0VH = 0x19;
  TPM3C0VL = 0x00;
  TPM3SC = 0b00101000;
  PTED = 0xFF-0x40;
  TPM3C1SC = 0x24;
  TPM3C1VH = 0x3D;
  TPM3C1VL = 0x80;
  TPM3SC = 0b00101000;
  TPM3C2SC = 0x24;
  TPM3C2VH = 0x29;
  TPM3C2VL = 0xAA;
  TPM3SC = 0b00101000;
  //word temp=0;
  
  for(;;) {
  //temp++;
   //if(temp>0xFFFE){
    //if(ano==1) ano=0;
   // else ano=1;
   // temp=0;
   // PTED_PTED7 = 0;
  // }
  
    if(ano==0){
    PTED_PTED6 = 0; 
    }else{
    PTED_PTED6 = 1;  
    }
    
    //__RESET_WATCHDOG(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

interrupt VectorNumber_Vtpm1ovf void TMP1_OVF_ISR(void){
  
 //control++;
// if(control >= 2){
    if(ano==1) ano=0;
    else ano=1;
  // control=0;
 //}
 PTED_PTED7 = 0;
 TPM1SC_TOF=0;
PTCD= (byte) (0xFF-((control) & 0xFF));
}         
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include <stdio.h> 
#include <math.h> 


#pragma DATA_SEG _DATA_ZEROPAGE
#pragma DATA_SEG DEFAULT

int control=0;
byte ano=1;
word val=0;
word dutyy=0;
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
  
  byte done = 0x00;
  ADCSC1 = 0x1F;                          /* Disable interrupts 
                                             Disable continuous conversions
                                             Disable ADC by setting all ADCH bits */
  ADCSC2 = 0x40;                          /* Select H/W trigger
                                             Disable compare function */
  ADCCFG = 0x07; 	                        /* ADIV=input clock/8
                                             ADLSMP=long sample time
                                             MODE=8-bit conversion
                                             ADICLK=async clock */
  APCTL1 = 0x01;                          // Enable ADP0 as ADC input
  
  RTCMOD = 0x01;                          // set ADC conversion period
  RTCSC  = 0x0B;                          // 10ms period, no interrupt
  
  ADCSC1_AIEN = 0x01;
  ADCSC1_ADCH = 0x00;
  
  PTEDD = 0xFF;
  PTCDD = 0xFF;
  PTCD = 0xFF;
  
  
  
  EnableInterrupts; /* enable interrupts */
  
  /*TPM1MODH = 0x09;
  TPM1MODL = 0x00;
  TPM1SC = 0b00101000;
  TPM1C1SC = 0x24;
  TPM1C1VH = 0x04;
  TPM1C1VL = 0x80; */
  TPM3MODH = 0x0F;
  TPM3MODL = 0xFF;
  TPM3C0SC = 0x24;
  TPM3C0VH = 0x07;
  TPM3C0VL = 0xFF;
  TPM3SC = 0b00101000;
  PTED = 0xFF-0x40;
  TPM3C1SC = 0x24;
  TPM3C1VH = 0x04;
  TPM3C1VL = 0x80;
  TPM3SC = 0b00101000;
  TPM3C2SC = 0x24;
  TPM3C2VH = 0x01;
  TPM3C2VL = 0xFF;
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

/*interrupt VectorNumber_Vtpm1ovf void TMP1_OVF_ISR(void){
  
 //control++;
// if(control >= 2){
    if(ano==1) ano=0;
    else ano=1;
  // control=0;
 //}
 PTED_PTED7 = 0;
 TPM1SC_TOF=0;
PTCD= (byte) (0xFF-((control) & 0xFF));
} */
interrupt VectorNumber_Vadc void   ADC_ISR(void) {

  word adc_val;
    
  adc_val  = (word) (ADCRH<<8);                     // Read ADC value, clear flag
  adc_val |= (word) (ADCRL);                       // 8-bit, ignore high byte
  word duty;
  word MOD=0x0FFF;
  val=log(0x1000-adc_val+3)*101;
  dutyy = (word)(MOD)/(val/100);
  /*if (MOD>100){
              val = (word)(val*100)/0x0FFF*(MOD/100);
           }
           else {
              val = (word)(val*100)/0x0FFF*MOD/100;
           }     */
   if(val<=70) val=70;        
  TPM3C1VH = (byte) (val>>8);
  TPM3C1VL = (byte) (val);
  TPM3MODH = 0x0F;
  TPM3MODL = 0xFF;
  TPM3C0SC = 0x24; 
  TPM3SC = 0b00101000;                   
 /* word uu=(word)((0x05F3+100*log(adc_val+1)));
  //word uu=(word)(1000*log10(adc_val));
  word vv= (word)(uu>>1);
  TPM1MODH = (byte) (uu>>8);
  TPM1MODL = (byte) (uu);
  TPM1C0VH = (byte) (vv>>8);
  TPM1C0VL = (byte) (vv); */
} 
interrupt VectorNumber_Vrtc void   RTC_ISR(void) {
  /*if (some_key_pressed) {
     RTCMOD = 0x01;                       // set period 10ms
     RTCSC  = 0x0B;                       // Disable RTIE
     key_press_debounced = yes;
  }     */
  RTCSC_RTIF = 1;                         // clear flag
}
        
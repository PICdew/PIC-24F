/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/
#define iPPSInput(fn,pin) fn=pin
#define USE_AND_OR
//#define FCY 16000000UL

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__PIC24E__)
    	#include <p24Exxxx.h>
    #elif defined (__PIC24F__)||defined (__PIC24FK__)
	#include <p24Fxxxx.h>
    #elif defined(__PIC24H__)
	#include <p24Hxxxx.h>
    #endif
#endif

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include <libpic30.h>
#include "user.h"          /* User funct/params, such as InitApp              */
#include "uart.h"
#include "PPS.h"
#include "outcompare.h"
#include "timer.h"
#include "spi.h"
#include "TLC_5940.h"

// int CONFIG2 __attribute__((space(prog), address(0x2ABFC))) = 0x9FF ;
_CONFIG2(
    POSCMOD_NONE &       // Primary Oscillator Select (Primary oscillator is disabled)
    IOL1WAY_OFF &         // IOLOCK One-Way Set Enable (The IOLOCK bit (OSCCON<6>) can be set once, provided the unlock sequence has been completed. Once set, the Peripheral Pin Select registers cannot be written to a second time.)
    OSCIOFNC_OFF &       // OSCO Pin Configuration (OSCO/CLKO/RC15 functions as CLKO (FOSC/2))
    FCKSM_CSDCMD &       // Clock Switching and Fail-Safe Clock Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
    FNOSC_FRCPLL &       // Initial Oscillator Select (Fast RC Oscillator with Postscaler and PLL module (FRCPLL))
    PLL96MHZ_ON &        // 96MHz PLL Startup Select (96 MHz PLL is enabled automatically on start-up)
    PLLDIV_NODIV &       // 96 MHz PLL Prescaler Select (Oscillator input is used directly (4 MHz input))
    IESO_OFF             // Internal External Switchover (IESO mode (Two-Speed Start-up) is disabled)
);
// int CONFIG1 __attribute__((space(prog), address(0x2ABFE))) = 0x7FFF ;
_CONFIG1(
    WDTPS_PS32768 &      // Watchdog Timer Postscaler (1:32,768)
    FWPSA_PR128 &        // WDT Prescaler (Prescaler ratio of 1:128)
    WINDIS_OFF &         // Windowed WDT (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
    FWDTEN_OFF &          // Watchdog Timer (Watchdog Timer is Disabled)
    ICS_PGx1 &           // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC1/PGED1)
    GWRP_OFF &           // General Segment Write Protect (Writes to program memory are allowed)
    GCP_OFF &            // General Segment Code Protect (Code protection is disabled)
    JTAGEN_OFF            // JTAG Port Enable (JTAG port is enabled)
);


unsigned char copFlash = 0;
unsigned char redPulseOnOff = 0;
unsigned char redPulse;

unsigned char seek_bar_value = 0;
unsigned char androidUpdate = 0;
unsigned char color;
unsigned char command;
unsigned char Rxdata[10];
unsigned char DataAvailable = 0;
unsigned char Txdata[] = "Microchip";
unsigned char phaseChange = 0;
unsigned char LED = 0;
unsigned char phaseChangeUpdate = 0;
unsigned int phaseSpeedDelay = 512;
unsigned int c, z, e = 0;

void __attribute__((interrupt,no_auto_psv)) _U1RXInterrupt(void){

  static unsigned char k = 0;

  U1RX_Clear_Intr_Status_Bit;
  LATDbits.LATD0 = LATDbits.LATD0^1;
//  LATDbits.LATD0 = 1;
  if(k == 0){
    //   junk = ReadUSART();
    //   TLC5940_Set_Red_GS(99);
       command = ReadUART1();
         if(command == 0x72 | command == 0x62 | command == 0x67 | command == 0x50 | command == 0x43 | command == 0x44 | command == 0x4B){
           color = command;
           k++;
         }
         else{
           k = 0;
         }
    }

    else if(k == 1){
    //   junk = ReadUSART();
    //   TLC5940_Set_Red_GS(0);
       seek_bar_value = ReadUART1();
       k = 0;
       androidUpdate = 1;
    }
  }

void __attribute__ ((interrupt,no_auto_psv)) _U1TXInterrupt(void)
{
   static UINT i=0;  
   LATDbits.LATD0 = 0;
   U1TX_Clear_Intr_Status_Bit;  
}

void __attribute__ ((interrupt,no_auto_psv)) _T1Interrupt(void)
{
  ir_high_priority_lpHandler();
  phaseChangeUpdate = 1;
  T1_Clear_Intr_Status_Bit;
 // ir_high_priority_lpHandler();
}
/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/

int16_t main(void)
{

    /* Configure the oscillator for the device */
  //   ConfigureOscillator();

  /* Initialize IO ports and peripherals */
//  InitApp();

    /* TODO <INSERT USER APPLICATION CODE HERE> */
    /*
  TRISDbits.TRISD0 = 0;
  TRISDbits.TRISD1 = 0;
  TRISDbits.TRISD2 = 0;
  TRISDbits.TRISD3 = 0;
  TRISDbits.TRISD4 = 0;
  TRISDbits.TRISD5 = 0;
  TRISDbits.TRISD6 = 0;
  TRISDbits.TRISD7 = 0;
  TRISDbits.TRISD11 = 1;
  LATDbits.LATD0 = 0;
  */
  PPSUnLock;
  iPPSInput(IN_FN_PPS_U1RX, IN_PIN_PPS_RP10);
  iPPSOutput(OUT_PIN_PPS_RP26, OUT_FN_PPS_U1TX);
  iPPSOutput(OUT_PIN_PPS_RP20, OUT_FN_PPS_OC1);
  iPPSOutput(OUT_PIN_PPS_RP23, OUT_FN_PPS_SDO1);
  iPPSOutput(OUT_PIN_PPS_RP24, OUT_FN_PPS_SCK1OUT);

  TRISDbits.TRISD0 = 0;
//  OpenOC1(OC_SYSCLK_SRC | OC_PWM_EDGE_ALIGN, OC_FALL_EDGE_TRANS_P1 | OC_SYNC_TRIG_IN_CURR_OC, 64, 0b11111);
  
  ConfigIntUART1(UART_RX_INT_EN | UART_RX_INT_PR4 | UART_TX_INT_EN | UART_TX_INT_PR4);
  OpenUART1(UART_EN | UART_NO_PAR_8BIT | UART_UEN_00 | UART_BRGH_FOUR | UART_DIS_ABAUD, UART_TX_ENABLE | UART_ADR_DETECT_DIS, 34);
   
   initialiseTlc5940();
   enable_pwm_and_interrupts();
   TLC5940_SetAllGS(0);
   __delay_ms(500);
   LATDbits.LATD0 = 0;
//   TLC5940_SetGS(9, 200);
//   TLC5940_Set_Green_GS(100);
/*
     while(1){
         __delay_ms(1000);
     //    IFS0bits.U1RXIF = 1;
     }
     }
*/
    while(1){

    e = e++;
    if(e > 5){
      increment_cop_counter();
      increment_red_sine_wave();
      e = 0;
      }
    c = c++;
    if(c > 512){
       c = 0;
    }
    if(phaseSpeedDelay < c){
      increment_RGB_sine_wave();
      c = 0;
    }
    if(U1STAbits.OERR == 1 | U1STAbits.FERR == 1 | U1STAbits.PERR == 1){
     z = U1STAbits.OERR;
   //  LATBbits.LATB1 = 1;
    }
    else{
   //   LATBbits.LATB1 = 0;
    }


/***********************************************************************************************************
This next section of code is going to check for two things:
(1) Should the sine wave phase change routine be happening.(As opposed to manual control)
(2) Is it time to update the gsdata[] array?

  - A Note should be made here about updating the gsdata[] array using the LED control functions.
    One must be aware of instances of the Interrupt while writing to the gsData[] array. If an interrupt occurs
    while writing to gsData[], the data could be corrupt.
***********************************************************************************************************/


      if((phaseChange == 1) && (phaseChangeUpdate == 1)){
        // increment_RGB_sine_wave();
         for(LED=1;LED<12;){
            implement_sine_wave(LED);
            LED = LED + 1;
         }
         phaseChangeUpdate = 0;
      }

      if((copFlash == 1)){

         for(LED=1;LED<12;){
            implement_cop_flasher(LED);
            LED = LED + 1;
         }
      }
      else{

      }

      if((redPulse == 1)){

         for(LED=1;LED<12;){
            implement_red_sine_wave(LED);
            LED = LED + 1;
         }
      }
      else{

      }

/***********************************************************************************************************
This next section of code is going to check a variable named "androidUpdate"
androidUpdate is set when a transmission is recieved on the USART. This Transmission sends two characters.
The first indicates which color to control(or enter phase change). and the second is a value for the corresponding color
intensity(or phase change speed).
***********************************************************************************************************/
   //   if(PORTBbits.RB0 == 1){
   //      while(PORTBbits.RB0 == 1);
   //      WriteUSART(0x79);
      //  LATAbits.LATA4 = 1;
  //    }


      if(androidUpdate == 1){

         switch(color){
           case 0x72:
             copFlash = 0;
             TLC5940_Set_Red_GS(seek_bar_value);
             androidUpdate = 0;
             phaseChange = 0;
           break;

           case 0x62:
             copFlash = 0;
             TLC5940_Set_Blue_GS(seek_bar_value);
             androidUpdate = 0;
             phaseChange = 0;
           break;

           case 0x67:
             copFlash = 0;
             TLC5940_Set_Green_GS(seek_bar_value);
             androidUpdate = 0;
             phaseChange = 0;
           break;

           case 0x50: // This isn't actually a color, it's the ASCII value P for phase change.
              copFlash = 0;
              TLC5940_SetAllGS(0);
              phaseChange = 1;
              androidUpdate = 0;
              phaseSpeedDelay = (512*((unsigned int)seek_bar_value))/100;
           break;

           case 0x43: // This isn't actually a color, it's the ASCII value "C" for "COP Lights". 0x43
              androidUpdate = 0;
              redPulse = 0;
              phaseChange = 0;
              TLC5940_SetAllGS(0);
              if(seek_bar_value > 0){
                 copFlash = 1;
              }
              else{
                  copFlash = 0;
              }
           break;

           case 0x44: // This isn't actually a color, it's the ASCII value D for Danger Red Pulse.
              androidUpdate = 0;

              if(seek_bar_value > 0){
                 redPulse = 1;
                 copFlash = 0;
              }
              else{
                 redPulse = 0;
               //  TLC5940_SetAllGS(0);
              }
           break;

           case 0x4B: // This isn't actually a color, it's the ASCII value "K" for kill.
              phaseChange = 0;
              androidUpdate = 0;
              copFlash = 0;
              redPulse = 0;
              TLC5940_SetAllGS(0);
           break;
         }
      }

   } 
}

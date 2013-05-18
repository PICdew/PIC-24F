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

#include "TLC_5940.h"
#include "PPS.h"
#include "outcompare.h"
#include "timer.h"
#include "spi.h"

#if (12 * NUMBEROF5940 > 255)
#define dcData_t unsigned int
#else
#define dcData_t unsigned char
#endif

#if (24 * NUMBEROF5940 > 255)
#define gsData_t unsigned int
#else
#define gsData_t unsigned char
#endif

#define dcDataSize (12 * NUMBEROF5940)
#define gsDataSize (24 * NUMBEROF5940)

/** ARRAYS *********************************************************/

const int sine_wave[] = {
4094 ,
4094 ,
4094 ,
4094 ,
4094 ,
4094 ,
4094 ,
4094 ,
4093 ,
4093 ,
4093 ,
4093 ,
4093 ,
4092 ,
4092 ,
4092 ,
4092 ,
4091 ,
4091 ,
4091 ,
4090 ,
4090 ,
4089 ,
4089 ,
4088 ,
4088 ,
4087 ,
4087 ,
4086 ,
4086 ,
4085 ,
4085 ,
4084 ,
4084 ,
4083 ,
4082 ,
4082 ,
4081 ,
4080 ,
4079 ,
4079 ,
4078 ,
4077 ,
4076 ,
4075 ,
4075 ,
4074 ,
4073 ,
4072 ,
4071 ,
4070 ,
4069 ,
4068 ,
4067 ,
4066 ,
4065 ,
4064 ,
4063 ,
4062 ,
4061 ,
4059 ,
4058 ,
4057 ,
4056 ,
4055 ,
4053 ,
4052 ,
4051 ,
4050 ,
4048 ,
4047 ,
4046 ,
4044 ,
4043 ,
4041 ,
4040 ,
4039 ,
4037 ,
4036 ,
4034 ,
4033 ,
4031 ,
4030 ,
4028 ,
4026 ,
4025 ,
4023 ,
4021 ,
4020 ,
4018 ,
4016 ,
4015 ,
4013 ,
4011 ,
4009 ,
4008 ,
4006 ,
4004 ,
4002 ,
4000 ,
3998 ,
3996 ,
3995 ,
3993 ,
3991 ,
3989 ,
3987 ,
3985 ,
3983 ,
3981 ,
3979 ,
3976 ,
3974 ,
3972 ,
3970 ,
3968 ,
3966 ,
3964 ,
3961 ,
3959 ,
3957 ,
3955 ,
3952 ,
3950 ,
3948 ,
3945 ,
3943 ,
3941 ,
3938 ,
3936 ,
3933 ,
3931 ,
3928 ,
3926 ,
3923 ,
3921 ,
3918 ,
3916 ,
3913 ,
3911 ,
3908 ,
3905 ,
3903 ,
3900 ,
3897 ,
3895 ,
3892 ,
3889 ,
3887 ,
3884 ,
3881 ,
3878 ,
3875 ,
3873 ,
3870 ,
3867 ,
3864 ,
3861 ,
3858 ,
3855 ,
3852 ,
3849 ,
3846 ,
3843 ,
3840 ,
3837 ,
3834 ,
3831 ,
3828 ,
3825 ,
3822 ,
3819 ,
3815 ,
3812 ,
3809 ,
3806 ,
3803 ,
3799 ,
3796 ,
3793 ,
3790 ,
3786 ,
3783 ,
3780 ,
3776 ,
3773 ,
3770 ,
3766 ,
3763 ,
3759 ,
3756 ,
3752 ,
3749 ,
3745 ,
3742 ,
3738 ,
3735 ,
3731 ,
3728 ,
3724 ,
3721 ,
3717 ,
3713 ,
3710 ,
3706 ,
3702 ,
3699 ,
3695 ,
3691 ,
3687 ,
3684 ,
3680 ,
3676 ,
3672 ,
3668 ,
3664 ,
3661 ,
3657 ,
3653 ,
3649 ,
3645 ,
3641 ,
3637 ,
3633 ,
3629 ,
3625 ,
3621 ,
3617 ,
3613 ,
3609 ,
3605 ,
3601 ,
3597 ,
3593 ,
3589 ,
3585 ,
3580 ,
3576 ,
3572 ,
3568 ,
3564 ,
3559 ,
3555 ,
3551 ,
3547 ,
3542 ,
3538 ,
3534 ,
3529 ,
3525 ,
3521 ,
3516 ,
3512 ,
3508 ,
3503 ,
3499 ,
3494 ,
3490 ,
3485 ,
3481 ,
3476 ,
3472 ,
3467 ,
3463 ,
3458 ,
3454 ,
3449 ,
3445 ,
3440 ,
3435 ,
3431 ,
3426 ,
3422 ,
3417 ,
3412 ,
3407 ,
3403 ,
3398 ,
3393 ,
3389 ,
3384 ,
3379 ,
3374 ,
3370 ,
3365 ,
3360 ,
3355 ,
3350 ,
3345 ,
3341 ,
3336 ,
3331 ,
3326 ,
3321 ,
3316 ,
3311 ,
3306 ,
3301 ,
3296 ,
3291 ,
3286 ,
3281 ,
3276 ,
3271 ,
3266 ,
3261 ,
3256 ,
3251 ,
3246 ,
3241 ,
3236 ,
3231 ,
3225 ,
3220 ,
3215 ,
3210 ,
3205 ,
3200 ,
3194 ,
3189 ,
3184 ,
3179 ,
3174 ,
3168 ,
3163 ,
3158 ,
3153 ,
3147 ,
3142 ,
3137 ,
3131 ,
3126 ,
3121 ,
3115 ,
3110 ,
3105 ,
3099 ,
3094 ,
3088 ,
3083 ,
3078 ,
3072 ,
3067 ,
3061 ,
3056 ,
3050 ,
3045 ,
3039 ,
3034 ,
3028 ,
3023 ,
3017 ,
3012 ,
3006 ,
3001 ,
2995 ,
2989 ,
2984 ,
2978 ,
2973 ,
2967 ,
2961 ,
2956 ,
2950 ,
2945 ,
2939 ,
2933 ,
2928 ,
2922 ,
2916 ,
2911 ,
2905 ,
2899 ,
2893 ,
2888 ,
2882 ,
2876 ,
2871 ,
2865 ,
2859 ,
2853 ,
2847 ,
2842 ,
2836 ,
2830 ,
2824 ,
2818 ,
2813 ,
2807 ,
2801 ,
2795 ,
2789 ,
2783 ,
2778 ,
2772 ,
2766 ,
2760 ,
2754 ,
2748 ,
2742 ,
2736 ,
2730 ,
2724 ,
2719 ,
2713 ,
2707 ,
2701 ,
2695 ,
2689 ,
2683 ,
2677 ,
2671 ,
2665 ,
2659 ,
2653 ,
2647 ,
2641 ,
2635 ,
2629 ,
2623 ,
2617 ,
2611 ,
2605 ,
2599 ,
2593 ,
2587 ,
2581 ,
2574 ,
2568 ,
2562 ,
2556 ,
2550 ,
2544 ,
2538 ,
2532 ,
2526 ,
2520 ,
2514 ,
2507 ,
2501 ,
2495 ,
2489 ,
2483 ,
2477 ,
2471 ,
2464 ,
2458 ,
2452 ,
2446 ,
2440 ,
2434 ,
2427 ,
2421 ,
2415 ,
2409 ,
2403 ,
2397 ,
2390 ,
2384 ,
2378 ,
2372 ,
2366 ,
2359 ,
2353 ,
2347 ,
2341 ,
2335 ,
2328 ,
2322 ,
2316 ,
2310 ,
2303 ,
2297 ,
2291 ,
2285 ,
2278 ,
2272 ,
2266 ,
2260 ,
2254 ,
2247 ,
2241 ,
2235 ,
2228 ,
2222 ,
2216 ,
2210 ,
2203 ,
2197 ,
2191 ,
2185 ,
2178 ,
2172 ,
2166 ,
2160 ,
2153 ,
2147 ,
2141 ,
2134 ,
2128 ,
2122 ,
2116 ,
2109 ,
2103 ,
2097 ,
2091 ,
2084 ,
2078 ,
2072 ,
2065 ,
2059 ,
2053 ,
2047 ,
2040 ,
2034 ,
2028 ,
2021 ,
2015 ,
2009 ,
2003 ,
1996 ,
1990 ,
1984 ,
1978 ,
1971 ,
1965 ,
1959 ,
1952 ,
1946 ,
1940 ,
1934 ,
1927 ,
1921 ,
1915 ,
1909 ,
1902 ,
1896 ,
1890 ,
1883 ,
1877 ,
1871 ,
1865 ,
1858 ,
1852 ,
1846 ,
1840 ,
1833 ,
1827 ,
1821 ,
1815 ,
1808 ,
1802 ,
1796 ,
1790 ,
1784 ,
1777 ,
1771 ,
1765 ,
1759 ,
1752 ,
1746 ,
1740 ,
1734 ,
1728 ,
1721 ,
1715 ,
1709 ,
1703 ,
1697 ,
1690 ,
1684 ,
1678 ,
1672 ,
1666 ,
1660 ,
1653 ,
1647 ,
1641 ,
1635 ,
1629 ,
1623 ,
1616 ,
1610 ,
1604 ,
1598 ,
1592 ,
1586 ,
1580 ,
1574 ,
1567 ,
1561 ,
1555 ,
1549 ,
1543 ,
1537 ,
1531 ,
1525 ,
1519 ,
1513 ,
1507 ,
1501 ,
1495 ,
1488 ,
1482 ,
1476 ,
1470 ,
1464 ,
1458 ,
1452 ,
1446 ,
1440 ,
1434 ,
1428 ,
1422 ,
1416 ,
1410 ,
1404 ,
1398 ,
1393 ,
1387 ,
1381 ,
1375 ,
1369 ,
1363 ,
1357 ,
1351 ,
1345 ,
1339 ,
1333 ,
1327 ,
1322 ,
1316 ,
1310 ,
1304 ,
1298 ,
1292 ,
1286 ,
1281 ,
1275 ,
1269 ,
1263 ,
1257 ,
1252 ,
1246 ,
1240 ,
1234 ,
1228 ,
1223 ,
1217 ,
1211 ,
1206 ,
1200 ,
1194 ,
1188 ,
1183 ,
1177 ,
1171 ,
1166 ,
1160 ,
1154 ,
1149 ,
1143 ,
1137 ,
1132 ,
1126 ,
1121 ,
1115 ,
1109 ,
1104 ,
1098 ,
1093 ,
1087 ,
1082 ,
1076 ,
1071 ,
1065 ,
1059 ,
1054 ,
1049 ,
1043 ,
1038 ,
1032 ,
1027 ,
1021 ,
1016 ,
1010 ,
1005 ,
1000 ,
994 ,
989 ,
983 ,
978 ,
973 ,
967 ,
962 ,
957 ,
951 ,
946 ,
941 ,
936 ,
930 ,
925 ,
920 ,
914 ,
909 ,
904 ,
899 ,
894 ,
888 ,
883 ,
878 ,
873 ,
868 ,
863 ,
858 ,
852 ,
847 ,
842 ,
837 ,
832 ,
827 ,
822 ,
817 ,
812 ,
807 ,
802 ,
797 ,
792 ,
787 ,
782 ,
777 ,
772 ,
767 ,
763 ,
758 ,
753 ,
748 ,
743 ,
738 ,
733 ,
729 ,
724 ,
719 ,
714 ,
709 ,
705 ,
700 ,
695 ,
691 ,
686 ,
681 ,
677 ,
672 ,
667 ,
663 ,
658 ,
653 ,
649 ,
644 ,
640 ,
635 ,
631 ,
626 ,
621 ,
617 ,
612 ,
608 ,
604 ,
599 ,
595 ,
590 ,
586 ,
581 ,
577 ,
573 ,
568 ,
564 ,
560 ,
555 ,
551 ,
547 ,
543 ,
538 ,
534 ,
530 ,
526 ,
521 ,
517 ,
513 ,
509 ,
505 ,
501 ,
497 ,
492 ,
488 ,
484 ,
480 ,
476 ,
472 ,
468 ,
464 ,
460 ,
456 ,
452 ,
448 ,
445 ,
441 ,
437 ,
433 ,
429 ,
425 ,
421 ,
418 ,
414 ,
410 ,
406 ,
402 ,
399 ,
395 ,
391 ,
388 ,
384 ,
380 ,
377 ,
373 ,
369 ,
366 ,
362 ,
359 ,
355 ,
352 ,
348 ,
345 ,
341 ,
338 ,
334 ,
331 ,
327 ,
324 ,
321 ,
317 ,
314 ,
311 ,
307 ,
304 ,
301 ,
297 ,
294 ,
291 ,
288 ,
284 ,
281 ,
278 ,
275 ,
272 ,
269 ,
266 ,
263 ,
259 ,
256 ,
253 ,
250 ,
247 ,
244 ,
241 ,
238 ,
235 ,
233 ,
230 ,
227 ,
224 ,
221 ,
218 ,
215 ,
213 ,
210 ,
207 ,
204 ,
202 ,
199 ,
196 ,
194 ,
191 ,
188 ,
186 ,
183 ,
180 ,
178 ,
175 ,
173 ,
170 ,
168 ,
165 ,
163 ,
160 ,
158 ,
156 ,
153 ,
151 ,
148 ,
146 ,
144 ,
141 ,
139 ,
137 ,
135 ,
132 ,
130 ,
128 ,
126 ,
124 ,
122 ,
119 ,
117 ,
115 ,
113 ,
111 ,
109 ,
107 ,
105 ,
103 ,
101 ,
99 ,
97 ,
95 ,
93 ,
92 ,
90 ,
88 ,
86 ,
84 ,
83 ,
81 ,
79 ,
77 ,
76 ,
74 ,
72 ,
71 ,
69 ,
67 ,
66 ,
64 ,
63 ,
61 ,
60 ,
58 ,
57 ,
55 ,
54 ,
52 ,
51 ,
50 ,
48 ,
47 ,
46 ,
44 ,
43 ,
42 ,
40 ,
39 ,
38 ,
37 ,
36 ,
34 ,
33 ,
32 ,
31 ,
30 ,
29 ,
28 ,
27 ,
26 ,
25 ,
24 ,
23 ,
22 ,
21 ,
20 ,
19 ,
19 ,
18 ,
17 ,
16 ,
15 ,
15 ,
14 ,
13 ,
12 ,
12 ,
11 ,
10 ,
10 ,
9 ,
9 ,
8 ,
7 ,
7 ,
6 ,
6 ,
5 ,
5 ,
5 ,
4 ,
4 ,
3 ,
3 ,
3 ,
2 ,
2 ,
2 ,
2 ,
1 ,
1 ,
1 ,
1 ,
1 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0 ,
0



};

dcData_t dcData[12 * NUMBEROF5940] = {

  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
  0b11111111,
};

 gsData_t gsData[24 * NUMBEROF5940] = {

  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b10000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00001000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b10000000,
};

unsigned int r = 0;
unsigned int b = 1020;
unsigned int g = 1020;
unsigned int r_sin = 0;
unsigned int b_sin = 1020;
unsigned int g_sin = 1020;
unsigned int cop_flash_counter = 0;
unsigned int flashSpeed = 300;
unsigned int q = 1;
unsigned int wave_index = 1;

void initialiseTlc5940(void){

  unsigned char ledChannel;
  unsigned char dotCorrectionValues[16 * NUMBEROF5940];
	for (ledChannel = 0; ledChannel < (16 * NUMBEROF5940); ledChannel++)
		dotCorrectionValues[ledChannel] = 63;

  setOutput(GSCLK_DDR, GSCLK_PIN);
  setOutput(SCLK_DDR, SCLK_PIN);
//  setOutput(DCPRG_DDR, DCPRG_PIN);
  setOutput(VPRG_DDR, VPRG_PIN);
  setOutput(XLAT_DDR, XLAT_PIN);
  setOutput(BLANK_DDR, BLANK_PIN);
  setOutput(SIN_DDR, SIN_PIN);

  setLow(GSCLK_LAT, GSCLK_PIN);
  setLow(SCLK_LAT, SCLK_PIN);
//  setLow(DCPRG_LAT, DCPRG_PIN);
  setHigh(VPRG_LAT, VPRG_PIN);
  setLow(XLAT_LAT, XLAT_PIN);
  setHigh(BLANK_LAT, BLANK_PIN);



//  setInitialDotCorrection(dotCorrectionValues);

  setInitialGrayScaleValues();

  OpenSPI1(ENABLE_SCK_PIN | SPI_MODE8_ON | MASTER_ENABLE_ON | SPI_CKE_ON | SPI_SMP_OFF | CLK_POL_ACTIVE_LOW, FRAME_ENABLE_OFF | SPI_ENH_BUFF_DISABLE, SPI_ENABLE);

}

// Set initial dot correction data
void setInitialDotCorrection(unsigned char *dotCorrectionValues)
{
    int ledChannel;
    unsigned char bitMask;
    int bitCounter;

	// Set VPRG high (Dot correction mode)
	setHigh(VPRG_LAT, VPRG_PIN);

	// We are passed an array of unsigned char values which are 8 bits each, however the dot
	// correction is expecting 6 bit data for each channel (0-63) so only send the 6 least
	// significant bits of each entry in the array.  The values need to be sent MSB first.
	for (ledChannel = 0; ledChannel < (16 * NUMBEROF5940); ledChannel++)
	{
		bitMask = 0b00100000;

		for (bitCounter = 5; bitCounter >= 0; bitCounter--)
		{
			// Set SIN to DC data bit
			TLC5940_SIN = 1; //(dotCorrectionValues[ledChannel] & bitMask) >> bitCounter;

			// Pulse the serial clock
			pulse(SCLK_LAT, SCLK_PIN);

			// Move to the next bit in the mask
			bitMask >>= 1;
		}
	}

	// Pulse XLAT
	pulse(XLAT_LAT, XLAT_PIN);
}

void setInitialGrayScaleValues(void){

    // Reset GSCLK_Counter = 0
	int GSCLKcounter = 0;

	// Reset Data_Counter = 0
	int dataCounter = 0;

	// Set VPRG = Low (Grayscale mode)
	setLow(VPRG_LAT, VPRG_PIN);

	// Set BLANK = High (Turn LED's Off)
	setHigh(BLANK_LAT, BLANK_PIN);

	for (GSCLKcounter = 0; GSCLKcounter < 4096; GSCLKcounter++){

		if (dataCounter > (NUMBEROF5940 * 192) )
		{
			// Pulse GSCLK
			pulse(GSCLK_LAT, GSCLK_PIN);
		}
		else
		{
			// Set SIN to the greyscale data bit
			TLC5940_SIN = 1; // We just output zero for everything during initialisation

			// Pulse SCLK
			pulse(SCLK_LAT, SCLK_PIN);

			// Increment Data_Counter
			dataCounter++;

			// Pulse GSCLK
			pulse(GSCLK_LAT, GSCLK_PIN);
		}
	}

	// Pulse XLAT to latch in GS data
	pulse(XLAT_LAT, XLAT_PIN);

	// Set BLANK = Low (Turn LED's on)
	setLow(BLANK_LAT, BLANK_PIN);

	// Send an extra SCLK pulse since this is the first grayscale cycle
	// after the dot correction data has been set

	// Pulse SCLK
	pulse(SCLK_LAT, SCLK_PIN);
}



void enable_pwm_and_interrupts(void){

  //------------Set up PWM1 to generate the GSCLK clock signal------------//

	// We want an overall PWM period of around 60Hz
	// There are 4096 PWM steps per period so we need to pulse
	// around 60Hz * 4096 steps = 245,760Hz, so we round up to
	// the next even value of 250,000 Hz
	//
	// PWM 1 is 250,000 Hz (250 KHz)
/***************************************************************************/

OpenOC1(OC_SYSCLK_SRC | OC_PWM_EDGE_ALIGN, OC_FALL_EDGE_TRANS_P1 | OC_SYNC_TRIG_IN_CURR_OC, 64, 0b11111);

/***************************************************************************/

  ConfigIntTimer1(T1_INT_PRIOR_1 | T1_INT_ON);
  OpenTimer1(T1_ON | T1_PS_1_8 | T1_SOURCE_INT, 0x7FFF);

}



// -------High priority interrupt procedure for XLAT and RGB LED fade processing---------//

void ir_high_priority_lpHandler(void){

    static unsigned char xlatNeedsPulse = 0;
    gsData_t i,j,k;



	// Is this timer1 interrupting (XLAT interrupt handler)?
      if(IFS0bits.T1IF){
      setHigh(BLANK_LAT, BLANK_PIN);

     // TRISAbits.TRISA6 = 1;
      if(LATDbits.LATD6 = 1){
     // if (outputState(VPRG_LAT, VPRG_PIN)){
        setLow(VPRG_PORT, VPRG_PIN);
        pulse(XLAT_LAT, XLAT_PIN);
        if (xlatNeedsPulse) {
          pulse(XLAT_LAT, XLAT_PIN);
          xlatNeedsPulse = 0;
        }
        pulse(SCLK_LAT, SCLK_PIN);
      }
      else{
        if (xlatNeedsPulse){
          pulse(XLAT_LAT, XLAT_PIN);
          xlatNeedsPulse = 0;
        }
      }
      setLow(BLANK_LAT, BLANK_PIN);


// Get ready for the next interrupt -----------------------------------
/*
	  TMR0H  = XLATCOUNTH;	// Reset the timer0 counter
	  TMR0L  = XLATCOUNTL;
	  INTCONbits.TMR0IF = 0; // Clear the timer0 interrupt flag
*/
// Below this we have 4096 cycles to shift in the data for the next cycle

      for (i = 0; i < gsDataSize; i++){

        // Start transmission

        WriteSPI1(gsData[i]);
        while (SPI1_Tx_Buf_Full);
        IFS0bits.SPI1IF = 0;
      }
      xlatNeedsPulse = 1;
      T1_Clear_Intr_Status_Bit;
     }
  
}


void TLC5940_ClockInDC(void) {

  unsigned char Counter = 0;
  dcData_t i;

  setHigh(DCPRG_LAT, DCPRG_PIN);
  setHigh(VPRG_LAT, VPRG_PIN);
/*
  for (i = 0; i < dcDataSize; i++) {
    PIR1bits.SSPIF = 0;
    // Start transmission
    SSPBUF = dcData[i];
    // Wait for transmission complete
    while (PIR1bits.SSPIF == 0);
    PIR1bits.SSPIF = 0;
  }
 */
  pulse(XLAT_LAT, XLAT_PIN);
}

void TLC5940_SetAllGS(unsigned int value) {

   unsigned char tmp1 = (unsigned char)(value >> 4);
   unsigned char tmp2 = (unsigned char)((value << 4) | (tmp1 >> 4));

   unsigned char i = 0;

    do {
       gsData[i] = tmp1; // bits: 11 10 09 08 07 06 05 04
       i++;
       gsData[i] = tmp2; // bits: 03 02 01 00 11 10 09 08
       i++;
       gsData[i] = (unsigned char)value; // bits: 07 06 05 04 03 02 01 00
       i++;
   } while (i < gsDataSize);
}


void TLC5940_SetGS(channel_t channel, unsigned int value) {

   unsigned int i;
   channel = numChannels - 1 - channel;  // 0 through 15 ?
   i = (unsigned int)channel * 3 / 2;


   switch (channel % 2) {
     case 0:
       gsData[i] = (value >> 4);
       i++;
       gsData[i] = (gsData[i] & 0x0F) | (unsigned char)(value << 4);
     break;

     default: // case 1:
       gsData[i] = (gsData[i] & 0xF0) | (value >> 8);
       i++;
       gsData[i] = (unsigned char)value;
     break;
   }
}



void increment_red_sine_wave(void){


     if((q==1)&&(r_sin<=1020)){
       r_sin = r_sin + 2;
     }

     else if((q==1)&&(r_sin==1022)){
       //r_sin = r_sin - 2;
       q = 0;
     }

     else if((q==0)&&(r_sin>700)){
       r_sin = r_sin - 2;
     }

     else if(r_sin==700){
      // r_sin = r_sin + 2;
       q = 1;
     }

}

void increment_RGB_sine_wave(void){


     if((g==1020)&&(r<=1016)){
       b = b - 4;
       r = r + 4;
     }

     else if((r==1020)&&(b<=1016)){
       g = g - 4;
       b = b + 4;
     }

     else if((b==1020)&&(g<=1016)){
       g = g + 4;
       r = r - 4;
     }
}


 void increment_cop_counter(void){
      cop_flash_counter = cop_flash_counter++;
 }

 void implement_sine_wave(unsigned char led){

       char output;
       output = 3*(led-1);
       TLC5940_SetGS(output, sine_wave[b]);
       TLC5940_SetGS(output + 1, sine_wave[r]);
       TLC5940_SetGS(output + 2, sine_wave[g]);

}

 void implement_red_sine_wave(unsigned char led){

       char output;
       output = 3*(led-1);
       TLC5940_SetGS(output + 0, sine_wave[r_sin]);

}

 void implement_cop_flasher(unsigned char led){

       char output;
       output = 3*(led-1);
       if(cop_flash_counter < (flashSpeed/2)){
          TLC5940_SetGS(output + 1, sine_wave[0]);
          TLC5940_SetGS(output, sine_wave[1020]);
       }
       if(cop_flash_counter > (flashSpeed/2)){
          TLC5940_SetGS(output + 1, sine_wave[1020]);
          TLC5940_SetGS(output, sine_wave[0]);
          if(cop_flash_counter > (flashSpeed)){
             cop_flash_counter = 0;
          }
       }
}

void TLC5940_Set_Red_GS(unsigned char value){

   unsigned long precent_brightness = ((unsigned long)value*100)/100;
   unsigned long value_2 = (4095*precent_brightness)/100;
   unsigned int value_3 = (unsigned int)value_2;
   unsigned char i = 0;

   for(i=0;i<((NUMBEROF5940*16)-1);){
      TLC5940_SetGS(i, value_3);
      i = i + 3;
   }

}

void TLC5940_Set_Blue_GS(unsigned char value){

   unsigned long precent_brightness = ((unsigned long)value*100)/100;
   unsigned long value_2 = (4095*precent_brightness)/100;
   unsigned int value_3 = (unsigned int)value_2;
   unsigned char i = 1;

   for(i=1;i<((NUMBEROF5940*16)-1);){           //((NUMBEROF5940*16)-1)
      TLC5940_SetGS(i, value_3);
      i = i + 3;
   }

}

void TLC5940_Set_Green_GS(unsigned char value){

   unsigned long precent_brightness = ((unsigned long)value*100)/100;
   unsigned long value_2 = (4095*precent_brightness)/100;
   unsigned int value_3 = (unsigned int)value_2;
   unsigned char i = 2;

   for(i=2;i<((NUMBEROF5940*16)-1);){
      TLC5940_SetGS(i, value_3);
      i = i + 3;
   }

}

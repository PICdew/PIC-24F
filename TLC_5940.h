
#define GSCLK_DDR TRISD
#define GSCLK_PORT PORTD
#define GSCLK_LAT LATD
#define GSCLK_PIN 5
#define SIN_DDR TRISD
#define TLC5940_SIN LATDbits.LATD2
#define SIN_PORT PORTD
#define SIN_LAT LATD
#define SIN_PIN 2
#define SCLK_DDR TRISD
#define SCLK_PORT PORTD
#define SCLK_LAT LATD
#define SCLK_PIN 1
#define BLANK_DDR TRISD
#define BLANK_PORT PORTD
#define BLANK_LAT LATD
#define BLANK_PIN 3
#define DCPRG_DDR TRISD
#define DCPRG_PORT PORTD
#define DCPRG_LAT LATD
#define DCPRG_PIN 1
#define VPRG_DDR TRISD
#define VPRG_PORT PORTD
#define VPRG_LAT LATD
#define VPRG_PIN 6
#define XLAT_DDR TRISD
#define XLAT_PORT PORTD
#define XLAT_LAT LATD
#define XLAT_PIN 4

#define XLATCOUNTH 		0xbF
#define XLATCOUNTL 		0xFF

#define setOutput(ddr, pin) ddr = ((ddr) &= ~(1 << (pin)))
#define setInput(ddr, pin) ddr = ((ddr) |= (1 << (pin)))
#define setLow(latch, pin) ((latch) &= ~(1 << (pin)))
#define setHigh(latch, pin) ((latch) |= (1 << (pin)))
#define pulse(latch, pin) do{ \
                             setHigh((latch), (pin)); \
                             setLow((latch), (pin)); \
                         } while (0)

#if (16 * NUMBEROF5940 > 255)
#define channel_t unsigned int
#else
#define channel_t unsigned char
#endif
#define numChannels (16 * NUMBEROF5940)

#define NUMBEROF5940 2
#define outputState(latch, pin) ((latch) &= (1 << (pin)))

void TLC5940_SetGS(channel_t channel, unsigned int value);

void TLC5940_ClockInDC(void);

void initialiseTlc5940(void);

void ir_high_priority_lpHandler(void);

void enable_pwm_and_interrupts(void);

void TLC5940_SetAllGS(unsigned int value);

void setInitialGrayScaleValues(void);

void setInitialDotCorrection(unsigned char *dotCorrectionValues);

void increment_RGB_sine_wave(void);

void increment_red_sine_wave(void);

void TLC5940_Set_Red_GS(unsigned char value);

void TLC5940_Set_Blue_GS(unsigned char value);

void TLC5940_Set_Green_GS(unsigned char value);

void implement_sine_wave(unsigned char led);

void implement_red_sine_wave(unsigned char led);

void increment_cop_counter(void);

void implement_cop_flasher(unsigned char led);

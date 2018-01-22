//***************************************************************************************
// MSP430 For a stepping Motor Controlling Panel
//By LUYAO
// Thanks to:http://processors.wiki.ti.com/index.php/MSP430_Launchpad_Shift_Register
//
//
//
//***************************************************************************************
#include <msp430x20x2.h>

#define DATA BIT1 // data from P1.1
#define CLOCK BIT2 // CLOCK P1.2
#define LATCH BIT5// LATCH P1.5
#define ENABLE BIT6 // ENABLE P1.6
void port1_init(void);
void spin(void);
void timer(int n);
int Delay = 25;



// Declare functions
void delay ( unsigned int );
void pulseClock ( void );
void shiftOut ( unsigned char );
void enable ( void );
void disable ( void );
void init ( void );
void pinWrite ( unsigned int, unsigned char );
void ConfigureAdc(void);
void spin(void);
void port1_init(void);
unsigned int ADC_value=0;



int main( void )
 {
    WDTCTL = WDTPW + WDTHOLD;
      P1DIR |= BIT7;
      P1DIR |= (DATA + CLOCK + LATCH + ENABLE);  // OUTPUT SETUP FOR SHIFT REGISTER
      enable();
      BCSCTL1 = CALBC1_1MHZ;          // Set range   DCOCTL = CALDCO_1MHZ;
      BCSCTL2 &= ~(DIVS_3);           // SMCLK = DCO = 1MHz
      P1SEL |= BIT3;                  // ADC input FROM p1.7
      ConfigureAdc();                  // Enable interrupts.
      port1_init();

    while(1){
              ADC10CTL0 |= ENC + ADC10SC;
              __bis_SR_register(CPUOFF + GIE);
              ADC_value = ADC10MEM;               // Assigns the value held in ADC10MEM to the integer called ADC_value
             if(ADC_value > 600){ //the total value is x/1024
                 count ++;
                         if(count>10){
                        i++;
                        P1OUT |= BIT7;
                        P1OUT &= ~BIT7;
                        count = 0;
                         }
                         }else if(ADC_value <400){
                             count++;
                             if(count>10){
                             i--;
                             count = 0;
                             }
             }
      shiftOut(tab[i]);
              if(i>8){
                  i = 6;
              }
              else if(i<1){
                i = 1;
              }
      spin();
        }
  }

void delay(unsigned int ms)
{
 while (ms--)
    {
        __delay_cycles(1000); // set for 16Mhz change it to 1000 for 1 Mhz
    }
}


void pinWrite( unsigned int bit, unsigned char val )
{
  if (val){
    P1OUT |= bit;
  } else {
    P1OUT &= ~bit;
  }
}

// Pulse the clock pin
void pulseClock( void )
{
  P1OUT |= CLOCK;
  P1OUT ^= CLOCK;

}

void shiftOut(unsigned char val)
{
  P1OUT &= ~LATCH;
  char i;
  for (i = 0; i < 8; i++)  {
      pinWrite(DATA, (val & (1 << i)));
      pulseClock();
  }

  // Pulse the latch pin to write the values into the storage register
  P1OUT |= LATCH;
  P1OUT &= ~LATCH;
}

void enable( void )
{
  P1OUT &= ~ENABLE;
}

void disable( void )
{
  P1OUT |= ENABLE;
}
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
    __bic_SR_register_on_exit(CPUOFF);        // Return to active mode }
}


void ConfigureAdc(void)
{
    ADC10CTL1 = INCH_3 + ADC10DIV_3 ;         // Channel 3, ADC10CLK/3
    ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;  // Vcc & Vss as reference, Sample and hold for 64 Clock cycles, ADC on, ADC interrupt enable
    ADC10AE0 |= BIT3;                         // ADC input enable P1.3
}
void ConfigureAdc(void);
int tab[8]={0xfc,0x60,0xda,0xf2,0x66,0xb6,0xbe,0xe0,0xfe,0xf6};
int i,count;
void timer(int n)   // n = 10 is 1ms

{

    while (n-- != 0)

        _delay_cycles(100);

}

void spin(void)

{
    const int full_cw[4] = {0x01, 0x02, 0x04, 0x08};
    const int full_ccw[4] = {0x08, 0x04, 0x02, 0x01};
    const int half_cw[8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};
    const int half_ccw[8] = {0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09};
    unsigned int step = 0;
            if(i>5 || i<2){
            for (step = 0; step < 4; step++)
            {
                if (i > 5)
                    P2OUT = full_cw[step];
                else// Direction == CCW
                    P2OUT = full_ccw[step];
                timer(Delay);
            }
            }
            else{
                for (step = 0; step < 8; step++)
                            {
                                if (i>=4)
                                    P2OUT = half_cw[step];
                                else // Direction == CCW
                                    P2OUT = half_ccw[step];
                                timer(Delay);
                            }
            }
}
void port1_init(void)
{
    P2OUT = 0x00;
    P2SEL = 0x00;
    P2DIR = 0xFF;
    P2REN = 0x00;
    P2IES = 0x00;
    P2IFG = 0x00;
    P2IE = 0x00;
}

//******************************************************************************
// RO_PINOSC_TA0_WDTp_One_Button example
// Touch the middle element to turn on/off the center button LED
// RO method capactiance measurement using PinOsc IO, TimerA0, and WDT+
//
//          Schematic Description: 
// 
//                         MSP430G2452
//                      +---------------+
//                      |
//           C----------|P2.5
//                      |
// 
//        The WDT+ interval represents the measurement window.  The number of 
//        counts within the TA0R that have accumulated during the measurement
//        window represents the capacitance of the element. This is lowest 
//        power option with either LPM3 (ACLK WDTp source) or LPM0 (SMCLK WDTp 
//        source).
//
//******************************************************************************

////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include "CTS_Layer.h"
#include "mujoe_common.h"
#include "muJoe_taskMgr.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////

// Uncomment to have this compiler directive run characterization functions only
// Comment to have this compiler directive run example application
#define ELEMENT_CHARACTERIZATION_MODE

#define DELAY 5000 		// Timer delay timeout count - 5000*0.1msec = 500 msec

#define DELAY_1SEC      10000

#ifdef ELEMENT_CHARACTERIZATION_MODE
// Delta Counts returned from the API function for the sensor during characterization
unsigned int dCnt;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

static void sleep(unsigned int time);
static void initCapMeasurement( void );
static void runCapMeasurement( void );
static void initBoard( void );
static void configClks( void );
static void configGPIO(void);
static void configTimers( void );

////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN()
////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{ 
  stopWDT();                            // Stop watchdog timer
  
  taskMgr_initTasks();                  // Initialize all tasks within task manager

  //initBoard();                          // Initialize board
  //initCapMeasurement();                 // Initialize capacitance measurement

  // Terminal Loop //////////////////////////////////////////////////////////////////////////////
  while (1)
  {
      //runCapMeasurement();              // Perform a capacitance measurement
      taskMgr_runSystem();                // Run Task Manager
  }

} // main

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

static void initBoard( void )
{
    // Configure system clocks
    //configClks();   // DEFAULT

    // BEGIN TEST
    sysClkConfig_t sysClkConfig =
    {
        .dcoFreq        = DCO_FREQ_16MHZ,
        .xtalPop        = TRUE,
        .xtalTankCap    = XTALCAP_12PF5,
        .smclkDivider   = SMCLK_DIV8,
        .aclkDivider    = ACLK_DIV4,
    };
    mujoeCommon_configClocks( sysClkConfig );
    // END TEST

    // Configure GPIOs
    configGPIO();

    // Configure Peripherals
    configTimers(); // TEST

} // initBoard

static void configClks( void )
{
    BCSCTL1 = CALBC1_1MHZ;                // Set DCO to 1, 8, 12 or 16MHz
    DCOCTL = CALDCO_1MHZ;
    BCSCTL2 |= DIVS_2;                    // divide SMCLK by 4 for 250khz
    BCSCTL3 |= LFXT1S_2;                  // LFXT1 = VLO

} // configClks

static void configGPIO(void)
{
    /*
    P1OUT = 0x00;                         // Clear Port 1 bits
    P1DIR |= BIT0;                        // Set P1.0 as output pin

    P2SEL &= ~(BIT6 + BIT7);              // Configure XIN (P2.6) and XOUT (P2.7) to GPIO
    P2OUT = 0x00;                         // Drive all Port 2 pins low
    P2DIR = 0xFF;                         // Configure all Port 2 pins outputs
    */

} // configGPIO

static void configTimers( void )
{
    // Uncomment when DCO Clock Frequency is 16 MHz
    TACTL |= ID_3;                      // TACLK/8 = (SMCLK/8)/8 = DCOCLK/64 = 16 MHz/64 = 250kHz = timer tick freq

} // configTimers

// Sleep Function
// Configures Timer A to run off ACLK, count in UP mode, places the CPU in LPM3
// and enables the interrupt vector to jump to ISR upon timeout
static void sleep(unsigned int time)
{
    TA0CCR0 = time;
    TA0CTL = TASSEL_1+MC_1+TACLR;
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL0 |= CCIE;
    __bis_SR_register(LPM3_bits+GIE);
    __no_operation();
}

static void initCapMeasurement( void )
{
  // Initialize Baseline measurement
  TI_CAPT_Init_Baseline(&fuelprobe_sensor);

  // Update baseline measurement (Average 5 measurements)
  TI_CAPT_Update_Baseline(&fuelprobe_sensor,5);

} // initCapMeasurement

static void runCapMeasurement( void )
{

#ifdef ELEMENT_CHARACTERIZATION_MODE
    // Get the raw delta counts for element characterization
    TI_CAPT_Custom(&fuelprobe_sensor,&dCnt);
    putBreakPtHere();   // TEST
    sleep(DELAY_1SEC);  // TEST
    //__no_operation();                   // (DEFAULT) Set breakpoint here
#endif // #ifdef ELEMENT_CHARACTERIZATION_MODE

#ifndef ELEMENT_CHARACTERIZATION_MODE
    // Check if the middle element sensor has been triggered. The API call
    // compares the value from the sensor against the threshold to determine
    // trigger condition
    if(TI_CAPT_Button(&fuelprobe_sensor))
    {
        // Do something
        P1OUT |= BIT0;                            // Turn on center LED
    }
    else
    {
        P1OUT &= ~BIT0;                           // Turn off center LED
    }

    // Put the MSP430 into LPM3 for a certain DELAY period
    sleep(DELAY);
#endif // #ifndef ELEMENT_CHARACTERIZATION_MODE

} // runCapMeasurement


////////////////////////////////////////////////////////////////////////////////////////////////
// INTERRUPT SERVICE ROUTINES
////////////////////////////////////////////////////////////////////////////////////////////////

/*
// Timer0_A0 Interrupt Service Routine: Disables the timer and exists LPM3 ////////////////////
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_Timer0_A0(void)
{
  TA0CTL &= ~(MC_1);
  TA0CCTL0 &= ~(CCIE);
  __bic_SR_register_on_exit(LPM3_bits+GIE);
}
*/

// Trap ISR ///////////////////////////////////////////////////////////////////////////////////
#pragma vector=PORT2_VECTOR,             \
  PORT1_VECTOR,                          \
  TIMER0_A1_VECTOR,                      \
  USI_VECTOR,                            \
  NMI_VECTOR,COMPARATORA_VECTOR,         \
  ADC10_VECTOR
__interrupt void ISR_trap(void)
{
  // the following will cause an access violation which results in a PUC reset
  WDTCTL = 0;
}

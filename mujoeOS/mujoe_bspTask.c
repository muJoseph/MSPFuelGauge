/*
 * mujoe_bspTask.c
 *
 *  Created on: Nov 26, 2017
 *      Author: joe
 */



////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include "mujoe_bspTask.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

static void bspTask_armInterrupts( void );

static void initMCU( void );
static void configureGPIO( void );
static void configurePeripherals( void );
static void configureClocks( void );

////////////////////////////////////////////////////////////////////////////////////////////////
// EXTERN VAR
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// LOCAL VAR
////////////////////////////////////////////////////////////////////////////////////////////////

static uint8    bspTask_taskId = 0;

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

void bspTask_init( uint8 taskId )
{
    bspTask_taskId = taskId;

    // Initialize MCU
    initMCU();

    // Set event to Arm MCU interrupts
    taskMgr_setEvent(bspTask_taskId, BSPTASK_ARM_INTERRUPTS_EVT);

} // bspTask_init

uint16 bspTask_evtProcessor( uint8 taskId, uint16 events )
{

    // Arm Interrupts Event
    if( events & BSPTASK_ARM_INTERRUPTS_EVT )
    {
        bspTask_armInterrupts();
        return (events ^ BSPTASK_ARM_INTERRUPTS_EVT);
    }

     // Disarm Interrupts Event
     if( events & BSPTASK_DISARM_INTERRUPTS_EVT )
     {

        return (events ^ BSPTASK_DISARM_INTERRUPTS_EVT);
     }

    // Discard unknown events
    return 0;

} // bspTask_evtProcessor

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

static void bspTask_armInterrupts( void )
{
    i2cSlave_initInterrupts();   // Enable USCI I2C Interrupts
    __enable_interrupt();        // Enable Global interrupts

} // bspTask_armInterrupts

static void initMCU( void )
{
    configureClocks();
    configureGPIO();
    configurePeripherals();

} // initMCU

static void configureClocks( void )
{
    sysClkConfig_t sysClkConfig =
    {
        .dcoFreq        = DCO_FREQ_16MHZ,
        .xtalPop        = TRUE,
        .xtalTankCap    = XTALCAP_12PF5,
        .smclkDivider   = SMCLK_DIV8,
        .aclkDivider    = ACLK_DIV4,
    };
    mujoeCommon_configClocks( sysClkConfig );

} // configureClocks

static void configureGPIO( void )
{

} // configureGPIO

static void configurePeripherals( void )
{
   bool status;

   // Un-comment when DCO Clock Frequency is 16 MHz
   // TACLK/8 = (SMCLK/8)/8 = DCOCLK/64 = 16 MHz/64 = 250kHz = timer tick freq
   TACTL |= ID_3;

   // Initialize muJoe I2C slave drivers
   status = i2cSlave_initDriver( DEFAULT_MYI2CSLAVE_ADDR, FALSE );
   while( !status );            // Trap MCU on failure
   // Initialize I2C hardware
   status = i2cSlave_initHardware();
   while( !status );            // Trap MCU on failure

   // Initialize CapSense API for Fuel Probe
   fuelProbeMgr_initProbe();

} // configurePeripherals


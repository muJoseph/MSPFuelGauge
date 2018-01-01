/*
 * mujoe_i2c.c
 *
 *  Created on: Nov 18, 2017
 *      Author: joe
 */



////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include <mujoe_i2cSlave.h>

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

static void i2cSlave_prepReceive( void );
static void i2cSlave_parseRxdPacket( void );
static void i2cSlave_handleI2cCommand( mspfg_i2cCmds_t i2cCmd );
static void i2cSlave_pauseUSCI( void );

////////////////////////////////////////////////////////////////////////////////////////////////
// EXTERN VAR
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// LOCAL VAR
////////////////////////////////////////////////////////////////////////////////////////////////

// Non-volatile variables accessed locally by i2cSlave driver fncs
static i2cSlave_t i2cSlave =
{
     .myI2cSlaveAddr        = 0,
     .cfg                   = 0x00,
     .contDataCollection    = FALSE,
};

// Volatile Variables accessed by USCI TX/RX ISRs
static volatile uint8 rxBuffer[RX_BUFFER_SIZE] = {0};

static i2cSlaveIsr_t   i2cSlaveIsr =
{
    .busActive              = FALSE,
    .rx                     = TRUE,
    .pRxBuff                = rxBuffer,
    .rxBuffIndex            = 0,
    .rxByteCnt              = 0,
    .txBuffIndex            = 0,
};

////////////////////////////////////////////////////////////////////////////////////////////////
// MACROS
////////////////////////////////////////////////////////////////////////////////////////////////

// Interrupt Masking/Unmasking
#define disableI2cTxInterrupt()                         (IE2 &= ~UCB0TXIE)                         // Disable I2C TX interrupt
#define enableI2cTxInterrupt()                          (IE2 |= UCB0TXIE)                          // Enable I2C TX interrupt

#define disableI2cRxInterrupt()                         (IE2 &= ~UCB0RXIE)                         // Disable I2C RX interrupt
#define enableI2cRxInterrupt()                          (IE2 |= UCB0RXIE)                          // Enable I2C RX interrupt

#define enableI2cStSpInterrupts()                       (UCB0I2CIE |= UCSTPIE + UCSTTIE)           // Enable interrupt generation for I2C start and stop conditions
#define disableI2cStSpInterrupts()                      (UCB0I2CIE &= ~(UCSTPIE + UCSTTIE))        // Disable interrupt generation for I2C start and stop conditions

// USCI operational state control
#define usciReset()                                     (UCB0CTL1 |= UCSWRST)                      // Put USCI module in reset state (refer to pg. 452 of UG for details)
#define usciResume()                                    (UCB0CTL1 &= ~UCSWRST)                     // Put USCI module in normal operation mode

// Interrupt flag Processing
#define clearI2cStartStopInterruptFlags()               (UCB0STAT &= ~(UCSTPIFG + UCSTTIFG))       // Clear I2C Start and Stop condition interrupt flags

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

// Returns TRUE if MSP Fuel Gauge is currently configured for
// continuous data collection (i.e. MSPFG_CMD_ST_CONT_DATA I2C cmd sent by
// I2C bus master ). FALSE otherwise
bool i2cSlave_contDataCollect( void )
{
   return i2cSlave.contDataCollection;

} // i2cSlave_contDataCollect

// Returns TRUE if USCI peripheral TX interrupts were successfully enabled
// Returns FALSE otherwise
bool i2cSlave_resumeI2cMasterReads( void )
{
    // Make sure that I2C Bus is idle before enabling TX interrupts
    if( !i2cSlaveIsr.busActive )
    {
        enableI2cTxInterrupt();
        return TRUE;
    }
    else
        return FALSE;

} // i2cSlave_resumeI2cMasterReads

// Returns TRUE if USCI peripheral TX interrupts were successfully disabled
// Returns FALSE otherwise
bool i2cSlave_suspendI2cMasterReads( void )
{
    // Make sure that I2C Bus is idle before disabling TX interrupts
    if( !i2cSlaveIsr.busActive )
    {
        disableI2cTxInterrupt();
        return TRUE;
    }
    else
        return FALSE;

} // i2cSlave_suspendI2cMasterReads

void i2cSlave_rxdPacketHandler( void )
{
    // Disable RX interrupt
    disableI2cRxInterrupt();

    // Parse the RX'd packet
    i2cSlave_parseRxdPacket();

    // Prepare for RX of next I2C master write
    i2cSlave_prepReceive();

}// i2cSlave_rxdPacketHandler

void i2cSlave_initInterrupts( void )
{
    enableI2cStSpInterrupts();       // Enable I2C Start and Stop condition interrupt generation
    enableI2cTxInterrupt();          // Un-mask I2C TX interrupts
    enableI2cRxInterrupt();          // Un-mask I2C RX interrupts (TEST)

}// i2cSlave_initInterrupts

// Prepare to transmit data to master
void i2cSlave_prepTransmit( void )
{
    UCB0TXBUF = mspfgRegMap[i2cSlaveIsr.txBuffIndex++];
    // If at end of the I2C register map, wrap around
    if( i2cSlaveIsr.txBuffIndex == MSPFG_NUM_REGISTERS ){ i2cSlaveIsr.txBuffIndex = 0; }
    //i2cMstrReading = FALSE;

} // i2cSlave_prepTransmit

bool i2cSlave_initDriver( uint8 myAddr, bool genCallRsp )
{
    i2cSlave.myI2cSlaveAddr = myAddr;

    if( genCallRsp )
       i2cSlave.cfg |= I2CSLAVE_CFG_GENCALLRSP;

    // Initialize I2C "Register Map"
    mspfg_initRegMap();

    return TRUE;

}// i2cSlave_initDriver

bool i2cSlave_initHardware( void )
{
    // Driver uninitialized, abort
    if(i2cSlave.myI2cSlaveAddr == 0)
        return FALSE;

    // Give USCI peripheral access to GPIOs P1.6 and P1.7
    P1SEL  |= BIT6 + BIT7;
    P1SEL2 |= BIT6 + BIT7;

    // Initialize ISR to receive
    i2cSlaveIsr.rx = TRUE;

    // Hold USCI logic in reset state in preparation for configuration
    usciReset();

    // Configure USCI module for synchronous I2C using 7-bit addressing
    UCB0CTL0 = UCMODE_3 + UCSYNC;
    // Set own I2C slave address
    UCB0I2COA = (uint16)i2cSlave.myI2cSlaveAddr;
    // Set general call response enable bit if necessary
    if( i2cSlave.cfg & I2CSLAVE_CFG_GENCALLRSP )
       UCB0I2COA |= UCGCEN;

    // Bring USCI logic out of reset state in preparation for normal operation
    usciResume();

    return TRUE;

} // i2cSlave_initHardware

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

static void i2cSlave_pauseUSCI( void )
{
    // Hold USCI peripheral in reset state:
    //- I2C communication stops
    //- SDA and SCL are high impedance
    //- UCBxI2CSTAT, bits 6-0 are cleared
    //- UCBxTXIE and UCBxRXIE are cleared
    //- UCBxTXIFG and UCBxRXIFG are cleared
    usciReset();

} //i2cSlave_pauseUSCI

// Prepare to receive data from master
static void i2cSlave_prepReceive( void )
{
    // Rst indices and RX/TX byte counts
    i2cSlaveIsr.rxBuffIndex = 0;
    i2cSlaveIsr.rxByteCnt = 0;

    // Enable RX interrupt
    enableI2cRxInterrupt();

} // i2cSlave_prepReceive

static void i2cSlave_parseRxdPacket( void )
{
    uint8 numRxBytes = i2cSlaveIsr.rxByteCnt;

    // I2C Command RX'd
    if( numRxBytes == 1 )
    {
        uint8 rxdI2cCmd = i2cSlaveIsr.pRxBuff[0];
        // Ensure that MSBit is set to signify I2C command
        if( rxdI2cCmd & 0x80 )
          i2cSlave_handleI2cCommand( (mspfg_i2cCmds_t)rxdI2cCmd );
    }
    // Register(s) write
    else if( numRxBytes >= 2 )
    {
        uint8 regStartAddr = i2cSlaveIsr.pRxBuff[0];
        uint8 i;
        for( i = 1; i < numRxBytes; i++ )
        {
            // Write to the register map starting at the address specified by the first
            // byte of the RX'd I2C packet
            if( i < RX_BUFFER_SIZE)
                mspfg_writeReg( (mspfg_regAddr_t)regStartAddr++, i2cSlaveIsr.pRxBuff[i] );
        }
    }

} //i2cSlave_parseRxdPacket

static void i2cSlave_handleI2cCommand( mspfg_i2cCmds_t i2cCmd )
{
    switch( i2cCmd )
    {
        case MSPFG_CMD_ST_CONT_DATA:
            i2cSlave.contDataCollection = TRUE;
            taskMgr_setEvent( mainTask_getTaskId(), MAINTASK_GET_FUEL_PROBE_MEAS_EVT );
            break;
        case MSPFG_CMD_SP_CONT_DATA:
            if( i2cSlave.contDataCollection )
            {
                i2cSlave.contDataCollection = FALSE;
                taskMgr_clearEventEx( mainTask_getTaskId(), MAINTASK_GET_FUEL_PROBE_MEAS_EVT );
            }
            break;
        case MSPFG_CMD_SINGLESHOT_DATA:
            i2cSlave.contDataCollection = FALSE;
            taskMgr_setEvent( mainTask_getTaskId(), MAINTASK_GET_FUEL_PROBE_MEAS_EVT );
            break;
        case MSPFG_CMD_SLEEP:
            break;
        case MSPFG_CMD_UPDATE_BASELINE:
            fuelProbeMgr_updateBaseline( 5 );
            break;
        case MSPFG_CMD_RESET:
            mujoeCommon_softwareReset();
            break;
        // Invalid Command
        default:
            break;
    }

} // i2cSlave_handleI2cCommand

////////////////////////////////////////////////////////////////////////////////////////////////
// INTERRUPT SERVICE ROUTINES
////////////////////////////////////////////////////////////////////////////////////////////////

// USCIAB0TX_ISR
//------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move data from MSP430 memory to the
// I2C master. PTxData points to the next byte to be transmitted, and TXByteCtr
// keeps track of the number of bytes transmitted.
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{

  // Following USCI_Bx Flags route here: UCBxTXIFG, UCBxRXIFG
  // Following USCI_Ax Flags route here: UCAxTXIFG

  // Check RX flag
  if( IFG2 & UCB0RXIFG )
  {
     // Store RX'd data in local RX buffer, inc buffer addr ptr to next index position
     // NOTE: Reading UCB0RXBUF resets IFG2.UCB0RXIFG flag
     i2cSlaveIsr.pRxBuff[i2cSlaveIsr.rxBuffIndex++] = UCB0RXBUF;

     // If at end of RX buffer, wrap around
     if(i2cSlaveIsr.rxBuffIndex == RX_BUFFER_SIZE ){ i2cSlaveIsr.rxBuffIndex = 0; }
     // Increment RX byte count
     i2cSlaveIsr.rxByteCnt++;
  }

  // Check TX flag
  if( IFG2 & UCB0TXIFG )
  {
      // Load value of the register at the specified index of the register map
      // NOTE: UCB0TXIFG is set when UCB0TXBUF is empty (i.e. master has read out data). Flag is
      //       is cleared when data is written to UCB0TXBUF
      UCB0TXBUF = mspfgRegMap[i2cSlaveIsr.txBuffIndex++];
      // If at end of the I2C register map, wrap around
      if( i2cSlaveIsr.txBuffIndex == MSPFG_NUM_REGISTERS ){ i2cSlaveIsr.txBuffIndex = 0; }
  }

}

// USCIAB0RX_ISR
//------------------------------------------------------------------------------
// The USCI_B0 state ISR is used to wake up the CPU from LPM0 in order to do
// processing in the main program after data has been transmitted. LPM0 is
// only exit in case of a (re-)start or stop condition when actual data
// was transmitted.
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCIAB0RX_ISR (void)
#else
#error Compiler not supported!
#endif
{
 // Following USCI_Bx Flags Route Here : UCSTTIFG, UCSTPIFG, UCNACKIFG, UCALIFG
 // Following USCI_Ax Flags Route here: UCAxRXIFG

  // I2C START Condition RX'd
  if( UCB0STAT & UCSTTIFG )
  {
     // I2C Master has set R/Wn slave addr bit (Master is reading, slave transmitting)
     if( UCB0CTL1 & UCTR )
     {
       // Clear RX flag
       i2cSlaveIsr.rx = FALSE;

       // Load first byte to by transmitted into TX buffer
       UCB0TXBUF = mspfgRegMap[i2cSlaveIsr.txBuffIndex++];
       // If at end of the I2C register map, wrap around
       if( i2cSlaveIsr.txBuffIndex == MSPFG_NUM_REGISTERS ){ i2cSlaveIsr.txBuffIndex = 0; }

     }
     // I2C Master has cleared R/Wn slave addr bit (Master is writing, slave receiving)
     else
       i2cSlaveIsr.rx = TRUE;

     i2cSlaveIsr.busActive = TRUE;      // Set USCI active flag (TEST)

     UCB0STAT &= ~UCSTTIFG;
  }

  // I2C STOP Condition RX'd
  if( UCB0STAT & UCSTPIFG )
  {
     // If USCI is in Slave Receiver Mode...
     //if( i2cSlaveIsr.rx && i2cSlaveIsr.rxByteCnt ) // DEFAULT
     if( i2cSlaveIsr.rx  )  // TEST
     {
         // Check if I2C Master sent only one byte and if it is a valid register address
         if( (i2cSlaveIsr.rxByteCnt == 1) && (i2cSlaveIsr.pRxBuff[0] < MSPFG_NUM_REGISTERS) )
         {
             // Load RX'd register address into TX index buffer in preparation of I2C Master Read
             i2cSlaveIsr.txBuffIndex = i2cSlaveIsr.pRxBuff[0];
             // Reset indices and RX/TX byte counts in preparation for subsequent I2C writes
             i2cSlaveIsr.rxBuffIndex = 0;
             i2cSlaveIsr.rxByteCnt = 0;
         }
         // Something other than register address was sent by I2C master, parse RX'd packet within main app
         else
            taskMgr_setEventISR( i2cTask_taskId, I2CTASK_HANDLE_MASTER_WRITE_EVT );
     }

     i2cSlaveIsr.busActive = FALSE;      // Clear USCI active flag (TEST)

     UCB0STAT &= ~UCSTPIFG;
  }
}

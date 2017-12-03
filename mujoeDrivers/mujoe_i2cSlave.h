/*
 * mujoe_i2c.h
 *
 *  Created on: Nov 18, 2017
 *      Author: joe
 */

#ifndef MUJOE_I2CSLAVE_H_
#define MUJOE_I2CSLAVE_H_

////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include <msp430.h>
#include <string.h>
#include "mujoe_i2cRegMap.h"
#include "mujoe_types.h"
#include "mujoe_taskMgr.h"
#include "mujoe_mainTask.h"
#include "mujoe_i2cTask.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////

// Default Slave Address
#define DEFAULT_MYI2CSLAVE_ADDR                       0x48

// Local data buffer sizes
#define RX_BUFFER_SIZE                                MSPFG_NUM_REGISTERS

// i2cSlave_t.cfg bit map
#define I2CSLAVE_CFG_GENCALLRSP                       0x01      // Respond to General Call

////////////////////////////////////////////////////////////////////////////////////////////////
// TYPEDEFS
////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct i2cSlave_def
{
    uint8           myI2cSlaveAddr;
    uint8           cfg;

}i2cSlave_t;

typedef struct i2cSlaveIsr_def
{
    volatile bool    rx;                 // If TRUE, USCI is in Slave Receiver mode. If FALSE, USCI is in Slave transmitter mode
    volatile uint8   *pRxBuff;           // Pointer to RX buffer
    volatile uint8   rxBuffIndex;        // Current index of the RX buffer
    volatile uint8   rxByteCnt;          // Rx Byte Counter
    volatile uint8   txBuffIndex;        // Current index of the TX buffer

}i2cSlaveIsr_t;

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

void i2cSlave_prepTransmit( void );
bool i2cSlave_initDriver( uint8 myAddr, bool genCallRsp );
bool i2cSlave_initHardware( void );
void i2cSlave_initInterrupts( void );
void i2cSlave_rxdPacketHandler( void );

#endif /* MUJOE_I2CSLAVE_H_ */

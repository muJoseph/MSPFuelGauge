/*
 * mujoe_i2cRegMap.h
 *
 *  Created on: Nov 23, 2017
 *      Author: joe
 */

#ifndef MUJOE_I2CREGMAP_H_
#define MUJOE_I2CREGMAP_H_

////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include "mujoe_types.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////

// Who Am I ID
#define  MSPFG_WHO_AM_I_ID                      0xFF

// Device Info Register Bit Map
#define MSPFG_HW_VER                            (0x01<<4)   // Device Info B[7:4] = Hardware Version
#define MSPFG_FW_VER                            0x01        // Device Info B[3:0] = Firmware Version

// Configuration Register Bit Map
#define MSPFG_CFG_EN_HW_INT                     0x01        // If set, enables generation of HW interrupt

// Status Register Bit Map
#define MSPFG_STAT_CAPOVRFLW                    0x01        // If set, raw capacitance count has overflowed during accumulation window. Updated with each capacitance measurement
#define MSPFG_STAT_HW_INT_PENDING               0x08        // If set, HW interrupt is asserted. Must be cleared by host to de-assert HW interrupt  line.

// MSPFuelGauge I2C Commands
/*#define MSPFG_CMD_ST_CONT_DATA                  0x81        // Start Continuous Data collection
#define MSPFG_CMD_SP_CONT_DATA                  0x82        // Stop Continuous Data collection
#define MSPFG_CMD_SINGLESHOT_DATA               0x83        // Trigger a single shot measurement
#define MSPFG_CMD_SLEEP                         0x84        // Put MSPFuelGauge to sleep
#define MSPFG_CMD_UPDATE_BASELINE               0x85        // Update baseline tracking of TIs Capacitive touch algorithm
*/

// Number of I2C registers with write access privileges
#define MSPFG_NUM_REG_WRITE_ACCESS              3

////////////////////////////////////////////////////////////////////////////////////////////////
// TYPEDEFS
////////////////////////////////////////////////////////////////////////////////////////////////

// MSPFuelGauge Register Addresses
typedef enum
{
  MSPFG_WHO_AM_I                = 0x00, // Who Am I Register (R)
  MSPFG_DEV_INFO,                       // Device Info Register (R)
  MSPFG_CFG,                            // Configuration Register (R/W)
  MSPFG_STATUS,                         // Status Register (R/W)
  MSPFG_CAP_ALGO_LSB,                   // Tracking Algo Capacitance Reading LSByte (R)
  MSPFG_CAP_ALGO_MSB,                   // Tracking Algo Capacitance Reading MSByte (R)
  MSPFG_CAP_RAW_LSB,                    // Raw Capacitance Reading LSByte (R)
  MSPFG_CAP_RAW_MSB,                    // Raw Capacitance Reading MSByte (R)
  MSPFG_FUEL_LVL,                       // Current Fuel Level (0 to 100%) (R)
  MSPFG_CAP_FULL_LSB,                   // Capacitance value when tank full LSByte (R/W)
  MSPFG_CAP_FULL_MSB,                   // Capacitance value when tank full MSByte (R/W)
  MSPFG_FUEL_LVL_CRIT_THRESH,           // Critical Fuel Level Threshold (0 to 100%) (R/W)
  MSPFG_NUM_REGISTERS,                  // Number of MSPFuelGauge Registers

}mspfg_regAddr_t;

// MSPFuelGauge I2C Commands
typedef enum
{
    MSPFG_CMD_ST_CONT_DATA =         0x81,        // Start Continuous Data collection
    MSPFG_CMD_SP_CONT_DATA =         0x82,        // Stop Continuous Data collection
    MSPFG_CMD_SINGLESHOT_DATA =      0x83,        // Trigger a single shot measurement
    MSPFG_CMD_SLEEP =                0x84,        // Put MSPFuelGauge to sleep
    MSPFG_CMD_UPDATE_BASELINE =      0x85,        // Update baseline tracking of TIs Capacitive touch algorithm
    MSPFG_CMD_RESET =                0x86,        // Performs a Power-On Reset of the MSP430

}mspfg_i2cCmds_t;

typedef void (*regWriteCb_t)( uint8 regData );
typedef void (*regReadCb_t)( void );

typedef struct mspfg_regWriteHndl_def
{
   mspfg_regAddr_t      addr;
   regWriteCb_t         regWriteCb;

}mspfg_regWriteHndl_t;

////////////////////////////////////////////////////////////////////////////////////////////////
// EXTERN VAR
////////////////////////////////////////////////////////////////////////////////////////////////

extern volatile uint8 mspfgRegMap[MSPFG_NUM_REGISTERS];

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

void mspfg_initRegMap( void );
bool mspfg_writeReg( mspfg_regAddr_t addr, uint8 regData );

#endif /* MUJOE_I2CREGMAP_H_ */

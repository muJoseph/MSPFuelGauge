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

// MSPFuelGauge I2C Commands
#define MSPFG_CMD_ST_CONT_DATA          0x81    // Start Continuous Data collection
#define MSPFG_CMD_SP_CONT_DATA          0x82    // Stop Continuous Data collection
#define MSPFG_CMD_SINGLESHOT_DATA       0x83    // Trigger a single shot measurement
#define MSPFG_CMD_SLEEP                 0x84    // Put MSPFuelGauge to sleep

// Number of I2C registers with write access privileges
#define MSPFG_NUM_REG_WRITE_ACCESS              2


////////////////////////////////////////////////////////////////////////////////////////////////
// TYPEDEFS
////////////////////////////////////////////////////////////////////////////////////////////////

// MSPFuelGauge Register Addresses
typedef enum
{
  MSPFG_WHO_AM_I                = 0x00, // Who Am I Register (R)
  MSPFG_DEV_INFO,                       // Device Info Register (R)
  MSPFG_CFG,                            // Configuration Register (R/W)
  MSPFG_CAP_FULL_LSB,                   // Capacitance value when tank full LSByte (R)
  MSPFG_CAP_FULL_MSB,                   // Capacitance value when tank full MSByte (R)
  MSPFG_CAP_ALGO_LSB,                   // Tracking Algo Capacitance Reading LSByte (R)
  MSPFG_CAP_ALGO_MSB,                   // Tracking Algo Capacitance Reading MSByte (R)
  MSPFG_CAP_RAW_LSB,                    // Raw Capacitance Reading LSByte (R)
  MSPFG_CAP_RAW_MSB,                    // Raw Capacitance Reading MSByte (R)
  MSPFG_FUEL_LVL_CRIT_THRESH,           // Critical Fuel Level Threshold (0 to 100%) (R/W)
  MSPFG_FUEL_LVL,                       // Current Fuel Level (0 to 100%) (R)
  MSPFG_NUM_REGISTERS,                  // Number of MSPFuelGauge Registers

}mspfg_regAddr_t;

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

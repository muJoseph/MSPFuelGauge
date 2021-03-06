/*
 * mujoe_i2cRegMap.c
 *
 *  Created on: Nov 23, 2017
 *      Author: joe
 */



////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include <mujoe_i2cRegMap.h>


////////////////////////////////////////////////////////////////////////////////////////////////
// EXTERN FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

extern void mspfg_configRegWriteCb( uint8 regData );
extern void mspfg_fuelLvlCritThreshRegWriteCb( uint8 regData );
extern void mspfg_statusRegWriteCb( uint8 regData );

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

static void mspfg_dispatchRegWriteHdlr( mspfg_regAddr_t addr, uint8 regData );

////////////////////////////////////////////////////////////////////////////////////////////////
// EXTERN VAR
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL VAR
////////////////////////////////////////////////////////////////////////////////////////////////

volatile uint8 mspfgRegMap[MSPFG_NUM_REGISTERS] = {0};

////////////////////////////////////////////////////////////////////////////////////////////////
// LOCAL VAR
////////////////////////////////////////////////////////////////////////////////////////////////

// Array contains all register write callbacks that have an I2C write access privilege
const static mspfg_regWriteHndl_t   mspfg_regWriteHndls[MSPFG_NUM_REG_WRITE_ACCESS] =
{
     // Configuration Register
     {
          .addr = MSPFG_CFG,
          .regWriteCb = mspfg_configRegWriteCb,
     },

     // Fuel Level Critical Threshold register
     {
          .addr = MSPFG_FUEL_LVL_CRIT_THRESH,
          .regWriteCb = mspfg_fuelLvlCritThreshRegWriteCb,
     },

     // Status register
     {
          .addr = MSPFG_STATUS,
          .regWriteCb = mspfg_statusRegWriteCb,
     },


}; // mspfg_regWriteHndls

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

void mspfg_initRegMap( void )
{
    mspfgRegMap[MSPFG_WHO_AM_I] = MSPFG_WHO_AM_I_ID;
    mspfgRegMap[MSPFG_DEV_INFO] = MSPFG_HW_VER + MSPFG_FW_VER;

} // mspfg_initRegMap

bool mspfg_writeReg( mspfg_regAddr_t addr, uint8 regData )
{
   bool retVal = FALSE;
   switch(addr)
   {
      // Configuration register
      case MSPFG_CFG:
        mspfgRegMap[MSPFG_CFG] = regData;
        mspfg_dispatchRegWriteHdlr( MSPFG_CFG, regData );
        retVal = TRUE;
        break;
      // Fuel Level Critical Threshold register
      case MSPFG_FUEL_LVL_CRIT_THRESH:
        mspfgRegMap[MSPFG_FUEL_LVL_CRIT_THRESH] = regData;
        mspfg_dispatchRegWriteHdlr( MSPFG_FUEL_LVL_CRIT_THRESH, regData );
        retVal = TRUE;
        break;
      // Status register
      case MSPFG_STATUS:
        mspfgRegMap[MSPFG_STATUS] = regData;
        mspfg_dispatchRegWriteHdlr( MSPFG_STATUS, regData );
        retVal = TRUE;
        break;
      default:
        break;
   }

   return retVal;
} // mspfg_writeReg

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

static void mspfg_dispatchRegWriteHdlr( mspfg_regAddr_t addr, uint8 regData )
{
    uint8 i;
    // Search thru callback array for the callback fnc ptr that is assigned to the register addr
    for(i = 0; i < MSPFG_NUM_REG_WRITE_ACCESS; i++ )
    {
        if(mspfg_regWriteHndls[i].addr == addr )
        {
            // Call callback function and return
            mspfg_regWriteHndls[i].regWriteCb( regData );
            return;
        }
    }

} // mspfg_dispatchRegWriteHdlr

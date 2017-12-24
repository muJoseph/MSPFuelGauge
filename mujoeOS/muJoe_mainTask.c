/*
 * muJoe_mainTask.c
 *
 *  Created on: Nov 18, 2017
 *      Author: joe
 */


////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include "muJoe_mainTask.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

//static void mainTask_getFuelProbeMeasurement( void );
static void mainTask_getFuelProbeMeasurement( genSm_t *pSm );

////////////////////////////////////////////////////////////////////////////////////////////////
// EXTERN VAR
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// LOCAL VAR
////////////////////////////////////////////////////////////////////////////////////////////////

static uint8            mainTask_taskId = 0;

static genSm_t         getFuelProbeMeasurement_sm =
{
     .s = 0,
     .f = 0,
};

static fuelProbeMgr_t   fuelProbeMgr =
{
     .countDelta = 0,
     .countRaw = 0,
};

// BEGIN DEBUG
//static uint16 fuelProbeCnt = 0;
//static uint8  fuelProbeSampleCnt = 0;
//static bool             takeCapSenseSample = TRUE;
// END DEBUG

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

uint8 mainTask_getTaskId( void )
{
    return mainTask_taskId;

} // mainTask_getTaskId

void mainTask_init( uint8 taskId )
{
    mainTask_taskId = taskId;

    // Init Task Here

    // Set event for first data sample event (DEBUG)
    //taskMgr_setEvent( mainTask_taskId, MAINTASK_GET_FUEL_PROBE_MEAS_EVT );

} // mainTask_init

uint16 mainTask_evtProcessor( uint8 taskId, uint16 events )
{
    // Measure Fuel Probe Capacitance Event
    if( events & MAINTASK_GET_FUEL_PROBE_MEAS_EVT )
    {
       mainTask_getFuelProbeMeasurement( &getFuelProbeMeasurement_sm );
       return (events ^ MAINTASK_GET_FUEL_PROBE_MEAS_EVT);
    }

    // Discard unknown events
    return 0;

} // mainTask_evtProcessor

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

static void mainTask_getFuelProbeMeasurement( genSm_t *pSm )
{
    uint16 eventBuff = 0;
    uint32 eventDelay = 0;

    switch( pSm->s )
    {
        // Perform Capacitive Measurement
        case 0:
            fuelProbeMgr = fuelProbeMgr_performMeasurement();
            pSm->s = 1;             // Go to next state: Update register map with new data and schedule next sample event
            //break;                // NOTE: Intentional fall-through!
        // Update register map with new data and schedule next sample event
        case 1:
            // Mask I2C TX interrupt while loading new data into register map
            if( i2cSlave_suspendI2cMasterReads() )
            {
                // Load new data into I2C register map
                mspfgRegMap[MSPFG_CAP_ALGO_LSB] = (uint8)fuelProbeMgr.countDelta;
                mspfgRegMap[MSPFG_CAP_ALGO_MSB] = (uint8)( fuelProbeMgr.countDelta >> 8 );
                mspfgRegMap[MSPFG_CAP_RAW_LSB] = (uint8)fuelProbeMgr.countRaw;
                mspfgRegMap[MSPFG_CAP_RAW_MSB] = (uint8)( fuelProbeMgr.countRaw >> 8 );

                if( fuelProbeMgr.rawOverFlow )
                    mspfgRegMap[MSPFG_STATUS] |= MSPFG_STAT_CAPOVRFLW;
                else
                    mspfgRegMap[MSPFG_STATUS] &= ~MSPFG_STAT_CAPOVRFLW;

                while( !i2cSlave_resumeI2cMasterReads() );  // Do not progress until I2C TX interrupt has been unmasked

                // Assert interrupt if enabled by host
                if( mspfgRegMap[MSPFG_CFG] & MSPFG_CFG_EN_HW_INT )
                    MSPINTn_bringLOW();

                // Refresh event if continuous data collection is enabled...
                if( i2cSlave_contDataCollect() )
                {
                    eventBuff = MAINTASK_GET_FUEL_PROBE_MEAS_EVT;
                    eventDelay = 1000;                       // Set event delay equal to sample period
                }
                pSm->s = 0;                                 // Go to next state: Perform Capacitive Measurement
                putBreakPtHere();                           // DEBUG
            }
            // Masking of USCI TX interrupt unsuccessful (i.e. I2C bus active)...
            else
            {
                eventBuff = MAINTASK_GET_FUEL_PROBE_MEAS_EVT;
                pSm->s = 1;                                 // Go to next state: Update register map with new data and schedule next sample event
            }
            break;
        default:
            break;
    }

    if( eventBuff )
    {
        if( eventDelay )
          taskMgr_setEventEx( mainTask_taskId, eventBuff, eventDelay );
        else
          taskMgr_setEvent( mainTask_taskId, eventBuff );
    }

    /*
    // BEGIN DEFAULT
    if( takeCapSenseSample )
    {
        //fuelProbeCnt = fuelProbeMgr_performMeasurement();                 // DEFAULT
        fuelProbeMgr = fuelProbeMgr_performMeasurement();                   // TEST
        fuelProbeSampleCnt++;
        putBreakPtHere();
    }
    if( taskMgr_setEventEx( mainTask_taskId, MAINTASK_GET_FUEL_PROBE_MEAS_EVT, 1000 ) != SUCCESS )
    {
        taskMgr_setEvent( mainTask_taskId, MAINTASK_GET_FUEL_PROBE_MEAS_EVT );
        takeCapSenseSample = FALSE;
    }
    else
        takeCapSenseSample = TRUE;
     // END DEFAULT
     */

} // mainTask_getFuelProbeMeasurement

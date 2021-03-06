/*
 * muJoe_mainTask.h
 *
 *  Created on: Nov 18, 2017
 *      Author: joe
 */

#ifndef MUJOE_MAINTASK_H_
#define MUJOE_MAINTASK_H_

////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include "mujoe_common.h"
#include "mujoe_taskMgr.h"
#include "mujoe_fuelProbeMgr.h"
#include "mujoe_i2cSlave.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////

// Main Task Event Flags
#define  MAINTASK_GET_FUEL_PROBE_MEAS_EVT               0x0001

////////////////////////////////////////////////////////////////////////////////////////////////
// TYPEDEFS
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

uint8 mainTask_getTaskId( void );
void mainTask_init( uint8 taskId );
uint16 mainTask_evtProcessor( uint8 taskId, uint16 events );

#endif /* MUJOE_MAINTASK_H_ */

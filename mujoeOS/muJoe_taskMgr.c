/*
 * muJoe_taskMgr.c
 *
 *  Created on: Nov 11, 2017
 *      Author: joe
 */

////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include "muJoe_taskMgr.h"

#include "muJoe_mainTask.h"
#include "muJoe_i2cTask.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// EXTERN VAR
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// LOCAL VAR
////////////////////////////////////////////////////////////////////////////////////////////////

const static taskEvtProcesssor_t taskEvtProcesssorArr[TASKMGR_NUM_TASKS] =
{
     mainTask_evtProcessor,         // TaskId = 0
     i2cTask_evtProcessor,          // TaskId = 1
};

static taskMgr_t    taskMgr =
{
   .eventsArr = {0},
};

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

void taskMgr_initTasks( void )
{
    uint8 taskId = 0;

    // Main Task Init
    mainTask_init(taskId++);

    // I2C Task Init
    i2cTask_init(taskId++);


} // taskMgr_initTasks

void taskMgr_startSystem( void )
{
} // taskMgr_startSystem

void taskMgr_runSystem( void )
{
  uint8 i;
  // Go thru each task registered with the taskMgr
  for( i = 0; i < TASKMGR_NUM_TASKS; i++ )
  {
      // Call event processor of respective task if any event flags are set
      if( taskMgr.eventsArr[i] )
      {
          taskMgr.eventsArr[i] = taskEvtProcesssorArr[i]( i, taskMgr.eventsArr[i] );
      }
  }
} // taskMgr_runSystem

uint8 taskMgr_setEvent( uint8 taskId, uint16 events )
{
    if( taskId >= TASKMGR_NUM_TASKS )
        return FAILURE;

    taskMgr.eventsArr[taskId] |= events;
    return SUCCESS;

} // taskMgr_setEvent

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

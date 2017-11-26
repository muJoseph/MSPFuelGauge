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

#include "mujoe_bspTask.h"
#include "muJoe_mainTask.h"
#include "muJoe_i2cTask.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

static void taskMgr_updateEventFlags( void );

////////////////////////////////////////////////////////////////////////////////////////////////
// EXTERN VAR
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// LOCAL VAR
////////////////////////////////////////////////////////////////////////////////////////////////

const static taskEvtProcesssor_t taskEvtProcesssorArr[TASKMGR_NUM_TASKS] =
{
     bspTask_evtProcessor,          // TaskId = 0
     i2cTask_evtProcessor,          // TaskId = 1
     mainTask_evtProcessor,         // TaskId = 2
}; // taskEvtProcesssorArr

static taskMgr_t    taskMgr =
{
   .currTaskID = 0,
   .eventsArrBuffered = {0},
   .eventsArr = {0},
}; // taskMgr

static uint8 timedEvtTaskId = 0;   // TEST
static uint16 timedEvtFlag = 0;     // TEST

////////////////////////////////////////////////////////////////////////////////////////////////
// MACROS
////////////////////////////////////////////////////////////////////////////////////////////////

#define taskMgr_setEventISR(X,Y)       if( (X) < TASKMGR_NUM_TASKS ){taskMgr.eventsArrBuffered[(X)] |= (Y);}

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

void taskMgr_initTasks( void )
{
    uint8 taskId = 0;

    // Board-Specific Task Init
    bspTask_init(taskId++);

    // I2C Task Init
    i2cTask_init(taskId++);

    // Main Task Init
    mainTask_init(taskId++);

} // taskMgr_initTasks

void taskMgr_runSystem( void )
{

  taskMgr_updateEventFlags();

  // Go thru each task registered with the taskMgr
  for( taskMgr.currTaskID = 0; taskMgr.currTaskID < TASKMGR_NUM_TASKS; taskMgr.currTaskID++ )
  {
      // Call event processor of respective task if any event flags are set
      if( taskMgr.eventsArr[taskMgr.currTaskID] )
      {
          taskMgr.eventsArr[taskMgr.currTaskID] = taskEvtProcesssorArr[taskMgr.currTaskID ]( taskMgr.currTaskID, taskMgr.eventsArr[taskMgr.currTaskID] );
      }
  }

} // taskMgr_runSystem

uint8 taskMgr_setEvent( uint8 taskId, uint16 events )
{
    if( taskId >= TASKMGR_NUM_TASKS )
        return FAILURE;

    taskMgr.eventsArrBuffered[taskId] |= events;
    return SUCCESS;

} // taskMgr_setEvent

// Configures Timer A to run off ACLK, count in UP mode, places the CPU in LPM3
// and enables the interrupt vector to jump to ISR upon timeout
void taskMgr_setEventEx( uint8 taskId, uint16 events, uint32 timeEx_ms )
{
    float countVal = ( (float)timeEx_ms ) * TIME_MS_TO_CCR0_COUNT;

    timedEvtTaskId = taskId;   // TEST
    timedEvtFlag = events;     // TEST

    // When ACLK source = 32.768 kHz crystal/4 = 8.192 Hz
    // Timer A clock source = ACLK
    // Timer A tick period = 1/(8.192kHz) = ~122 usec
    TA0CCR0 = (uint16)countVal;
    TA0CTL = TASSEL_1+MC_1+TACLR;
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL0 |= CCIE;
    __bis_SR_register(LPM3_bits+GIE);
    __no_operation();

} // taskMgr_setEventEx

uint8 taskMgr_clearEvent( uint8 taskId, uint16 events )
{
    if( taskId >= TASKMGR_NUM_TASKS )
        return FAILURE;

    taskMgr.eventsArr[taskId] &= ~events;
    return SUCCESS;

} // taskMgr_clearEvent

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

static void taskMgr_updateEventFlags( void )
{
    uint8 len = sizeof( taskMgr.eventsArr );
    // Copy buffered event flags into current taskMgr loop iteration context
    memcpy( taskMgr.eventsArr, taskMgr.eventsArrBuffered, len );
    // Clear buffered event flags
    memset( taskMgr.eventsArrBuffered, 0, len );

} // taskMgr_updateEventFlags

////////////////////////////////////////////////////////////////////////////////////////////////
// INTERRUPT SERVICE ROUTINES
////////////////////////////////////////////////////////////////////////////////////////////////

// Timer0_A0 Interrupt Service Routine: Disables the timer and exists LPM3 ////////////////////
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_Timer0_A0(void)
{
  taskMgr_setEventISR(timedEvtTaskId,timedEvtFlag); // TEST
  TA0CTL &= ~(MC_1);
  TA0CCTL0 &= ~(CCIE);
  __bic_SR_register_on_exit(LPM3_bits+GIE);
}

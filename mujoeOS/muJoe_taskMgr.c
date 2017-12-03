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
// GLOBAL VAR
////////////////////////////////////////////////////////////////////////////////////////////////

taskMgr_t    taskMgr =
{
   .currTaskID = 0,
   .eventsArrBuffered = {0},
   .eventsArr = {0},
}; // taskMgr

////////////////////////////////////////////////////////////////////////////////////////////////
// LOCAL VAR
////////////////////////////////////////////////////////////////////////////////////////////////

const static taskEvtProcesssor_t taskEvtProcesssorArr[TASKMGR_NUM_TASKS] =
{
     bspTask_evtProcessor,          // TaskId = 0
     i2cTask_evtProcessor,          // TaskId = 1
     mainTask_evtProcessor,         // TaskId = 2
}; // taskEvtProcesssorArr

static volatile uint8 timedEvtTaskId = 0;
static volatile uint16 timedEvtFlag = 0;

////////////////////////////////////////////////////////////////////////////////////////////////
// MACROS
////////////////////////////////////////////////////////////////////////////////////////////////

#define taskMgr_enterCriticalSection()              (__disable_interrupt())         // Disable Global interrupts
#define taskMgr_exitCriticalSection()               (__enable_interrupt())          // Enable Global interrupts

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

  // Copy buffered event flags into current task manager loop iteration
  // TODO: Validate if masking global interrupts is really necessary. Although this works now,
  //       asynchronous events (i.e. I2C Master Read/Writes) could potentially cause problems.
  //       Further testing required...
  taskMgr_enterCriticalSection();
  taskMgr_updateEventFlags();
  taskMgr_exitCriticalSection();

  // Go thru each task registered with the taskMgr
  for( taskMgr.currTaskID = 0; taskMgr.currTaskID < TASKMGR_NUM_TASKS; taskMgr.currTaskID++ )
  {
      // Call event processor of respective task if any event flags are set
      if( taskMgr.eventsArr[taskMgr.currTaskID] )
      {
          taskMgr.eventsArr[taskMgr.currTaskID] = taskEvtProcesssorArr[taskMgr.currTaskID]( taskMgr.currTaskID, taskMgr.eventsArr[taskMgr.currTaskID] );
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
uint8 taskMgr_setEventEx( uint8 taskId, uint16 events, uint32 timeEx_ms )
{
    // Check if Timer is currently suspended
    if( ( TA0CTL & MC_3 ) ==  MC_0 )
    {
        // Convert msec interval to CCR0 compare value (dec)
        float countVal = ( (float)timeEx_ms ) * TIME_MS_TO_CCR0_COUNT;

        // TODO: Abstract this further
        timedEvtTaskId = taskId;
        timedEvtFlag = events;

        // When ACLK source = 32.768 kHz crystal/4 = 8.192 Hz
        // Timer A clock source = ACLK
        // Timer A tick period = 1/(8.192kHz) = ~122 usec
        // Mode: Up
        TA0CCR0 = (uint16)countVal;
        TA0CTL = TASSEL_1 + MC_1 + TACLR;
        TA0CCTL0 &= ~CCIFG;
        TA0CCTL0 |= CCIE;
        //__bis_SR_register(LPM3_bits+GIE); // DEFAULT
        //__no_operation();                 // DEFAULT
        return SUCCESS;
    }
    else
        return FAILURE;

} // taskMgr_setEventEx

uint8 taskMgr_clearEventEx( uint8 taskId, uint16 events )
{
    TA0CTL &= ~MC_3;    // Stop Timer
    TA0CCR0 = 0;        // Clear Timer compare register

    return taskMgr_clearEvent( taskId, events );

} // taskMgr_clearEventEx

uint8 taskMgr_clearEvent( uint8 taskId, uint16 events )
{
    if( taskId >= TASKMGR_NUM_TASKS )
        return FAILURE;

    taskMgr.eventsArr[taskId] &= ~events;
    taskMgr.eventsArrBuffered[taskId] &= ~events;   // TEST: Clear the buffered events flags as well

    return SUCCESS;

} // taskMgr_clearEvent

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

static void taskMgr_updateEventFlags( void )
{
    uint8 i;
    for(i = 0; i < TASKMGR_NUM_TASKS; i++ )
    {
        // OR in event flags from last OS loop iteration into current loop iteration.
        // NOTE: Current loop iteration will contain unserviced event flags from last OS
        //       loop iteration. Therefore, higher priority events (i.e. if() statements
        //       that are higher sequentially within the event processor for that task)
        //       will always be serviced before lower priority events within the same task.
        taskMgr.eventsArr[i] |= taskMgr.eventsArrBuffered[i];
        // Clear the buffered event flags in preparation of current cycle.
        taskMgr.eventsArrBuffered[i] = 0;
    }

} // taskMgr_updateEventFlags

////////////////////////////////////////////////////////////////////////////////////////////////
// INTERRUPT SERVICE ROUTINES
////////////////////////////////////////////////////////////////////////////////////////////////

// Timer0_A0 Interrupt Service Routine: Disables the timer and exists LPM3 ////////////////////
#pragma vector=TIMER0_A0_VECTOR
__interrupt void ISR_Timer0_A0(void)
{
  taskMgr_setEventISR( timedEvtTaskId, timedEvtFlag );
  //TA0CTL &= ~(MC_1);  // DEFAULT
  TA0CTL &= ~(MC_3);    // TEST
  TA0CCTL0 &= ~(CCIE);    // DEFAULT
  //__bic_SR_register_on_exit(LPM3_bits+GIE); // DEFAULT
}

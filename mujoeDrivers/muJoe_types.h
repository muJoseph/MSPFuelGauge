/*
 * muJoe_types.h
 *
 *  Created on: Nov 18, 2017
 *      Author: joe
 */

#ifndef MUJOE_TYPES_H_
#define MUJOE_TYPES_H_

////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////

// Generic Status Codes
#define SUCCESS                     0x00
#define FAILURE                     0x01

////////////////////////////////////////////////////////////////////////////////////////////////
// TYPEDEFS
////////////////////////////////////////////////////////////////////////////////////////////////

// Generic Data Types
typedef unsigned char       uint8;
typedef signed char         int8;

typedef unsigned short      uint16;
typedef signed short        int16;

typedef unsigned int        uint32;
typedef signed int          int32;

typedef enum
{
   FALSE    = 0x00,
   TRUE     = 0x01,
}bool;

typedef bool boolean;
typedef bool bool_t;

// Generic State Machine Variable
typedef struct genericStateMachine_def
{
    uint8       s;      // Current State Machine State
    uint8       f;      // Current State Machine Flags

}genSm_t;

#endif /* MUJOE_TYPES_H_ */

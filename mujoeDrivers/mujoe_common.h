/*
 * mujoe_common.h
 *
 *  Created on: Nov 25, 2017
 *      Author: joe
 */

#ifndef MUJOEDRIVERS_MUJOE_COMMON_H_
#define MUJOEDRIVERS_MUJOE_COMMON_H_



////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include <msp430.h>
#include "mujoe_types.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////////////////////

// MSPFuelGauge Pin Map
#define P1_2_FUEL_PROBE                     0x04    // P1.2
#define P2_2_REF_CAP                        0x04    // P2.2
#define P2_3_MSP_INTn                       0x08    // P2.3
#define P2_4_TESTPT                         0x10    // P2.4

#define P1_6_I2C_SCL                        0x40    // P1.6
#define P1_7_I2C_SDA                        0x80    // P1.7

////////////////////////////////////////////////////////////////////////////////////////////////
// TYPEDEFS
////////////////////////////////////////////////////////////////////////////////////////////////

// DCO Clock Frequencies
typedef enum
{
    DCO_FREQ_1MHZ = 0x00,
    DCO_FREQ_8MHZ,
    DCO_FREQ_12MHZ,
    DCO_FREQ_16MHZ,

}dcoClkFreq_t;

typedef enum
{
    XTALCAP_DISABLED = XCAP_0,
    XTALCAP_6PF      = XCAP_1,
    XTALCAP_10PF     = XCAP_2,
    XTALCAP_12PF5    = XCAP_3,

}xtalTankCap_t;

typedef enum
{
    SMCLK_DIV1 = DIVS_0,
    SMCLK_DIV2 = DIVS_1,
    SMCLK_DIV4 = DIVS_2,
    SMCLK_DIV8 = DIVS_3,

}smclk_div_t;

typedef enum
{
    ACLK_DIV1 = DIVA_0,
    ACLK_DIV2 = DIVA_1,
    ACLK_DIV4 = DIVA_2,
    ACLK_DIV8 = DIVA_3,

}aclk_div_t;

typedef struct sysClkConfig_def
{
    dcoClkFreq_t        dcoFreq;        // DCO Clock Frequency
    bool                xtalPop;        // If TRUE, ACLK source = external 32.768kHz crystal. If FALSE, ACLK source = internal VLO @ 12 kHz (typ.)
    xtalTankCap_t       xtalTankCap;    // Internal Tank Capacitance for external 32.768kHz crystal
    smclk_div_t         smclkDivider;   // SMCLK Source frequency divider
    aclk_div_t          aclkDivider;    // ACLK Source frequency divider

}sysClkConfig_t;

////////////////////////////////////////////////////////////////////////////////////////////////
// MACROS
////////////////////////////////////////////////////////////////////////////////////////////////

#define stopWDT()                   (WDTCTL = WDTPW + WDTHOLD)
#define startWDT()                  (WDTCTL = WDTPW)

// Use as a dummy line of code to put break points on
#define putBreakPtHere()            (__no_operation())

// MSP_INTn OUTPUT control
#define MSPINTn_bringLOW()          (P2OUT &=~P2_3_MSP_INTn)
#define MSPINTn_bringHIGH()         (P2OUT |=P2_3_MSP_INTn)

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

void mujoeCommon_configClocks( sysClkConfig_t sysClkConfig );
void mujoeCommon_softwareReset( void );

#endif /* MUJOEDRIVERS_MUJOE_COMMON_H_ */

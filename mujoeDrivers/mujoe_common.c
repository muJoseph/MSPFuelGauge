/*
 * mujoe_common.c
 *
 *  Created on: Nov 25, 2017
 *      Author: joe
 */


////////////////////////////////////////////////////////////////////////////////////////////////
// INCLUDE
////////////////////////////////////////////////////////////////////////////////////////////////

#include "mujoe_common.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTION PROTOS
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// EXTERN VAR
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// LOCAL VAR
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

void mujoeCommon_configClocks( sysClkConfig_t sysClkConfig )
{
    // 1 MHz
    if( sysClkConfig.dcoFreq == DCO_FREQ_1MHZ )
    {
         if (CALBC1_1MHZ==0xFF)                    // If calibration constant erased
           while(1);                               // do not load, trap CPU!!

         DCOCTL = 0;                               // Select lowest DCOx and MODx settings
         BCSCTL1 = CALBC1_1MHZ;                    // Set range
         DCOCTL = CALDCO_1MHZ;                     // Set DCO step + modulation
    }
    // 8 MHz
    else if( sysClkConfig.dcoFreq == DCO_FREQ_8MHZ )
    {
        if (CALBC1_8MHZ==0xFF)                    // If calibration constant erased
           while(1);                              // do not load, trap CPU!!

        DCOCTL = 0;                               // Select lowest DCOx and MODx settings
        BCSCTL1 = CALBC1_8MHZ;                    // Set range
        DCOCTL = CALDCO_8MHZ;                     // Set DCO step + modulation */
    }
    // 12 MHz
    else if( sysClkConfig.dcoFreq == DCO_FREQ_12MHZ )
    {
        if (CALBC1_12MHZ==0xFF)                   // If calibration constant erased
           while(1);                              // do not load, trap CPU!!

        DCOCTL = 0;                               // Select lowest DCOx and MODx settings
        BCSCTL1 = CALBC1_12MHZ;                   // Set range
        DCOCTL = CALDCO_12MHZ;                    // Set DCO step + modulation*/
    }
    // 16 MHz
    else if( sysClkConfig.dcoFreq == DCO_FREQ_16MHZ )
    {
        if (CALBC1_16MHZ==0xFF)                   // If calibration constant erased
           while(1);                              // do not load, trap CPU!!

        DCOCTL = 0;                               // Select lowest DCOx and MODx settings
        BCSCTL1 = CALBC1_16MHZ;                   // Set range
        DCOCTL = CALDCO_16MHZ;                    // Set DCO step + modulation*/
    }

    // Set ACLK Source frequency divider
    BCSCTL1 &= ~ACLK_DIV8;                        // Clear BCSCTL1.DIVAx bit field
    BCSCTL1 |= sysClkConfig.aclkDivider;          // Set ACLK divider bit field

    // Set SMCLK Source frequency divider
    BCSCTL2 &= ~SMCLK_DIV8;                       // Clear BCSCTL2.DIVSx bit field
    BCSCTL2 |= sysClkConfig.smclkDivider;         // Set SMCLK divider bit field

    // Enable external XTAL if necessary
    if( sysClkConfig.xtalPop )
    {
        BCSCTL3 &= ~LFXT1S_3;                     // Set BCSCTL3.LFXT1Sx = 00b = 32768-Hz crystal on LFXT1 (i.e LFXT1 = XTAL)
        BCSCTL3 &= ~XCAP_3;                       // Clear XCAP tank capacitance bit field
        BCSCTL3 |= sysClkConfig.xtalTankCap;      // Set XCAP tank capacitance bit field
        while( BCSCTL3 & LFXT1OF );               // If LFXtal fault detected, trap forever
    }
    else
        BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO

} // mujoeCommon_configClocks

////////////////////////////////////////////////////////////////////////////////////////////////
// STATIC FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////////////

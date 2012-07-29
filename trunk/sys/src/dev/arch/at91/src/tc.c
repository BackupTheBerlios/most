/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */  
    
#include "dev/arch/at91/tc.h"
    
DEV_at91_TC_configure (AT91S_TC * pTc, unsigned int mode) 
{
    
        // Disable TC clock
        pTc->TC_CCR = AT91C_TC_CLKDIS;
    
        // Disable interrupts
        pTc->TC_IDR = 0xFFFFFFFF;
    
        // Clear status register
        pTc->TC_SR;
    
        // Set mode
        pTc->TC_CMR = mode;


DEV_at91_TC_start (AT91S_TC * pTc) 
{
    


DEV_at91_TC_stop (AT91S_TC * pTc) 
{
    


DEV_at91_TC_find_mck_divisor (
                              
{
    
    
    
        // Satisfy lower bound
        while (freq < ((mck / divisors[index]) / 65536))
    {
        
        
            // If no divisor can be found, return 0
            if (index == 5)
        {
            
        
    
    
        // Try to maximise DIV while satisfying upper bound
        while (index < 4)
    {
        
        {
            
        
        
    
    
        // Store results
        if (div)
    {
        
    
    
    {
        
    
    



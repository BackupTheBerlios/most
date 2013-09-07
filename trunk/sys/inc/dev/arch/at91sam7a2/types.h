/*-----------------------------------------------------------------------------
*   EUROPE TECHNOLOGIES Software Support
*------------------------------------------------------------------------------
* The software is delivered "AS IS" without warranty or condition of any
* kind, either express, implied or statutory. This includes without
* limitation any warranty or condition with respect to merchantability or
* fitness for any particular purpose, or against the infringements of
* intellectual property rights of others.
*------------------------------------------------------------------------------
*
* File Name       : csp_types.h
* Description     : Definitions and Structures 
* Library Version : 2.00
*
*       +----- (NEW | MODify | ADD | DELete)                                 
*       |                                                                    
*  No   |   When       Who                What               
*-----+---+----------+------------------+--------------------------------------
* 000  NEW  01/05/99   Patrice VILCHEZ    Creation
* 001  MOD  01/04/01   Olivier MAZUYER    Clean up
* 002  MOD  08/06/01   Frederic SAMSON    Clean Up 
* 003  MOD  29/10/01   Christophe GARDIN  Clean Up
* 004  MOD  04/04/02   Christophe GARDIN  #ifndef FALSE and #ifndef TRUE
*----------------------------------------------------------------------------*/

#ifndef CSP_TYPES_H
#define CSP_TYPES_H

#include <ace/stddef.h>

/******************************************************************************
*********************************** TYPES *************************************
******************************************************************************/

/******************************************************************************
* Data Types
******************************************************************************/
/* Signed Types                                                              */
typedef ACE_s8_t        S8_T;
typedef ACE_s16_t       S16_T;
typedef ACE_s32_t       S32_T;

/* Unsigned Types                                                            */
typedef ACE_u8_t        U8_T;
typedef ACE_u16_t       U16_T;
typedef ACE_u32_t       U32_T;

/* Float Types                                                               */
typedef float           F32_T;
typedef double          F64_T;


/******************************************************************************
* Peripherals Type
******************************************************************************/
typedef volatile U32_T CSP_REGISTER_T; 


#endif   /* CSP_TYPE_H */

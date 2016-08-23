#pragma once

#ifndef _INTEROPTYPES_H_
#define _INTEROPTYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

#ifdef __APPLE__
#define __int64		long long	// This needs to be defined on the mac
#endif
    
#define _UINT64
#define _UINT32
#define _UINT16
#define _UINT8
#define _INT64
#define _INT32
#define _INT16
#define _INT8

/*********************************************************************/

typedef void*                                                   echandle;                           /* Handle */
typedef unsigned char                                           uint8;                              /* unsigned */
typedef unsigned short int                                      uint16;
//typedef unsigned long int                                       uint32;
typedef unsigned __int64                                        uint64;
typedef signed char                                             int8;                               /* signed */
typedef signed short int                                        int16;
//typedef signed long int                                         int32;
typedef signed __int64                                          int64;
typedef int16                                                   fixed16;                            /* 8.8 Fixedpoint */
//typedef int32                                                   fixed32;                            /* 16.16 Fixedpoint */
typedef int64                                                   fixed64;                            /* 32.32 Fixedpoint */
typedef float                                                   float32;                            /* single precision floating point */
typedef double                                                  float64;                            /* double precision floating point */

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif

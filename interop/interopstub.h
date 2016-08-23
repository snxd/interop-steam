#pragma once

/*! \file interopstub.h
 * Interop implementation functions
 */

#ifndef _INTEROPSTUB_H_
#define _INTEROPSTUB_H_

#include "interop.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************/

/*! 
 * Called when an instance is created
 * \param TypeName type of object to be created
 * \param InstanceId id of the object to be created
 * \param InstanceIdLength maximum length of the InstanceId
 * \param ExecuteUserPtr user pointer stored with execute callback
 * \param Execute execute callback
 * \param InvokeInstance pointer to the instance invocation callback function
 * \param RemoveInstance pointer to the instance remove callback function
 * \param ProcessInstance pointer to the instance process callback function
 * \param UserPtr pointer associated with instance id
 * \return TRUE on success, FALSE otherwise
 */
int32 Interop_CreateInstance(char *TypeName, char *InstanceId, int32 InstanceIdLength, 
                             void *ExecuteUserPtr, InteropExecuteCallback Execute, 
                             InteropInvokeInstanceCallback *InvokeInstance,
                             InteropRemoveInstanceCallback *RemoveInstance,
                             InteropProcessInstanceCallback *ProcessInstance,
                             void **UserPtr);


/*! 
 * Set an interop option by key
 * \param Key identifier of option
 * \param Value value of option
 * \return TRUE on success, FALSE otherwise
 */
int32 Interop_SetOption(char *Key, char *Value);

/*! 
 * Initializes interop functions
 * \param InteropHost information about the interop host
 * \return TRUE on success, FALSE otherwise
 */
int32 Interop_Init(InteropHostStruct *InteropHost);
/*! 
 * Uninitialize interop functions
 * \return TRUE on success, FALSE otherwise
 */
int32 Interop_Remove(void);

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif
#pragma once

/*! \file interop.h
 * Interop functions and callbacks
 */

#ifndef _INTEROP_H_
#define _INTEROP_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_JSON_STRINGLENGTH           (1024 * 8) //!< Max JSON string length supported   

/*********************************************************************/
// Public

/*! 
 * Function definition for string expansion
 * \param String original string requiring expansion
 * \param ResultString string buffer with the expanded version
 * \param ResultStringLength maximum length of result string
 * \return TRUE on success, FALSE otherwise
 */
typedef int32 (*AppProxy_ExpandStringFunc)(char *String, char *ResultString, int32 ResultStringLength);
/*! 
 * Function definition for retrieving the size of the app.config.xml file
 * \param BufferSize the size of the xml file
 * \return TRUE on success, FALSE otherwise
 */
typedef int32 (*AppProxy_GetConfigSizeFunc)(int32 *BufferSize);
/*! 
 * Function definition for retrieving the app.config.xml file
 * \param BufferPtr string buffer containing the app.config.xml file
 * \param BufferSize maximum size of the buffer contained in buffer ptr
 * \return TRUE on success, FALSE otherwise
 */
typedef int32 (*AppProxy_GetConfigFunc)(uint8* BufferPtr, int32 BufferSize);

typedef int32 (*DebugProxy_Flags)(void);
typedef int32 (*DebugProxy_Print)(char *String);
typedef int32 (*EventQueueProxy_Add)(void *EventQueueContext, void *Instance, void *InternalEvent, int32 InternalEventSize, int32 (*FlushEvent)(void *Instance, void *InternalEvent));
typedef int32 (*EventQueueProxy_FlushInstance)(void *EventQueueContext, void *Instance);
typedef int32 (*InteropWebGet_ConstructURLCallback)(char *URL, int32 MaxURL);
typedef int32 (*InteropWebGet_AuthenticateCallback)(char *WebGetContextId, int32 Type, char *Host, char *Realm);
typedef int32 (*InteropWebGet_ErrorCallback)(char *URL, uint8 *IP, int32 Port, int32 Type, int32 Code);

typedef int32 (*InteropAuthenticate_EnabledCallback)();
typedef int32 (*InteropAuthenticate_ValidateCallback)(char *CommonName);

typedef int32 (*InteropRegistry_AuthorizeCallback)(int32 RequestedPermission, int32 Class, char *Path);

typedef int32 (*WebServerProxy_ConnectionGetMethodPtr)(echandle WebServerHandle, echandle ConnectionHandle, char **MethodPtr);
typedef int32 (*WebServerProxy_ConnectionGetURLPtr)(echandle WebServerHandle, echandle ConnectionHandle, char **URLPtr);
typedef int32 (*WebServerProxy_ConnectionGetFilenamePtr)(echandle WebServerHandle, echandle ConnectionHandle, char **FilenamePtr);
typedef int32 (*WebServerProxy_ConnectionGetQueryPtr)(echandle WebServerHandle, echandle ConnectionHandle, char **QueryPtr);
typedef int32 (*WebServerProxy_ConnectionGetVersionPtr)(echandle WebServerHandle, echandle ConnectionHandle, char **VersionPtr);
typedef int32 (*WebServerProxy_ConnectionGetConnectTime)(echandle WebServerHandle, echandle ConnectionHandle, float64 *ConnectTime);
typedef int32 (*WebServerProxy_ConnectionGetResponseCode)(echandle WebServerHandle, echandle ConnectionHandle, int32 *ResponseCode);
typedef int32 (*WebServerProxy_ConnectionSetStallTime)(echandle WebServerHandle, echandle ConnectionHandle, int32 StallTime);
typedef int32 (*WebServerProxy_ConnectionGetHeaderValue)(echandle WebServerHandle, echandle ConnectionHandle, char *Name, char *Value, int32 MaxValue);
typedef int32 (*WebServerProxy_ConnectionSetHeaderValue)(echandle WebServerHandle, echandle ConnectionHandle, char *Name, char *Value);
typedef int32 (*WebServerProxy_ConnectionFreeHeaders)(echandle WebServerHandle, echandle ConnectionHandle);
typedef int32 (*WebServerProxy_ConnectionSetResponse)(echandle WebServerHandle, echandle ConnectionHandle, int32 ResponseCode, int64 ContentLength);
typedef int32 (*WebServerProxy_ConnectionSetExpires)(echandle WebServerHandle, echandle ConnectionHandle, float64 DateEncoded);
typedef int32 (*WebServerProxy_ConnectionSetDateEncoded)(echandle WebServerHandle, echandle ConnectionHandle, float64 DateEncoded);

/*! \struct InteropHostStruct
 *  Structure used to store global information about the interop host
 */
typedef struct InteropHostStruct
{
    int32                                           Size;                                   //!< Size of structure
    DebugProxy_Flags                                Debug_Flags;                            //!< Debug flags function
    DebugProxy_Print                                Debug_Print;                            //!< Log debugging function
    EventQueueProxy_Add                             EventQueue_Add;                         //!< Event queue add function
    EventQueueProxy_FlushInstance                   EventQueue_FlushInstance;               //!< Event queue flush instance function
    AppProxy_ExpandStringFunc                       App_ExpandString;                       //!< String expansion function
    AppProxy_GetConfigSizeFunc                      App_GetConfigSize;                      //!< Size of app.config.xml
    AppProxy_GetConfigFunc                          App_GetConfig;                          //!< Retrieve app.config.xml
    InteropWebGet_ConstructURLCallback              WebGet_ConstructUrlCallback;            //!< Construct URL callback
    InteropWebGet_AuthenticateCallback              WebGet_AuthenticateCallback;            //!< Authenticate site callback
    InteropWebGet_ErrorCallback                     WebGet_ErrorCallback;                   //!< WebGet error callback
    InteropAuthenticate_EnabledCallback             Authenticate_EnabledCallback;           //!< File authentication callback
    InteropAuthenticate_ValidateCallback            Authenticate_ValidateCallback;          //!< File validation callback
    InteropRegistry_AuthorizeCallback               Registry_AuthorizeCallback;             //!< Registry path authorization
    void                                            *FileOverridePtr;                       //!< File override pointer
} InteropHostStruct;

/*! 
 * Function definition for script execution
 * \param UserPtr pointer stored when callback is setup
 * \param InstanceId id of the object to invoke
 * \param CallbackInfo json string representing the method invocation
 * \param ResultString json string representing the return value
 * \param ResultStringLength maximum length of the ResultString
 * \return TRUE on success, FALSE otherwise
 */
typedef int32 (*Interop_ExecuteCallback)(void* UserPtr, char *InstanceId, char *CallbackInfo, char *ResultString, int32 ResultStringLength);
/*! 
 * Function definition for instance removal
 * \param UserPtr pointer stored when callback is setup
 * \param InstanceId id of the object to remove
 * \return TRUE on success, FALSE otherwise
 */
typedef int32 (*Interop_RemoveInstanceCallback)(void* UserPtr, char *InstanceId);
/*! 
 * Function definition for native execution
 * \param UserPtr pointer stored when callback is setup
 * \param InstanceId id of the object to invoke
 * \param String json string representing the method invocation
 * \param ResultString json string representing the return value
 * \param ResultStringLength maximum length of the ResultString
 * \return TRUE on success, FALSE otherwise
 */
typedef int32 (*Interop_InvokeInstanceCallback)(void* UserPtr, char *InstanceId, char *String, char *ResultString, int32 ResultStringLength);
/*! 
 * Function definition for instance processing
 * \param UserPtr pointer stored when callback is setup
 * \param InstanceId id of the object to process
 * \param Continue object still has data waiting to be processed
 * \return TRUE on success, FALSE otherwise
 */
typedef int32 (*Interop_ProcessInstanceCallback)(void* UserPtr, char *InstanceId, int32 *Continue);

typedef struct InteropWebServerConnectionStruct InteropWebServerConnectionStructForward;

typedef int32 (*InteropWebServer_ConnectionPostCallback)(InteropWebServerConnectionStructForward *Connection, uint8 *Buffer, int32 BufferSize);
typedef int32 (*InteropWebServer_ConnectionResponseCallback)(InteropWebServerConnectionStructForward *Connection);
typedef int32 (*InteropWebServer_ConnectionBodyCallback)(InteropWebServerConnectionStructForward *Connection, int32 *Done, uint8 *Buffer, int32 BufferSize, int32 *BufferFilled);
typedef int32 (*InteropWebServer_ConnectionCloseCallback)(InteropWebServerConnectionStructForward *Connection);

/*! \struct InteropWebServerConnectionStruct
 *  Structure used to store information about the interop web server connection
 */
typedef struct InteropWebServerConnectionStruct
{
    echandle                                    WebServerHandle;
    echandle                                    ConnectionHandle;
    char                                        Filename[320];
    void                                        *UserPtr;
    WebServerProxy_ConnectionGetMethodPtr       GetMethodPtr;
    WebServerProxy_ConnectionGetURLPtr          GetURLPtr;
    WebServerProxy_ConnectionGetFilenamePtr     GetFilenamePtr;
    WebServerProxy_ConnectionGetQueryPtr        GetQueryPtr;
    WebServerProxy_ConnectionGetVersionPtr      GetVersionPtr;
    WebServerProxy_ConnectionGetConnectTime     GetConnectTime;
    WebServerProxy_ConnectionGetResponseCode    GetResponseCode;
    WebServerProxy_ConnectionSetStallTime       SetStallTime;
    WebServerProxy_ConnectionGetHeaderValue     GetHeaderValue;
    WebServerProxy_ConnectionSetHeaderValue     SetHeaderValue;
    WebServerProxy_ConnectionFreeHeaders        FreeHeaders;
    WebServerProxy_ConnectionSetResponse        SetResponse;
    WebServerProxy_ConnectionSetExpires         SetExpires;
    WebServerProxy_ConnectionSetDateEncoded     SetDateEncoded;
    InteropWebServer_ConnectionPostCallback     PostCallback;
    InteropWebServer_ConnectionResponseCallback ResponseCallback;
    InteropWebServer_ConnectionBodyCallback     BodyCallback;
    InteropWebServer_ConnectionCloseCallback    CloseCallback;
} InteropWebServerConnectionStruct;

/*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif

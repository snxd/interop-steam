#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#include "steam_api.h"

#include "steamapp.h"
#include "steamutils.h"

/*********************************************************************/

typedef struct SteamUtilsStruct
{
    ClassStruct         Class;
} SteamUtilsStruct;

/********************************************************************/
// Concrete Functions

int32 SteamUtils_IsOverlayEnabled(void *SteamUtilsContext)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUtils()->IsOverlayEnabled() != 0;
}

int32 SteamUtils_IsSteamRunningInVR(void *SteamUtilsContext)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUtils()->IsSteamRunningInVR() != 0;
}

int32 SteamUtils_IsSteamInBigPictureMode(void *SteamUtilsContext)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUtils()->IsSteamInBigPictureMode() != 0;
}

int32 SteamUtils_IsVRHeadsetStreamingEnabled(void *SteamUtilsContext)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    return SteamUtils()->IsVRHeadsetStreamingEnabled() != 0;
}

int32 SteamUtils_GetServerRealTime(void *SteamUtilsContext, int64 *ServerRealTime)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *ServerRealTime = SteamUtils()->GetServerRealTime();
    return TRUE;
}

int32 SteamUtils_GetIPCountry(void *SteamUtilsContext, char *IPCountry, int32 MaxIPCountry)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    String_CopyLength(IPCountry, (char *)SteamUtils()->GetIPCountry(), MaxIPCountry);
    return TRUE;
}

int32 SteamUtils_GetCurrentBatteryPower(void *SteamUtilsContext, int32 *CurrentBatteryPower)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *CurrentBatteryPower = (int32)SteamUtils()->GetCurrentBatteryPower();
    return TRUE;
}

int32 SteamUtils_GetAppID(void *SteamUtilsContext, int32 *AppID)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    *AppID = (int32)SteamUtils()->GetAppID();
    return TRUE;
}

int32 SteamUtils_GetImageWidth(void *SteamUtilsContext, int32 Index, int32 *Width)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    uint32 Height = 0;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    if (!SteamUtils()->GetImageSize(Index, (uint32 *)Width, &Height))
        return FALSE;
    return TRUE;
}

int32 SteamUtils_GetImageHeight(void *SteamUtilsContext, int32 Index, int32 *Height)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    uint32 Width = 0;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    if (!SteamUtils()->GetImageSize(Index, &Width, (uint32 *)Height))
        return FALSE;
    return TRUE;
}

int32 SteamUtils_GetImageRGBAPtr(void *SteamUtilsContext, int32 Index, uint8 **Base64Buffer, int32 *Base64BufferSize)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    int32 Width = 0;
    int32 Height = 0;
    int32 Base64Length = 0;
    int32 BufferSize = 0;
    uint8 *Buffer = NULL;
    
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    if (!SteamUtils()->GetImageSize(Index, (uint32 *)&Width, (uint32 *)&Height))
        return FALSE;

    BufferSize = 4 * Width * Height * sizeof(char);
    Buffer = (uint8 *)malloc(BufferSize);
    SteamUtils()->GetImageRGBA(Index, Buffer, BufferSize);

    *Base64BufferSize = 0;
    *Base64Buffer = NULL;

    Base64_CalculateEncodeSize(BufferSize, Base64BufferSize);
    *Base64Buffer = (uint8 *)malloc(*Base64BufferSize);
    Base64_Encode(Buffer, BufferSize, (char *)*Base64Buffer, *Base64BufferSize, &Base64Length);

    return TRUE;
}

int32 SteamUtils_ReleaseImageRGBAPtr(void *SteamUtilsContext, uint8 **Base64Buffer, int32 *Base64BufferSize)
{
    if (Base64Buffer != NULL && *Base64Buffer != NULL)
    {
        free(*Base64Buffer);
        *Base64Buffer = NULL;
    }
    if (Base64BufferSize != NULL)
    {
        *Base64BufferSize = 0;
    }
    return TRUE;
}

int32 SteamUtils_StartVRDashboard(void *SteamUtilsContext)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    int32 Width = 0;
    if (SteamApp_IsInitialized() == FALSE)
        return FALSE;
    SteamUtils()->StartVRDashboard();
    return TRUE;
}

/*********************************************************************/
// Interop Functions

int32 SteamUtils_GetInstanceId(void *SteamUtilsContext, char *String, int32 MaxString)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct *)SteamUtilsContext;
    String_CopyLength(String, Class_InstanceId(Utils), MaxString);
    return TRUE;
}

int32 SteamUtils_Process(void *SteamUtilsContext)
{
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return TRUE;
}

int32 SteamUtils_Invoke(void *SteamUtilsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle)
{
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    SteamUtilsStruct *Utils = (SteamUtilsStruct*)SteamUtilsContext;
    echandle ItemHandle = NULL;
    int64 Value64 = 0;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    int32 Value32 = 0;
    char *Method = NULL;
    char *ValueString = NULL;
    char Value[120] = { 0 };

    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "isOverlayEnabled") == TRUE)
    {
        ReturnValue = SteamUtils_IsOverlayEnabled(Utils);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isSteamRunningInVR") == TRUE)
    {
        ReturnValue = SteamUtils_IsSteamRunningInVR(Utils);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isSteamInBigPictureMode") == TRUE)
    {
        ReturnValue = SteamUtils_IsSteamInBigPictureMode(Utils);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "isVRHeadsetStreamingEnabled") == TRUE)
    {
        ReturnValue = SteamUtils_IsVRHeadsetStreamingEnabled(Utils);
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    }
    else if (String_Compare(Method, "getServerRealTime") == TRUE)
    {
        RetVal = SteamUtils_GetServerRealTime(Utils, &Value64);
        IDictionary_AddInt64(ReturnDictionaryHandle, "returnValue", Value64, &ItemHandle);
    }
    else  if (String_Compare(Method, "getIPCountry") == TRUE)
    {
        RetVal = SteamUtils_GetIPCountry(Utils, Value, Element_Count(Value));
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Value, &ItemHandle);
    }
    else if (String_Compare(Method, "getCurrentBatteryPower") == TRUE)
    {
        RetVal = SteamUtils_GetCurrentBatteryPower(Utils, &Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    }
    else if (String_Compare(Method, "getAppID") == TRUE)
    {
        RetVal = SteamUtils_GetAppID(Utils, &Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    }
    else if (String_Compare(Method, "getImageWidth") == TRUE)
    {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE)
            RetVal = SteamUtils_GetImageWidth(Utils, Value32, &Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    }
    else if (String_Compare(Method, "getImageHeight") == TRUE)
    {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE)
            RetVal = SteamUtils_GetImageHeight(Utils, Value32, &Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    }
    else if (String_Compare(Method, "getImageRGBA") == TRUE)
    {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE)
        {
            uint8 *Base64Buffer = NULL;
            int32 Base64BufferSize = 0;
            RetVal = SteamUtils_GetImageRGBAPtr(Utils, Value32, &Base64Buffer, &Base64BufferSize);
            if (RetVal == TRUE)
            {
                IDictionary_AddString(ReturnDictionaryHandle, "returnValue", (char *)Base64Buffer, &ItemHandle);
                SteamUtils_ReleaseImageRGBAPtr(Utils, &Base64Buffer, &Base64BufferSize);
            }
        }
        if (RetVal == FALSE)
            IDictionary_AddNull(ReturnDictionaryHandle, "returnValue", &ItemHandle);
    }
    else if (String_Compare(Method, "startVRDashboard") == TRUE)
    {
        RetVal = SteamUtils_StartVRDashboard(Utils);
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", RetVal, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/
// Creation/Deletion Functions

int32 SteamUtils_Create(void **SteamUtilsContext)
{
    SteamUtilsStruct *Utils = NULL;

    Utils = (SteamUtilsStruct*)malloc(sizeof(SteamUtilsStruct));
    memset(Utils, 0, sizeof(SteamUtilsStruct));
    Interop_GenerateInstanceId(Utils->Class.InstanceId, 40);

    Utils->Class.RefCount = 1;

    *SteamUtilsContext = Utils;
    return TRUE;
}

void *SteamUtils_AddRef(void *SteamUtilsContext)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct *)SteamUtilsContext;
    Utils->Class.RefCount += 1;
    return Utils;
}

int32 SteamUtils_Release(void **SteamUtilsContext)
{
    SteamUtilsStruct *Utils = (SteamUtilsStruct *)*SteamUtilsContext;

    if (--Utils->Class.RefCount == 0)
    {
        free(Utils);
    }

    *SteamUtilsContext = NULL;
    return TRUE;
}

/*********************************************************************/

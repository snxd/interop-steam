#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#include "steam_api.h"

#include "steamapi.h"
#include "steamutils.h"

/*********************************************************************/
// Concrete Functions

static int32 SteamUtils_IsOverlayEnabled(void) {
    return SteamUtils()->IsOverlayEnabled() != 0;
}

static int32 SteamUtils_IsSteamRunningInVR(void) {
    return SteamUtils()->IsSteamRunningInVR() != 0;
}

static int32 SteamUtils_IsSteamInBigPictureMode(void) {
    return SteamUtils()->IsSteamInBigPictureMode() != 0;
}

static int32 SteamUtils_IsVRHeadsetStreamingEnabled(void) {
    return SteamUtils()->IsVRHeadsetStreamingEnabled() != 0;
}

static int32 SteamUtils_GetServerRealTime(int64 *ServerRealTime) {
    *ServerRealTime = SteamUtils()->GetServerRealTime();
    return TRUE;
}

static int32 SteamUtils_GetIPCountry(char *IPCountry, int32 MaxIPCountry) {
    String_CopyLength(IPCountry, (char *)SteamUtils()->GetIPCountry(), MaxIPCountry);
    return TRUE;
}

static int32 SteamUtils_GetCurrentBatteryPower(int32 *CurrentBatteryPower) {
    *CurrentBatteryPower = (int32)SteamUtils()->GetCurrentBatteryPower();
    return TRUE;
}

static int32 SteamUtils_GetAppID(int32 *AppID) {
    *AppID = (int32)SteamUtils()->GetAppID();
    return TRUE;
}

static int32 SteamUtils_GetImageWidth(int32 Index, int32 *Width) {
    uint32 Height = 0;
    if (!SteamUtils()->GetImageSize(Index, (uint32 *)Width, &Height))
        return FALSE;
    return TRUE;
}

static int32 SteamUtils_GetImageHeight(int32 Index, int32 *Height) {
    uint32 Width = 0;
    if (!SteamUtils()->GetImageSize(Index, &Width, (uint32 *)Height))
        return FALSE;
    return TRUE;
}

static int32 SteamUtils_GetImageRGBAPtr(int32 Index, uint8 **Base64Buffer, int32 *Base64BufferSize) {
    int32 Width = 0;
    int32 Height = 0;
    int32 Base64Length = 0;
    int32 BufferSize = 0;
    uint8 *Buffer = NULL;
    
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

static int32 SteamUtils_ReleaseImageRGBAPtr(uint8 **Base64Buffer, int32 *Base64BufferSize) {
    if (Base64Buffer != NULL && *Base64Buffer != NULL) {
        free(*Base64Buffer);
        *Base64Buffer = NULL;
    }
    if (Base64BufferSize != NULL) {
        *Base64BufferSize = 0;
    }
    return TRUE;
}

static int32 SteamUtils_StartVRDashboard(void) {
    SteamUtils()->StartVRDashboard();
    return TRUE;
}

/*********************************************************************/
// Interop Functions

int32 SteamUtils_Process(void *SteamUtilsContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return TRUE;
}

int32 SteamUtils_Invoke(void *SteamUtilsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = NULL;
    int64 Value64 = 0;
    int32 RetVal = FALSE;
    int32 ReturnValue = FALSE;
    int32 Value32 = 0;
    char *Method = NULL;
    char *ValueString = NULL;
    char Value[120] = {0};

    if (SteamAPI_IsInitialized() == FALSE)
        return FALSE;
    if (IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method) == FALSE)
        return FALSE;

    if (String_Compare(Method, "isOverlayEnabled") == TRUE) {
        ReturnValue = SteamUtils_IsOverlayEnabled();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isSteamRunningInVR") == TRUE) {
        ReturnValue = SteamUtils_IsSteamRunningInVR();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isSteamInBigPictureMode") == TRUE) {
        ReturnValue = SteamUtils_IsSteamInBigPictureMode();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "isVRHeadsetStreamingEnabled") == TRUE) {
        ReturnValue = SteamUtils_IsVRHeadsetStreamingEnabled();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (String_Compare(Method, "getServerRealTime") == TRUE) {
        RetVal = SteamUtils_GetServerRealTime(&Value64);
        IDictionary_AddInt64(ReturnDictionaryHandle, "returnValue", Value64, &ItemHandle);
    } else if (String_Compare(Method, "getIPCountry") == TRUE) {
        RetVal = SteamUtils_GetIPCountry(Value, Element_Count(Value));
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Value, &ItemHandle);
    } else if (String_Compare(Method, "getCurrentBatteryPower") == TRUE) {
        RetVal = SteamUtils_GetCurrentBatteryPower(&Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "getAppID") == TRUE) {
        RetVal = SteamUtils_GetAppID(&Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "getImageWidth") == TRUE) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE)
            RetVal = SteamUtils_GetImageWidth(Value32, &Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "getImageHeight") == TRUE) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE)
            RetVal = SteamUtils_GetImageHeight(Value32, &Value32);
        IDictionary_AddInt32(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (String_Compare(Method, "getImageRGBA") == TRUE) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal == TRUE) {
            uint8 *Base64Buffer = NULL;
            int32 Base64BufferSize = 0;
            RetVal = SteamUtils_GetImageRGBAPtr(Value32, &Base64Buffer, &Base64BufferSize);
            if (RetVal == TRUE) {
                IDictionary_AddString(ReturnDictionaryHandle, "returnValue", (char *)Base64Buffer, &ItemHandle);
                SteamUtils_ReleaseImageRGBAPtr(&Base64Buffer, &Base64BufferSize);
            }
        }
        if (RetVal == FALSE)
            IDictionary_AddNull(ReturnDictionaryHandle, "returnValue", &ItemHandle);
    } else if (String_Compare(Method, "startVRDashboard") == TRUE) {
        RetVal = SteamUtils_StartVRDashboard();
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", RetVal, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/

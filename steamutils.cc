#include <string.h>
#include <stdlib.h>

#include "interoplib.h"
#include "interopstub.h"
#include "dictionaryi.h"

#include "steam_api.h"

#include "steamapi.h"
#include "steamutils.h"

/*********************************************************************/
// Concrete functions

static bool SteamUtils_IsOverlayEnabled(void) {
    return SteamUtils()->IsOverlayEnabled() != 0;
}

static bool SteamUtils_IsSteamRunningInVR(void) {
    return SteamUtils()->IsSteamRunningInVR() != 0;
}

static bool SteamUtils_IsSteamInBigPictureMode(void) {
    return SteamUtils()->IsSteamInBigPictureMode() != 0;
}

static bool SteamUtils_IsVRHeadsetStreamingEnabled(void) {
    return SteamUtils()->IsVRHeadsetStreamingEnabled() != 0;
}

static bool SteamUtils_GetServerRealTime(int64_t *ServerRealTime) {
    *ServerRealTime = SteamUtils()->GetServerRealTime();
    return true;
}

static bool SteamUtils_GetIPCountry(char *IPCountry, int32_t MaxIPCountry) {
    strncpy(IPCountry, (char *)SteamUtils()->GetIPCountry(), MaxIPCountry);
    IPCountry[MaxIPCountry - 1] = 0;
    return true;
}

static bool SteamUtils_GetCurrentBatteryPower(int32_t *CurrentBatteryPower) {
    *CurrentBatteryPower = (int32_t)SteamUtils()->GetCurrentBatteryPower();
    return true;
}

static bool SteamUtils_GetAppID(int32_t *AppID) {
    *AppID = (int32)SteamUtils()->GetAppID();
    return true;
}

static bool SteamUtils_GetImageWidth(int32_t Index, int32_t *Width) {
    uint32_t Height = 0;
    if (!SteamUtils()->GetImageSize(Index, (uint32_t *)Width, &Height))
        return false;
    return true;
}

static bool SteamUtils_GetImageHeight(int32_t Index, int32_t *Height) {
    uint32_t Width = 0;
    if (!SteamUtils()->GetImageSize(Index, &Width, (uint32_t *)Height))
        return false;
    return true;
}

static bool SteamUtils_GetImageRGBAPtr(int32_t Index, uint8_t **Base64Buffer, int32_t *Base64BufferSize) {
    int32_t Width = 0;
    int32_t Height = 0;
    int32_t Base64Length = 0;

    if (!SteamUtils()->GetImageSize(Index, (uint32_t *)&Width, (uint32_t *)&Height))
        return false;

    int32_t BufferSize = 4 * Width * Height * sizeof(char);
    uint8_t *Buffer = (uint8_t *)malloc(BufferSize);
    SteamUtils()->GetImageRGBA(Index, Buffer, BufferSize);

    *Base64BufferSize = 0;
    *Base64Buffer = nullptr;

    Base64_CalculateEncodeSize(BufferSize, Base64BufferSize);
    *Base64Buffer = (uint8_t *)malloc(*Base64BufferSize);
    Base64_Encode(Buffer, BufferSize, (char *)*Base64Buffer, *Base64BufferSize, &Base64Length);

    return true;
}

static bool SteamUtils_ReleaseImageRGBAPtr(uint8_t **Base64Buffer, int32_t *Base64BufferSize) {
    if (Base64Buffer && *Base64Buffer) {
        free(*Base64Buffer);
        *Base64Buffer = nullptr;
    }
    if (Base64BufferSize) {
        *Base64BufferSize = 0;
    }
    return true;
}

static bool SteamUtils_StartVRDashboard(void) {
    SteamUtils()->StartVRDashboard();
    return true;
}

/*********************************************************************/
// Interop functions

bool SteamUtils_Process(void *SteamUtilsContext) {
    // This function is called once per tick and can be used to process simple operations and
    // thread synchronization.
    return true;
}

bool SteamUtils_Invoke(void *SteamUtilsContext, echandle MethodDictionaryHandle, echandle ReturnDictionaryHandle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    echandle ItemHandle = nullptr;
    int64_t Value64 = 0;
    bool RetVal = false;
    int32_t ReturnValue = false;
    int32_t Value32 = 0;
    const char *Method = nullptr;
    char *ValueString = nullptr;
    char Value[120]{};

    if (!SteamAPI_IsInitialized())
        return false;
    if (!IDictionary_GetStringPtrByKey(MethodDictionaryHandle, "method", &Method))
        return false;

    if (strcmp(Method, "isOverlayEnabled") == 0) {
        ReturnValue = SteamUtils_IsOverlayEnabled();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "isSteamRunningInVR") == 0) {
        ReturnValue = SteamUtils_IsSteamRunningInVR();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "isSteamInBigPictureMode") == 0) {
        ReturnValue = SteamUtils_IsSteamInBigPictureMode();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "isVRHeadsetStreamingEnabled") == 0) {
        ReturnValue = SteamUtils_IsVRHeadsetStreamingEnabled();
        RetVal = IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", ReturnValue, &ItemHandle);
    } else if (strcmp(Method, "getServerRealTime") == 0) {
        RetVal = SteamUtils_GetServerRealTime(&Value64);
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", Value64, &ItemHandle);
    } else if (strcmp(Method, "getIPCountry") == 0) {
        RetVal = SteamUtils_GetIPCountry(Value, sizeof(Value));
        IDictionary_AddString(ReturnDictionaryHandle, "returnValue", Value, &ItemHandle);
    } else if (strcmp(Method, "getCurrentBatteryPower") == 0) {
        RetVal = SteamUtils_GetCurrentBatteryPower(&Value32);
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (strcmp(Method, "getAppID") == 0) {
        RetVal = SteamUtils_GetAppID(&Value32);
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (strcmp(Method, "getImageWidth") == 0) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal)
            RetVal = SteamUtils_GetImageWidth(Value32, &Value32);
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (strcmp(Method, "getImageHeight") == 0) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal)
            RetVal = SteamUtils_GetImageHeight(Value32, &Value32);
        IDictionary_AddInt(ReturnDictionaryHandle, "returnValue", Value32, &ItemHandle);
    } else if (strcmp(Method, "getImageRGBA") == 0) {
        RetVal = IDictionary_GetInt32ByKey(MethodDictionaryHandle, "index", &Value32);
        if (RetVal) {
            uint8_t *Base64Buffer = nullptr;
            int32_t Base64BufferSize = 0;
            RetVal = SteamUtils_GetImageRGBAPtr(Value32, &Base64Buffer, &Base64BufferSize);
            if (RetVal) {
                IDictionary_AddString(ReturnDictionaryHandle, "returnValue", (char *)Base64Buffer, &ItemHandle);
                SteamUtils_ReleaseImageRGBAPtr(&Base64Buffer, &Base64BufferSize);
            }
        }
        if (RetVal == false)
            IDictionary_AddNull(ReturnDictionaryHandle, "returnValue", &ItemHandle);
    } else if (strcmp(Method, "startVRDashboard") == 0) {
        RetVal = SteamUtils_StartVRDashboard();
        IDictionary_AddBoolean(ReturnDictionaryHandle, "returnValue", RetVal, &ItemHandle);
    }

    return RetVal;
}

/*********************************************************************/

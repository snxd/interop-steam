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

namespace {

bool GetIPCountry(char *ip_country, int32_t max_ip_country) {
    strncpy(ip_country, SteamUtils()->GetIPCountry(), max_ip_country);
    ip_country[max_ip_country - 1] = 0;
    return true;
}

bool GetImageWidth(int32_t index, int32_t *width) {
    uint32_t height = 0;
    return SteamUtils()->GetImageSize(index, reinterpret_cast<uint32_t *>(width), &height);
}

bool GetImageHeight(int32_t index, int32_t *height) {
    uint32_t width = 0;
    return SteamUtils()->GetImageSize(index, &width, reinterpret_cast<uint32_t *>(height));
}

bool GetImageRGBAPtr(int32_t index, uint8_t **base64_buffer, int32_t *base64_buffer_size) {
    int32_t width = 0;
    int32_t height = 0;
    int32_t base64_length = 0;

    if (!SteamUtils()->GetImageSize(index, reinterpret_cast<uint32_t *>(&width), reinterpret_cast<uint32_t *>(&height)))
        return false;

    int32_t buffer_size = 4 * width * height * sizeof(char);
    auto *buffer = reinterpret_cast<uint8_t *>(malloc(buffer_size));
    SteamUtils()->GetImageRGBA(index, buffer, buffer_size);

    *base64_buffer_size = 0;
    *base64_buffer = nullptr;

    Base64_CalculateEncodeSize(buffer_size, base64_buffer_size);
    *base64_buffer = reinterpret_cast<uint8_t *>(malloc(*base64_buffer_size));
    Base64_Encode(buffer, buffer_size, (char *)*base64_buffer, *base64_buffer_size, &base64_length);
    return true;
}

bool ReleaseImageRGBAPtr(uint8_t **base64_buffer, int32_t *base64_buffer_size) {
    if (base64_buffer && *base64_buffer) {
        free(*base64_buffer);
        *base64_buffer = nullptr;
    }
    if (base64_buffer_size) {
        *base64_buffer_size = 0;
    }
    return true;
}

}  // namespace

/*********************************************************************/
// Interop functions

bool SteamUtils_Invoke(void *handle, echandle method_dictionary_handle, echandle return_dictionary_handle) {
    // EVERYTHING is marshaled in AND out as a JSON string, use any type supported by JSON and
    // it should marshal ok.

    bool ret = false;
    const char *method = nullptr;

    if (!SteamAPI_IsInitialized())
        return false;
    if (!IDictionary_GetStringPtrByKey(method_dictionary_handle, "method", &method))
        return false;

    if (strcmp(method, "isOverlayEnabled") == 0) {
        const auto is_enabled = SteamUtils()->IsOverlayEnabled();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_enabled, nullptr);
    } else if (strcmp(method, "isSteamRunningInVR") == 0) {
        const auto is_running = SteamUtils()->IsSteamRunningInVR() != 0;
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_running, nullptr);
    } else if (strcmp(method, "isSteamInBigPictureMode") == 0) {
        const auto is_big_picture_mode = SteamUtils()->IsSteamInBigPictureMode() != 0;
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_big_picture_mode, nullptr);
    } else if (strcmp(method, "isVRHeadsetStreamingEnabled") == 0) {
        const auto is_vr_enabled = SteamUtils()->IsVRHeadsetStreamingEnabled() != 0;
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", is_vr_enabled, nullptr);
    } else if (strcmp(method, "getServerRealTime") == 0) {
        const auto server_real_time = SteamUtils()->GetServerRealTime();
        ret = IDictionary_AddInt(return_dictionary_handle, "returnValue", server_real_time, nullptr);
    } else if (strcmp(method, "getIPCountry") == 0) {
        char ip_country[320]{};
        ret = GetIPCountry(ip_country, sizeof(ip_country));
        IDictionary_AddString(return_dictionary_handle, "returnValue", ip_country, nullptr);
    } else if (strcmp(method, "getCurrentBatteryPower") == 0) {
        const auto current_battery_power = SteamUtils()->GetCurrentBatteryPower();
        ret = IDictionary_AddInt(return_dictionary_handle, "returnValue", current_battery_power, nullptr);
    } else if (strcmp(method, "getAppID") == 0) {
        const auto app_id = SteamUtils()->GetAppID();
        ret = IDictionary_AddInt(return_dictionary_handle, "returnValue", app_id, nullptr);
    } else if (strcmp(method, "getImageWidth") == 0) {
        int32_t index = 0;
        int32_t width = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "index", &index);
        if (ret)
            ret = GetImageWidth(index, &width);
        IDictionary_AddInt(return_dictionary_handle, "returnValue", width, nullptr);
    } else if (strcmp(method, "getImageHeight") == 0) {
        int32_t index = 0;
        int32_t height = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "index", &index);
        if (ret)
            ret = GetImageHeight(index, &height);
        IDictionary_AddInt(return_dictionary_handle, "returnValue", height, nullptr);
    } else if (strcmp(method, "getImageRGBA") == 0) {
        int32_t index = 0;
        ret = IDictionary_GetInt32ByKey(method_dictionary_handle, "index", &index);
        if (ret) {
            uint8_t *base64_buffer = nullptr;
            int32_t base64_buffer_size = 0;
            ret = GetImageRGBAPtr(index, &base64_buffer, &base64_buffer_size);
            if (ret) {
                IDictionary_AddString(return_dictionary_handle, "returnValue", reinterpret_cast<char *>(base64_buffer),
                                      nullptr);
                ReleaseImageRGBAPtr(&base64_buffer, &base64_buffer_size);
            }
        }
        if (!ret)
            IDictionary_AddNull(return_dictionary_handle, "returnValue", nullptr);
    } else if (strcmp(method, "startVRDashboard") == 0) {
        SteamUtils()->StartVRDashboard();
        ret = IDictionary_AddBoolean(return_dictionary_handle, "returnValue", true, nullptr);
    }

    return ret;
}

/*********************************************************************/

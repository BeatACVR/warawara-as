#include "utils/logger.h"
#include "utils/FSUtils.hpp"
#include <coreinit/filesystem.h>
#include <wups.h>
#include <wups/button_combo/api.h>
#include <vpad/input.h>
#include <coreinit/thread.h>

#include <forward_list>

#include <malloc.h>

#define STR_VALUE(arg)          #arg
#define VERSION_STRING(x, y, z) "v" STR_VALUE(x) "." STR_VALUE(y) "." STR_VALUE(z)

/**
    Mandatory plugin information.
    If not set correctly, the loader will refuse to use the plugin.
**/
WUPS_PLUGIN_NAME("warawara_as");
WUPS_PLUGIN_DESCRIPTION("Autoswap WaraWara");
WUPS_PLUGIN_VERSION(VERSION_STRING(VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH));
WUPS_PLUGIN_AUTHOR("deReeperJosh");
WUPS_PLUGIN_LICENSE("GPLv2");

#define LOG_FS_OPEN_CONFIG_ID             "logFSOpen"
#define BUTTON_COMBO_PRESS_DOWN_CONFIG_ID "pressDownItem"
#define BUTTON_COMBO_HOLD_CONFIG_ID       "holdItem"
#define OTHER_EXAMPLE_BOOL_CONFIG_ID      "otherBoolItem"
#define OTHER_EXAMPLE2_BOOL_CONFIG_ID     "other2BoolItem"
#define INTEGER_RANGE_EXAMPLE_CONFIG_ID   "intRangeExample"

#define TAG_EMULATION_PATH                std::string("/vol/external01/wiiu/warawara_as/")

DEINITIALIZE_PLUGIN() {
    // Remove all button combos from this plugin.
    DEBUG_FUNCTION_LINE("DEINITIALIZE_PLUGIN of warawara_as!");
    FSUtils::Finalize();
}

/**
    Gets called when an application starts.
**/
ON_APPLICATION_START() {
    initLogging();
    DEBUG_FUNCTION_LINE("ON_APPLICATION_START called in swap screens plugin!");

    // only trigger in home menu
    u64 titleID;
    OSDGetTitleID(&titleID);
    if(titleID != 0x000500101004A700ULL) // home menu
        return;

    // tiny delay to let home menu frame load
    svcSleepThread(4000000000); // 50ms

    // press X
    VPADStatus status;
    memset(&status, 0, sizeof(VPADStatus));
    status.btns_h = VPAD_BUTTON_X;
    VPADWrite(0, &status, sizeof(VPADStatus));

    // release X after short delay
    svcSleepThread(50000000); // 50ms
    status.btns_h = 0;
    VPADWrite(0, &status, sizeof(VPADStatus));
}

/**
 * Gets called when an application actually ends
 */
ON_APPLICATION_ENDS() {
    DEBUG_FUNCTION_LINE("ON_APPLICATION_ENDS of warawara_as!");
    deinitLogging();
}

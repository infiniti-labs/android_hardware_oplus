/*
 * SPDX-FileCopyrightText: Paranoid Android
 * SPDX-License-Identifier: Apache-2.0
 */

#include <aidl/vendor/aospa/power/BnPowerFeature.h>
#include <aidl/vendor/oplus/hardware/touch/IOplusTouch.h>
#include <android-base/logging.h>
#include <android/binder_manager.h>

#include <OplusTouchConstants.h>

#include <string>
#include <unordered_map>

using aidl::vendor::oplus::hardware::touch::IOplusTouch;

namespace aidl {
namespace vendor {
namespace aospa {
namespace power {

// Gesture bits of the touchpanel double_tap_enable_indep mask.
static const std::unordered_map<Feature, int> GESTURE_MAP = {
    {Feature::DRAW_V, 2},
    {Feature::DRAW_INVERSE_V, 3},
    {Feature::DRAW_ARROW_RIGHT, 4},
    {Feature::DRAW_ARROW_LEFT, 5},
    {Feature::DRAW_O, 6},
    {Feature::TWO_FINGER_SWIPE, 7},
    {Feature::ONE_FINGER_SWIPE_RIGHT, 8},
    {Feature::ONE_FINGER_SWIPE_LEFT, 9},
    {Feature::ONE_FINGER_SWIPE_DOWN, 10},
    {Feature::ONE_FINGER_SWIPE_UP, 11},
    {Feature::DRAW_M, 12},
    {Feature::DRAW_W, 13},
    {Feature::SINGLE_TAP, 16},
    {Feature::DRAW_S, 18},
};

static std::shared_ptr<IOplusTouch> getOplusTouch() {
    static const std::string instance = std::string(IOplusTouch::descriptor) + "/default";
    return IOplusTouch::fromBinder(ndk::SpAIBinder(AServiceManager_waitForService(instance.c_str())));
}

bool setDeviceSpecificFeature(Feature feature, bool enabled) {
    auto gesture = GESTURE_MAP.find(feature);
    if (gesture == GESTURE_MAP.end()) {
        return false;
    }

    std::shared_ptr<IOplusTouch> oplusTouch = getOplusTouch();
    if (!oplusTouch) {
        LOG(ERROR) << "Failed to get IOplusTouch";
        return false;
    }

    std::string tmp;
    if (!oplusTouch->touchReadNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                       OplusTouchConstants::DOUBLE_TAP_INDEP_NODE, &tmp)
                 .isOk()) {
        return false;
    }

    int contents = std::stoi(tmp, nullptr, 16);
    if (enabled) {
        contents |= (1 << gesture->second);
    } else {
        contents &= ~(1 << gesture->second);
    }

    int result = 0;
    oplusTouch->touchWriteNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                   OplusTouchConstants::DOUBLE_TAP_ENABLE_NODE, "1", &result);
    return oplusTouch
            ->touchWriteNodeFile(OplusTouchConstants::DEFAULT_TP_IC_ID,
                                 OplusTouchConstants::DOUBLE_TAP_INDEP_NODE,
                                 std::to_string(contents), &result)
            .isOk();
}

}  // namespace power
}  // namespace aospa
}  // namespace vendor
}  // namespace aidl

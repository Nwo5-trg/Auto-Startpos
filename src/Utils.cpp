#include "Utils.hpp"

using namespace geode::prelude;

namespace Utils {
    bool getPadDirection(float rotation) {
        float m = fmodf(rotation, 360.0f);
        if (m < -180.0f) m += 360.0f;
        return m >= -90.0f && m <= 90.0f;
    }
}

namespace Settings {
    void updateSettings() {
        auto mod = Mod::get();

        advancedGravity = mod->getSettingValue<bool>("advanced-gravity");

        for (int i = 0; i < 6; i++) {
            enabledModes[i] = mod->getSettingValue<bool>(utils::numToString(i));
        }
    };
}

$on_mod(Loaded) {
    Settings::updateSettings();
     
    listenForAllSettingChangesV3([] (std::shared_ptr<geode::SettingV3>){
        Settings::updateSettings();
    });
}
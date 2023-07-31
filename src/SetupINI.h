#pragma once
#include <SimpleIni.h>

CSimpleIniA ini;

void SetupINI() {

    ini.SetUnicode();
    ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    const char* formulaCom = "# 1 = Requiem-like (Recommended)| 2 = Easy ";
    const char* notificationCom = "# false = HUD messages off | true = HUD messages on";
    const char* logsCom = "# false = Log messages off | true = Log messages on";

    if (ini.IsEmpty()) {
        ini.SetLongValue("Gameplay", "Formula", 1, formulaCom, false, true);
        ini.SetBoolValue("Misc", "Notifications", false, notificationCom, true);
        ini.SetBoolValue("Misc", "Logs", false, logsCom, true);
        ini.SaveFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");
        ini.Reset();
        ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");
    }
}

void INILogCheck() {

    if (ini.GetLongValue("Gameplay", "Formula", 1) == 1) {
        logger::info("Preset: Requiem-like");
    }
    else if (ini.GetLongValue("Gameplay", "Formula", 1) == 2) {
        logger::info("Easy preset");
    }
    if (ini.GetBoolValue("Misc", "Notifications", false) == true) {
        logger::info("Notifications: On");
    }
    else if (ini.GetBoolValue("Misc", "Notifications", false) == false) {
        logger::info("Notifications: Off");
    }
    if (ini.GetBoolValue("Misc", "Logs", false) == true) {
        logger::info("Logs: On");
    }
    else if (ini.GetBoolValue("Misc", "Logs", false) == false) {
        logger::info("Logs: Off");
    }
}
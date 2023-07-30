#pragma once
#include <SimpleIni.h>

CSimpleIniA ini;

void SetupINI() {

    ini.SetUnicode();
    ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    const char* formulaCom = "# 0 = Easy | 1 = Requiem-like (Recommended)";
    const char* notificationCom = "# Turn HUD notifications on/off";

    if (ini.IsEmpty()) {
        ini.SetLongValue("Gameplay", "Formula", 1, formulaCom, false, false);
        ini.SetBoolValue("Misc", "Notifications", false, notificationCom, false);
        ini.SaveFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");
    }
}
#pragma once
#include <SimpleIni.h>

CSimpleIniA ini;

void SetupINI() {

    ini.SetUnicode();
    ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    const char* formulaCom = "# 1 = Requiem-like (Recommended)| 2 = Easy ";
    const char* notificationCom = "# false = HUD notifications off | true = HUD notifications on";

    if (ini.IsEmpty()) {
        ini.SetLongValue("Gameplay", "Formula", 1, formulaCom, false, false);
        ini.SetBoolValue("Misc", "Notifications", false, notificationCom, false);
        ini.SaveFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");
    }
    else if (!ini.IsEmpty()) {
        //do nothing
    }
}
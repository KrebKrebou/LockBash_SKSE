#pragma once

std::string data;
CSimpleIniA ini;


void SetupINI() {

    ini.SetUnicode();
    ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    if (ini.IsEmpty()) {

        ini.SetValue("General", "Notifications", "true");
        ini.SaveFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    }

}
#include "SetupINI.h"

void SetupINI() {

    ini.SetUnicode();
    ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    const char* formulaCom = "# 1 = Requiem-like (Default) | 2 = Custom";
    const char* weapDetcom = "# 1 = Hybrid (Default) | 2 = by Skill";
    const char* crimeDetcom = "# true = Crime on (Default) | false = Crime off";
    const char* customCom = "# Configure your custom preset here!";
    const char* notificationCom = "# false = HUD messages off (Default) | true = HUD messages on";
    const char* logsCom = "# false = Log messages off (Default) | true = Log messages on";

    if (ini.IsEmpty()) {
        ini.SetLongValue("Gameplay", "Difficulty", 1, formulaCom, false, true);
        ini.SetLongValue("Gameplay", "Weapon Detection", 1, weapDetcom, false, true);
        ini.SetBoolValue("Gameplay", "Crime", true, crimeDetcom, true);
        ini.SetBoolValue("Gameplay", "CrimeEvenIfUnlocked", false, NULL, true);
        ini.SetBoolValue("Gameplay", "WarnBeforeReporting", true, NULL, true);

        ini.SetValue("Custom", NULL, NULL, customCom, true);
        ini.SetLongValue("Custom", "Very Easy", 350, NULL, false, true);
        ini.SetLongValue("Custom", "Easy", 450, NULL, false, true);
        ini.SetLongValue("Custom", "Average", 550, NULL, false, true);
        ini.SetLongValue("Custom", "Hard", 750, NULL, false, true);
        ini.SetLongValue("Custom", "Very Hard", 850, NULL, false, true);

        ini.SetBoolValue("Misc", "Notifications", false, notificationCom, true);
        ini.SetBoolValue("Misc", "Logs", true, logsCom, true);

        ini.SaveFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");
        ini.Reset();
        ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");
    }
}

void INILogCheck() {
    if (ini.GetLongValue("Gameplay", "Difficulty", 1) == 1) {
        logger::trace("Difficulty: Requiem-like");
    } else if (ini.GetLongValue("Gameplay", "Difficulty", 1) == 2) {
        logger::trace("Difficulty: Custom");
    }

    if (ini.GetBoolValue("Gameplay", "Crime", true) == true) {
        logger::trace("Crime: On");
    } else if (ini.GetBoolValue("Gameplay", "Crime", true) == false) {
        logger::trace("Crime: Off");
    }

    if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 1) {
        logger::trace("Weapon Detection: Hybrid");
    } else if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 2) {
        logger::trace("Weapon Detection: Skill");
    } 

    if (ini.GetBoolValue("Misc", "Notifications", false) == true) {
        logger::trace("Notifications: On");
    } else if (ini.GetBoolValue("Misc", "Notifications", false) == false) {
        logger::trace("Notifications: Off");
    }

    if (ini.GetBoolValue("Misc", "Logs", false) == true) {
        logger::trace("Logs: On");
    } else if (ini.GetBoolValue("Misc", "Logs", false) == false) {
        logger::trace("Logs: Off");
    }
    logger::debug("-*-");
}
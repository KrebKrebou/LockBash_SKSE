#pragma once

#include "Timer.h"

CSimpleIniA ini;

void Unlock(int milliseconds, RE::TESObjectREFRPtr trg, RE::TESObjectREFRPtr act) {
    ini.SetUnicode();
    ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    std::string canBreak = " broke the lock";
    std::string Break = act->GetBaseObject()->GetName() + canBreak;
    const char *BreakMsg = Break.c_str();

    std::thread timer_thread(timer, milliseconds);
    timer_thread.join();

    if (ini.GetBoolValue("General", "Notifications", true)) {
        if (act->As<RE::Actor>()->IsPlayerRef()) {
            RE::DebugNotification("Lock broken");
        } else if (act->As<RE::Actor>()->IsPlayerTeammate()) {
            RE::DebugNotification(BreakMsg);
        }
    } 

    RE::PlaySound("TRPTripwireSD");
    trg->GetLock()->SetLocked(false);

    timer_thread.~thread();
}

void NoUnlock(int milliseconds, RE::TESObjectREFRPtr act) {
    ini.SetUnicode();
    ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    std::string cantBreak = " can't break lock";
    std::string noBreak = act->GetBaseObject()->GetName() + cantBreak;
    const char *noBreakMsg = noBreak.c_str();

    std::thread timer_thread(timer, milliseconds);
    timer_thread.join();

    if (ini.GetBoolValue("General", "Notifications", true)) {
        if (act->As<RE::Actor>()->IsPlayerRef()) {
            RE::DebugNotification("Can't break lock");
        } else if (act->As<RE::Actor>()->IsPlayerTeammate()) {
            RE::DebugNotification(noBreakMsg);
        }
    } 

    RE::PlaySound("DRSLockedSD");

    timer_thread.~thread();
}

void Unlocked(int milliseconds, RE::TESObjectREFRPtr act) {
    ini.SetUnicode();
    ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    std::string cantBreak = " can't break lock, already unlocked";
    std::string noBreak = act->GetBaseObject()->GetName() + cantBreak;
    const char *noBreakMsg = noBreak.c_str();

    std::thread timer_thread(timer, milliseconds);
    timer_thread.join();

    if (ini.GetBoolValue("General", "Notifications", true)) {
        if (act->As<RE::Actor>()->IsPlayerRef()) {
            RE::DebugNotification("Already unlocked");
        } else if (act->As<RE::Actor>()->IsPlayerTeammate()) {
            RE::DebugNotification(noBreakMsg);
        }
    } 

    // RE::PlaySound("DRSLockedSD");

    timer_thread.~thread();
}

void KeyUnlock(int milliseconds, RE::TESObjectREFRPtr act) {
    ini.SetUnicode();
    ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    std::string cantBreak = " can't break lock, requires a key";
    std::string noBreak = act->GetBaseObject()->GetName() + cantBreak;
    const char *noBreakMsg = noBreak.c_str();

    std::thread timer_thread(timer, milliseconds);
    timer_thread.join();

   if (ini.GetBoolValue("General", "Notifications", true)) {
        if (act->As<RE::Actor>()->IsPlayerRef()) {
            RE::DebugNotification("Needs a key to unlock");
        } else if (act->As<RE::Actor>()->IsPlayerTeammate()) {
            RE::DebugNotification(noBreakMsg);
        }
    } 

    RE::PlaySound("DRSLockedSD");

    timer_thread.~thread();
}

void SpecialNoUnlock(int milliseconds, RE::TESObjectREFRPtr act) {
    ini.SetUnicode();
    ini.LoadFile("Data\\SKSE\\Plugins\\LockBashSKSE.ini");

    std::string cantBreak = " can't break lock, lock won't budge";
    std::string noBreak = act->GetBaseObject()->GetName() + cantBreak;
    const char *noBreakMsg = noBreak.c_str();

    std::thread timer_thread(timer, milliseconds);
    timer_thread.join();

    if (ini.GetBoolValue("General", "Notifications", true)) {
        if (act->As<RE::Actor>()->IsPlayerRef()) {
            RE::DebugNotification("Lock won't budge");
        } else if (act->As<RE::Actor>()->IsPlayerTeammate()) {
            RE::DebugNotification(noBreakMsg);
        }
    } 

    RE::PlaySound("DRSLockedSD");

    timer_thread.~thread();
}

#include "Unlocker.h"

void CrimeCheck(RE::TESObjectREFR* center, float radius, RE::Actor* act) {

    if (ini.GetBoolValue("Gameplay", "Crime", true) == true) {
        /*if (act->GetActorRuntimeData().)*/

        int detCount = 0;
        RE::TES::GetSingleton()->ForEachReferenceInRange(center, radius, [act, center, &detCount](RE::TESObjectREFR& ref) {

            auto keywordNPC = RE::BGSKeyword::LookupByID<RE::BGSKeyword>(0x0013794);
            if (/*ref.As<RE::Actor>() != act && */!ref.IsPlayerRef() && ref.GetFormType() == RE::FormType::ActorCharacter && ref.HasKeyword(keywordNPC) && !ref.IsDead()) {

                auto refACT = ref.As<RE::Actor>();
                auto refFAC = refACT->GetCrimeFaction();
                auto refDetection = refACT->RequestDetectionLevel(act); // 0=Undetected 10=VeryLow 20=Low 30=Normal 40=High 50=Critical
                auto refMorality = refACT->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMorality); //0=AnyCrime 1=CrimeAgainstEnemy 2=ProperyCrime 3=NoCrime
                auto crimeItem = RE::TESForm::LookupByID(0x000000f);
                auto crimeGold = 50;
                if (refFAC != nullptr) {
                    crimeGold = refFAC->crimeData.crimevalues.pickpocketCrimeGold / refFAC->crimeData.crimevalues.stealCrimeGoldMult;
                }

                //Getting Owner
                auto owner = center->GetOwner();
                if (!owner) {
                    if (center->GetFormType() == RE::FormType::Door) {
                        if (auto Teleport = center->extraList.GetByType<RE::ExtraTeleport>()) {
                            if (auto TeleportData = Teleport->teleportData) {
                                if (auto DoorREF = TeleportData->linkedDoor.get()) {
                                    if (auto CellREF = DoorREF->GetParentCell()) {
                                        owner = CellREF->GetOwner();
                                    }
                                    else {
                                        if (refFAC != nullptr) {
                                            owner = refFAC;
                                        }
                                    }
                                }
                                else {
                                    if (refFAC != nullptr) {
                                        owner = refFAC;
                                    }
                                }
                            }
                            else {
                                if (refFAC != nullptr) {
                                    owner = refFAC;
                                }
                            }
                        }
                        else {
                            if (refFAC != nullptr) {
                                owner = refFAC;
                            }
                        }
                    }
                    else {
                        if (refFAC != nullptr) {
                            owner = refFAC;
                        }
                    }
                }


                // Detection Check
                if (refDetection > 0 && detCount == 0) {
                    if (owner != nullptr) {
                        if (refMorality == 3 || refMorality == 1) {

                            // Faction Check
                            if (owner == refFAC) {
                                if (!owner->As<RE::TESFaction>()->IgnoresStealing() || !owner->As<RE::TESFaction>()->IgnoresTrespass()) {
                                    if (refACT->IsGuard()) {
                                        if (ini.GetBoolValue("Misc", "Logs", false) == true)
                                        {
                                            logger::info("{} fac_report {}", refACT->GetName(), refDetection);
                                        }
                                        act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, true);
                                        //refACT->GetActorRuntimeData().currentProcess->SetActorsDetectionEvent(act, center->GetPosition(), 100, center);
                                        detCount++;
                                        return RE::BSContainer::ForEachResult::kStop;
                                    }
                                }
                            }
                            else if (owner != refFAC) {
                                if (refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                                    if (ini.GetBoolValue("Misc", "Logs", false) == true)
                                    {
                                        logger::info("{} owner_report {}", refACT->GetName(), refDetection);
                                    }
                                    act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, false);
                                    detCount++;
                                    return RE::BSContainer::ForEachResult::kStop;
                                }
                                else if (!refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                                    if (ini.GetBoolValue("Misc", "Logs", false) == true)
                                    {
                                        logger::info("{} non-owner_report {}", refACT->GetName(), refDetection);
                                    }
                                    act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, true);
                                    detCount++;
                                    return RE::BSContainer::ForEachResult::kStop;
                                }
                            }
                        }

                        else if (refMorality == 0 || refMorality == 2) {
                            // Faction Check
                            if (owner != refFAC) {
                                if (refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                                    if (ini.GetBoolValue("Misc", "Logs", false) == true)
                                    {
                                        logger::info("{} lowM_report {}", refACT->GetName(), refMorality);
                                    }
                                    act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, false);
                                    detCount++;
                                    return RE::BSContainer::ForEachResult::kStop;
                                }
                                else if (!refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                                    if (ini.GetBoolValue("Misc", "Logs", false) == true)
                                    {
                                        logger::info("{} lowM_no_report {}", refACT->GetName(), refMorality);
                                    }
                                    //detCount++;
                                    return RE::BSContainer::ForEachResult::kContinue;
                                }
                            }  // for lower moralities, use below line
                        }
                    }
                }
            }
            return RE::BSContainer::ForEachResult::kContinue;
            });
    }
}

void TryUnlock(int milliseconds, RE::TESObjectREFRPtr trg, RE::TESObjectREFRPtr act, int cond) {

    // 0 = Unlock 1 = noUnlock 2 = Unlocked 3 = keyLocked 4 = specLocked
    if (cond == 0) {
        std::string canBreak = " broke the lock";
        std::string Break = act->GetBaseObject()->GetName() + canBreak;
        const char* BreakMsg = Break.c_str();

        std::thread timer_thread(timer, milliseconds);
        timer_thread.join();

        if (ini.GetBoolValue("Misc", "Notifications", false) == true) {
            if (act->As<RE::Actor>()->IsPlayerRef()) {RE::DebugNotification("Lock broken");}
            else if (act->As<RE::Actor>()->IsPlayerTeammate()) {RE::DebugNotification(BreakMsg);}
        }

        RE::PlaySound("TRPTripwireSD");
        trg->GetLock()->SetLocked(false);
        timer_thread.~thread();
    }

    else if (cond == 1) {
        std::string cantBreak = " can't break lock";
        std::string noBreak = act->GetBaseObject()->GetName() + cantBreak;
        const char* noBreakMsg = noBreak.c_str();

        std::thread timer_thread(timer, milliseconds);
        timer_thread.join();

        if (ini.GetBoolValue("Misc", "Notifications", false) == true) {
            if (act->As<RE::Actor>()->IsPlayerRef()) {RE::DebugNotification("Can't break lock");}
            else if (act->As<RE::Actor>()->IsPlayerTeammate()) {RE::DebugNotification(noBreakMsg);}
        }

        RE::PlaySound("DRSLockedSD");
        timer_thread.~thread();
    }

    else if (cond == 2) {
        std::string cantBreak = " can't break lock, already unlocked";
        std::string noBreak = act->GetBaseObject()->GetName() + cantBreak;
        const char* noBreakMsg = noBreak.c_str();

        std::thread timer_thread(timer, milliseconds);
        timer_thread.join();

        if (ini.GetBoolValue("Misc", "Notifications", false) == true) {
            if (act->As<RE::Actor>()->IsPlayerRef()) {RE::DebugNotification("Already unlocked");}
            else if (act->As<RE::Actor>()->IsPlayerTeammate()) {RE::DebugNotification(noBreakMsg);}
        }

        // RE::PlaySound("DRSLockedSD");
        timer_thread.~thread();
    }

    else if (cond == 3) {
        std::string cantBreak = " can't break lock, requires a key";
        std::string noBreak = act->GetBaseObject()->GetName() + cantBreak;
        const char* noBreakMsg = noBreak.c_str();

        std::thread timer_thread(timer, milliseconds);
        timer_thread.join();

        if (ini.GetBoolValue("Misc", "Notifications", false) == true) {
            if (act->As<RE::Actor>()->IsPlayerRef()) {RE::DebugNotification("Needs a key to unlock");}
            else if (act->As<RE::Actor>()->IsPlayerTeammate()) {RE::DebugNotification(noBreakMsg);}
        }

        RE::PlaySound("DRSLockedSD");
        timer_thread.~thread();
    }
    else if (cond == 4) {
        std::string cantBreak = " can't break lock, lock won't budge";
        std::string noBreak = act->GetBaseObject()->GetName() + cantBreak;
        const char* noBreakMsg = noBreak.c_str();

        std::thread timer_thread(timer, milliseconds);
        timer_thread.join();

        if (ini.GetBoolValue("Misc", "Notifications", false) == true) {
            if (act->As<RE::Actor>()->IsPlayerRef()) {RE::DebugNotification("Lock won't budge");}
            else if (act->As<RE::Actor>()->IsPlayerTeammate()) {RE::DebugNotification(noBreakMsg);}
        }

        RE::PlaySound("DRSLockedSD");
        timer_thread.~thread();
    }
    RE::PlayerCharacter::GetSingleton()->UpdateCrosshairs();

}

void LockCheck(RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr, float radius) {

    float p_kVEasy = 200;
    float p_kEasy = 300;
    float p_kAverage = 400;
    float p_kHard = 500;
    float p_kVHard = 600;

    if (ini.GetLongValue("Gameplay", "Difficulty", 1) == 1) {
        p_kVEasy = 350;
        p_kEasy = 450;
        p_kAverage = 550;
        p_kHard = 750;
        p_kVHard = 800;
    }
    else if (ini.GetLongValue("Gameplay", "Difficulty", 1) == 2) {
        p_kVEasy = ini.GetLongValue("Custom", "Very Easy");
        p_kEasy = ini.GetLongValue("Custom", "Easy");
        p_kAverage = ini.GetLongValue("Custom", "Average");
        p_kHard = ini.GetLongValue("Custom", "Hard");
        p_kVHard = ini.GetLongValue("Custom", "Very Hard");
    }

    switch (refPtr->GetLockLevel()) {
    case RE::LOCK_LEVEL::kVeryEasy:
        if (Power >= p_kVEasy) {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 0);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        else {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 1);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        break;
    case RE::LOCK_LEVEL::kEasy:
        if (Power >= p_kEasy) {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 0);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        else {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 1);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        break;
    case RE::LOCK_LEVEL::kAverage:
        if (Power >= p_kAverage) {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 0);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        else {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 1);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        break;
    case RE::LOCK_LEVEL::kHard:
        if (Power >= p_kHard) {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 0);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        else {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 1);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        break;
    case RE::LOCK_LEVEL::kVeryHard:
        if (Power >= p_kVHard) {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 0);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        else {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 1);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        break;
    case RE::LOCK_LEVEL::kUnlocked:
        if (Power >= 0 || Power <= 0) {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 2);
            unlock_thread.detach();
            unlock_thread.~thread();
            if (ini.GetBoolValue("Gameplay", "CrimeEvenIfUnlocked", false) == true) {
                CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
            }
        }
        break;
    case RE::LOCK_LEVEL::kRequiresKey:
        if (Power >= 0 || Power <= 0) {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 3);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        break;
    default:
        if (Power >= 0 || Power <= 0) {
            std::thread unlock_thread(TryUnlock, 500, refPtr, actPtr, 4);
            unlock_thread.detach();
            unlock_thread.~thread();
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
        }
        break;
    }
}
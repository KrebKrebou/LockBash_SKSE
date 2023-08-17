#include "Unlocker.h"

namespace LB {

    void Unlocker::LockCheck(RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr, float radius, double power) {
        const char* soundBreak = "";
        std::string canBreak = "";
        std::string actorName = "";
        std::string combinedMsg = "";
        const char* breakMsg = "";

        long p_kVEasy = 200;
        long p_kEasy = 300;
        long p_kAverage = 400;
        long p_kHard = 500;
        long p_kVHard = 600;

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
            if (power >= p_kVEasy) {
                canBreak = " broke the lock.";
                soundBreak = "TRPTripwireSD";
                refPtr->GetLock()->SetLocked(false);
            }
            else {
                canBreak = " can't break the lock.";
                soundBreak = "DRSLockedSD";
            }
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
            break;
        case RE::LOCK_LEVEL::kEasy:
            if (power >= p_kEasy) {
                canBreak = " broke the lock.";
                soundBreak = "TRPTripwireSD";
                refPtr->GetLock()->SetLocked(false);
            }
            else {
                canBreak = " can't break the lock.";
                soundBreak = "DRSLockedSD";
            }
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
            break;
        case RE::LOCK_LEVEL::kAverage:
            if (power >= p_kAverage) {
                canBreak = " broke the lock.";
                soundBreak = "TRPTripwireSD";
                refPtr->GetLock()->SetLocked(false);
            }
            else {
                canBreak = " can't break the lock.";
                soundBreak = "DRSLockedSD";
            }
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
            break;
        case RE::LOCK_LEVEL::kHard:
            if (power >= p_kHard) {
                canBreak = " broke the lock.";
                soundBreak = "TRPTripwireSD";
                refPtr->GetLock()->SetLocked(false);
            }
            else {
                canBreak = " can't break the lock.";
                soundBreak = "DRSLockedSD";
            }
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
            break;
        case RE::LOCK_LEVEL::kVeryHard:
            if (power >= p_kVHard) {
                canBreak = " broke the lock.";
                soundBreak = "TRPTripwireSD";
                refPtr->GetLock()->SetLocked(false);
            }
            else {
                canBreak = " can't break the lock.";
                soundBreak = "DRSLockedSD";
            }
            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
            break;
        case RE::LOCK_LEVEL::kUnlocked:
            canBreak = " can't break the lock, already unlocked.";
            soundBreak = "";

            if (ini.GetBoolValue("Gameplay", "CrimeEvenIfUnlocked", false) == true) {
                CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
            }
            break;
        case RE::LOCK_LEVEL::kRequiresKey:
            canBreak = " can't break the lock, needs a key.";
            soundBreak = "DRSLockedSD";

            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
            break;
        default:
            canBreak = " can't break the lock, won't budge.";
            soundBreak = "DRSLockedSD";

            CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
            break;
        }
        RE::PlaySound(soundBreak);
        RE::PlayerCharacter::GetSingleton()->UpdateCrosshairs();

        if (ini.GetBoolValue("Misc", "Notifications", false) == true) {
            if (actPtr->As<RE::Actor>()->IsPlayerRef()) {
                actorName = "I";
            }
            else if (actPtr->As<RE::Actor>()->IsPlayerTeammate()) {
                actorName = actPtr->GetBaseObject()->GetName();
            }
            combinedMsg = actorName + canBreak;
            breakMsg = combinedMsg.c_str();
            RE::DebugNotification(breakMsg);
        }
    }

    void Unlocker::CrimeCheck(RE::TESObjectREFR* center, float radius, RE::Actor* act) {
        if (ini.GetBoolValue("Gameplay", "Crime", true) == true) {
            if (Wait::Active) {
                if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Crime Cooldown Active"); }
                return;
            }
            else if (!Wait::Active) {
                std::thread cooldown_thread(&Wait::Cooldown, 5000);
                cooldown_thread.detach();

                int detCount = 0;
                RE::TES::GetSingleton()->ForEachReferenceInRange(center, radius, [act, center, &detCount](RE::TESObjectREFR& ref) {
                    auto keywordNPC = RE::BGSKeyword::LookupByID<RE::BGSKeyword>(0x0013794);
                    if (!ref.IsPlayerRef() && ref.GetFormType() == RE::FormType::ActorCharacter && ref.HasKeyword(keywordNPC)) {
                        if (ref.As<RE::Actor>() != act && !ref.IsDead()) {
                            auto refACT = ref.As<RE::Actor>();
                            auto refFAC = refACT->GetCrimeFaction();
                            auto refDetection = refACT->RequestDetectionLevel(act); // 0=Undetected 10=VeryLow 20=Low 30=Normal 40=High 50=Critical
                            auto refMorality = refACT->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMorality); //0=AnyCrime 1=CrimeAgainstEnemy 2=ProperyCrime 3=NoCrime
                            auto crimeItem = RE::TESForm::LookupByID(0x000000f);
                            int32_t crimeGold = 50;
                            if (refFAC) {
                                crimeGold = refFAC->crimeData.crimevalues.pickpocketCrimeGold * 2;
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
                                                    if (refFAC) {
                                                        owner = refFAC;
                                                    }
                                                }
                                            }
                                            else {
                                                if (refFAC) {
                                                    owner = refFAC;
                                                }
                                            }
                                        }
                                        else {
                                            if (refFAC) {
                                                owner = refFAC;
                                            }
                                        }
                                    }
                                    else {
                                        if (refFAC) {
                                            owner = refFAC;
                                        }
                                    }
                                }
                                else {
                                    if (refFAC) {
                                        owner = refFAC;
                                    }
                                }
                            }

                            // Detection Check
                            if (owner) {
                                if (refDetection > 0 && detCount == 0) {
                                    if (refMorality == 3 || refMorality == 1) {
                                        // Faction Check
                                        if (owner == refFAC) {
                                            if (!owner->As<RE::TESFaction>()->IgnoresStealing() || !owner->As<RE::TESFaction>()->IgnoresTrespass()) {
                                                if (refACT->IsGuard()) {
                                                    if (ini.GetBoolValue("Misc", "Logs", false) == true)
                                                    {
                                                        logger::info("Report: {} [{}]", refACT->GetName(), refDetection);
                                                    }
                                                    act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, true);
                                                    detCount++;
                                                    return RE::BSContainer::ForEachResult::kStop;
                                                }
                                            }
                                        }
                                        else if (owner != refFAC) {
                                            if (refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                                                if (ini.GetBoolValue("Misc", "Logs", false) == true)
                                                {
                                                    logger::info("Report: {} [{}]", refACT->GetName(), refDetection);
                                                }
                                                act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, false);
                                                detCount++;
                                                return RE::BSContainer::ForEachResult::kStop;
                                            }
                                            else if (!refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                                                if (ini.GetBoolValue("Misc", "Logs", false) == true)
                                                {
                                                    logger::info("Report: {} [{}]", refACT->GetName(), refDetection);
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
                                                    logger::info("Report: {} [{}]", refACT->GetName(), refDetection);
                                                }
                                                act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, false);
                                                detCount++;
                                                return RE::BSContainer::ForEachResult::kStop;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    return RE::BSContainer::ForEachResult::kContinue;
                    });
            }
        }
    }
}




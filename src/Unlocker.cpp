#include "Unlocker.h"

namespace LB {

    void Unlocker::LockCheck(RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr, float radius, double power) {
        std::thread t0(&Wait::Timer, 250);
        t0.join();

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
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            else {
                canBreak = " can't break the lock.";
                soundBreak = "DRSLockedSD";
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            //CrimeCheck(refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
            break;
        case RE::LOCK_LEVEL::kEasy:
            if (power >= p_kEasy) {
                canBreak = " broke the lock.";
                soundBreak = "TRPTripwireSD";
                refPtr->GetLock()->SetLocked(false);
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            else {
                canBreak = " can't break the lock.";
                soundBreak = "DRSLockedSD";
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            break;
        case RE::LOCK_LEVEL::kAverage:
            if (power >= p_kAverage) {
                canBreak = " broke the lock.";
                soundBreak = "TRPTripwireSD";
                refPtr->GetLock()->SetLocked(false);
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            else {
                canBreak = " can't break the lock.";
                soundBreak = "DRSLockedSD";
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            break;
        case RE::LOCK_LEVEL::kHard:
            if (power >= p_kHard) {
                canBreak = " broke the lock.";
                soundBreak = "TRPTripwireSD";
                refPtr->GetLock()->SetLocked(false);
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            else {
                canBreak = " can't break the lock.";
                soundBreak = "DRSLockedSD";
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            break;
        case RE::LOCK_LEVEL::kVeryHard:
            if (power >= p_kVHard) {
                canBreak = " broke the lock.";
                soundBreak = "TRPTripwireSD";
                refPtr->GetLock()->SetLocked(false);
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            else {
                canBreak = " can't break the lock.";
                soundBreak = "DRSLockedSD";
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            break;
        case RE::LOCK_LEVEL::kUnlocked:
            if (power >= 0 && power <= 0) {
                canBreak = " can't break the lock, already unlocked.";
                soundBreak = "";
                if (ini.GetBoolValue("Gameplay", "CrimeEvenIfUnlocked", false) == true) {
                    std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                    t1.detach();
                }
            }
            break;
        case RE::LOCK_LEVEL::kRequiresKey:
            if (power >= 0 && power <= 0) {
                canBreak = " can't break the lock, needs a key.";
                soundBreak = "DRSLockedSD";
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
            break;
        default:
            if (power >= 0 && power <= 0) {
                canBreak = " can't break the lock, won't budge.";
                soundBreak = "DRSLockedSD";
                std::thread t1(&Unlocker::CrimeCheck, refPtr->AsReference(), radius, actPtr->As<RE::Actor>());
                t1.detach();
            }
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
                std::thread t1(&Wait::Timer, 800);
                std::thread t2(&Wait::Cooldown, 5000);
                t1.join();
                t2.detach();

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
                                                    return RE::BSContainer::ForEachResult::kContinue;
                                                }
                                            }
                                        }
                                        else if (owner != refFAC) {
                                            if (refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                                                if (ini.GetBoolValue("Misc", "Logs", false) == true)
                                                {
                                                    logger::info("Report: {} [{}]", refACT->GetName(), refDetection);
                                                }
                                                act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, true);
                                                detCount++;
                                                return RE::BSContainer::ForEachResult::kContinue;
                                            }
                                            else if (!refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                                                if (ini.GetBoolValue("Misc", "Logs", false) == true)
                                                {
                                                    logger::info("Report: {} [{}]", refACT->GetName(), refDetection);
                                                }
                                                act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, true);
                                                detCount++;
                                                return RE::BSContainer::ForEachResult::kContinue;
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
                                                act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, true);
                                                detCount++;
                                                return RE::BSContainer::ForEachResult::kContinue;
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




#pragma once
#include "Unlocker.h"

void ActorRefCheck(RE::TESObjectREFR* center, float radius, RE::Actor* act) {
    int detCount = 0;
    RE::TES::GetSingleton()->ForEachReferenceInRange(center, radius, [act, center, &detCount](RE::TESObjectREFR& ref) {

        auto keywordNPC = RE::BGSKeyword::LookupByID<RE::BGSKeyword>(0x0013794);
        if (!ref.IsPlayerRef() && ref.GetFormType() == RE::FormType::ActorCharacter && ref.HasKeyword(keywordNPC)) {
            auto plyFAC = RE::TESForm::LookupByID<RE::TESFaction>(0x0000DB1);
            auto refACT = ref.As<RE::Actor>();
            auto refFAC = refACT->GetCrimeFaction();
            auto refDetection = refACT->RequestDetectionLevel(act); // 0=Undetected 10=VeryLow 20=Low 30=Normal 40=High 50=Critical
            auto refMorality = refACT->AsActorValueOwner()->GetActorValue(RE::ActorValue::kMorality); //0=AnyCrime 1=CrimeAgainstEnemy 2=ProperyCrime 3=NoCrime
            auto crimeItem = RE::TESForm::LookupByID(0x000000f);
            auto crimeGold = 25;
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
                                } else {
                                    if (refFAC != nullptr) {
                                        owner = refFAC;
                                    }
                                }
                            } else {
                                if (refFAC != nullptr) {
                                    owner = refFAC;
                                }
                            }
                        } else {
                            if (refFAC != nullptr) {
                                owner = refFAC;
                            }
                        }
                    } else {
                        if (refFAC != nullptr) {
                            owner = refFAC;
                        }
                    }
                } else {
                    if (refFAC != nullptr) {
                        owner = refFAC;
                    }
                }
            }


// Detection Check
            if (refDetection > 0 && detCount == 0) {
                if (refMorality == 3 || refMorality == 1) {

                  // Faction Check
                    if (owner == refFAC) {

                        if (refACT->IsGuard()) {
                            logger::info("{} fac_report {}", refACT->GetName(), refDetection);
                            act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, true);
                            detCount++;
                            return RE::BSContainer::ForEachResult::kStop;
                        }
                    } else if (owner != refFAC) {

                        if (refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                            logger::info("{} owner_report {}", refACT->GetName(), refDetection);
                            act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, false);
                            detCount++;
                            return RE::BSContainer::ForEachResult::kStop;
                        } else if (!refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                            logger::info("{} non-owner_report {}", refACT->GetName(), refDetection);
                            act->StealAlarm(center, crimeItem, crimeGold * 2, 1, owner, true);
                            detCount++;
                            return RE::BSContainer::ForEachResult::kStop;
                        }
                    } 
                }  // for lower moralities paste here
            }
        }
        return RE::BSContainer::ForEachResult::kContinue;
    });
}


void RegisterForEvent_Hit() {
    struct EventSink : public RE::BSTEventSink<RE::TESHitEvent> {
        RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent *event, RE::BSTEventSource<RE::TESHitEvent> *source) {
            // cause
            auto actorREFptr = event->cause;
            auto actorREF = actorREFptr->AsReference();
            auto actorName = actorREFptr->GetBaseObject()->GetName();
            auto actorACT = actorREFptr->As<RE::Actor>();

            auto Health = actorACT->AsActorValueOwner()->GetBaseActorValue(RE::ActorValue::kHealth);
            auto Stamina = actorACT->AsActorValueOwner()->GetBaseActorValue(RE::ActorValue::kHealth);
            auto Power = Health + Stamina;

            auto weapFormType = RE::TESForm::LookupByID(event->source)->GetFormType();
            auto weap = RE::TESForm::LookupByID(event->source);
          
            // target
            auto targetREFptr = event->target;
            auto targetREF = targetREFptr->AsReference();
            auto targetName = targetREFptr->GetBaseObject()->GetName();
            auto targetFormType = targetREFptr->GetBaseObject()->GetFormType();

            // Check for form type
            if (targetFormType == RE::FormType::Door || targetFormType == RE::FormType::Container) {

                // Check for player
                if (weapFormType == RE::FormType::Weapon) {
                    
                    switch (targetREFptr->GetLockLevel()) {
                        case RE::LOCK_LEVEL::kVeryEasy:
                            if (Power >= 100) {
                                std::thread unlock_thread(Unlock, 500, targetREFptr, actorREFptr);
                                unlock_thread.detach();
                                unlock_thread.~thread();
                                ActorRefCheck(targetREF, 2000, actorACT);
                            } else {
                                std::thread nounlock_thread(NoUnlock, 500, actorREFptr);
                                nounlock_thread.detach();
                                nounlock_thread.~thread();
                                ActorRefCheck(targetREF, 2000, actorACT);
                            }
                            break;
                        case RE::LOCK_LEVEL::kEasy:
                            if (Power >= 200) {
                                std::thread unlock_thread(Unlock, 500, targetREFptr, actorREFptr);
                                unlock_thread.detach();
                                unlock_thread.~thread();
                                ActorRefCheck(targetREF, 2000, actorACT);
                            } else {
                                std::thread nounlock_thread(NoUnlock, 500, actorREFptr);
                                nounlock_thread.detach();
                                nounlock_thread.~thread();
                                ActorRefCheck(targetREF, 2000, actorACT);
                            }
                            break;
                        case RE::LOCK_LEVEL::kAverage:
                            if (Power >= 300) {
                                std::thread unlock_thread(Unlock, 500, targetREFptr, actorREFptr);
                                unlock_thread.detach();
                                unlock_thread.~thread();
                                ActorRefCheck(targetREF, 2000, actorACT);
                            } else {
                                std::thread nounlock_thread(NoUnlock, 500, actorREFptr);
                                nounlock_thread.detach();
                                nounlock_thread.~thread();
                                ActorRefCheck(targetREF, 2000, actorACT);
                            }
                            break;
                        case RE::LOCK_LEVEL::kHard:
                            if (Power >= 400) {
                                std::thread unlock_thread(Unlock, 500, targetREFptr, actorREFptr);
                                unlock_thread.detach();
                                unlock_thread.~thread();
                                ActorRefCheck(targetREF, 2000, actorACT);
                            } else {
                                std::thread nounlock_thread(NoUnlock, 500, actorREFptr);
                                nounlock_thread.detach();
                                nounlock_thread.~thread();
                                ActorRefCheck(targetREF, 2000, actorACT);
                            }
                            break;
                        case RE::LOCK_LEVEL::kVeryHard:
                            if (Power >= 500) {
                                std::thread unlock_thread(Unlock, 500, targetREFptr, actorREFptr);
                                unlock_thread.detach();
                                unlock_thread.~thread();
                                ActorRefCheck(targetREF, 2000, actorACT);
                            } else {
                                std::thread nounlock_thread(NoUnlock, 500, actorREFptr);
                                nounlock_thread.detach();
                                nounlock_thread.~thread();
                                ActorRefCheck(targetREF, 2000, actorACT);
                            }
                            break;
                        case RE::LOCK_LEVEL::kRequiresKey:
                            if (Power >= 0 || Power <= 0) {
                                std::thread keyunlock_thread(KeyUnlock, 500, actorREFptr);
                                keyunlock_thread.detach();
                                keyunlock_thread.~thread();
                                ActorRefCheck(targetREF, 500, actorACT);
                            }
                            break;
                        case RE::LOCK_LEVEL::kUnlocked:
                            if (Power >= 0 || Power <= 0) {
                                std::thread unlocked_thread(Unlocked, 500, actorREFptr);
                                unlocked_thread.detach();
                                unlocked_thread.~thread();
                                ActorRefCheck(targetREF, 500, actorACT);
                            }
                            break;
                        default:
                            if (Power >= 0 || Power <= 0) {
                                std::thread default_thread(SpecialNoUnlock, 500, actorREFptr);
                                default_thread.detach();
                                default_thread.~thread();
                                ActorRefCheck(targetREF, 500, actorACT);
                            }
                            break;
                    }
                }
            }
            return RE::BSEventNotifyControl::kContinue;
        }
    };

    RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(new EventSink);
    delete new EventSink;
}

/*else if ((refMorality == 0 || refMorality == 2)) {
                    if (owner == refFAC) {
                        logger::info("{} !fac_report {}", refACT->GetName(), refDetection);
                        return RE::BSContainer::ForEachResult::kStop;
                    } else if (owner != refFAC) {
                        if (refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                            logger::info("{} !non-owner_report {}", refACT->GetName(), refDetection);
                            return RE::BSContainer::ForEachResult::kStop;
                        } else if (!refACT->IsInFaction(owner->As<RE::TESFaction>())) {
                            logger::info("{} !owner_report {}", refACT->GetName(), refDetection);
                            return RE::BSContainer::ForEachResult::kStop;
                        }
                    }
                }*/
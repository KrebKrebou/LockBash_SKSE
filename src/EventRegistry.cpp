#include "EventRegistry.h"

void RegisterForEvent_Hit() {
        struct EventSink : public RE::BSTEventSink<RE::TESHitEvent> {
            RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* event, RE::BSTEventSource<RE::TESHitEvent>* /*source*/) {
                if (event->cause) {
                    if (event->cause->Is(RE::FormType::ActorCharacter)) {
                        if (event->source) {
                            if (event->target) {
                                if (event->target->GetBaseObject()->Is(RE::FormType::Door) || event->target->GetBaseObject()->Is(RE::FormType::Container)) {

                                    auto targetREFptr = event->target;
                                    auto actorREFptr = event->cause;
                                    auto actorACT = actorREFptr->As<RE::Actor>();
                                    auto health = actorACT->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
                                    auto stamina = actorACT->AsActorValueOwner()->GetBaseActorValue(RE::ActorValue::kStamina);
                                    auto power = (1.5 * health) + stamina;
                                    auto weap = RE::TESForm::LookupByID(event->source);

                                    /*Check for weapon*/
                                    if (!event->projectile) {
                                        if (weap->GetFormType() == RE::FormType::Weapon) {
                                            auto attackData = actorACT->GetActorRuntimeData().currentProcess->high->attackData;
                                            if (!attackData) {
                                                return RE::BSEventNotifyControl::kContinue;
                                            }
                                            auto weapWEAP = weap->As<RE::TESObjectWEAP>();
                                            auto weapSkill = weapWEAP->weaponData.skill;

                                            auto kBashAttack = RE::AttackData::AttackFlag::kBashAttack;
                                            auto kChargeAttack = RE::AttackData::AttackFlag::kChargeAttack;
                                            auto kContinuousAttack = RE::AttackData::AttackFlag::kContinuousAttack;
                                            auto kIgnoreWeapon = RE::AttackData::AttackFlag::kIgnoreWeapon;
                                            auto kNone = RE::AttackData::AttackFlag::kNone;
                                            auto kOverrideData = RE::AttackData::AttackFlag::kOverrideData;
                                            auto kPowerAttack = RE::AttackData::AttackFlag::kPowerAttack;
                                            auto kRotatingAttack = RE::AttackData::AttackFlag::kRotatingAttack;

                                            if (attackData->data.flags.none(kBashAttack, kChargeAttack, kContinuousAttack, kIgnoreWeapon, kNone, kOverrideData, kPowerAttack, kRotatingAttack)) {
                                                power = power * 1;
                                                if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 1) {
                                                    power = HybridDetection(weapSkill, weapWEAP, power);
                                                }
                                                else if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 2) {
                                                    power = SkillDetection(weapSkill, power);
                                                }
                                                if (ini.GetBoolValue("Misc", "Logs", false) == true) {
                                                    logger::info("Attack: Normal");
                                                    logger::info("Power: {}", power);
                                                    logger::info("Target: {}", event->target->GetName());
                                                }
                                                LockCheck(targetREFptr, actorREFptr, 1500, power);
                                            }
                                            else if (attackData->data.flags.any(kBashAttack)) {
                                                power = power * 1.1;
                                                if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 1) {
                                                    power = HybridDetection(weapSkill, weapWEAP, power);
                                                }
                                                else if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 2) {
                                                    power = SkillDetection(weapSkill, power);
                                                }
                                                if (ini.GetBoolValue("Misc", "Logs", false) == true) {
                                                    logger::info("Attack: Bash");
                                                    logger::info("Power: {}", power);
                                                    logger::info("Target: {}", event->target->GetName());
                                                }
                                                LockCheck(targetREFptr, actorREFptr, 1700, power);
                                            }
                                            else if (attackData->data.flags.any(kPowerAttack)) {
                                                power = power * 1.2;
                                                if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 1) {
                                                    power = HybridDetection(weapSkill, weapWEAP, power);
                                                }
                                                else if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 2) {
                                                    power = SkillDetection(weapSkill, power);
                                                }
                                                if (ini.GetBoolValue("Misc", "Logs", false) == true) {
                                                    logger::info("Attack: Power Attack");
                                                    logger::info("Power: {}", power);
                                                    logger::info("Target: {}", event->target->GetName());
                                                }
                                                LockCheck(targetREFptr, actorREFptr, 2000, power);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                return RE::BSEventNotifyControl::kContinue;
            }
        };

        RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(new EventSink);
        delete new EventSink;
    }


//auto weapType = weapWEAP->GetWeaponType();
//auto targetREF = targetREFptr->AsReference();
//auto targetName = targetREFptr->GetBaseObject()->GetName();
//auto targetFormType = targetREFptr->GetBaseObject()->GetFormType();
//auto actorREF = actorREFptr->AsReference();
//auto actorName = actorREFptr->GetBaseObject()->GetName();
//auto weapWEAP = weap->As<RE::TESObjectWEAP>();
//auto weapState = actorACT->AsActorState()->GetWeaponState();
//auto attackState = actorACT->AsActorState()->GetAttackState();
//auto proj = RE::TESForm::LookupByID(event->projectile);


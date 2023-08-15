#include "EventRegistry.h"

void RegisterForEvent_Hit() {
    struct EventSink : public RE::BSTEventSink<RE::TESHitEvent> {
        RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent* event, RE::BSTEventSource<RE::TESHitEvent>* /*source*/) {

            if (event->cause.get() != nullptr) {
                if (event->cause.get()->Is(RE::FormType::ActorCharacter)) {
                    if (event->target->GetBaseObject()->Is(RE::FormType::Door) || event->target->GetBaseObject()->Is(RE::FormType::Container)) {

                        /*target*/
                        auto targetREFptr = event->target;
                        /*actor*/
                        auto actorREFptr = event->cause;
                        auto actorACT = actorREFptr->As<RE::Actor>();
                        /*actorvalues*/
                        auto Health = actorACT->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
                        auto Stamina = actorACT->AsActorValueOwner()->GetBaseActorValue(RE::ActorValue::kStamina);
                        Power = (1.5 * Health) + Stamina;

                        /*weapons*/
                        auto weap = RE::TESForm::LookupByID(event->source);

                        /*Check for weapon*/
                        if (!event->projectile) {
                            if (weap->GetFormType() == RE::FormType::Weapon) {

                                auto attackData = actorACT->GetActorRuntimeData().currentProcess->high->attackData;
                                if (!attackData) {
                                    return RE::BSEventNotifyControl::kContinue;
                                }
                                auto weapWEAP = weap->As<RE::TESObjectWEAP>();
                                auto weapType = weapWEAP->GetWeaponType();
                                auto weapSkill = weapWEAP->weaponData.skill;

                                /*attacktypes*/
                                auto kBashAttack = RE::AttackData::AttackFlag::kBashAttack;
                                auto kChargeAttack = RE::AttackData::AttackFlag::kChargeAttack;
                                auto kContinuousAttack = RE::AttackData::AttackFlag::kContinuousAttack;
                                auto kIgnoreWeapon = RE::AttackData::AttackFlag::kIgnoreWeapon;
                                auto kNone = RE::AttackData::AttackFlag::kNone;
                                auto kOverrideData = RE::AttackData::AttackFlag::kOverrideData;
                                auto kPowerAttack = RE::AttackData::AttackFlag::kPowerAttack;
                                auto kRotatingAttack = RE::AttackData::AttackFlag::kRotatingAttack;

                                if (attackData->data.flags.none(kBashAttack, kChargeAttack, kContinuousAttack, kIgnoreWeapon, kNone, kOverrideData, kPowerAttack, kRotatingAttack)) {
                                   if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Attack Type: kNormalAttack"); }
                                         Power = Power * 1;
                                         if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 1) {
                                             HybridDetection(weapSkill, weapWEAP, weapType);
                                         } else if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 2) {
                                             SkillDetection(weapSkill);
                                         }
                                     LockCheck(targetREFptr, actorREFptr, 1000);
                                }
                                else if (attackData->data.flags.any(kBashAttack)) {
                                   if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Attack Type: kBashAttack"); }
                                         Power = Power * 1.1;
                                         if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 1) {
                                             HybridDetection(weapSkill, weapWEAP, weapType);
                                         } else if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 2) {
                                             SkillDetection(weapSkill);
                                         }
                                     LockCheck(targetREFptr, actorREFptr, 1500);
                                }
                                else if (attackData->data.flags.any(kPowerAttack)) {
                                   if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Attack Type: kPowerAttack"); }
                                         Power = Power * 1.2;
                                         if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 1) {
                                             HybridDetection(weapSkill, weapWEAP, weapType);
                                         } else if (ini.GetLongValue("Gameplay", "Weapon Detection", 1) == 2) {
                                             SkillDetection(weapSkill);
                                         }
                                     LockCheck(targetREFptr, actorREFptr, 1750);
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

//auto targetREF = targetREFptr->AsReference();
//auto targetName = targetREFptr->GetBaseObject()->GetName();
//auto targetFormType = targetREFptr->GetBaseObject()->GetFormType();
//auto actorREF = actorREFptr->AsReference();
//auto actorName = actorREFptr->GetBaseObject()->GetName();
//auto weapWEAP = weap->As<RE::TESObjectWEAP>();
//auto weapState = actorACT->AsActorState()->GetWeaponState();
//auto attackState = actorACT->AsActorState()->GetAttackState();
//auto proj = RE::TESForm::LookupByID(event->projectile);


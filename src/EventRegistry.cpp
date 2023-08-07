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
                        auto Stamina = actorACT->AsActorValueOwner()->GetBaseActorValue(RE::ActorValue::kHealth);
                        auto Power = Health + Stamina;
                        if (ini.GetLongValue("Gameplay", "Formula", 1) == 1) {Power = 2 * Health + Stamina;}
                        /*weapons*/
                        auto weap = RE::TESForm::LookupByID(event->source);
                        auto weapFormType = weap->GetFormType();
                        auto proj = event->projectile;
                        /*attacktypes*/
                        auto kBashAttack = RE::AttackData::AttackFlag::kBashAttack;
                        auto kChargeAttack = RE::AttackData::AttackFlag::kChargeAttack;
                        auto kContinuousAttack = RE::AttackData::AttackFlag::kContinuousAttack;
                        auto kIgnoreWeapon = RE::AttackData::AttackFlag::kIgnoreWeapon;
                        auto kNone = RE::AttackData::AttackFlag::kNone;
                        auto kOverrideData = RE::AttackData::AttackFlag::kOverrideData;
                        auto kPowerAttack = RE::AttackData::AttackFlag::kPowerAttack;
                        auto kRotatingAttack = RE::AttackData::AttackFlag::kRotatingAttack;

                        /*Check for weapon*/
                        if (weapFormType == RE::FormType::Weapon && !proj) {
                            auto attackData = actorACT->GetActorRuntimeData().currentProcess->high->attackData->data.flags;
                            if (attackData.none(kBashAttack, kChargeAttack, kContinuousAttack, kIgnoreWeapon, kNone, kOverrideData, kPowerAttack, kRotatingAttack)) {
                                if (ini.GetBoolValue("Misc", "Logs", false) == true) {
                                    logger::info("kNormalAttack no bonus to Power");
                                }
                                LockCheck(targetREFptr, actorREFptr, Power, 1200);
                            }
                            else if (attackData.any(kBashAttack)) {
                                if (ini.GetBoolValue("Misc", "Logs", false) == true) {
                                    logger::info("kBashAttack +50 bonus to Power");
                                }
                                Power = Power + 50;
                                LockCheck(targetREFptr, actorREFptr, Power, 1600);
                            }
                            else if (attackData.any(kPowerAttack)) {
                                if (ini.GetBoolValue("Misc", "Logs", false) == true) {
                                    logger::info("kPowerAttack +100 bonus to Power");
                                }
                                Power = Power + 100;
                                LockCheck(targetREFptr, actorREFptr, Power, 2000);
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


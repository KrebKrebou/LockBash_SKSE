#pragma once
#include "Unlocker.h"

void RegisterForEvent_Hit() {
    struct EventSink : public RE::BSTEventSink<RE::TESHitEvent> {
        RE::BSEventNotifyControl ProcessEvent(const RE::TESHitEvent *event, RE::BSTEventSource<RE::TESHitEvent> *source) {
            // actor
            auto actorREFptr = event->cause;
            auto actorREF = actorREFptr->AsReference();
            auto actorName = actorREFptr->GetBaseObject()->GetName();
            auto actorACT = actorREFptr->As<RE::Actor>();

            auto Health = actorACT->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
            auto Stamina = actorACT->AsActorValueOwner()->GetActorValue(RE::ActorValue::kHealth);
            auto Power = Health + Stamina;

            if (ini.GetLongValue("Gameplay", "Formula", 1)) {
                Power = 2 * Health + Stamina;
            }

            auto weap = RE::TESForm::LookupByID(event->source);
            auto weapWEAP = weap->As<RE::TESObjectWEAP>();
            auto weapFormType = weap->GetFormType();
            auto weapState = actorACT->AsActorState()->GetWeaponState();
            auto attackState = actorACT->AsActorState()->GetAttackState();
            auto proj = event->projectile;

            auto kPowerAttack = RE::AttackData::AttackFlag::kPowerAttack;
            auto kBashAttack = RE::AttackData::AttackFlag::kBashAttack;
            auto kChargeAttack = RE::AttackData::AttackFlag::kChargeAttack;

            // target
            auto targetREFptr = event->target;
            auto targetREF = targetREFptr->AsReference();
            auto targetName = targetREFptr->GetBaseObject()->GetName();
            auto targetFormType = targetREFptr->GetBaseObject()->GetFormType();


            // Check for form type
            if (targetFormType == RE::FormType::Door || targetFormType == RE::FormType::Container) {
                // Check for weapon
                if (weapFormType == RE::FormType::Weapon && !proj) {
                    auto attackData = actorACT->GetActorRuntimeData().currentProcess->high->attackData->data.flags;
                    if (!attackData.any(kBashAttack, kPowerAttack, kChargeAttack)) {
                        logger::info("kNormalAttack");
                        LockCheck(targetREFptr, actorREFptr, Power, 1200);
                    }
                    else if (attackData.any(kBashAttack)) {
                        logger::info("kBashAttack");
                        Power = Power + 50;
                        LockCheck(targetREFptr, actorREFptr, Power, 1600);
                    }
                    else if (attackData.any(kPowerAttack)) {
                        logger::info("kPowerAttack");
                        Power = Power + 100;
                        LockCheck(targetREFptr, actorREFptr, Power, 2000);
                    }
                    else if (attackData.any(kChargeAttack)) {
                        logger::info("kChargeAttack");
                        Power = Power + 150;
                        LockCheck(targetREFptr, actorREFptr, Power, 2400);
                    }
                }
            }
            return RE::BSEventNotifyControl::kContinue;
        }
    };

    RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(new EventSink);
    delete new EventSink;
}
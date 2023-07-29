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

            auto Health = actorACT->AsActorValueOwner()->GetBaseActorValue(RE::ActorValue::kHealth);
            auto Stamina = actorACT->AsActorValueOwner()->GetBaseActorValue(RE::ActorValue::kHealth);
            auto Power = Health + Stamina;

            auto weap = RE::TESForm::LookupByID(event->source);
            auto weapWEAP = weap->As<RE::TESObjectWEAP>();
            auto weapFormType = weap->GetFormType();
            auto weapState = actorACT->AsActorState()->GetWeaponState();
            auto attackState = actorACT->AsActorState()->GetAttackState();
            auto proj = event->projectile;

            auto kPowerAttack = RE::AttackData::AttackFlag::kPowerAttack;
            auto kBashAttack = RE::AttackData::AttackFlag::kBashAttack;

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
                    if (attackData.any(kPowerAttack)) {
                        LockCheck(targetREFptr, actorREFptr, Power);
                    }
                    else if (attackData.any(kBashAttack)) {
                        LockCheck(targetREFptr, actorREFptr, Power);
                    }
                }
            }
            return RE::BSEventNotifyControl::kContinue;
        }
    };

    RE::ScriptEventSourceHolder::GetSingleton()->AddEventSink(new EventSink);
    delete new EventSink;
}
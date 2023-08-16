#pragma once
#include "Unlocker.h"

//void AttackDataCheck(SKSE::stl::enumeration<RE::AttackData::AttackFlag, uint32_t> attackData, RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr);

double HybridDetection(SKSE::stl::enumeration<RE::ActorValue, uint32_t> weapSkill, RE::TESObjectWEAP* weap, double pwr);

double SkillDetection(SKSE::stl::enumeration<RE::ActorValue, uint32_t> weapSkill, double pwr);

//void KeywordDetection(RE::TESObjectWEAP* weap);

//void AnimTypeDetection(RE::WEAPON_TYPE weapType);

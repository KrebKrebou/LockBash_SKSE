#pragma once
#include "Unlocker.h"

//void AttackDataCheck(SKSE::stl::enumeration<RE::AttackData::AttackFlag, uint32_t> attackData, RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr, float pwr);

void HybridDetection(SKSE::stl::enumeration<RE::ActorValue, uint32_t> weapSkill, RE::TESObjectWEAP* weap, RE::WEAPON_TYPE weapType, float pwr);

void SkillDetection(SKSE::stl::enumeration<RE::ActorValue, uint32_t> weapSkill, float pwr);

//void KeywordDetection(RE::TESObjectWEAP* weap, float pwr);
//
//void AnimTypeDetection(RE::WEAPON_TYPE weapType, float pwr);

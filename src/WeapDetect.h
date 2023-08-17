#pragma once
#include "Unlocker.h"

namespace LB {

	class  WeaponDetection{
	public:
		static double Hybrid(SKSE::stl::enumeration<RE::ActorValue, uint32_t> weapSkill, RE::TESObjectWEAP* weap, double pwr);
		static double Skill(SKSE::stl::enumeration<RE::ActorValue, uint32_t> weapSkill, double pwr);
		//void KeywordDetection(RE::TESObjectWEAP* weap);

	};
}


#pragma once
#include "Timer.h"
namespace LB {
	class Unlocker {
	public:
		static void LockCheck(RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr, float radius, double power);

	private:
		static void CrimeCheck(RE::TESObjectREFR* center, float radius, RE::Actor* act);

	};
}




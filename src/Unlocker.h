#pragma once
#include "Timer.h"
namespace LB {
	class Unlocker {
	public:
		static void LockCheck(RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr, float radius, double power);

	private:
        inline static const char* soundBreak;
        inline static std::string canBreak;
        inline static std::string actorName;
        inline static std::string combinedMsg;
        inline static const char* breakMsg;

        inline static long p_kVEasy;
        inline static long p_kEasy;
        inline static long p_kAverage;
        inline static long p_kHard;
        inline static long p_kVHard;

		static void CrimeCheck(RE::TESObjectREFR* center, float radius, RE::Actor* act);

	};
}




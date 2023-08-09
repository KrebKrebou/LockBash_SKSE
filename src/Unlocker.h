#pragma once
#include "Timer.h"

inline float Power;

void CrimeCheck(RE::TESObjectREFR* center, float radius, RE::Actor* act);

void TryUnlock(int milliseconds, RE::TESObjectREFRPtr trg, RE::TESObjectREFRPtr act, int cond);

void LockCheck(RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr, float radius);
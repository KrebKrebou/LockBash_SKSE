#pragma once
#include "Timer.h"

//inline float Power;

void CrimeCheck(RE::TESObjectREFR* center, float radius, RE::Actor* act);

void TryUnlock(RE::TESObjectREFRPtr trg, RE::TESObjectREFRPtr act, float radius, int cond);

void LockCheck(RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr, float radius, double power);


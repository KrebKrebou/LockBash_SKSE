#include "WeapDetect.h"

//void AttackDataCheck(SKSE::stl::enumeration<RE::AttackData::AttackFlag, uint32_t> attackData, RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr, float pwr){
//    /*attacktypes*/
//    auto kBashAttack = RE::AttackData::AttackFlag::kBashAttack;
//    auto kChargeAttack = RE::AttackData::AttackFlag::kChargeAttack;
//    auto kContinuousAttack = RE::AttackData::AttackFlag::kContinuousAttack;
//    auto kIgnoreWeapon = RE::AttackData::AttackFlag::kIgnoreWeapon;
//    auto kNone = RE::AttackData::AttackFlag::kNone;
//    auto kOverrideData = RE::AttackData::AttackFlag::kOverrideData;
//    auto kPowerAttack = RE::AttackData::AttackFlag::kPowerAttack;
//    auto kRotatingAttack = RE::AttackData::AttackFlag::kRotatingAttack;
//
//    if (attackData.none(kBashAttack, kChargeAttack, kContinuousAttack, kIgnoreWeapon, kNone, kOverrideData, kPowerAttack, kRotatingAttack)) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Attack Type: kNormalAttack"); }
//        pwr = pwr * 1;
//        LockCheck(refPtr, actPtr, pwr, 1000);
//    }
//    else if (attackData.any(kBashAttack)) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Attack Type: kBashAttack"); }
//        pwr = pwr * 1.2;
//        LockCheck(refPtr, actPtr, pwr, 1500);
//    }
//    else if (attackData.any(kPowerAttack)) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Attack Type: kPowerAttack"); }
//        pwr = pwr * 1.3;
//        LockCheck(refPtr, actPtr, pwr, 1750);
//    }
//}

void HybridDetection(SKSE::stl::enumeration<RE::ActorValue, uint32_t> weapSkill, RE::TESObjectWEAP* weap, RE::WEAPON_TYPE weapType, float pwr){

    // hybrid system
    if (weapSkill.get() == RE::ActorValue::kNone) {
        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Skill: kNone"); }
         switch (weap->GetWeaponType()) {
         case RE::WEAPON_TYPE::kHandToHandMelee:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: kHandToHandMelee"); }
             pwr = pwr - 100;
             break;
         default:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: defaultNone"); }
             pwr = pwr - 75;
             break;
         }     
    }

    else if (weapSkill.get() == RE::ActorValue::kOneHanded) {
        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Skill: kOneHanded"); }
         switch(weap->GetWeaponType()){
         case RE::WEAPON_TYPE::kHandToHandMelee:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: kHandToHandMelee"); }
             pwr = pwr - 100;
             break;
         case RE::WEAPON_TYPE::kOneHandDagger:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: kOneHandDagger"); }
             pwr = pwr - 50;
             break;
         case RE::WEAPON_TYPE::kOneHandSword:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: kOneHandSword"); }
             pwr = pwr + 25;
             break;
         case RE::WEAPON_TYPE::kOneHandAxe:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: kOneHandAxe"); }
             pwr = pwr + 50;
             break;
         case RE::WEAPON_TYPE::kOneHandMace:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: kOneHandMace"); }
             pwr = pwr + 75;
             break;
         default:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: defaultOneHand"); }
             pwr = pwr + 0;
             break;
         }
    }

    else if (weapSkill.get() == RE::ActorValue::kTwoHanded) {
         if (ini.GetBoolValue("Misc", "Logs", false) == true) {logger::info("Skill: kTwoHanded");}
         switch (weap->GetWeaponType()) {
         case RE::WEAPON_TYPE::kTwoHandSword:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: kTwoHandSword"); }
             pwr = pwr + 100;
             break;
         case RE::WEAPON_TYPE::kTwoHandAxe:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: kTwoHandAxe / Warhammer"); }
             pwr = pwr + 125;
             break;
         default:
             if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Type: defaultTwoHand"); }
             pwr = pwr + 75;
             break;
         }
    }
    if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Power = {}", pwr); }
}

void SkillDetection(SKSE::stl::enumeration<RE::ActorValue, uint32_t> weapSkill, float pwr) {
    // by governing skill
    if (weapSkill.get() == RE::ActorValue::kNone) {
        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Unarmed"); }
        pwr = pwr - 75;
    }
    else if (weapSkill.get() == RE::ActorValue::kOneHanded) {
        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("OneHanded"); }
        pwr = pwr + 0;
    }
    else if (weapSkill.get() == RE::ActorValue::kTwoHanded) {
        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("TwoHanded"); }
        pwr = pwr + 75;
    }
}

//void KeywordDetection(RE::TESObjectWEAP *weap, float pwr){
//    // by keyword
//    if (weap->HasKeywordString("WeapTypeDagger") || weap->HasKeywordString("WeapTypeCestus") || weap->HasKeywordString("WeapTypeClaw")) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Keyword: Dagger / Cestus / Claw"); }
//    }
//    else if (weap->HasKeywordString("WeapTypeSpear") || weap->HasKeywordString("WeapTypeRapier")) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Keyword: Spear / Rapier"); }
//    }
//    else if (weap->HasKeywordString("WeapTypeSword") || weap->HasKeywordString("WeapTypeKatana")) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Keyword: Sword / Katana"); }
//    }
//    else if (weap->HasKeywordString("WeapTypeWarAxe") || weap->HasKeywordString("WeapTypePike")) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Keyword: WarAxe / Pike"); }
//    }
//    else if (weap->HasKeywordString("WeapTypeMace")) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Keyword: Mace"); }
//    }
//    else if (weap->HasKeywordString("WeapTypeQuarterstaff") || weap->HasKeywordString("WeapTypeQtrStaff")) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Keyword: Quarterstaff"); }
//    }
//    else if (weap->HasKeywordString("WeapTypeGreatsword")) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Keyword: Greatsword"); }
//    }
//    else if (weap->HasKeywordString("WeapTypeBattleaxe") || weap->HasKeywordString("WeapTypeHalberd")) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Keyword: Battleaxe / Halberd"); }
//    }
//    else if (weap->HasKeywordString("WeapTypeWarhammer")) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Keyword: Warhammer"); }
//    }
//}
//
//void AnimTypeDetection(RE::WEAPON_TYPE weapType, float pwr){
//    // by anim type
//if (weapType == RE::WEAPON_TYPE::kHandToHandMelee) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("H2H -100 Power"); }
//    pwr = pwr - 100;
//}
//else if (weapType == RE::WEAPON_TYPE::kOneHandDagger) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("1HDagger -75 Power"); }
//    pwr = pwr - 75;
//}
//else if (weapType == RE::WEAPON_TYPE::kOneHandSword) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("1HSword -25 Power"); }
//    pwr = pwr - 25;
//}
//else if (weapType == RE::WEAPON_TYPE::kOneHandAxe) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("1HAxe +25 Power"); }
//    pwr = pwr + 25;
//}
//else if (weapType == RE::WEAPON_TYPE::kOneHandMace) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("1HMace +50 Power"); }
//    pwr = pwr + 50;
//}
//else if (weapType == RE::WEAPON_TYPE::kTwoHandSword) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("2HSword +75 Power"); }
//    pwr = pwr + 75;
//}
//else if (weapType == RE::WEAPON_TYPE::kTwoHandAxe) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("2HAxe +100 Power"); }
//    pwr = pwr + 100;
//}
//}












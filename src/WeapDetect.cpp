#include "WeapDetect.h"

//void AttackDataCheck(SKSE::stl::enumeration<RE::AttackData::AttackFlag, uint32_t> attackData, RE::TESObjectREFRPtr refPtr, RE::TESObjectREFRPtr actPtr, float Power){
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
//        Power = Power * 1;
//        LockCheck(refPtr, actPtr, Power, 1000);
//    }
//    else if (attackData.any(kBashAttack)) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Attack Type: kBashAttack"); }
//        Power = Power * 1.2;
//        LockCheck(refPtr, actPtr, Power, 1500);
//    }
//    else if (attackData.any(kPowerAttack)) {
//        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Attack Type: kPowerAttack"); }
//        Power = Power * 1.3;
//        LockCheck(refPtr, actPtr, Power, 1750);
//    }
//}

double HybridDetection(SKSE::stl::enumeration<RE::ActorValue, uint32_t> weapSkill, RE::TESObjectWEAP* weap, double pwr) {
    // hybrid system
    if (weapSkill.get() == RE::ActorValue::kNone) {
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Skill: None"); }
        switch (weap->GetWeaponType()) {
        case RE::WEAPON_TYPE::kHandToHandMelee:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Hand to Hand"); }
            pwr = pwr - 100;
            break;
        case RE::WEAPON_TYPE::kOneHandDagger:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Dagger"); }
            pwr = pwr - 50;
            break;
        case RE::WEAPON_TYPE::kOneHandSword:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Sword"); }
            pwr = pwr + 25;
            break;
        case RE::WEAPON_TYPE::kOneHandAxe:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Axe"); }
            pwr = pwr + 50;
            break;
        case RE::WEAPON_TYPE::kOneHandMace:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Mace"); }
            pwr = pwr + 75;
            break;
        case RE::WEAPON_TYPE::kTwoHandSword:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Greatsword"); }
            pwr = pwr + 100;
            break;
        case RE::WEAPON_TYPE::kTwoHandAxe:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Battleaxe / Warhammer"); }
            pwr = pwr + 125;
            break;
        default:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: default"); }
            pwr = pwr + 0;
            break;
        }
    }
    else if (weapSkill.get() == RE::ActorValue::kOneHanded) {
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Skill: One-Handed"); }
        switch (weap->GetWeaponType()) {
        case RE::WEAPON_TYPE::kHandToHandMelee:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Hand to Hand"); }
            pwr = pwr - 100;
            break;
        case RE::WEAPON_TYPE::kOneHandDagger:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Dagger"); }
            pwr = pwr - 50;
            break;
        case RE::WEAPON_TYPE::kOneHandSword:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Sword"); }
            pwr = pwr + 25;
            break;
        case RE::WEAPON_TYPE::kOneHandAxe:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Axe"); }
            pwr = pwr + 50;
            break;
        case RE::WEAPON_TYPE::kOneHandMace:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Mace"); }
            pwr = pwr + 75;
            break;
        default:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("WeaponType: default"); }
            pwr = pwr + 0;
            break;
        }
    }
    else if (weapSkill.get() == RE::ActorValue::kTwoHanded) {
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Skill: Two-Handed"); }
        switch (weap->GetWeaponType()) {
        case RE::WEAPON_TYPE::kTwoHandSword:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Greatsword"); }
            pwr = pwr + 100;
            break;
        case RE::WEAPON_TYPE::kTwoHandAxe:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: Battleaxe / Warhammer"); }
            pwr = pwr + 125;
            break;
        default:
            if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Weapon: default"); }
            pwr = pwr + 75;
            break;
        }
    }
    return pwr;
} 

double SkillDetection(SKSE::stl::enumeration<RE::ActorValue, uint32_t> weapSkill, double pwr) {
    // by governing skill
    if (weapSkill.get() == RE::ActorValue::kNone) {
        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Unarmed"); }
        pwr = pwr - 100;
    }
    else if (weapSkill.get() == RE::ActorValue::kOneHanded) {
        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("One-Handed"); }
        pwr = pwr + 0;
    }
    else if (weapSkill.get() == RE::ActorValue::kTwoHanded) {
        if (ini.GetBoolValue("Misc", "Logs", false) == true) { logger::info("Two-Handed"); }
        pwr = pwr + 100;
    }
    return pwr;
}

//void KeywordDetection(RE::TESObjectWEAP *weap, float Power){
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
//void AnimTypeDetection(RE::WEAPON_TYPE weapType, float Power){
//    // by anim type
//if (weapType == RE::WEAPON_TYPE::kHandToHandMelee) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("H2H -100 Power"); }
//    Power = Power - 100;
//}
//else if (weapType == RE::WEAPON_TYPE::kOneHandDagger) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("1HDagger -75 Power"); }
//    Power = Power - 75;
//}
//else if (weapType == RE::WEAPON_TYPE::kOneHandSword) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("1HSword -25 Power"); }
//    Power = Power - 25;
//}
//else if (weapType == RE::WEAPON_TYPE::kOneHandAxe) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("1HAxe +25 Power"); }
//    Power = Power + 25;
//}
//else if (weapType == RE::WEAPON_TYPE::kOneHandMace) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("1HMace +50 Power"); }
//    Power = Power + 50;
//}
//else if (weapType == RE::WEAPON_TYPE::kTwoHandSword) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("2HSword +75 Power"); }
//    Power = Power + 75;
//}
//else if (weapType == RE::WEAPON_TYPE::kTwoHandAxe) {
//    if (ini.GetBoolValue("Misc", "Logs", false) == true)
//    { logger::info("2HAxe +100 Power"); }
//    Power = Power + 100;
//}
//}












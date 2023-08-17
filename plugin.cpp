#include "src/EventRegistry.h"


SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SetupLog();
    SetupINI();
    SKSE::log::trace("LockBashSKSE initializing!");
    INILogCheck();
    
    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message){
        if (message->type == SKSE::MessagingInterface::kDataLoaded) {

            LB::RegisterEvent::Hit();
            RE::ConsoleLog::GetSingleton()->Print("LockBashSKSE Initialized");

        }

    });

    return true;
}
#include <iostream>       // std::cout, std::endl
#include "src/ext/logger_with_debugger_support.h"
#include "src/SetupINI.h"
#include "src/EventRegistry.h"


SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SetupLog();
    SetupINI();

    SKSE::log::trace("LockBashSKSE initializing!");
    INILogCheck();

     ////////////////////
    ///Add event sink///
   ////////////////////
   
    RegisterForEvent_Hit();

    
    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message){
        if (message->type == SKSE::MessagingInterface::kDataLoaded)

            RE::ConsoleLog::GetSingleton()->Print("LockBashSKSE Initialized");

    });

    return true;
}
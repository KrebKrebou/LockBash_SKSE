#include <iostream>       // std::cout, std::endl
#include "src/ext/logger_with_debugger_support.h"
#include "src/SetupINI.h"
#include "src/EventRegistry.h"


SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
    SetupLog();
    SetupINI();
    SKSE::log::info("Kreb SKSE test initializing!");

     ////////////////////
    ///Add event sink///
   ////////////////////
   
    RegisterForEvent_Hit();

    
    SKSE::GetMessagingInterface()->RegisterListener([](SKSE::MessagingInterface::Message *message){
        if (message->type == SKSE::MessagingInterface::kDataLoaded)

            RE::ConsoleLog::GetSingleton()->Print("Initialized");

    });

    return true;
}
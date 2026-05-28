#include "KCSE/KCSEAPI.h"
#include "Offsets/Offsets.h"
#include "entitymodule/C_Actor.h"
#include "combatmodule/C_CombatActor.h"
#include <cstdlib>

int alwaysPB = 0;       // 1 = always auto perfect block
int alwaysMS = 0;       // 1 = always auto master strike when riposte window open
int chancePB = 0;       // 0-100: percentage chance to auto PB per trigger (0 = disabled)
int chanceMS = 0;       // 0-100: percentage chance to auto MS per trigger (0 = disabled)
int alwaysHuntPB = 0;   // 1 = auto PB specifically during hunt/gap-close attacks
int alwaysHuntMS = 0;   // 1 = auto MS specifically during hunt attacks
int autoCounter = 0;    // consumable charge: auto-react this many times then stop, decrements each use

static wh::combatmodule::C_CombatActor* GetPlayerCombatActor()
{
    auto* iactor = Offsets::GetCCryAction()->GetClientActor();
    if (!iactor) return nullptr;
    return reinterpret_cast<wh::entitymodule::C_Actor*>(iactor)->m_pCombatActor;
}

static bool RollChance(int percent)
{
    return percent > 0 && (rand() % 100) < percent;
}

static void AutoPerfectBlock()
{
    auto* player = GetPlayerCombatActor();
    if (!player) return;

    auto* state = player->m_pState;
    if (!state || !state->m_pOpponent) return;

    if (state->m_pbTriggerCount <= 0 || state->m_isBlocking || state->m_isPerfectBlocking)
        return;

    // TODO: detect hunt attacks from combat state to gate alwaysHuntPB/MS separately
    bool isHunt = state->m_isInHuntAttack;

    bool wantMS = alwaysMS || (isHunt && alwaysHuntMS) || RollChance(chanceMS) || autoCounter > 0;
    bool wantPB = alwaysPB || (isHunt && alwaysHuntPB) || RollChance(chancePB) || autoCounter > 0;

    if (!wantMS && !wantPB)
        return;

    // Prefer master strike over perfect block
    if (wantMS && state->m_syncRiposteTriggerCount > 0) {
        wh::combatmodule::I_CombatActorActionPtr outAction;
        player->DispatchCounterAction(&outAction,
            wh::combatmodule::E_CounterActionType::SyncRiposte, 0);
        if (autoCounter > 0) --autoCounter;
        return;
    }

    if (wantPB) {
        wh::combatmodule::I_CombatActorActionPtr outAction;
        player->DispatchCounterAction(&outAction,
            wh::combatmodule::E_CounterActionType::PerfectBlock, 0);
        if (autoCounter > 0) --autoCounter;
    }
}

static void FrameTick()
{
    AutoPerfectBlock();
    KCSE::GetTaskInterface()->AddTask(FrameTick);
}

KCSE_PLUGIN_INFO("Combat Assist", "JerryYOJ", 1);
KCSE_PLUGIN_LOAD(kcse)
{
    kcse->GetMessagingInterface()->RegisterListener([](KCSE::Message* msg) {
        if (msg->type == KCSE::IMessagingInterface::kMessage_DataLoaded) {
            auto* con = SSystemGlobalEnvironment::GetInstance()->pConsole;

            con->RegisterCVarInt("kcse_ca_alwaysPB",     &alwaysPB,     0, VF_NULL, "1 = always auto perfect block");
            con->RegisterCVarInt("kcse_ca_alwaysMS",     &alwaysMS,     0, VF_NULL, "1 = always auto master strike");
            con->RegisterCVarInt("kcse_ca_chancePB",     &chancePB,     0, VF_NULL, "0-100: % chance to auto perfect block");
            con->RegisterCVarInt("kcse_ca_chanceMS",     &chanceMS,     0, VF_NULL, "0-100: % chance to auto master strike");
            con->RegisterCVarInt("kcse_ca_alwaysHuntPB", &alwaysHuntPB, 0, VF_NULL, "1 = auto PB during hunt attacks");
            con->RegisterCVarInt("kcse_ca_alwaysHuntMS", &alwaysHuntMS, 0, VF_NULL, "1 = auto MS during hunt attacks");
            con->RegisterCVarInt("kcse_ca_autoCounter",  &autoCounter,  0, VF_NULL, "N = auto-react N times then stop");

            SSystemGlobalEnvironment::GetInstance()->pLog->LogAlways("[CombatAssist] CVars registered, DataLoaded");
        }
    });

    KCSE::GetTaskInterface()->AddTask(FrameTick);
    return true;
}

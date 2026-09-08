/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "IndividualProgression.h"
#include "LFGMgr.h"
#include "LFG.h"
#include "ScriptMgr.h"

// The in-game Dungeon Finder shipped with patch 3.3.0 (Icecrown Citadel), well after WotLK's
// early raids. DisableRDF gates the finder off entirely; this ties its return to reaching WotLK
// content instead, so a character unlocks it the moment they'd blizzlike-ly have had access.
class GlobalRDFProgressionScript : public GlobalScript
{
public:
    GlobalRDFProgressionScript() : GlobalScript("GlobalRDFProgressionScript") { }

    void OnInitializeLockedDungeons(Player* player, uint8& /*level*/, uint32& lockData, lfg::LFGDungeonData const* dungeon) override
    {
        if (!sIndividualProgression->enabled || !sIndividualProgression->DisableRDF)
            return;

        if (!dungeon || dungeon->seasonal) // seasonal bosses stay reachable regardless of RDF
            return;

        if (player->IsGameMaster() || !sIndividualProgression->isNormalAccount(player))
            return;

        if (!sIndividualProgression->hasPassedProgression(player, PROGRESSION_TBC_TIER_5))
            lockData = lfg::LFG_LOCKSTATUS_INSUFFICIENT_EXPANSION;
    }
};

void AddSC_global_rdf_progression()
{
    new GlobalRDFProgressionScript();
}

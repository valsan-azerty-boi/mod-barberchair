#ifndef MODULE_MODBARBERCHAIR_H
#define MODULE_MODBARBERCHAIR_H

#include "Config.h"
#include "Chat.h"
#include "GameObject.h"  
#include "GameObjectAI.h"
#include "GameObjectScript.h"
#include "Group.h"
#include "ObjectAccessor.h"
#include "ObjectGuid.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "WorldScript.h"
#include "WorldState.h"
#include <vector>

#define ACTION_BASE_BARBER           35987651
#define ACTION_APPEARANCE_CHANGE     35987652
#define ACTION_RACE_CHANGE           35987653
#define ACTION_FACTION_CHANGE        35987654
#define ACTION_NAME_CHANGE           35987655

class ModBarberchairScript : public GameObjectScript, public WorldScript
{
public:
    ModBarberchairScript();

    void OnAfterConfigLoad(bool /*reload*/) override;
    bool OnGossipHello(Player* /*player*/, GameObject* /*go*/) override;
    bool OnGossipSelect(Player* /*player*/, GameObject* /*go*/, uint32 /*sender*/, uint32 /*action*/) override;

    struct ModBarberchairAI : GameObjectAI
    {
        explicit ModBarberchairAI(GameObject* object) : GameObjectAI(object) { }
        bool CanBeSeen(Player const* player) override;
    };

    GameObjectAI* GetAI(GameObject* object) const override
    {
        return new ModBarberchairAI(object);
    }

private:
    static bool ModBarberchairEnabled;
    static bool BaseFeatureEnabled;
    static bool FullAppearanceChangeFeatureEnabled;
    static bool RaceChangeFeatureEnabled;
    static bool FactionChangeFeatureEnabled;
    static bool NameChangeFeatureEnabled;
    static std::vector<uint8> BaseFeatureRaceExclusion;
    static bool ModIndividualProgressionIntegrationEnabled;

    static void MoveCharacterOnBarberchair(Player* player, GameObject* go);
    static std::vector<uint8> ParseRaceExclusions(const std::string& exclusionStr);

    static char const* GetBarberGossipString(LocaleConstant locale, int gossipId);
    static char const* GetBarberCommandString(LocaleConstant locale, int gossipId);
};

void Addmod_barberchairScripts();

#endif // MODULE_MODBARBERCHAIR_H

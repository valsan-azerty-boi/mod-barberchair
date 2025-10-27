#include "ModBarberchair.h"

#include "Chat.h"
#include "Config.h"
#include "GameObject.h"
#include "GameObjectScript.h"
#include "Player.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "ObjectGuid.h"
#include "WorldScript.h"
#include <vector>

ModBarberchairScript::ModBarberchairScript() : GameObjectScript("mod_barberchair"), WorldScript("ModBarberchairWorldScript") {}

bool ModBarberchairScript::ModBarberchairEnabled = false;
bool ModBarberchairScript::BaseFeatureEnabled = false;
bool ModBarberchairScript::FullAppearanceChangeFeatureEnabled = false;
bool ModBarberchairScript::RaceChangeFeatureEnabled = false;
bool ModBarberchairScript::FactionChangeFeatureEnabled = false;
bool ModBarberchairScript::NameChangeFeatureEnabled = false;
std::vector<uint8> ModBarberchairScript::BaseFeatureRaceExclusion;
bool ModBarberchairScript::ModIndividualProgressionIntegrationEnabled = false;

void ModBarberchairScript::OnAfterConfigLoad(bool /*reload*/)
{
    ModBarberchairEnabled = sConfigMgr->GetOption<bool>("Barberchair.Enable", false);
    BaseFeatureEnabled = sConfigMgr->GetOption<bool>("Barberchair.BaseFeature.Enable", true);
    FullAppearanceChangeFeatureEnabled = sConfigMgr->GetOption<bool>("Barberchair.FullAppearanceChangeFeature.Enable", true);
    RaceChangeFeatureEnabled = sConfigMgr->GetOption<bool>("Barberchair.RaceChangeFeature.Enable", false);
    FactionChangeFeatureEnabled = sConfigMgr->GetOption<bool>("Barberchair.FactionChangeFeature.Enable", false);
    NameChangeFeatureEnabled = sConfigMgr->GetOption<bool>("Barberchair.NameChangeFeature.Enable", false);
    BaseFeatureRaceExclusion = ParseRaceExclusions(sConfigMgr->GetOption<std::string>("Barberchair.BaseFeature.RaceExclusion", "9,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33"));
    ModIndividualProgressionIntegrationEnabled = sConfigMgr->GetOption<bool>("Barberchair.ModIndividualProgression.Integration.Enable", false);
}

bool ModBarberchairScript::OnGossipHello(Player* player, GameObject* go)
{
    if (!ModBarberchairEnabled)
        return false;

    if (!go || go->GetGoType() != GAMEOBJECT_TYPE_BARBER_CHAIR)
        return false;

    ClearGossipMenuFor(player);
    LocaleConstant playerLocale = player->GetSession()->GetSessionDbLocaleIndex();

    uint32 availableOptions = 0;
    uint32 uniqueActionId = 0;

    if (BaseFeatureEnabled)
    {
        uint8 charRace = player->getRace(true);
        bool isRaceExcluded = std::find(BaseFeatureRaceExclusion.begin(), BaseFeatureRaceExclusion.end(), charRace) != BaseFeatureRaceExclusion.end();

        if (!isRaceExcluded)
        {
            AddGossipItemFor(player, GOSSIP_ICON_DOT, GetBarberGossipString(playerLocale, ACTION_BASE_BARBER), GOSSIP_SENDER_MAIN, ACTION_BASE_BARBER);
            availableOptions++;
            uniqueActionId = ACTION_BASE_BARBER;
        }
    }

    if (FullAppearanceChangeFeatureEnabled)
    {
        AddGossipItemFor(player, GOSSIP_ICON_DOT, GetBarberGossipString(playerLocale, ACTION_APPEARANCE_CHANGE), GOSSIP_SENDER_MAIN, ACTION_APPEARANCE_CHANGE);
        availableOptions++;
        uniqueActionId = ACTION_APPEARANCE_CHANGE;
    }

    if (RaceChangeFeatureEnabled)
    {
        AddGossipItemFor(player, GOSSIP_ICON_DOT, GetBarberGossipString(playerLocale, ACTION_RACE_CHANGE), GOSSIP_SENDER_MAIN, ACTION_RACE_CHANGE);
        availableOptions++;
        uniqueActionId = ACTION_RACE_CHANGE;
    }

    if (FactionChangeFeatureEnabled)
    {
        AddGossipItemFor(player, GOSSIP_ICON_DOT, GetBarberGossipString(playerLocale, ACTION_FACTION_CHANGE), GOSSIP_SENDER_MAIN, ACTION_FACTION_CHANGE);
        availableOptions++;
        uniqueActionId = ACTION_FACTION_CHANGE;
    }

    if (NameChangeFeatureEnabled)
    {
        AddGossipItemFor(player, GOSSIP_ICON_DOT, GetBarberGossipString(playerLocale, ACTION_NAME_CHANGE), GOSSIP_SENDER_MAIN, ACTION_NAME_CHANGE);
        availableOptions++;
        uniqueActionId = ACTION_NAME_CHANGE;
    }

    if (availableOptions == 0)
    {
        MoveCharacterOnBarberchair(player, go);
        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP, 1);
        return true;
    }
    else if (availableOptions == 1)
    {
        OnGossipSelect(player, go, GOSSIP_SENDER_MAIN, uniqueActionId);
        return true;
    }
    else if (availableOptions > 1)
    {
        SendGossipMenuFor(player, 5454, go->GetGUID()); // 5454 = "How may I be of service?"
        return true;
    }

    return false;
}

bool ModBarberchairScript::OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 action)
{
    if (!ModBarberchairEnabled)
        return false;

    ClearGossipMenuFor(player);
    LocaleConstant playerLocale = player->GetSession()->GetSessionDbLocaleIndex();

    switch ((int)action)
    {
        case ACTION_BASE_BARBER:
        {
            if (ModBarberchairScript::BaseFeatureEnabled)
            {
                MoveCharacterOnBarberchair(player, go);
                WorldPacket data(SMSG_ENABLE_BARBER_SHOP, 0);
                player->GetSession()->SendPacket(&data);
            }
            break;
        }
        case ACTION_APPEARANCE_CHANGE:
        {
            if (ModBarberchairScript::FullAppearanceChangeFeatureEnabled)
            {
                MoveCharacterOnBarberchair(player, go);
                player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE);
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP, 1);
                player->SendPlaySpellVisual(7961); // 7961 = Level Up visual effect
                player->SendSystemMessage(GetBarberCommandString(playerLocale, ACTION_APPEARANCE_CHANGE));
            }
            break;
        }
        case ACTION_RACE_CHANGE:
        {
            if (ModBarberchairScript::RaceChangeFeatureEnabled)
            {
                MoveCharacterOnBarberchair(player, go);
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE);
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP, 1);
                player->SendPlaySpellVisual(7961); // 7961 = Level Up visual effect
                player->SendSystemMessage(GetBarberCommandString(playerLocale, ACTION_RACE_CHANGE));
            }
            break;
        }
        case ACTION_FACTION_CHANGE:
        {
            if (ModBarberchairScript::FactionChangeFeatureEnabled)
            {
                MoveCharacterOnBarberchair(player, go);
                player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION);
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP, 1);
                player->SendPlaySpellVisual(7961); // 7961 = Level Up visual effect
                player->SendSystemMessage(GetBarberCommandString(playerLocale, ACTION_FACTION_CHANGE));
            }
            break;
        }
        case ACTION_NAME_CHANGE:
        {
            if (ModBarberchairScript::NameChangeFeatureEnabled)
            {
                MoveCharacterOnBarberchair(player, go);
                player->SetAtLoginFlag(AT_LOGIN_RENAME);
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP, 1);
                player->SendPlaySpellVisual(7961); // 7961 = Level Up visual effect
                player->SendSystemMessage(GetBarberCommandString(playerLocale, ACTION_NAME_CHANGE));
            }
            break;
        }
    }

    CloseGossipMenuFor(player);
    return true;
}

bool ModBarberchairScript::ModBarberchairAI::CanBeSeen(Player const* player)
{
    if (player->IsGameMaster())
        return true;
    if (!ModBarberchairEnabled)
        return true;
    if (!ModIndividualProgressionIntegrationEnabled)
        return true;
    
    bool hasRequiredLevel = player->GetLevel() > 70;
    bool hasRequiredRaid = player->HasAchieved(698); // 698 = Sunwell raid achievement

    return hasRequiredLevel || hasRequiredRaid;
}

void ModBarberchairScript::MoveCharacterOnBarberchair(Player* player, GameObject* go)
{
    if (go && player)
    {
        GameObjectTemplate const* info = go->GetGOInfo();
        if (info)
        {
            player->TeleportTo(go->GetMapId(), go->GetPositionX(), go->GetPositionY(), go->GetPositionZ(), go->GetOrientation(), TELE_TO_NOT_LEAVE_TRANSPORT | TELE_TO_NOT_LEAVE_COMBAT | TELE_TO_NOT_UNSUMMON_PET);
            player->SetStandState(UNIT_STAND_STATE_SIT_LOW_CHAIR + info->barberChair.chairheight);
        }
    }
}

std::vector<uint8> ModBarberchairScript::ParseRaceExclusions(const std::string& exclusionStr)
{
    std::stringstream ss(exclusionStr);
    std::string segment;
    std::vector<uint8> resultVector;

    while (std::getline(ss, segment, ','))
    {
        segment.erase(0, segment.find_first_not_of(" \t"));
        segment.erase(segment.find_last_not_of(" \t") + 1);
        if (!segment.empty())
            resultVector.push_back(static_cast<uint8>(::atoi(segment.c_str())));
    }

    return resultVector;
}

void Addmod_barberchairScripts()
{
    new ModBarberchairScript();
}

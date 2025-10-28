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
#include <sstream>
#include <string>

ModBarberchairScript::ModBarberchairScript() : GameObjectScript("mod_barberchair"), WorldScript("ModBarberchairWorldScript") {}

bool ModBarberchairScript::ModBarberchairEnabled = false;
bool ModBarberchairScript::BaseFeatureEnabled = false;
bool ModBarberchairScript::FullAppearanceChangeFeatureEnabled = false;
uint32 ModBarberchairScript::FullAppearanceChangeCost = 0;
bool ModBarberchairScript::RaceChangeFeatureEnabled = false;
uint32 ModBarberchairScript::RaceChangeCost = 0;
bool ModBarberchairScript::FactionChangeFeatureEnabled = false;
uint32 ModBarberchairScript::FactionChangeCost = 0;
bool ModBarberchairScript::NameChangeFeatureEnabled = false;
uint32 ModBarberchairScript::NameChangeCost = 0;
std::vector<uint8> ModBarberchairScript::BaseFeatureRaceExclusion;
bool ModBarberchairScript::ModIndividualProgressionIntegrationEnabled = false;

static const std::string goldIcon = "|TInterface\\MoneyFrame\\UI-GoldIcon:0:0:1|t";
static const std::string silverIcon = "|TInterface\\MoneyFrame\\UI-SilverIcon:0:0:1|t";
static const std::string copperIcon = "|TInterface\\MoneyFrame\\UI-CopperIcon:0:0:1|t";

void ModBarberchairScript::OnAfterConfigLoad(bool /*reload*/)
{
    ModBarberchairEnabled = sConfigMgr->GetOption<bool>("Barberchair.Enable", false);
    BaseFeatureEnabled = sConfigMgr->GetOption<bool>("Barberchair.BaseFeature.Enable", true);
    FullAppearanceChangeFeatureEnabled = sConfigMgr->GetOption<bool>("Barberchair.FullAppearanceChangeFeature.Enable", true);
    FullAppearanceChangeCost = sConfigMgr->GetOption<uint32>("Barberchair.FullAppearanceChangeFeature.MoneyPrice", 10000);
    RaceChangeFeatureEnabled = sConfigMgr->GetOption<bool>("Barberchair.RaceChangeFeature.Enable", false);
    RaceChangeCost = sConfigMgr->GetOption<uint32>("Barberchair.RaceChangeFeature.MoneyPrice", 1000000);
    FactionChangeFeatureEnabled = sConfigMgr->GetOption<bool>("Barberchair.FactionChangeFeature.Enable", false);
    FactionChangeCost = sConfigMgr->GetOption<uint32>("Barberchair.FactionChangeFeature.MoneyPrice", 1000000);
    NameChangeFeatureEnabled = sConfigMgr->GetOption<bool>("Barberchair.NameChangeFeature.Enable", false);
    NameChangeCost = sConfigMgr->GetOption<uint32>("Barberchair.NameChangeFeature.MoneyPrice", 500000);
    BaseFeatureRaceExclusion = ParseRaceExclusions(sConfigMgr->GetOption<std::string>("Barberchair.BaseFeature.RaceExclusion", "9,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33"));
    ModIndividualProgressionIntegrationEnabled = sConfigMgr->GetOption<bool>("Barberchair.ModIndividualProgression.Integration.Enable", false);
}

bool ModBarberchairScript::OnGossipHello(Player* player, GameObject* go)
{
    if (!ModBarberchairEnabled)
        return false;

    if (!go || go->GetGoType() != GAMEOBJECT_TYPE_BARBER_CHAIR)
        return false;

    MoveCharacterOnBarberchair(player, go);

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
        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP, 1);
        return true;
    }
    else if (availableOptions >= 1)
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

    MoveCharacterOnBarberchair(player, go);

    LocaleConstant playerLocale = player->GetSession()->GetSessionDbLocaleIndex();

    if (action == ACTION_CONFIRM_YES)
    {
        uint32 cost = 0;
        switch ((int)sender)
        {
            case ACTION_APPEARANCE_CHANGE: cost = FullAppearanceChangeCost; break;
            case ACTION_RACE_CHANGE:       cost = RaceChangeCost; break;
            case ACTION_FACTION_CHANGE:    cost = FactionChangeCost; break;
            case ACTION_NAME_CHANGE:       cost = NameChangeCost; break;
            default: CloseGossipMenuFor(player); return true;
        }

        if (cost > 0 && !player->HasEnoughMoney(cost))
        {
            player->SendSystemMessage(GetTechnicalString(playerLocale, TECH_MSG_ERR_MONEY));
            CloseGossipMenuFor(player);
            return true;
        }

        if (cost > 0)
            player->ModifyMoney(-((int32)cost));

        player->SendPlaySpellVisual(7961); // 7961 = Level up visual effect
        
        switch ((int)sender)
        {
            case ACTION_APPEARANCE_CHANGE: player->SetAtLoginFlag(AT_LOGIN_CUSTOMIZE); break;
            case ACTION_RACE_CHANGE:       player->SetAtLoginFlag(AT_LOGIN_CHANGE_RACE); break;
            case ACTION_FACTION_CHANGE:    player->SetAtLoginFlag(AT_LOGIN_CHANGE_FACTION); break;
            case ACTION_NAME_CHANGE:       player->SetAtLoginFlag(AT_LOGIN_RENAME); break;
        }

        player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_VISIT_BARBER_SHOP, 1);
        player->SendSystemMessage(GetBarberCommandString(playerLocale, sender));
        ClearGossipMenuFor(player);
        CloseGossipMenuFor(player);
        return true;
    }

    if (action == ACTION_CONFIRM_NO)
    {
        ClearGossipMenuFor(player);
        CloseGossipMenuFor(player);
        return true;
    }
    else 
    {
        uint32 cost = 0;
        std::string serviceName;
        
        switch ((int)action)
        {
            case ACTION_BASE_BARBER:       if (!BaseFeatureEnabled) return true; cost = 0; serviceName = GetBarberGossipString(playerLocale, action); break;
            case ACTION_APPEARANCE_CHANGE: if (!FullAppearanceChangeFeatureEnabled) return true; cost = FullAppearanceChangeCost; serviceName = GetBarberGossipString(playerLocale, action); break;
            case ACTION_RACE_CHANGE:       if (!RaceChangeFeatureEnabled) return true; cost = RaceChangeCost; serviceName = GetBarberGossipString(playerLocale, action); break;
            case ACTION_FACTION_CHANGE:    if (!FactionChangeFeatureEnabled) return true; cost = FactionChangeCost; serviceName = GetBarberGossipString(playerLocale, action); break;
            case ACTION_NAME_CHANGE:       if (!NameChangeFeatureEnabled) return true; cost = NameChangeCost; serviceName = GetBarberGossipString(playerLocale, action); break;
            default: CloseGossipMenuFor(player); return true;
        }

        if (action == ACTION_BASE_BARBER)
        {
            ClearGossipMenuFor(player);
            CloseGossipMenuFor(player);
            WorldPacket data(SMSG_ENABLE_BARBER_SHOP, 0);
            player->GetSession()->SendPacket(&data);
            return true;
        }
        
        std::string confirmationText;
        confirmationText = serviceName + " ?";
        
        std::string priceText = FormatPrice(cost);

        if (!priceText.empty())
            confirmationText += " " + priceText;
        
        ClearGossipMenuFor(player);
        AddGossipItemFor(player, GOSSIP_ICON_DOT, confirmationText, action, ACTION_CONFIRM_YES);
        AddGossipItemFor(player, GOSSIP_ICON_DOT, GetTechnicalString(playerLocale, TECH_MSG_CANCEL), GOSSIP_SENDER_MAIN, ACTION_CONFIRM_NO);
        SendGossipMenuFor(player, 5454, go->GetGUID()); // 5454 = "How may I be of service?"
        return true;
    }

    ClearGossipMenuFor(player);
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

std::string ModBarberchairScript::FormatPrice(uint32 price)
{
    if (price == 0)
        return "";

    uint32 gold = price / 10000;
    uint32 silver = (price % 10000) / 100;
    uint32 copper = price % 100;
    
    std::string result = "";
    if (gold > 0)
        result += std::to_string(gold) + goldIcon + " ";
    if (silver > 0)
        result += std::to_string(silver) + silverIcon + " ";
    if (copper > 0)
        result += std::to_string(copper) + copperIcon;

    return result;
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

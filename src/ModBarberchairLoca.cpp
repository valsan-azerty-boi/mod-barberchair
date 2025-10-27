#include "ModBarberchair.h"

#include <string>
#include <sstream>

char const* ModBarberchairScript::GetBarberGossipString(LocaleConstant locale, int gossipId)
{
    if (locale == LOCALE_deDE)
    {
        switch (gossipId)
        {
        case ACTION_BASE_BARBER:
            return "Ändere dein Aussehen";
        case ACTION_APPEARANCE_CHANGE:
            return "Ändere dein Aussehen vollständig";
        case ACTION_RACE_CHANGE:
            return "Ändere dein Volk";
        case ACTION_FACTION_CHANGE:
            return "Ändere deine Fraktion";
        case ACTION_NAME_CHANGE:
            return "Ändere deinen Namen";
        default:
            return "";
        }
    }
    else if (locale == LOCALE_esES || locale == LOCALE_esMX)
    {
        switch (gossipId)
        {
        case ACTION_BASE_BARBER:
            return "Cambia tu apariencia";
        case ACTION_APPEARANCE_CHANGE:
            return "Cambia tu apariencia por completo";
        case ACTION_RACE_CHANGE:
            return "Cambia tu raza";
        case ACTION_FACTION_CHANGE:
            return "Cambia tu facción";
        case ACTION_NAME_CHANGE:
            return "Cambia tu nombre";
        default:
            return "";
        }
    }
    else if (locale == LOCALE_frFR)
    {
        switch (gossipId)
        {
        case ACTION_BASE_BARBER:
            return "Modifier son apparence";
        case ACTION_APPEARANCE_CHANGE:
            return "Obtenir un changement d'apparence complet";
        case ACTION_RACE_CHANGE:
            return "Obtenir un changement de race";
        case ACTION_FACTION_CHANGE:
            return "Obtenir un changement de faction";
        case ACTION_NAME_CHANGE:
            return "Obtenir un changement de nom";
        default:
            return "";
        }
    }
    else
    {
        switch (gossipId)
        {
        case ACTION_BASE_BARBER:
            return "Change your appearance";
        case ACTION_APPEARANCE_CHANGE:
            return "Get a full appearance change";
        case ACTION_RACE_CHANGE:
            return "Get a race change";
        case ACTION_FACTION_CHANGE:
            return "Get a faction change";
        case ACTION_NAME_CHANGE:
            return "Get a name change";
        default:
            return "";
        }
    }
}

char const* ModBarberchairScript::GetBarberCommandString(LocaleConstant locale, int gossipId)
{
    if (locale == LOCALE_deDE)
    {
        switch (gossipId)
        {
        case ACTION_APPEARANCE_CHANGE:
            return "Du hast gerade eine vollständige Änderung des Aussehens für diesen Charakter erhalten. Du kannst sie über die Charakterauswahl aufrufen.";
        case ACTION_RACE_CHANGE:
            return "Du hast gerade eine Rassenänderung für diesen Charakter erhalten. Du kannst sie über die Charakterauswahl aufrufen.";
        case ACTION_FACTION_CHANGE:
            return "Du hast gerade eine Fraktionsänderung für diesen Charakter erhalten. Du kannst sie über die Charakterauswahl aufrufen.";
        case ACTION_NAME_CHANGE:
            return "Du hast gerade eine Namensänderung für diesen Charakter erhalten. Du kannst sie aufrufen, indem du dich ab- und wieder anmeldest";
        default:
            return "";
        }
    }
    else if (locale == LOCALE_esES || locale == LOCALE_esMX)
    {
        switch (gossipId)
        {
        case ACTION_APPEARANCE_CHANGE:
            return "Acabas de recibir un cambio de apariencia completo para este personaje. Puedes acceder a él en la pantalla de selección de personaje.";
        case ACTION_RACE_CHANGE:
            return "Acabas de recibir un cambio de raza para este personaje. Puedes acceder a él en la pantalla de selección de personaje.";
        case ACTION_FACTION_CHANGE:
            return "Acabas de recibir un cambio de facción para este personaje. Puedes acceder a él en la pantalla de selección de personaje.";
        case ACTION_NAME_CHANGE:
            return "Acabas de recibir un cambio de nombre para este personaje. Puedes acceder a él cerrando sesión y volviendo a iniciarla.";
        default:
            return "";
        }
    }
    else if (locale == LOCALE_frFR)
    {
        switch (gossipId)
        {
        case ACTION_APPEARANCE_CHANGE:
            return "Vous venez d'obtenir un changement d'apparence complet pour ce personnage. Vous pourrez y accéder à l'écran de sélection des personnages.";
        case ACTION_RACE_CHANGE:
            return "Vous venez d'obtenir un changement de race pour ce personnage. Vous pourrez y accéder à l'écran de sélection des personnages.";
        case ACTION_FACTION_CHANGE:
            return "Vous venez d'obtenir un changement de faction pour ce personnage. Vous pourrez y accéder à l'écran de sélection des personnages.";
        case ACTION_NAME_CHANGE:
            return "Vous venez d'obtenir un changement de nom pour ce personnage. Vous pourrez y accéder en déconnectant et reconnectant le personnage.";
        default:
            return "";
        }
    }
    else
    {
        switch (gossipId)
        {
        case ACTION_APPEARANCE_CHANGE:
            return "You have just received a full appearance change for this character. You can access it on the character select screen.";
        case ACTION_RACE_CHANGE:
            return "You have just received a race change for this character. You can access it on the character select screen.";
        case ACTION_FACTION_CHANGE:
            return "You have just received a faction change for this character. You can access it on the character select screen.";
        case ACTION_NAME_CHANGE:
            return "You have just received a name change for this character. You can access it by logging out and back in.";
        default:
            return "";
        }
    }
}

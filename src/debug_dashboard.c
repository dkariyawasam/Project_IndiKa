// External mGBA testing interface. Dormant until a matching desktop bridge
// writes the magic value and a command; no in-game menu or save-layout changes.
#include "global.h"
#include "debug_dashboard.h"
#include "event_data.h"
#include "field_player_avatar.h"
#include "item.h"
#include "money.h"
#include "palette.h"
#include "player_appearance.h"
#include "pokemon.h"
#include "script.h"
#include "script_pokemon_util.h"
#include "constants/flags.h"
#include "constants/battle_setup.h"
#include "constants/items.h"
#include "constants/moves.h"
#include "constants/opponents.h"
#include "constants/species.h"
#include "constants/trainers.h"
#include "constants/vars.h"

#define DEBUG_MAGIC 0x4B444247
// magic, command, status (0 queued, 1 accepted, 2 invalid, 3 busy), six args.
EWRAM_DATA volatile u32 gDebugDashboardMailbox[10] = {0};
static EWRAM_DATA u8 sDebugDashboardScript[32] = {0};
static const u8 sDebugBattleText[] = _("Debug battle complete.");
// The desktop reads compiled offsets instead of assuming the save structure.
const u32 gDebugDashboardLayout[] = {
    DEBUG_MAGIC, 1, sizeof(struct SaveBlock1), sizeof(struct SaveBlock2),
    offsetof(struct SaveBlock1, flags), offsetof(struct SaveBlock1, vars),
    offsetof(struct SaveBlock2, playerGender), offsetof(struct SaveBlock2, appearanceVersion),
    offsetof(struct SaveBlock2, playerAccent), offsetof(struct SaveBlock2, playerSecondaryAccent),
    sizeof(struct Pokemon), offsetof(struct Pokemon, level), offsetof(struct Pokemon, hp),
    offsetof(struct Pokemon, maxHP), offsetof(struct Pokemon, status), FLAGS_COUNT, VARS_COUNT,
};

static void Half(u8 i, u16 value)
{
    sDebugDashboardScript[i] = value;
    sDebugDashboardScript[i + 1] = value >> 8;
}

static void Word(u8 i, u32 value)
{
    Half(i, value);
    Half(i + 2, value >> 16);
}

static void RunDebugScript(void)
{
    LockPlayerFieldControls();
    ScriptContext_SetupScript(sDebugDashboardScript);
}

static u32 ExecuteDebugCommand(u32 command, const u32 *a)
{
    u8 value;
    if (gPaletteFade.active || ArePlayerFieldControlsLocked() || ScriptContext_IsEnabled()
        || gPlayerAvatar.objectEventId >= OBJECT_EVENTS_COUNT
        || gPlayerAvatar.tileTransitionState == 1
        || gPlayerAvatar.runningState == 2)
        return 3;
    switch (command)
    {
    case 1: // Warp: catalog-validated group/map and coordinates.
        if (a[0] > 255 || a[1] > 255 || a[2] > 32767 || a[3] > 32767)
            return 2;
        sDebugDashboardScript[0] = 0x39;
        sDebugDashboardScript[1] = a[0];
        sDebugDashboardScript[2] = a[1];
        sDebugDashboardScript[3] = 255;
        Half(4, a[2]); Half(6, a[3]);
        sDebugDashboardScript[8] = 0x27;
        sDebugDashboardScript[9] = 0x02;
        RunDebugScript();
        break;
    case 2: // Add a Pokemon, never silently send it to storage.
        if (gPlayerPartyCount >= PARTY_SIZE || a[0] == 0 || a[0] >= NUM_SPECIES || a[1] < 1 || a[1] > 100)
            return 2;
        CreateMon(&gPlayerParty[gPlayerPartyCount], a[0], a[1], USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
        gPlayerPartyCount++;
        break;
    case 3:
        HealPlayerParty();
        break;
    case 4:
        if (a[0] == 0 || a[0] >= FLAGS_COUNT || a[1] > 1)
            return 2;
        if (a[1]) FlagSet(a[0]); else FlagClear(a[0]);
        break;
    case 5:
        if (a[0] < VARS_START || a[0] > VARS_END || a[1] > 65535)
            return 2;
        VarSet(a[0], a[1]);
        break;
    case 6:
        if (a[0] == 0 || a[0] >= ITEMS_COUNT || a[1] < 1 || a[1] > 99 || !AddBagItem(a[0], a[1]))
            return 2;
        break;
    case 7: // Scripted wild battle.
        if (!gPlayerPartyCount || a[0] == 0 || a[0] >= NUM_SPECIES || a[1] < 1 || a[1] > 100)
            return 2;
        sDebugDashboardScript[0] = 0xB6; Half(1, a[0]);
        sDebugDashboardScript[3] = a[1]; Half(4, 0);
        sDebugDashboardScript[6] = 0xB7;
        sDebugDashboardScript[7] = 0x6B;
        sDebugDashboardScript[8] = 0x02;
        RunDebugScript();
        break;
    case 8: // Trainer battle, using the normal trainer table and battle engine.
        if (!gPlayerPartyCount || a[0] == 0 || a[0] >= MAX_TRAINERS_COUNT)
            return 2;
        sDebugDashboardScript[0] = 0x5C;
        if (a[1] && gPlayerPartyCount < 2)
            return 2;
        sDebugDashboardScript[1] = a[1] ? TRAINER_BATTLE_DOUBLE_NO_INTRO_TEXT : TRAINER_BATTLE_SINGLE_NO_INTRO_TEXT;
        Half(2, a[0]); Half(4, 0); Word(6, (u32)sDebugBattleText);
        if (a[1])
        {
            Word(10, (u32)sDebugBattleText);
            sDebugDashboardScript[14] = 0x6B;
            sDebugDashboardScript[15] = 0x02;
        }
        else
        {
            sDebugDashboardScript[10] = 0x6B;
            sDebugDashboardScript[11] = 0x02;
        }
        RunDebugScript();
        break;
    case 9: // Saved appearance. Reload the map through Warp to refresh sprites.
        if (a[0] > 1 || a[1] > 2 || a[2] > 2)
            return 2;
        gSaveBlock2Ptr->playerGender = a[0];
        gSaveBlock2Ptr->appearanceVersion = PLAYER_APPEARANCE_VERSION;
        gSaveBlock2Ptr->playerAccent = a[1];
        gSaveBlock2Ptr->playerSecondaryAccent = a[2];
        gSaveBlock2Ptr->optionsWindowFrameType = a[2];
        break;
    case 10:
        if (a[0] >= gPlayerPartyCount || a[1] > 3 || a[2] >= MOVES_COUNT)
            return 2;
        SetMonMoveSlot(&gPlayerParty[a[0]], a[2], a[1]);
        break;
    case 11:
        if (a[0] >= gPlayerPartyCount || a[1] > 255)
            return 2;
        value = a[1];
        SetMonData(&gPlayerParty[a[0]], MON_DATA_FRIENDSHIP, &value);
        break;
    case 12:
        if (a[0] > 999999)
            return 2;
        SetMoney(&gSaveBlock1Ptr->money, a[0]);
        break;
    default:
        return 2;
    }
    return 1;
}

void DebugDashboard_Poll(void)
{
    u32 args[6];
    u32 i;
    u32 command = gDebugDashboardMailbox[1];
    if (gDebugDashboardMailbox[0] != DEBUG_MAGIC || command == 0)
        return;
    for (i = 0; i < 6; i++)
        args[i] = gDebugDashboardMailbox[4 + i];
    // Publish completion last: an emulator frame callback may run mid-command.
    gDebugDashboardMailbox[2] = ExecuteDebugCommand(command, args);
    gDebugDashboardMailbox[1] = 0;
}

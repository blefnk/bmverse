/* ---------------------------------------------------------------------------- */
/*      Blefonix RolePlay Pickup System                                         */
/* ---------------------------------------------------------------------------- */

#if defined _inc_libs_picks
	#undef _inc_libs_picks
#endif
#if defined _libs_picks_included
	#endinput
#endif
#define _libs_picks_included

/* ---------------------------------------------------------------------------- */
/*      LIBS                                                                    */
/* ---------------------------------------------------------------------------- */

#include <YSI_Coding\y_hooks>

/* ---------------------------------------------------------------------------- */
/*      VARS/ENUMS                                                              */
/* ---------------------------------------------------------------------------- */

new GlobalString[4096];

enum pickupInfo {
    pickupTitle[64],
    Float:pickupPosEnter_x,
    Float:pickupPosEnter_y,
    Float:pickupPosEnter_z,
    pickupEnterWorld,
    pickupEnterInt,
    Float:pickupPosExit_x,
    Float:pickupPosExit_y,
    Float:pickupPosExit_z,
    Float:pickupPosExit_a,
    pickupExitWorld,
    pickupExitInt,
    spWaitTime
}

new EnterExit[20][pickupInfo] = {
    /* title                                pickupX     pickupY     pickupZ     pW  pI  spX         spY         spZ         spA     spW spI spWT */
    { "Enter |> LS Cityhall",               1481.2112,  -1791.7321, 16.870900,  0,  0,  1492.6343,  -1755.4150, 1002.0271,  180.0,  1,  1 }, /* ,  1200 }, */
    { "Exit  |> LS Cityhall",               1492.6051,  -1751.5957, 1001.0270,  1,  1,  1481.2391,  -1788.7421, 16.870901,  0.000,  0,  0 }, /* ,  0    }, */
    { "Enter |> LS Shelter",                1422.5901,  -1357.3192, 13.569000,  0,  0,  1426.5902,  -1322.0219, 1402.3601,  90.00,  0,  0 }, /* ,  600  }, */
    { "Exit  |> LS Shelter",                1428.8618,  -1321.7955, 1401.3600,  0,  0,  1422.6200,  -1354.4949, 13.569701,  0.000,  0,  0 }, /* ,  0    }, */
    { "Enter |> LS Shelter > Kitchen",      1387.8817,  -1307.5225, 1401.3600,  0,  0,  1385.0074,  -1307.1311, 1401.3690,  90.00,  0,  0 }, /* ,  0    }, */
    { "Exit  |> LS Shelter > Kitchen",      1386.9188,  -1307.4744, 1401.3600,  0,  0,  1389.7002,  -1307.3315, 1401.3691,  270.0,  0,  0 }, /* ,  0    }, */
    { "Enter |> LS Airport: Floor 1",       1642.2299,  -2335.4983, 13.546900,  0,  0,  1420.1244,  -2459.6941, 1427.1637,  359.0,  1,  1 }, /* ,  600  }, */
    { "Exit  |> LS Airport: Floor 1",       1420.3859,  -2462.7915, 1426.1637,  1,  1,  1642.4410,  -2332.0564, 13.546901,  0.000,  0,  0 }, /* ,  0    }, */
    { "Enter |> LS Airport: Floor 2",       1685.8064,  -2335.4978, -2.679700,  0,  0,  1420.5846,  -2458.5417, 1412.1361,  0.000,  1,  1 }, /* ,  600  }, */
    { "Exit  |> LS Airport: Floor 2",       1420.3635,  -2462.7583, 1411.1361,  1,  1,  1685.8253,  -2331.1033, -2.679701,  0.000,  0,  0 }, /* ,  0    }, */
    { "Elevator |> LS Airport: Floor 2",    1446.6152,  -2443.8163, 1411.1361,  1,  1,  1443.3496,  -2443.7859, 1426.1637,  90.00,  1,  1 }, /* ,  0    }, */
    { "Elevator |> LS Airport: Floor 1",    1446.6144,  -2443.7725, 1426.1637,  1,  1,  1443.5192,  -2443.6497, 1411.1361,  90.00,  1,  1 }, /* ,  0    }, */
    { "Enter |> LS Airport: Corridor",      1420.4778,  -2431.3359, 1410.1361,  1,  1,  1381.8868,  -2441.8762, 1410.6903,  90.00,  1,  1 }, /* ,  0    }, */
    { "Exit  |> LS Airport: Corridor",      1384.0604,  -2441.6699, 1409.6903,  1,  1,  1420.6152,  -2432.9861, 1411.1361,  180.0,  1,  1 }, /* ,  0    }, */
    { "Enter |> LS Hospital",               2025.2507,  -1401.6702, 17.209800,  0,  0,  2037.6627,  -1431.2664, 1003.0001,  360.0,  1,  1 }, /* ,  1200 }, */
    { "Exit  |> LS Hospital",               2037.8293,  -1435.1671, 1001.0000,  1,  1,  2025.2344,  -1405.1561, 17.209701,  177.0,  0,  0 }, /* ,  0    }, */
    { "Enter |> SF Hospital",               -2637.458,  694.887500, 27.934600,  0,  0,  2037.6627,  -1431.2664, 1003.0001,  360.0,  1,  1 }, /* ,  1200 }, */
    { "Exit  |> SF Hospital",               2037.8293,  -1435.1671, 1001.0000,  2,  2,  2025.2344,  -1405.1561, 17.209701,  177.0,  0,  0 }, /* ,  0    }, */
    { "Enter |> LV Hospital",               2362.7864,  2306.30220, 8.1406000,  0,  0,  2037.6627,  -1431.2664, 1003.0001,  360.0,  1,  1 }, /* ,  1200 }, */
    { "Exit  |> LV Hospital",               2037.8293,  -1435.1671, 1001.0000,  3,  3,  2025.2344,  -1405.1561, 17.209701,  177.0,  0,  0 } /* ,  0    } */
};

new pickupCreate[sizeof(EnterExit)] = { -1, ... };

/* ---------------------------------------------------------------------------- */
/*      PUBLICS                                                                 */
/* ---------------------------------------------------------------------------- */

hook OnScriptInit() {
    #if !defined BRPSA_NO_LOG
        Logger_ToggleDebug("libs-picks", true);
    #endif
    #if !defined BRPSA_NO_INFO
        print("OnScriptInit(brpsa-libs > libs-picks)");
    #endif
    PickupHints();
    return true;
}

hook OnPlayerPickUpPickup(playerid, pickupid) {
    if (pickupid >= pickupCreate[0] && pickupid <= pickupCreate[sizeof(EnterExit) - 1]) {
        new pickup = pickupid - pickupCreate[0];
        SetPlayerFacingAngle(playerid, EnterExit[pickup][pickupPosExit_a]);
        SetPlayerPos(playerid, EnterExit[pickup][pickupPosExit_x], EnterExit[pickup][pickupPosExit_y], EnterExit[pickup][pickupPosExit_z]);
        // SetPlayerPosComp(playerid, EnterExit[pickup][pickupPosExit_x], EnterExit[pickup][pickupPosExit_y], EnterExit[pickup][pickupPosExit_z], EnterExit[pickup][spWaitTime]);
        SetPlayerInterior(playerid, EnterExit[pickup][pickupExitInt]);
        SetPlayerVirtualWorld(playerid, EnterExit[pickup][pickupExitWorld]);
        return true;
    }
    return true;
}

stock PickupHints() {
    for (new i; i < sizeof(EnterExit); i++) {
        GlobalString[0] = EOS;
        strcat(GlobalString, EnterExit[i][pickupTitle]), strcat(GlobalString, "\n");
        CreateDynamic3DTextLabel(GlobalString, -1, EnterExit[i][pickupPosEnter_x], EnterExit[i][pickupPosEnter_y], EnterExit[i][pickupPosEnter_z], 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, EnterExit[i][pickupEnterWorld], EnterExit[i][pickupEnterInt]);
        pickupCreate[i] = CreatePickup(19130, 1, EnterExit[i][pickupPosEnter_x], EnterExit[i][pickupPosEnter_y], EnterExit[i][pickupPosEnter_z], EnterExit[i][pickupEnterWorld]);
    }
}

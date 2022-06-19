/* =============================================================================
 | Blefonix Metaverse hub.blefonix.com
 | By Nazar Kornienko and Petro Melnyk
 ============================================================================ */

#if defined _inc_SVINCLUDE
	#undef _inc_SVINCLUDE
#endif

/* -----------------------------------------------------------------------------
 | TABLE OF CONTENT:
 | CONSTANTS / INCLUDES / LIBRARIES / FORWARDS (DEFINING)
 | ENUMERATIONS / VARIABLES / TIMERS / STOCKS / CONDITIONS / PUBLICS
 | FUNCTIONS / FORWARDS (CALLING) / INCLUDES / MODULES / DIALOGS / COMMANDS
 ---------------------------------------------------------------------------- */

#if defined _SVINCLUDE_included
	#endinput
#endif

/* -----------------------------------------------------------------------------
 | CONSTANTS
 ---------------------------------------------------------------------------- */

#define _SVINCLUDE_included

/* -----------------------------------------------------------------------------
 | INCLUDES
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | LIBRARIES
 ---------------------------------------------------------------------------- */

#include <YSI_Coding\y_hooks>

/* -----------------------------------------------------------------------------
 | FORWARDS (DEFINING)
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | ENUMERATIONS
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | VARIABLES
 ---------------------------------------------------------------------------- */

new SV_GSTREAM:gstream = SV_NULL;
new SV_LSTREAM:lstream[MAX_PLAYERS] = { SV_NULL, ... };

/* -----------------------------------------------------------------------------
 | TIMERS
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | STOCKS
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | CONDITIONS
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | PUBLICS
 ---------------------------------------------------------------------------- */

public SV_VOID:OnPlayerActivationKeyPress(SV_UINT:playerid, SV_UINT:keyid) {
	// The public OnPlayerActivationKeyPress and OnPlayerActivationKeyRelease
	// are needed in order to redirect the player's audio traffic to the
	// corresponding streams when the corresponding keys are pressed.
    // == === === === === === === === === === === === === === === ==
	// Attach player to local stream as speaker if 'B' key is pressed
    if (keyid == 0x42 && lstream[playerid]) SvAttachSpeakerToStream(lstream[playerid], playerid);
    // Attach the player to the global stream as a speaker if the 'Z' key is pressed
    if (keyid == 0x5A && gstream) SvAttachSpeakerToStream(gstream, playerid);
	return true;
}

public SV_VOID:OnPlayerActivationKeyRelease(SV_UINT:playerid, SV_UINT:keyid) {
	// The public OnPlayerActivationKeyPress and OnPlayerActivationKeyRelease
	// are needed in order to redirect the player's audio traffic to the
	// corresponding streams when the corresponding keys are pressed.
    // == === === === === === === === === === === === === === === ==
    // Detach the player from the local stream if the 'B' key is released
    if (keyid == 0x42 && lstream[playerid]) SvDetachSpeakerFromStream(lstream[playerid], playerid);
    // Detach the player from the global stream if the 'Z' key is released
    if (keyid == 0x5A && gstream) SvDetachSpeakerFromStream(gstream, playerid);
	return true;
}

hook OnGameModeInit() {
    // SvDebug(SV_TRUE); // Uncom for debug mode
	// == === === === === === === === === === ==
    gstream = SvCreateGStream(0xffff0000, "Global");
	return true;
}

hook OnGameModeExit() {
    if (gstream) SvDeleteStream(gstream);
	return true;
}

hook OnPlayerConnect(playerid) {
    // Checking for plugin availability
    if (SvGetVersion(playerid) == SV_NULL) {
        SendClientMessage(playerid, -1, "Could not find plugin sampvoice.");
    }
    // Checking for a microphone
    else if (SvHasMicro(playerid) == SV_FALSE) {
        SendClientMessage(playerid, -1, "The microphone could not be found.");
    }
    // Create a local stream with an audibility distance of 40.0, an unlimited number of listeners
    // and the name 'Local' (the name 'Local' will be displayed in red in the players' speakerlist)
    else if ((lstream[playerid] = SvCreateDLStreamAtPlayer(40.0, SV_INFINITY, playerid, 0xff0000ff, "Local"))) {
        SendClientMessage(playerid, -1, "Press B to talk to local chat and Z to talk to global chat.");
        // Attach the player to the global stream as a listener
        if (gstream) SvAttachListenerToStream(gstream, playerid);
        // Assign microphone activation keys to the player
        SvAddKey(playerid, 0x42);
        SvAddKey(playerid, 0x5A);
    }
	return true;
}

hook OnPlayerDisconnect(playerid, reason) {
    // Removing the player's local stream after disconnecting
    if (lstream[playerid]) {
        SvDeleteStream(lstream[playerid]);
        lstream[playerid] = SV_NULL;
    }
	return true;
}

/* -----------------------------------------------------------------------------
 | FUNCTIONS
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | FORWARDS (CALLING)
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | INCLUDES
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | MODULES
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | DIALOGS
 ---------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 | COMMANDS
 ---------------------------------------------------------------------------- */

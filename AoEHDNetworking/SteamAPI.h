#pragma once

#include "steam/steam_api.h"
#include "steam/isteamclient017.h"

#include <cstddef>

class CSteamAPI {
public:
    CSteamAPI();

    bool (S_CALLTYPE* Init)();
    void (S_CALLTYPE* Shutdown)();
    bool (S_CALLTYPE* RestartAppIfNecessary)(uint32 unOwnAppID);
    void (S_CALLTYPE* RunCallbacks)();
    void (S_CALLTYPE* RegisterCallback)(class CCallbackBase* pCallback, int iCallback);
    void (S_CALLTYPE* UnregisterCallback)(class CCallbackBase* pCallback);
    void (S_CALLTYPE* RegisterCallResult)(class CCallbackBase* pCallback, SteamAPICall_t hAPICall);
    void (S_CALLTYPE* UnregisterCallResult)(class CCallbackBase* pCallback, SteamAPICall_t hAPICall);
    void (S_CALLTYPE* WriteMiniDump)(uint32 uStructuredExceptionCode, void* pvExceptionInfo, uint32 uBuildID);

    HSteamPipe (S_CALLTYPE* GetHSteamPipe)();
    HSteamUser (S_CALLTYPE* GetHSteamUser)();

    void* (S_CALLTYPE* SteamUtils)();
    void* (S_CALLTYPE* SteamUserStats)();
    void* (S_CALLTYPE* SteamMatchmaking)();
    void* (S_CALLTYPE* SteamFriends)();
    void* (S_CALLTYPE* SteamUser)();
    void* (S_CALLTYPE* SteamApps)();
    void* (S_CALLTYPE* SteamUGC)();
    ISteamClient017* (S_CALLTYPE* SteamClient)();
    //void* (S_CALLTYPE* SteamNetworking)();

    ISteamNetworkingMessages* SteamNetworkingMessages();
    ISteamNetworkingUtils* SteamNetworkingUtils();
};

CSteamAPI* SteamAPI();

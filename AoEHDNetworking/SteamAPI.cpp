#include "SteamAPI.h"
#include "SteamNetworking.h"
#include "CallbackManager.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static void FatalError(const char* msg) {
    MessageBoxA(NULL, msg, "Fatal error", MB_OK | MB_ICONERROR);
    ExitProcess(1);
}

CSteamAPI* SteamAPI() {
    static CSteamAPI* instance = nullptr;
    if (instance == nullptr) {
        instance = new CSteamAPI();
    }
    return instance;
}

CSteamAPI::CSteamAPI() {
    HMODULE mod = LoadLibraryA("valve_api.dll");
    if (mod == NULL) {
        FatalError("Couldn't load valve_api.dll");
        return;
    }

    Init = (bool (S_CALLTYPE*)())GetProcAddress(mod, "SteamAPI_Init");
    Shutdown = (void (S_CALLTYPE*)())GetProcAddress(mod, "SteamAPI_Shutdown");
    RestartAppIfNecessary = (bool (S_CALLTYPE*)(uint32))GetProcAddress(mod, "SteamAPI_RestartAppIfNecessary");
    RunCallbacks = (void (S_CALLTYPE*)())GetProcAddress(mod, "SteamAPI_RunCallbacks");
    RegisterCallback = (void (S_CALLTYPE*)(class CCallbackBase*, int))GetProcAddress(mod, "SteamAPI_RegisterCallback");
    UnregisterCallback = (void (S_CALLTYPE*)(class CCallbackBase*))GetProcAddress(mod, "SteamAPI_UnregisterCallback");
    RegisterCallResult = (void (S_CALLTYPE*)(class CCallbackBase*, SteamAPICall_t))GetProcAddress(mod, "SteamAPI_RegisterCallResult");
    UnregisterCallResult = (void (S_CALLTYPE*)(class CCallbackBase*, SteamAPICall_t))GetProcAddress(mod, "SteamAPI_UnregisterCallResult");
    WriteMiniDump = (void (S_CALLTYPE*)(uint32, void*, uint32))GetProcAddress(mod, "SteamAPI_WriteMiniDump");

    GetHSteamPipe = (HSteamPipe(S_CALLTYPE*)())GetProcAddress(mod, "SteamAPI_GetHSteamPipe");
    GetHSteamUser = (HSteamUser(S_CALLTYPE*)())GetProcAddress(mod, "SteamAPI_GetHSteamUser");

    SteamUtils = (void* (S_CALLTYPE*)())GetProcAddress(mod, "SteamUtils");
    SteamUserStats = (void* (S_CALLTYPE*)())GetProcAddress(mod, "SteamUserStats");
    SteamMatchmaking = (void* (S_CALLTYPE*)())GetProcAddress(mod, "SteamMatchmaking");
    SteamFriends = (void* (S_CALLTYPE*)())GetProcAddress(mod, "SteamFriends");
    SteamUser = (void* (S_CALLTYPE*)())GetProcAddress(mod, "SteamUser");
    SteamApps = (void* (S_CALLTYPE*)())GetProcAddress(mod, "SteamApps");
    SteamUGC = (void* (S_CALLTYPE*)())GetProcAddress(mod, "SteamUGC");
    SteamClient = (ISteamClient017* (S_CALLTYPE*)())GetProcAddress(mod, "SteamClient");
    //SteamNetworking = (void* (S_CALLTYPE*)())GetProcAddress(mod, "SteamNetworking");
}

ISteamNetworkingMessages* CSteamAPI::SteamNetworkingMessages() {
    static ISteamNetworkingMessages* instance = nullptr;
    if (instance == nullptr) {
        instance = (ISteamNetworkingMessages*)SteamClient()->GetISteamGenericInterface(
            SteamAPI_GetHSteamPipe(), SteamAPI_GetHSteamUser(), STEAMNETWORKINGMESSAGES_INTERFACE_VERSION);
    }
    return instance;
}

ISteamNetworkingUtils* CSteamAPI::SteamNetworkingUtils() {
    static ISteamNetworkingUtils* instance = nullptr;
    if (instance == nullptr) {
        instance = (ISteamNetworkingUtils*)SteamClient()->GetISteamGenericInterface(
            SteamAPI_GetHSteamPipe(), SteamAPI_GetHSteamUser(), STEAMNETWORKINGUTILS_INTERFACE_VERSION);
    }
    return instance;
}

S_API bool S_CALLTYPE SteamAPI_Init() {
    return SteamAPI()->Init();
}

S_API void S_CALLTYPE SteamAPI_Shutdown() {
    SteamAPI()->Shutdown();
}

S_API bool S_CALLTYPE SteamAPI_RestartAppIfNecessary(uint32 unOwnAppID) {
    return SteamAPI()->RestartAppIfNecessary(unOwnAppID);
}

S_API void S_CALLTYPE SteamAPI_RunCallbacks() {
    SteamAPI()->RunCallbacks();
    CallbackManager()->Run();
}

S_API void S_CALLTYPE SteamAPI_RegisterCallback(class CCallbackBase* pCallback, int iCallback) {
    switch (iCallback) {
    case P2PSessionRequest_t::k_iCallback:
    case P2PSessionConnectFail_t::k_iCallback:
        CallbackManager()->Register(pCallback, iCallback);
        return;
    }

    SteamAPI()->RegisterCallback(pCallback, iCallback);
}

S_API void S_CALLTYPE SteamAPI_UnregisterCallback(class CCallbackBase* pCallback) {
    switch (pCallback->GetICallback()) {
    case P2PSessionRequest_t::k_iCallback:
    case P2PSessionConnectFail_t::k_iCallback:
        CallbackManager()->Unregister(pCallback);
        return;
    }

    SteamAPI()->UnregisterCallback(pCallback);
}

S_API void S_CALLTYPE SteamAPI_RegisterCallResult(class CCallbackBase* pCallback, SteamAPICall_t hAPICall) {
    SteamAPI()->RegisterCallResult(pCallback, hAPICall);
}

S_API void S_CALLTYPE SteamAPI_UnregisterCallResult(class CCallbackBase* pCallback, SteamAPICall_t hAPICall) {
    SteamAPI()->UnregisterCallResult(pCallback, hAPICall);
}

S_API void S_CALLTYPE SteamAPI_WriteMiniDump(uint32 uStructuredExceptionCode, void* pvExceptionInfo, uint32 uBuildID) {
    SteamAPI()->WriteMiniDump(uStructuredExceptionCode, pvExceptionInfo, uBuildID);
}

S_API HSteamPipe S_CALLTYPE SteamAPI_GetHSteamPipe() {
    return SteamAPI()->GetHSteamPipe();
}

S_API HSteamUser S_CALLTYPE SteamAPI_GetHSteamUser() {
    return SteamAPI()->GetHSteamUser();
}

S_API void* S_CALLTYPE SteamUtils() {
    return SteamAPI()->SteamUtils();
}

S_API void* S_CALLTYPE SteamUserStats() {
    return SteamAPI()->SteamUserStats();
}

S_API void* S_CALLTYPE SteamMatchmaking() {
    return SteamAPI()->SteamMatchmaking();
}

S_API void* S_CALLTYPE SteamFriends() {
    return SteamAPI()->SteamFriends();
}

S_API void* S_CALLTYPE SteamUser() {
    return SteamAPI()->SteamUser();
}

S_API void* S_CALLTYPE SteamApps() {
    return SteamAPI()->SteamApps();
}

S_API void* S_CALLTYPE SteamUGC() {
    return SteamAPI()->SteamUGC();
}

S_API void* S_CALLTYPE SteamClient() {
    return SteamAPI()->SteamClient();
}

S_API void* S_CALLTYPE SteamNetworking() {
    static void* instance = nullptr;
    if (instance == nullptr) {
        instance = (void*)new CSteamNetworking;
    }
    return instance;
}

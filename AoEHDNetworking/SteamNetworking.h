#pragma once

#include "SteamAPI.h"
#include "steam/isteamnetworking005.h"

class CSteamNetworking : public ISteamNetworking005 {
public:
    STEAM_CALLBACK(CSteamNetworking, OnSessionRequest, SteamNetworkingMessagesSessionRequest_t);
    STEAM_CALLBACK(CSteamNetworking, OnSessionFailed, SteamNetworkingMessagesSessionFailed_t);

    CSteamNetworking();
    virtual ~CSteamNetworking();

    bool SendP2PPacket(CSteamID steamIDRemote, const void* pubData, uint32 cubData, EP2PSend eP2PSendType, int nChannel = 0);
    bool IsP2PPacketAvailable(uint32* pcubMsgSize, int nChannel = 0);
    bool ReadP2PPacket(void* pubDest, uint32 cubDest, uint32* pcubMsgSize, CSteamID* psteamIDRemote, int nChannel = 0);
    bool AcceptP2PSessionWithUser(CSteamID steamIDRemote);
    bool CloseP2PSessionWithUser(CSteamID steamIDRemote);
    bool CloseP2PChannelWithUser(CSteamID steamIDRemote, int nChannel);
    bool GetP2PSessionState(CSteamID steamIDRemote, P2PSessionState_t* pConnectionState);
    bool AllowP2PPacketRelay(bool bAllow);
    
    SNetListenSocket_t CreateListenSocket(int nVirtualP2PPort, uint32 nIP, uint16 nPort, bool bAllowUseOfPacketRelay) {
        return 0;
    }

    SNetSocket_t CreateP2PConnectionSocket(CSteamID steamIDTarget, int nVirtualPort, int nTimeoutSec, bool bAllowUseOfPacketRelay) {
        return 0;
    }

    SNetSocket_t CreateConnectionSocket(uint32 nIP, uint16 nPort, int nTimeoutSec) {
        return 0;
    }

    bool DestroySocket(SNetSocket_t hSocket, bool bNotifyRemoteEnd) {
        return false;
    }

    bool DestroyListenSocket(SNetListenSocket_t hSocket, bool bNotifyRemoteEnd) {
        return false;
    }

    bool SendDataOnSocket(SNetSocket_t hSocket, void* pubData, uint32 cubData, bool bReliable) {
        return false;
    }

    bool IsDataAvailableOnSocket(SNetSocket_t hSocket, uint32* pcubMsgSize) {
        return false;
    }

    bool RetrieveDataFromSocket(SNetSocket_t hSocket, void* pubDest, uint32 cubDest, uint32* pcubMsgSize) {
        return false;
    }

    bool IsDataAvailable(SNetListenSocket_t hListenSocket, uint32* pcubMsgSize, SNetSocket_t* phSocket) {
        return false;
    }

    bool RetrieveData(SNetListenSocket_t hListenSocket, void* pubDest, uint32 cubDest, uint32* pcubMsgSize, SNetSocket_t* phSocket) {
        return false;
    }

    bool GetSocketInfo(SNetSocket_t hSocket, CSteamID* pSteamIDRemote, int* peSocketStatus, uint32* punIPRemote, uint16* punPortRemote) {
        return false;
    }

    bool GetListenSocketInfo(SNetListenSocket_t hListenSocket, uint32* pnIP, uint16* pnPort) {
        return false;
    }

    ESNetSocketConnectionType GetSocketConnectionType(SNetSocket_t hSocket) {
        return k_ESNetSocketConnectionTypeNotConnected;
    }

    int GetMaxPacketSize(SNetSocket_t hSocket) {
        return 0;
    }

private:
    SteamNetworkingMessage_t* pendingMessage;
};

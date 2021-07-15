#include "SteamNetworking.h"
#include "Log.h"
#include "CallbackManager.h"

CSteamNetworking::CSteamNetworking() : pendingMessage(nullptr) {
    SteamAPI()->SteamNetworkingUtils()->SetGlobalConfigValueInt32(k_ESteamNetworkingConfig_P2P_Transport_ICE_Enable, k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_Relay);
    SteamAPI()->SteamNetworkingUtils()->InitRelayNetworkAccess();
}

CSteamNetworking::~CSteamNetworking() {
    if (pendingMessage != nullptr) {
        pendingMessage->Release();
        pendingMessage = nullptr;
    }
}

bool CSteamNetworking::SendP2PPacket(CSteamID steamIDRemote, const void* pubData, uint32 cubData, EP2PSend eP2PSendType, int nChannel) {
    SteamNetworkingIdentity identity;
    identity.SetSteamID(steamIDRemote);
    auto result = SteamAPI()->SteamNetworkingMessages()->SendMessageToUser(
        identity, pubData, cubData, k_nSteamNetworkingSend_Reliable | k_nSteamNetworkingSend_AutoRestartBrokenSession, nChannel);
    return result == k_EResultOK;
    return true;
}

bool CSteamNetworking::IsP2PPacketAvailable(uint32* pcubMsgSize, int nChannel) {
    if (pendingMessage != nullptr) {
        *pcubMsgSize = pendingMessage->GetSize();
        return true;
    }

    if (SteamAPI()->SteamNetworkingMessages()->ReceiveMessagesOnChannel(nChannel, &pendingMessage, 1) != 1) {
        return false;
    }

    *pcubMsgSize = pendingMessage->GetSize();
    return true;
}

bool CSteamNetworking::ReadP2PPacket(void* pubDest, uint32 cubDest, uint32* pcubMsgSize, CSteamID* psteamIDRemote, int nChannel) {
    uint32 msgSize;
    if (!IsP2PPacketAvailable(&msgSize, nChannel)) {
        return false;
    }

    memcpy(pubDest, pendingMessage->GetData(), std::min(msgSize, cubDest));

    if (pcubMsgSize != nullptr) {
        *pcubMsgSize = msgSize;
    }

    if (psteamIDRemote != nullptr) {
        *psteamIDRemote = pendingMessage->m_identityPeer.GetSteamID();
    }

    pendingMessage->Release();
    pendingMessage = nullptr;
    return true;
}

bool CSteamNetworking::AcceptP2PSessionWithUser(CSteamID steamIDRemote) {
    SteamNetworkingIdentity identity;
    identity.SetSteamID(steamIDRemote);
    return SteamAPI()->SteamNetworkingMessages()->AcceptSessionWithUser(identity);
}

bool CSteamNetworking::CloseP2PSessionWithUser(CSteamID steamIDRemote) {
    SteamNetworkingIdentity identity;
    identity.SetSteamID(steamIDRemote);
    return SteamAPI()->SteamNetworkingMessages()->CloseSessionWithUser(identity);
}

bool CSteamNetworking::CloseP2PChannelWithUser(CSteamID steamIDRemote, int nChannel) {
    SteamNetworkingIdentity identity;
    identity.SetSteamID(steamIDRemote);
    return SteamAPI()->SteamNetworkingMessages()->CloseChannelWithUser(identity, nChannel);
}

bool CSteamNetworking::GetP2PSessionState(CSteamID steamIDRemote, P2PSessionState_t* pConnectionState) {
    SteamNetworkingIdentity identity;
    identity.SetSteamID(steamIDRemote);
    SteamNetConnectionInfo_t info;
    auto state = SteamAPI()->SteamNetworkingMessages()->GetSessionConnectionInfo(identity, &info, nullptr);

    pConnectionState->m_bConnectionActive = state == k_ESteamNetworkingConnectionState_Connected;
    pConnectionState->m_bConnecting = state == k_ESteamNetworkingConnectionState_Connecting || state == k_ESteamNetworkingConnectionState_FindingRoute;
    pConnectionState->m_eP2PSessionError = k_EP2PSessionErrorNone;
    pConnectionState->m_bUsingRelay = info.m_idPOPRelay != 0;
    pConnectionState->m_nBytesQueuedForSend = 0;
    pConnectionState->m_nPacketsQueuedForSend = 0;
    pConnectionState->m_nRemoteIP = 0;
    pConnectionState->m_nRemotePort = 0;

    return true;
}

bool CSteamNetworking::AllowP2PPacketRelay(bool bAllow) {
    return SteamAPI()->SteamNetworkingUtils()->SetGlobalConfigValueInt32(k_ESteamNetworkingConfig_P2P_Transport_ICE_Enable,
        bAllow ? k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_Relay : k_nSteamNetworkingConfig_P2P_Transport_ICE_Enable_Disable);
    return true;
}

void CSteamNetworking::OnSessionRequest(SteamNetworkingMessagesSessionRequest_t* pParam) {
    P2PSessionRequest_t param;
    param.m_steamIDRemote = pParam->m_identityRemote.GetSteamID();
    CallbackManager()->Queue(P2PSessionRequest_t::k_iCallback, &param, sizeof(param));
}

void CSteamNetworking::OnSessionFailed(SteamNetworkingMessagesSessionFailed_t* pParam) {
    P2PSessionConnectFail_t param;
    param.m_steamIDRemote = pParam->m_info.m_identityRemote.GetSteamID();
    param.m_eP2PSessionError = k_EP2PSessionErrorTimeout;
    CallbackManager()->Queue(P2PSessionConnectFail_t::k_iCallback, &param, sizeof(param));
}

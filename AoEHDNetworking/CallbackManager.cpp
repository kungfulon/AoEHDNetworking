#include "CallbackManager.h"

CCallbackMgr* CallbackManager() {
    static CCallbackMgr* callbackManager = nullptr;
    if (callbackManager == nullptr) {
        callbackManager = new CCallbackMgr;
    }
    return callbackManager;
}

void CCallbackMgr::Register(CCallbackBase* callback, int callbackID) {
    callback->m_iCallback = callbackID;
    list[callbackID].insert(callback);
}

void CCallbackMgr::Unregister(CCallbackBase* callback) {
    list[callback->GetICallback()].erase(callback);
}

void CCallbackMgr::Queue(int callbackID, void* data, size_t dataSize) {
    queue.emplace(callbackID, data, dataSize);
}

void CCallbackMgr::Run() {
    while (!queue.empty()) {
        auto& p = queue.front();
        for (const auto& cb : list[p.callbackID]) {
            cb->Run((void*)p.data.data());
        }
        queue.pop();
    }
}

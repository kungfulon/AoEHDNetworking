#pragma once

#include "SteamAPI.h"

#include <queue>
#include <memory>
#include <unordered_set>
#include <unordered_map>

struct CallbackData {
    CallbackData(int callbackID, void* data, size_t dataSize)
        : callbackID(callbackID), data((char *)data, dataSize) {}
    int callbackID;
    std::string data;
};

typedef std::queue<CallbackData> CallbackQueue;
typedef std::unordered_map<int, std::unordered_set<CCallbackBase*>> CallbackList;

class CCallbackMgr {
public:
    void Register(CCallbackBase* callback, int callbackID);
    void Unregister(CCallbackBase* callback);
    void Queue(int callbackID, void* data, size_t dataSize);
    void Run();
private:
    CallbackQueue queue;
    CallbackList list;
};

CCallbackMgr* CallbackManager();

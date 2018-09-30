//
//  LockEvent.h
//  LockEvent
//
//  Created by Роман on 30.09.18.
//
//

#ifndef LockEvent_h
#define LockEvent_h

#include "../AddInNative.h"

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

namespace infostart_event_2018_lockevent
{
    
#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

class LockStateWrapper
{
private:
    
    LockStateWrapper();
    ~LockStateWrapper();
    
    void addEvent(const wchar_t* event);
    
    bool active;
    AddinNative *adcomp;
    int notify_token;

    
public:

    static LockStateWrapper* getInstance()
    {
        static LockStateWrapper* p_instance;
        if (!p_instance) {
            p_instance = new LockStateWrapper();
        }
        
        return p_instance;
    }
    
    void AddComponent(AddinNative* comp);
    void Activate(bool state);
    bool isActive();
    void addLockEvent();
    void addUnlockEvent();
    void deleteListener();
};

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)
    
}

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

#endif /* LockEvent_h */

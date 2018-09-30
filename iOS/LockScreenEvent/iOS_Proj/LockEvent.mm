//
//  LockEvent.m
//  LockEvent
//
//  Created by Роман on 30.09.18.
//
//


#include "LockEvent.h"
#import <Foundation/Foundation.h>
#import <notify.h>


#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

namespace infostart_event_2018_lockevent
{
    
#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY
    
    void LockStateWrapper::deleteListener()
    {
        if (notify_token != 0) {
            notify_cancel(notify_token);
        }
    }
    
    void LockStateWrapper::addEvent(const wchar_t * event)
    {
        if (!adcomp) {
            return;
        }
        adcomp->addEvent(event, NULL);
    }
    
    void LockStateWrapper::addLockEvent()
    {
        addEvent(L"Lock");
    }
    
    void LockStateWrapper::addUnlockEvent()
    {
        addEvent(L"Unlock");
    }
    
    void LockStateWrapper::AddComponent(AddinNative * comp)
    {
        adcomp = comp;
    }
    
    LockStateWrapper::LockStateWrapper(): active(false), notify_token(0)
    {
    }
    
    LockStateWrapper::~LockStateWrapper()
    {
        adcomp = NULL;
    }
    
    bool LockStateWrapper::isActive()
    {
        return active;
    }
    
    void LockStateWrapper::Activate(bool state)
    {
        deleteListener();
        
        if (state) {
            
            notify_register_dispatch("com.apple.springboard.lockstate",
                                     &notify_token,
                                     dispatch_get_main_queue(),
                                     ^ (int token)
                   {
                       uint64_t state = 0;
                       notify_get_state(token, &state);
                       
                       LockStateWrapper * inst = LockStateWrapper::getInstance();
                       
                       if (state == 0) {
                           
                           inst->addUnlockEvent();
                           
                       } else {
                           
                           inst->addLockEvent();
                           
                       }
                       
                   });
        }
        
        active = state;
    }
    
#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)
    
}

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY


#include "AddInNative.h"
#include "ConversionWchar.h"
#include "include/mobile.h"
#if defined(__APPLE__)||defined(__ANDROID__)

#include <wchar.h>
#include "iOS_Proj/LockEvent.h"

#endif

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

namespace infostart_event_2018_lockevent
{

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

    static const wchar_t *g_MethodNames[] =
    {L"ActivateListener"};
    
    static const wchar_t *g_MethodNamesRu[] =
    {L"АктивироватьСлежение"};
    
    static const wchar_t *g_PropNames[] =
    {L"IsActive"};
    
    static const wchar_t *g_PropNamesRu[] =
    {L"Активно"};
    

static const wchar_t g_kClassNames[] = L"AddinNative"; //|OtherClass1|OtherClass2";

static WcharWrapper s_kClassNames(g_kClassNames);

static const wchar_t subSystem[] = L"LockEventListener";
static const wchar_t g_ComponentNameType[] = L"LockEventExtension";

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

static const char g_ComponentName[] = "infostart_event_2018_lockevent";

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

// This component supports 2.1 version
const long g_VersionAddIn = 2100;

static AppCapabilities g_capabilities = eAppCapabilitiesInvalid;

//---------------------------------------------------------------------------//
long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
    #if defined(__APPLE__)||defined(__ANDROID__)
    if(!*pInterface)
    {
        *pInterface = new AddinNative();
        return (long)*pInterface;
    }
    #endif
    return 0;
}
    
//---------------------------------------------------------------------------//
AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities)
{
    g_capabilities = capabilities;
    return eAppCapabilitiesLast;
}
    
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pInterface)
{

    if(!*pInterface)
        return -1;

    delete *pInterface;
    *pInterface = 0;

    return 0;
}
    
//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
    return s_kClassNames;
}
//---------------------------------------------------------------------------//

AddinNative::AddinNative()
{
}
//---------------------------------------------------------------------------//
AddinNative::~AddinNative()
{
    stateWrapper->deleteListener();
    stateWrapper = NULL;
}
//---------------------------------------------------------------------------//
bool AddinNative::Init(void* pConnection)
{
#if defined(__APPLE__) || defined(__ANDROID__)
    m_iConnect = (IAddInDefBaseEx*)pConnection;
    if (m_iConnect)
    {
        stateWrapper = LockStateWrapper::getInstance();
        stateWrapper->AddComponent(this);
        return true;
    }
#endif
    return false;
}
//---------------------------------------------------------------------------//
long AddinNative::GetInfo()
{ 
    return g_VersionAddIn;
}
//---------------------------------------------------------------------------//
void AddinNative::Done()
{

}
//---------------------------------------------------------------------------//
bool AddinNative::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{ 
    const wchar_t *wsExtension = g_ComponentNameType;
    uint32_t iActualSize = static_cast<uint32_t>(::wcslen(wsExtension) + 1);
    
    if (m_iMemory)
    {
        if(m_iMemory->AllocMemory((void**)wsExtensionName, iActualSize * sizeof(WCHAR_T)))
        {
            convToShortWchar(wsExtensionName, wsExtension, iActualSize);
            return true;
        }
    }
    
    return true;
}
//---------------------------------------------------------------------------//
long AddinNative::GetNProps()
{ 
    return eLastProp;
}
//---------------------------------------------------------------------------//
long AddinNative::FindProp(const WCHAR_T* wsPropName)
{ 
    long plPropNum = -1;
    wchar_t* propName = 0;
    convFromShortWchar(&propName, wsPropName);
    
    plPropNum = findName(g_PropNames, propName, eLastProp);
    
    if (plPropNum == -1)
        plPropNum = findName(g_PropNamesRu, propName, eLastProp);
    
    delete[] propName;
    
    return plPropNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* AddinNative::GetPropName(long lPropNum, long lPropAlias)
{ 
    if (lPropNum >= eLastProp)
        return NULL;
    
    wchar_t *wsCurrentName = NULL;
    WCHAR_T *wsPropName = NULL;
    
    switch(lPropAlias)
    {
        case 0: // First language (english)
            wsCurrentName = (wchar_t*)g_PropNames[lPropNum];
            break;
        case 1: // Second language (local)
            wsCurrentName = (wchar_t*)g_PropNamesRu[lPropNum];
            break;
        default:
            return 0;
    }
    
    uint32_t iActualSize = static_cast<uint32_t>(wcslen(wsCurrentName) + 1);
    
    if (m_iMemory && wsCurrentName)
    {
        if (m_iMemory->AllocMemory((void**)&wsPropName, iActualSize * sizeof(WCHAR_T)))
            convToShortWchar(&wsPropName, wsCurrentName, iActualSize);
    }
    
    return wsPropName;
}
//---------------------------------------------------------------------------//
bool AddinNative::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{
    if (ePropIsActive == lPropNum) {
        TV_VT(pvarPropVal) = VTYPE_BOOL;
        TV_BOOL(pvarPropVal) = stateWrapper->isActive();
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------//
bool AddinNative::SetPropVal(const long lPropNum, tVariant* varPropVal)
{ 
    return false;
}
//---------------------------------------------------------------------------//
bool AddinNative::IsPropReadable(const long lPropNum)
{
    if (ePropIsActive == lPropNum) {
        return true;
    }
    return false;
}
//---------------------------------------------------------------------------//
bool AddinNative::IsPropWritable(const long lPropNum)
{
    return false;
}
//---------------------------------------------------------------------------//
long AddinNative::GetNMethods()
{ 
    return eLastMethod;
}
//---------------------------------------------------------------------------//
long AddinNative::FindMethod(const WCHAR_T* wsMethodName)
{ 
    long plMethodNum = -1;
    wchar_t* name = 0;
    convFromShortWchar(&name, wsMethodName);
    
    plMethodNum = findName(g_MethodNames, name, eLastMethod);
    
    if (plMethodNum == -1)
        plMethodNum = findName(g_MethodNamesRu, name, eLastMethod);
    
    delete[] name;
    
    return plMethodNum;
}
//---------------------------------------------------------------------------//
const WCHAR_T* AddinNative::GetMethodName(const long lMethodNum,
                            const long lMethodAlias)
{ 
    if (lMethodNum >= eLastMethod)
        return NULL;
    
    wchar_t *wsCurrentName = NULL;
    WCHAR_T *wsMethodName = NULL;
    
    switch(lMethodAlias)
    {
        case 0: // First language (english)
            wsCurrentName = (wchar_t*)g_MethodNames[lMethodNum];
            break;
        case 1: // Second language (local)
            wsCurrentName = (wchar_t*)g_MethodNamesRu[lMethodNum];
            break;
        default:
            return 0;
    }
    
    uint32_t iActualSize = static_cast<uint32_t>(wcslen(wsCurrentName) + 1);
    
    if (m_iMemory && wsCurrentName)
    {
        if(m_iMemory->AllocMemory((void**)&wsMethodName, iActualSize * sizeof(WCHAR_T)))
            convToShortWchar(&wsMethodName, wsCurrentName, iActualSize);
    }
    
    return wsMethodName;
}
//---------------------------------------------------------------------------//
long AddinNative::GetNParams(const long lMethodNum)
{
    long nParams = 0;

    if (eMethActivate == lMethodNum) {
        nParams = 1;
    }
    
    return nParams;
}
//---------------------------------------------------------------------------//
bool AddinNative::GetParamDefValue(const long lMethodNum, const long lParamNum,
                          tVariant *pvarParamDefValue)
{ 
    return false;
} 
//---------------------------------------------------------------------------//
bool AddinNative::HasRetVal(const long lMethodNum)
{
    return false;
}
//---------------------------------------------------------------------------//
bool AddinNative::CallAsProc(const long lMethodNum,
                    tVariant* paParams, const long lSizeArray)
{
    bool result = false;

    if (lMethodNum == eMethActivate) {
        stateWrapper->Activate(TV_BOOL(paParams));
        result = true;
    }
    
    return result;
}
//---------------------------------------------------------------------------//
bool AddinNative::CallAsFunc(const long lMethodNum,
                tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{
    return false;
}
//---------------------------------------------------------------------------//
void AddinNative::SetLocale(const WCHAR_T* loc)
{
}
//---------------------------------------------------------------------------//
bool AddinNative::setMemManager(void* mem)
{
    m_iMemory = (IMemoryManager*)mem;
    return m_iMemory != 0;
}
//---------------------------------------------------------------------------//
    
//---------------------------------------------------------------------------//
long AddinNative::findName(const wchar_t* names[], const wchar_t* name,
                               const uint32_t size) const
{
    long ret = -1;
    for (uint32_t i = 0; i < size; i++)
    {
        if (!wcscmp(names[i], name))
        {
            ret = i;
            break;
        }
    }
    return ret;
}
    
void AddinNative::addError(uint32_t wcode, const wchar_t* descriptor, uint32_t len)
{
    if (m_iConnect)
    {
        WCHAR_T *err = 0;
        WCHAR_T *descr = 0;
            
        convToShortWchar(&err, g_kClassNames);
        convToShortWchar(&descr, descriptor, len);
            
        m_iConnect->AddError(wcode, err, descr, ADDIN_E_FAIL);
            
        delete[] descr;
        delete[] err;
    }
}
    
//---------------------------------------------------------------------------//
bool AddinNative::addEvent(const wchar_t *eventName, const wchar_t* param)
{
    
    if (m_iConnect && m_iMemory)
    {
        WCHAR_T *params = 0;
        WCHAR_T *event = 0;
        WCHAR_T *clsName = 0;
        
        uint32_t eventSize = static_cast<uint32_t>(wcslen(eventName) + 1);
        uint32_t clsNameSize = static_cast<uint32_t>(wcslen(subSystem) + 1);
        uint32_t paramSize = 0;
        if (param != NULL) {
            uint32_t paramSize = static_cast<uint32_t>(wcslen(param) + 1);
        }
        
        
        if (m_iMemory->AllocMemory((void **) &event, eventSize * sizeof(WCHAR_T))
            && m_iMemory->AllocMemory((void **) &clsName, clsNameSize * sizeof(WCHAR_T))
               && param == NULL
                || m_iMemory->AllocMemory((void **) &params, paramSize * sizeof(WCHAR_T))) {
         
            convToShortWchar(&event, eventName);
            convToShortWchar(&clsName, subSystem);
            if (param != NULL) {
                convToShortWchar(&params, param);
            }
            
            return m_iConnect->ExternalEvent(clsName, event, params);
            
        } else {
            return false;
        }
    }
    return false;
}
    
#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

};

namespace infostart_event_2018_lockevent
{
    
    static LPCVOID addin_exports[] =
    {
        "GetClassObject", (LPCVOID)GetClassObject,
        "DestroyObject", (LPCVOID)DestroyObject,
        "GetClassNames", (LPCVOID)GetClassNames,
        "SetPlatformCapabilities", (LPCVOID)SetPlatformCapabilities,
        NULL
    };

    DECLARE_DLL((const char*)g_ComponentName, addin_exports);
}

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

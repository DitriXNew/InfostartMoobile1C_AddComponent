#ifndef __ADDINNATIVE_H__
#define __ADDINNATIVE_H__

#include "include/ComponentBase.h"
#include "AddInDefBase.h"
#include "IMemoryManager.h"

#if defined(__APPLE__) || defined(__ANDROID__)



#endif

#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

namespace vn_namespace
{

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

///////////////////////////////////////////////////////////////////////////////
class AddinNative : public IComponentBase
{
public:
    enum Props
    {
        eLastProp      // Always last
    };

    enum Methods
    {
        eLastMethod      // Always last
    };

    AddinNative(void);
    virtual ~AddinNative();
    // IInitDoneBase
    virtual bool ADDIN_API Init(void*);
    virtual bool ADDIN_API setMemManager(void* mem);
    virtual long ADDIN_API GetInfo();
    virtual void ADDIN_API Done();
    // ILanguageExtenderBase
    virtual bool ADDIN_API RegisterExtensionAs(WCHAR_T**);
    virtual long ADDIN_API GetNProps();
    virtual long ADDIN_API FindProp(const WCHAR_T* wsPropName);
    virtual const WCHAR_T* ADDIN_API GetPropName(long lPropNum, long lPropAlias);
    virtual bool ADDIN_API GetPropVal(const long lPropNum, tVariant* pvarPropVal);
    virtual bool ADDIN_API SetPropVal(const long lPropNum, tVariant* varPropVal);
    virtual bool ADDIN_API IsPropReadable(const long lPropNum);
    virtual bool ADDIN_API IsPropWritable(const long lPropNum);
    virtual long ADDIN_API GetNMethods();
    virtual long ADDIN_API FindMethod(const WCHAR_T* wsMethodName);
    virtual const WCHAR_T* ADDIN_API GetMethodName(const long lMethodNum, 
                            const long lMethodAlias);
    virtual long ADDIN_API GetNParams(const long lMethodNum);
    virtual bool ADDIN_API GetParamDefValue(const long lMethodNum, const long lParamNum,
                            tVariant *pvarParamDefValue);   
    virtual bool ADDIN_API HasRetVal(const long lMethodNum);
    virtual bool ADDIN_API CallAsProc(const long lMethodNum,
                    tVariant* paParams, const long lSizeArray);
    virtual bool ADDIN_API CallAsFunc(const long lMethodNum,
                tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray);
    // ILocaleBase
    virtual void ADDIN_API SetLocale(const WCHAR_T* loc);
    
    void addError(uint32_t wcode, const wchar_t* descriptor, uint32_t len);
    bool addEvent(const wchar_t *eventName, const wchar_t* param);

    IAddInDefBaseEx    *m_iConnect;

private:
    long findName(const wchar_t* names[], const wchar_t* name, const uint32_t size) const;
    IMemoryManager     *m_iMemory;
};
    
#if defined(__APPLE__) && !defined(BUILD_DYNAMIC_LIBRARY)

};

#endif //__APPLE__ && !BUILD_DYNAMIC_LIBRARY

#endif //__ADDINNATIVE_H__

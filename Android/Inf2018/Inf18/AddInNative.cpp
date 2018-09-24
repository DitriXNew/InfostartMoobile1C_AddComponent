
#include "AddInNative.h"
#include "ConversionWchar.h"
#include "wchar.h"
#include "../jni/jnienv.h"
#include "../include/IAndroidComponentHelper.h"
#include <chrono>
#include <thread>

static const wchar_t *g_PropNames[] = 
{
	// Проект 1
	L"SerialNumber",
	// Проект 2
	L"Screen"
};

static const wchar_t *g_MethodNames[] =
{
};

static const wchar_t *g_PropNamesRu[] = 
{
	// Проект 1
	L"СерийныйНомер",
	// Проект 2
	L"Экран"
};

static const wchar_t *g_MethodNamesRu[] =
{
};

static const wchar_t g_ComponentNameAddIn[] = L"Infostart2018";
static WcharWrapper s_ComponentClass(g_ComponentNameAddIn);
// This component supports 2.1 version
const long g_VersionAddIn = 2100;
static AppCapabilities g_capabilities = eAppCapabilitiesInvalid;

//---------------------------------------------------------------------------//
long GetClassObject(const WCHAR_T* wsName, IComponentBase** pInterface)
{
	if (!*pInterface)
	{
		*pInterface = new Infostart2018AddIn();
		return (long)*pInterface;
	}
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
	if (!*pInterface)
		return -1;

	delete *pInterface;
	*pInterface = 0;
	return 0;
}

//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
	return s_ComponentClass;
}

Infostart2018AddIn::Infostart2018AddIn() : m_iConnect(nullptr), m_iMemory(nullptr), isScreenOn(false)
{
}

Infostart2018AddIn::~Infostart2018AddIn()
{
}

/////////////////////////////////////////////////////////////////////////////
// IInitDoneBase
//---------------------------------------------------------------------------//
bool Infostart2018AddIn::Init(void* pConnection)
{
	m_iConnect = (IAddInDefBaseEx*)pConnection;
	return m_iConnect != nullptr;
}

//---------------------------------------------------------------------------//
bool Infostart2018AddIn::setMemManager(void* mem)
{
	m_iMemory = (IMemoryManager*)mem;
	return m_iMemory != nullptr;
}

//---------------------------------------------------------------------------//
long Infostart2018AddIn::GetInfo()
{
	return g_VersionAddIn;
}

//---------------------------------------------------------------------------//
void Infostart2018AddIn::Done()
{
	m_iConnect = nullptr;
	m_iMemory = nullptr;
}

/////////////////////////////////////////////////////////////////////////////
// ILanguageExtenderBase
//---------------------------------------------------------------------------//
bool Infostart2018AddIn::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{
	const wchar_t *wsExtension = g_ComponentNameAddIn;
	uint32_t iActualSize = static_cast<uint32_t>(::wcslen(wsExtension) + 1);

	if (m_iMemory)
	{
		if (m_iMemory->AllocMemory((void**)wsExtensionName, iActualSize * sizeof(WCHAR_T)))
		{
			convToShortWchar(wsExtensionName, wsExtension, iActualSize);
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------//
long Infostart2018AddIn::GetNProps()
{
	// You may delete next lines and add your own implementation code here
	return ePropLast;
}

//---------------------------------------------------------------------------//
long Infostart2018AddIn::FindProp(const WCHAR_T* wsPropName)
{
	long plPropNum = -1;
	wchar_t* propName = 0;
	convFromShortWchar(&propName, wsPropName);

	plPropNum = findName(g_PropNames, propName, ePropLast);

	if (plPropNum == -1)
		plPropNum = findName(g_PropNamesRu, propName, ePropLast);

	delete[] propName;
	return plPropNum;
}

//---------------------------------------------------------------------------//
const WCHAR_T* Infostart2018AddIn::GetPropName(long lPropNum, long lPropAlias)
{
	if (lPropNum >= ePropLast)
		return NULL;

	wchar_t *wsCurrentName = NULL;
	WCHAR_T *wsPropName = NULL;

	switch (lPropAlias)
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
bool Infostart2018AddIn::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{
	switch (lPropNum)
	{
	// Проект1
	case ePropSerial:
	{
		IAndroidComponentHelper* helper = (IAndroidComponentHelper*)m_iConnect->GetInterface(eIAndroidComponentHelper);
		pvarPropVal->vt = VTYPE_EMPTY;
		if (helper)
		{
			WCHAR_T* className = nullptr;
			convToShortWchar(&className, L"android.os.Build$VERSION");
			jclass ccloc = helper->FindClass(className);
			delete[] className;
			className = nullptr;
			if (ccloc)
			{
				JNIEnv* env = getJniEnv();
				convToShortWchar(&className, L"android.os.Build");
				jclass cbloc = helper->FindClass(className);
				delete[] className;
				jfieldID fldID = env->GetStaticFieldID(ccloc, "SDK_INT", "I");
				jstring jSerial = nullptr;
				if (env->GetStaticIntField(ccloc, fldID) > 27)
				{
					jmethodID methID = env->GetStaticMethodID(cbloc, "getSerial", "()Ljava/lang/String;");
					jSerial = (jstring)env->CallStaticObjectMethod(cbloc, methID);
				}
				else
				{
					fldID = env->GetStaticFieldID(cbloc, "SERIAL", "Ljava/lang/String;");
					jSerial = (jstring)env->GetStaticObjectField(cbloc, fldID);
				}
				int lSize;
				WCHAR_T* vSerial = ToV8StringJni(jSerial, &lSize);
				env->DeleteLocalRef(jSerial);
				env->DeleteLocalRef(ccloc);
				env->DeleteLocalRef(cbloc);
				pvarPropVal->vt = VTYPE_PWSTR;
				pvarPropVal->wstrLen = lSize;
				pvarPropVal->pwstrVal = vSerial;
			}
		}
	}
	return true;
	case ePropScreen:
		pvarPropVal->vt = VTYPE_BOOL;
		pvarPropVal->bVal = isScreenOn;
		return true;
	default:
		return false;
	}
}

//---------------------------------------------------------------------------//
bool Infostart2018AddIn::SetPropVal(const long lPropNum, tVariant *varPropVal)
{
	switch (lPropNum)
	{
	case ePropScreen:
		m_devState.Initialize(m_iConnect, m_iMemory);
		if (varPropVal->vt == VTYPE_BOOL)
		{
			isScreenOn = varPropVal->bVal;
			if (isScreenOn)
				m_devState.Start();
			else
				m_devState.Stop();
		}
		return true;
	default:
		return false;
	}
}

//---------------------------------------------------------------------------//
bool Infostart2018AddIn::IsPropReadable(const long lPropNum)
{
	return true;
}

//---------------------------------------------------------------------------//
bool Infostart2018AddIn::IsPropWritable(const long lPropNum)
{
	switch (lPropNum)
	{
	case ePropScreen:
		return true;
	default:
		return false;
	}
}

//---------------------------------------------------------------------------//
long Infostart2018AddIn::GetNMethods()
{
	return eMethLast;
}

//---------------------------------------------------------------------------//
long Infostart2018AddIn::FindMethod(const WCHAR_T* wsMethodName)
{
	long plMethodNum = -1;
	wchar_t* name = 0;
	convFromShortWchar(&name, wsMethodName);

	plMethodNum = findName(g_MethodNames, name, eMethLast);

	if (plMethodNum == -1)
		plMethodNum = findName(g_MethodNamesRu, name, eMethLast);

	delete[] name;

	return plMethodNum;
}

//---------------------------------------------------------------------------//
const WCHAR_T* Infostart2018AddIn::GetMethodName(const long lMethodNum, const long lMethodAlias)
{
	if (lMethodNum >= eMethLast)
		return NULL;

	wchar_t *wsCurrentName = NULL;
	WCHAR_T *wsMethodName = NULL;

	switch (lMethodAlias)
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
		if (m_iMemory->AllocMemory((void**)&wsMethodName, iActualSize * sizeof(WCHAR_T)))
			convToShortWchar(&wsMethodName, wsCurrentName, iActualSize);
	}

	return wsMethodName;
}

//---------------------------------------------------------------------------//
long Infostart2018AddIn::GetNParams(const long lMethodNum)
{
	switch (lMethodNum)
	{
	default:
		return 0;
	}
}

//---------------------------------------------------------------------------//
bool Infostart2018AddIn::GetParamDefValue(const long lMethodNum, const long lParamNum,	tVariant *pvarParamDefValue)
{
	switch (lMethodNum)
	{
	default:
		return false;
	}
}

//---------------------------------------------------------------------------//
bool Infostart2018AddIn::HasRetVal(const long lMethodNum)
{
	return false;
}

//---------------------------------------------------------------------------//
bool Infostart2018AddIn::CallAsProc(const long lMethodNum, tVariant* paParams, const long lSizeArray)
{
	switch (lMethodNum)
	{
	default:
		return false;
	}
}

//---------------------------------------------------------------------------//
bool Infostart2018AddIn::CallAsFunc(const long lMethodNum, tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// ILocaleBase
//---------------------------------------------------------------------------//
void Infostart2018AddIn::SetLocale(const WCHAR_T* loc)
{
}

/////////////////////////////////////////////////////////////////////////////
// Other

//---------------------------------------------------------------------------//
void Infostart2018AddIn::addError(uint32_t wcode, const wchar_t* source, const wchar_t* descriptor, long code)
{
	if (m_iConnect)
	{
		WCHAR_T *err = 0;
		WCHAR_T *descr = 0;

		convToShortWchar(&err, source);
		convToShortWchar(&descr, descriptor);

		m_iConnect->AddError(wcode, err, descr, code);

		delete[] descr;
		delete[] err;
	}
}

//---------------------------------------------------------------------------//
long Infostart2018AddIn::findName(const wchar_t* names[], const wchar_t* name, const uint32_t size) const
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

void Infostart2018AddIn::ToV8String(const wchar_t* wstr, tVariant* par)
{
	if (wstr)
	{
		int len = wcslen(wstr);
		m_iMemory->AllocMemory((void**)&par->pwstrVal, (len + 1) * sizeof(WCHAR_T));
		convToShortWchar(&par->pwstrVal, wstr);
		par->vt = VTYPE_PWSTR;
		par->wstrLen = len;
	}
	else
		par->vt = VTYPE_EMPTY;
}

WCHAR_T* Infostart2018AddIn::ToV8StringJni(jstring jstr, int* lSize)
{
	WCHAR_T* ret = NULL;
	if (jstr)
	{
		JNIEnv *jenv = getJniEnv();
		*lSize = jenv->GetStringLength(jstr);
		const WCHAR_T* pjstr = jenv->GetStringChars(jstr, NULL);
		m_iMemory->AllocMemory((void**)&ret, (*lSize + 1) * sizeof(WCHAR_T));
		for (auto i = 0; i < *lSize; ++i)
			ret[i] = pjstr[i];
		ret[*lSize] = 0;
		jenv->ReleaseStringChars(jstr, pjstr);
	}
	return ret;
}

bool Infostart2018AddIn::isNumericParameter(tVariant* par)
{
	return par->vt == VTYPE_I4 || par->vt == VTYPE_UI4 || par->vt == VTYPE_R8;
}

long Infostart2018AddIn::numericValue(tVariant* par)
{
	long ret = 0;
	switch (par->vt)
	{
	case VTYPE_I4:
		ret = par->lVal;
		break;
	case VTYPE_UI4:
		ret = par->ulVal;
		break;
	case VTYPE_R8:
		ret = par->dblVal;
		break;
	}
	return ret;
}

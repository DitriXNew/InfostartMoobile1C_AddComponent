#include <wchar.h>
#include "LockState.h"
#include "ConversionWchar.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Примеры к докладу "Создание внешних компонент для мобильной платформы 1С ОС Андроид"
// на конференции INFOSTART 2018 EVENT EDUCATION https://event.infostart.ru/2018/
//
// Пример 1: Информация об устройстве - получение серийного номера
// Пример 2: Блокировка устройства - передача в 1С внешнего события об изменении состояния экрана
//
// Copyright Игорь Кисиль 2018
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

LockState::LockState() : cc(nullptr), obj(nullptr)
{
}

LockState::~LockState()
{
	if (obj)
	{
		Stop(); // вызываем, чтобы гарантированно удалить регистрацию BroadcastReceiver'а
		JNIEnv *env = getJniEnv();
		env->DeleteGlobalRef(obj);
		env->DeleteGlobalRef(cc);
	}
}

void LockState::Initialize(IAddInDefBaseEx* cnn, const IMemoryManager* mem)
{
	if (!obj)
	{
		IAndroidComponentHelper* helper = (IAndroidComponentHelper*)cnn->GetInterface(eIAndroidComponentHelper);
		if (helper)
		{
			WCHAR_T* className = nullptr;
			convToShortWchar(&className, L"ru.infostart.education.LockState");
			jclass ccloc = helper->FindClass(className);
			delete[] className;
			className = nullptr;
			if (ccloc)
			{
				JNIEnv* env = getJniEnv();
				cc = static_cast<jclass>(env->NewGlobalRef(ccloc));
				env->DeleteLocalRef(ccloc);
				jobject activity = helper->GetActivity();
				jmethodID methID = env->GetMethodID(cc, "<init>", "(Landroid/app/Activity;JJ)V");
				jobject objloc = env->NewObject(cc, methID, activity, (jlong)cnn, (jlong)mem);
				obj = static_cast<jobject>(env->NewGlobalRef(objloc));
				env->DeleteLocalRef(objloc);
				methID = env->GetMethodID(cc, "show", "()V");
				env->CallVoidMethod(obj, methID);
				env->DeleteLocalRef(activity);
			}
		}
	}
}

void LockState::Start() const
{
	if (obj)
	{
		JNIEnv* env = getJniEnv();
		jmethodID methID = env->GetMethodID(cc, "start", "()V");
		env->CallVoidMethod(obj, methID);
	}
}

void LockState::Stop() const
{
	if (obj)
	{
		JNIEnv* env = getJniEnv();
		jmethodID methID = env->GetMethodID(cc, "stop", "()V");
		env->CallVoidMethod(obj, methID);
	}
}

static const wchar_t g_EventSource[] = L"ru.infostart.education";
static const wchar_t g_EventName[] = L"LockChanged";
static WcharWrapper s_EventSource(g_EventSource);
static WcharWrapper s_EventName(g_EventName);

// имя функции построено в соответствии с правилами Java native call
//
extern "C" JNIEXPORT void JNICALL Java_ru_infostart_education_LockState_OnLockChanged(JNIEnv* env, jclass jClass, jlong pObject, jlong pMemory, jboolean isLocked)
{
	IAddInDefBaseEx* pAddIn = (IAddInDefBaseEx*)pObject;
	IMemoryManager* pMem = (IMemoryManager*)pMemory;
	WCHAR_T *wData;
	wchar_t* sData;
	if (isLocked)
		sData = L"Locked";
	else
		sData = L"Unlocked";
	pMem->AllocMemory((void**)&wData, (wcslen(sData) + 1) * sizeof(WCHAR_T));
	convToShortWchar(&wData, sData);

	pAddIn->ExternalEvent(s_EventSource, s_EventName, wData);
}
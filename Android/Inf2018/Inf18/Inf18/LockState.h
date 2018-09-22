#pragma once

#include "../include/AddInDefBase.h"
#include "../include/IAndroidComponentHelper.h"
#include "../jni/jnienv.h"
#include "../include/IMemoryManager.h"

class LockState
{
private:

	jclass cc;
	jobject obj;

public:

	LockState();
	~LockState();

	void Initialize(IAddInDefBaseEx*, const IMemoryManager*);

	void Start() const;
	void Stop() const;
};
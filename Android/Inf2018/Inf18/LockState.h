#pragma once

#include "../include/AddInDefBase.h"
#include "../include/IAndroidComponentHelper.h"
#include "../jni/jnienv.h"
#include "../include/IMemoryManager.h"

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

/* Класс-обертка для вызова кода java библиотеки ru.infostart.education*/

class LockState
{
private:

	jclass cc;
	jobject obj;

public:

	LockState();
	~LockState();

	void Initialize(IAddInDefBaseEx*, const IMemoryManager*);

	void Start() const; // начать отслеживать состояние устройства
	void Stop() const; // закончить отслеживание
};
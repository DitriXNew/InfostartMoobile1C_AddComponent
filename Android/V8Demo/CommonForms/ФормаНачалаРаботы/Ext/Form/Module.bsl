﻿&НаКлиенте
Перем Компонента;

#Область СобытияФормы

&НаКлиенте
Процедура ПриОткрытии(Отказ)
	Если ПодключитьВнешнююКомпоненту("ОбщийМакет.Компонента", "Demo", ТипВнешнейКомпоненты.Native) Тогда
		Компонента = Новый("AddIn.Demo.Infostart2018");
		СерийныйНомер = Компонента.СерийныйНомер;
	Иначе
		Оп = Новый ОписаниеОповещения("ЗавершениеРаботыПослеОшибки",ОбщегоНазначенияКлиент.ЭтотОбъект);
		ПоказатьПредупреждение(Оп,НСтр("ru = 'Не удалось подключить внешнюю компоненту'"),10);
	КонецЕсли;
КонецПроцедуры

&НаКлиенте
Процедура ВнешнееСобытие(Источник, Событие, Данные)
	Если Источник = "ru.infostart.education" и Событие = "LockChanged" Тогда
		Если Данные = "Locked" Тогда
			ОткрытьФорму("ОбщаяФорма.ФормаАктивации",,ЭтаФорма,,,,,РежимОткрытияОкнаФормы.БлокироватьВесьИнтерфейс);
		КонецЕсли;
	КонецЕсли;
КонецПроцедуры

#КонецОбласти

#Область СобытияЭлементов

&НаКлиенте
Процедура БлокироватьПриложениеПриИзменении(Элемент)
	Компонента.Экран = БлокироватьПриложение;
КонецПроцедуры

#КонецОбласти
package ru.infostart.education;

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Примеры к докладу "Создание внешних компонент для мобильной платформы 1С ОС Андроид"
// на конференции INFOSTART 2018 EVENT EDUCATION https://event.infostart.ru/2018/
//
// Пример 1: Информация об устройстве - получение серийного номера (не нуждается в JAVA коде, поэтому
//           в проекте не содержится
// Пример 2: Блокировка устройства - передача в 1С внешнего события об изменении состояния экрана
//
// Copyright Игорь Кисиль 2018
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

// Проект 2 start

public class LockState implements Runnable {

  // эта функция в C/C++ должна иметь имя Java__ru_infostart_education_LockState_OnLockChanged
  static native void OnLockChanged(long pObject,long pMemory,boolean isLocked);

  private long m_V8Object;
  private long m_MemoryManager;
  private Activity m_Activity; // хранит контекст приложения 1С
  private BroadcastReceiver m_Receiver;

  public LockState(Activity activity, long v8Object, long v8Memory)
  {
    m_Activity = activity;
    m_V8Object = v8Object;
    m_MemoryManager = v8Memory;
  }

  public void run()
  {
    System.loadLibrary("ru_infostart_education");
  }

  public void show()
  {
    m_Activity.runOnUiThread(this);
  }

  public void start()
  {
    if (m_Receiver==null)
    {
      m_Receiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
          switch (intent.getAction())
          {
            case Intent.ACTION_SCREEN_OFF:
              OnLockChanged(m_V8Object, m_MemoryManager, false);
              break;
            case Intent.ACTION_SCREEN_ON:
              OnLockChanged(m_V8Object, m_MemoryManager, true);
              break;
          }
        }
      };

      IntentFilter filter = new IntentFilter();
      filter.addAction(Intent.ACTION_SCREEN_OFF);
      filter.addAction(Intent.ACTION_SCREEN_ON);
      m_Activity.registerReceiver(m_Receiver, filter);
    }
  }

  public void stop()
  {
    if (m_Receiver != null)
    {
      m_Activity.unregisterReceiver(m_Receiver);
      m_Receiver = null;
    }
  }
}

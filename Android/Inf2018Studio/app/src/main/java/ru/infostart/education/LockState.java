package ru.infostart.education;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

public class LockState implements Runnable {

  static native void OnLockChanged(long pObject,long pMemory,boolean isLocked);

  private long m_V8Object;
  private long m_MemoryManager;
  private Activity m_Activity;
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

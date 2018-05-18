using System;
using System.Reflection;
using System.Threading;
using System.Diagnostics;
using System.Collections.Generic;

namespace dzn
{
  public class runtime_error : SystemException {
    public runtime_error(String msg) : base(msg) {}
  }
  public class logic_error : runtime_error {
    public logic_error(String msg) : base(msg) {}
  }
  public class forced_unwind: runtime_error
  {
    public forced_unwind(): base("forced_unwind") {}
  };
  public class context : IDisposable
  {
    static Dictionary<int,int> m = new Dictionary<int,int>();

    enum State {INITIAL, RELEASED, BLOCKED, FINAL};
    String to_string(State state)
    {
      switch(state)
      {
      case State.INITIAL: return "INITIAL";
      case State.RELEASED: return "RELEASED";
      case State.BLOCKED: return "BLOCKED";
      case State.FINAL: return "FINAL";
      }
      throw new logic_error("UNKNOWN STATE");
    }

    State state;
    Action rel;
    Action<Action<context>> work;
    Thread thread;
    public context()
    {
      this.state = State.INITIAL;
      this.work = null;
      this.thread = new Thread(() =>
        {
          try
          {
            Debug.WriteLine("[" + this.get_id() + "] create");
            context.lck(this, () => {
              while(this.state != State.FINAL)
              {
                do_block(this);
                if(this.state == State.FINAL) break;
                if(this.work == null) break;
                Monitor.Exit(this);
                this.work((c) => {this.yield(c);});
                Monitor.Enter(this);
                if(state == State.FINAL) break;
                if(this.rel != null) this.rel();
              }
              });
          }
          catch (forced_unwind) {
            Debug.WriteLine("[" + this.get_id() + "] ignoring forced_unwind");
          }
        });
      this.thread.Start();
      context.lck(this, () => {
          while(state != State.BLOCKED) Monitor.Wait(this);
        });
    }
    public context(bool b)
    {
    }
    public context(Action<Action<context>> work) : this ()
    {
      context.lck(this, () => {
          this.work = work;
        });
    }
    ~context()
    {
      Dispose(false);
    }
    protected virtual void Dispose(bool gc)
    {
      if (gc)
      {
        context.lck(this, () => {
            do_finish(this);
            rel = null;
            work = null;
            thread = null;
          });
      }
    }
    public void Dispose()
    {
      Dispose(true);
      GC.SuppressFinalize(this);
    }
    public int get_id()
    {
      return context.lck(m, () => {
          int i = Thread.CurrentThread.ManagedThreadId;
          if (!m.ContainsKey(i)) m.Add(i,m.Count);
          return m[i];
        });
    }
    public void finish()
    {
      context.lck(this, () => {
          do_finish(this);
        });
    }
    public void block()
    {
      context.lck(this, () => {
          do_block(this);
        });
    }
    public void release()
    {
      context.lck(this, () => {
          do_release(this);
        });
    }
    public void call(context c)
    {
      Debug.WriteLine("[" + this.get_id() + "] call");
      context.lck(this, () => {
          do_release(this);

          Monitor.Enter(c);
          c.state = State.BLOCKED;
        });

      do { Monitor.Wait(c); } while(c.state == State.BLOCKED);
      Monitor.Exit(c);
    }
    public void yield(context to)
    {
      if(to == this) return;
      context.lck(this, () => {
          to.release();
          do_block(this);
        });
    }
    private void do_block(Object mutex)
    {
      state = State.BLOCKED;
      Monitor.Pulse(this);
      Debug.WriteLine("[" + this.get_id() + "] do_block0");
      do { Monitor.Wait(mutex); } while(state == State.BLOCKED);
      Debug.WriteLine("[" + this.get_id() + "] do_block1");
      if(state == State.FINAL) throw new forced_unwind();
    }
    private void do_release(Object mutex)
    {
      if(state != State.BLOCKED)
        throw new runtime_error("[" + this.get_id() + "] not allowed to release a call which is "
                                + to_string(state));
      state = State.RELEASED;
      Debug.WriteLine("[" + this.get_id() + "] do_release0");
      Monitor.Pulse(mutex);
      Debug.WriteLine("[" + this.get_id() + "] do_release1");
    }
    private void do_finish(Object mutex)
    {
      Debug.WriteLine("[" + this.get_id() + "] finish0");
      state = State.FINAL;
      Monitor.PulseAll(this);
      Monitor.Exit(mutex);
      System.Diagnostics.Debug.Assert(this.thread != null);
      this.thread.Join();
    }
    public static void lck(object o, Action f) {
      Monitor.Enter(o);
      try { f(); }
      finally { if(Monitor.IsEntered(o)) Monitor.Exit(o); }
    }
    public static T lck<T>(object o, Func<T> f) {
      Monitor.Enter(o);
      try { return f(); }
      finally { if(Monitor.IsEntered(o)) Monitor.Exit(o); }
    }
  };
}


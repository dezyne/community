using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;

namespace dzn
{
  public class list<T>: List<T>, IDisposable where T: IDisposable
  {
    public list() : base() {}
    public list(IEnumerable<T> b)
    {
      foreach (T t in b) this.Add(t);
    }
    public void Dispose()
    {
      foreach (T t in this) t.Dispose();
    }
  }

  public class coroutine : IDisposable
  {
    public int id;

    public context context;
    public Action<context> yield;

    public Object port;
    public bool finished;
    public bool released;
    public bool skip_block;
    public coroutine(Action worker)
    {
      this.id = -2;
      this.yield = null;
      this.port = null;
      this.finished = false;
      this.released = false;
      this.skip_block = false;

      this.context = new context ((yield) => {
          this.id = context.get_id();
          this.yield = yield;
          worker();
        });
    }
    public coroutine()
    {
      this.id = -1;
      this.context = new context (false);
      this.yield = null;
    }
    public void Dispose()
    {
      if(this.context != null) {
        this.context.Dispose();
        this.context = null;
      }
    }
    public void yield_to(coroutine c)
    {
      this.yield(c.context);
    }
    public void call(coroutine c)
    {
      this.context.call(c.context);
    }
    public void release()
    {
      this.context.release();
    }
  };
}


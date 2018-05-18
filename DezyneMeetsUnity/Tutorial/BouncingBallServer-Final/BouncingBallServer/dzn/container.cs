using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Reflection;
using System.Threading;

namespace dzn
{
  public class container<TSystem> : Component, IDisposable where TSystem : Component
  {
    public TSystem system;

    public Dictionary<String, Action> lookup;
    public Queue<String> expect;
    public pump pump;

    public container(Func<Locator,String,Component> new_system, bool flush, Locator locator)
    : base(locator, "<internal>", null)
    {
      this.pump = new pump();
      this.expect = new Queue<String>();
      this.system = (TSystem)new_system(locator.set(this.pump),"sut");
      this.system.dzn_runtime.infos[this].flushes = flush;
    }
    public container(Func<Locator,String,Component> new_system, bool flush)
    : this (new_system, flush, new Locator().set(new Runtime()))
    {}
    ~container()
    {
      Dispose(false);
    }
    protected virtual void Dispose(bool gc)
    {
      if (gc)
      {
        pump p = system.dzn_locator.get<pump>();
        if(p != this.pump) pump.execute(() => {p.stop();});
        this.pump.Dispose();
        p.Dispose();
      }
    }
    public void Dispose()
    {
      Debug.WriteLine("container.Dispose");
      Dispose(true);
      GC.SuppressFinalize(this);
    }
    public String match_return()
    {
      return context.lck(this, () => {
          while (expect.Count == 0) Monitor.Wait(this);
          String tmp = this.expect.Dequeue();
          Action e = this.lookup.ContainsKey(tmp) ? this.lookup[tmp] : null;
          while(e != null)
          {
            e();
            while (this.expect.Count == 0) Monitor.Wait(this);
            tmp = this.expect.Dequeue();
            e = this.lookup.ContainsKey(tmp) ? this.lookup[tmp] : null;
          }
          return tmp;
        });
    }
    public void match(String actual)
    {
      String tmp = match_return();
      if(actual != tmp)
        throw new runtime_error("unmatched expectation: \"" + actual + "\" got: \"" + tmp + "\"");
    }
    public void run(Dictionary<String, Action> lookup, List<String> required_ports)
    {
      this.lookup = lookup;
      String port = "";
      String str;

      while((str = System.Console.ReadLine()) != null)
      {
        Action e = this.lookup.ContainsKey(str) ? this.lookup[str] : null;
        if(e == null || port != "")
        {
          String p = str.Split('.')[0];
          if(e == null && required_ports.Find(o => {return o == p;}) != null)
          {
            if(port == "" || port != p) port = p;
            else port = "";
          }
          context.lck(this, () => {
              Monitor.Pulse(this);
              this.expect.Enqueue(str);
            });
        }
        else
        {
          pump.execute(e);
          port = "";
        }
        Thread.Sleep(100);
      }
    }
    public static R string_to_value<R>(String s) where R: struct, IComparable, IConvertible {
      R r = new R();
      String v = s.Substring(s.IndexOf(".")+1);
      if (r.GetType().Equals(typeof(bool))) return (R)Convert.ChangeType(v,typeof(R));
      if (r.GetType().Equals(typeof(int))) return (R)Convert.ChangeType(v,typeof(R));
      foreach (R e in Enum.GetValues(typeof(R))) {
        if ((typeof(R).Name + "_" + e.ToString()).Equals(s)) {
          return e;
        }
      }
      throw new System.ArgumentException("No such value: ", s);
    }
    public String to_string<R>(R r) where R: struct, IComparable, IConvertible {
      if(r.GetType().Equals(typeof(int))) return r.ToString();
      if(r.GetType().Equals(typeof(bool))) return (bool)Convert.ChangeType(r,typeof(bool)) ? "true" : "false";
      return r.GetType().Name + "_" + r.ToString();
    }
  }
}


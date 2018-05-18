using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace dzn
{
	public class V<T> where T : new()
    {
			public T v;
			public V() { this.v = new T(); }
			public V(T v) { this.v = v; }
    }

	public class RuntimeException : SystemException
	{
		public RuntimeException(String msg) : base(msg) { }
	}

	abstract public class ComponentBase
	{
		public Locator dzn_locator;
		public Runtime dzn_runtime;
  	public Meta dzn_meta;
		public ComponentBase(Locator locator, String name, Meta parent)
	  {
		  this.dzn_locator = locator; this.dzn_meta = new Meta(name, parent);
			this.dzn_runtime = locator.get<Runtime>();
	  }
	}

	public class Component : ComponentBase
	{
		public Component(Locator locator, String name = "", Meta parent = null)
			: base(locator, name, parent)
		{
			this.dzn_runtime.infos.Add(this, new Runtime.info());
		}
	}

	abstract public class SystemComponent : Component
	{
		public SystemComponent(Locator locator, String name, Meta parent)
			: base(locator, name, parent)
		{ }
	}

	public static class RuntimeHelper
	{
		public static void apply(Meta m, Action<Meta> a)
		{
			a(m);
			foreach (var c in m.children) apply(c, a);
		}

		public static void check_bindings(Meta m)
		{
			apply(m, (Meta mm) => {foreach (var p in mm.ports_connected) p();});
		}
	}

	public class Runtime
	{

		public class info
		{
			public bool handling;
			public Component deferred;
			public Queue<Action> q;
			public bool flushes;
			public info() { this.q = new Queue<Action>(); }
		}

		public Dictionary<Component, info> infos;
		public Action illegal;
		public Runtime(Action illegal = null)
		{
			if (illegal == null)
			{
				illegal = () => { throw new RuntimeException("illegal"); };
			}
			this.illegal = illegal;
			this.infos = new Dictionary<Component, info>();
		}
		public static bool external(Component c)
		{
			return c.dzn_runtime.infos[c] == null;
		}
		public static void flush(Component c)
		{
			if (!external(c))
			{
				while (c.dzn_runtime.infos[c].q.Count > 0)
				{
					handle(c, c.dzn_runtime.infos[c].q.Dequeue());
				}
				if (c.dzn_runtime.infos[c].deferred != null)
				{
					Component t = c.dzn_runtime.infos[c].deferred;
					c.dzn_runtime.infos[c].deferred = null;
					if (!t.dzn_runtime.infos[t].handling)
					{
						flush(t);
					}
				}
			}
		}
		public static void defer(Component i, Component o, Action f)
		{
			if (!(i != null && i.dzn_runtime.infos[i].flushes) && !o.dzn_runtime.infos[o].handling)
			{
				handle(o, f);
			}
			else
			{
				o.dzn_runtime.infos[o].q.Enqueue(f);
				if (i != null)
				{
					i.dzn_runtime.infos[i].deferred = o;
				}
			}
		}
		public static R valued_helper<R>(Component c, Func<R> f) where R : struct, IComparable, IConvertible
	  {
		  if (c.dzn_runtime.infos[c].handling)
		  {
		    throw new RuntimeException("a valued event cannot be deferred");
			}
			c.dzn_runtime.infos[c].handling = true;
			R r = f();
			c.dzn_runtime.infos[c].handling = false;
			flush(c);
			return r;
		}
		public static void handle(Component c, Action f)
		{
			if (!c.dzn_runtime.infos[c].handling)
			{
				c.dzn_runtime.infos[c].handling = true;
				f();
				c.dzn_runtime.infos[c].handling = false;
				flush(c);
			}
			else
			{
				throw new RuntimeException("component already handling an event");
			}
		}
		public static void callIn(Component c, Action f, port.Meta m, String e)
		{
			traceIn(m, e);
			handle(c, f);
			traceOut(m, "return");
		}
		public static R callIn<R>(Component c, Func<R> f, port.Meta m, String e) where R : struct, IComparable, IConvertible
		{
		  traceIn(m, e);
		  R r = valued_helper(c, f);
		  String s;
			if (r.GetType().Equals(typeof(bool)))
			  s = (bool)Convert.ChangeType(r, typeof(bool)) ? "true" : "false";
			else if (r.GetType().Equals(typeof(int)))
			  s = r.ToString();
			else
			  s = r.GetType().Name + "_" + Enum.GetName(r.GetType(), r);
			traceOut(m, s);
			return r;
		}
		public static void callOut(Component c, Action f, port.Meta m, String e)
		{
			traceOut(m, e);
			defer(m.provides.component, c, f);
		}
		public static String path(Meta m, String p = "")
		{
			p = p == "" ? p : "." + p;
			if (m == null) return "<external>" + p;
			if (m.parent == null)
				return (m.name != "" ? m.name : "<external>") + p;
			return path(m.parent, m.name + p);
		}
		public static void traceIn(port.Meta m, String e)
		{
			System.Console.Error.WriteLine(path(m.requires.meta, m.requires.name) + "." + e + " -> "
																		 + path(m.provides.meta, m.provides.name) + "." + e);
		}
		public static void traceOut(port.Meta m, String e)
		{
			System.Console.Error.WriteLine(path(m.provides.meta, m.provides.name) + "." + e + " -> "
																		 + path(m.requires.meta, m.requires.name) + "." + e);
		}
	}
}


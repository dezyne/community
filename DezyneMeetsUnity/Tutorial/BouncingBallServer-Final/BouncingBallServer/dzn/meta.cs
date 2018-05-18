using System;
using System.Collections.Generic;

namespace dzn
{
  public class Meta
  {
    public String name;
    public Meta parent;
    public List<port.Meta> requires;
    public List<Meta> children;
    public List<Action> ports_connected;

		public Meta(String name = "", Meta parent = null, List<port.Meta> requires = null, List<Meta> children = null, List<Action> ports_connected = null)
    {
      this.name = name;
      this.parent = parent;
      this.requires = requires;
      this.children = children;
      this.ports_connected = ports_connected;
    }
  }

  namespace port
  {
    public class Meta
    {
      public class Provides
      {
        public String name = null;
        public Component component;
        public dzn.Meta meta = new dzn.Meta();
      }
      public Provides provides = new Provides();
      public class Requires
      {
        public String name = null;
        public Component component;
        public dzn.Meta meta = new dzn.Meta();
      }
      public Requires requires = new Requires();
    }
  }

  public static class MetaHelper //Static helper class required: methods are not allowed in namespace in C#
  {
    public static string path(Meta m, string p = "")
    {
      p = string.IsNullOrEmpty(p) ? p : "." + p;
      if (m == null) return "<external>" + p;
      if (m.parent == null) return m.name + p;
      return path(m.parent, m.name + p);
    }
  }

  public class binding_error : Exception
  {
    public binding_error(port.Meta m, string msg)
      : base("not connected: " + MetaHelper.path(m.provides.component != null ? m.provides.meta : m.requires.meta,
                                                 m.provides.component != null ? m.provides.name : m.requires.name) + "." + msg)
    {}
  }
}


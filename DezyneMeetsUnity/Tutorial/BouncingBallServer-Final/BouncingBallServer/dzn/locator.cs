using System;
using System.Collections.Generic;

namespace dzn {
    public class Locator {
        public class Services : Dictionary<String, Object> {public Services(){}public Services(Services o):base(o) {}};
        Services services;
        public Locator():this(new Services()) {}
        public Locator(Services services) {this.services = services;}
        public static String key(Type c, String key) {
            return c.Name + key;
        }
        public static String key(Object o, String key) {
            return Locator.key(o.GetType(), key);
        }
        public Locator set(Object o, String key="") {
            services[Locator.key(o,key)] = o;
            return this;
        }
        public R get<R>(String key="") {
            return (R)services[Locator.key(typeof(R), key)];
        }
        public Locator clone() {return new Locator(new Services(services));}
    }
}


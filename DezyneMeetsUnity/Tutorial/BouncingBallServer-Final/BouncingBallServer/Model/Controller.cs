// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
using System;
using System.Collections.Generic;

public class Controller : dzn.Component {


  Action out_ia;


  public iCommand ia;
  public iUnity iu;

  public Controller(dzn.Locator locator, String name="", dzn.Meta parent=null) : base(locator, name, parent) {
    this.dzn_runtime.infos[this].flushes = true;
    this.out_ia = null;
    this.ia = new iCommand();
    this.ia.dzn_meta.provides.name = "ia";
    this.ia.dzn_meta.provides.meta = this.dzn_meta;
    this.ia.dzn_meta.provides.component = this;
    this.iu = new iUnity();
    this.iu.dzn_meta.requires.name = "iu";
    this.iu.dzn_meta.requires.component = this;
    this.iu.dzn_meta.requires.meta = this.dzn_meta;

    this.dzn_meta.requires = new List<dzn.port.Meta> {
      this.iu.dzn_meta,

    };
    this.dzn_meta.children = new List<dzn.Meta> { };
    this.dzn_meta.ports_connected = new List<Action>
    {
      ia.check_bindings,
      iu.check_bindings,

    };

    this.ia.inport.start = () => {dzn.Runtime.callIn(this, () => {ia_start();}, this.ia.dzn_meta, "start");};

    this.iu.outport.wallHit = () => {dzn.Runtime.callOut(this, () => {iu_wallHit();}, this.iu.dzn_meta, "wallHit");};

  }
  public void ia_start() {
    iu.inport.start();
  }

  public void iu_wallHit() {
    iu.inport.reverse();
  }

  public void check_bindings()
  {
    dzn.RuntimeHelper.check_bindings(this.dzn_meta);
  }
}

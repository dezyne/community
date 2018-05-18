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

public class iCommand{
  public static string to_string(bool b) {return b ? "true" : "false";}
  public class In {
    public Action start;


  }
  public class Out {

  }
  public dzn.port.Meta dzn_meta;
  public In inport;
  public Out outport;
  public iCommand() {
    dzn_meta = new dzn.port.Meta ();
    inport = new In();
    outport = new Out();
  }
  public static void connect(iCommand provided, iCommand required) {
    provided.outport = required.outport;
    required.inport = provided.inport;
    provided.dzn_meta.requires = required.dzn_meta.requires;
    required.dzn_meta.provides = provided.dzn_meta.provides;
  }
  public void check_bindings()
  {
    if (inport.start	== null) throw new dzn.binding_error(dzn_meta, "inport.start");


  }
}

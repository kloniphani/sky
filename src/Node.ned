//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package sky;

//
// TODO auto-generated type
//

module P {
    parameters:
         @display("i=block/routing");
    gates:
        inout gate[];
    connections allowunconnected:
}

simple Node {
    parameters:
        double RE = default(0);
        bool sendMsgOnInit = default(false);
        
    gates:
        inout gate[];
}

simple Provider extends Node {
    parameters:
        sendMsgOnInit = false;
        
        @display("i=block/routing");

}

simple UAV extends Node{
    parameters:
        sendMsgOnInit = true;
        
        @display("i=abstract/router_s");
        
        @signal[arrival](type="long");
        @statistic[hopCount](title="hop count"; source="arrival"; record=vector,stats; interpolationmode=none);
        
        
      	@figure[description](type=text; pos=5,20; font=,,bold; 
        	text="Random routing example - displaying last hop count");
        @figure[lasthopcount](type=text; pos=5,35; text="last hopCount: N/A");
}

channel Link extends ned.DatarateChannel {
    parameters:
        int cost = default(0);
        int SNR = default(0);
        
        datarate = default(1024bps);
        delay = default(5ms);
        ber = default(0); // bit error rate (BER)
        per = default(0); // packet error rate (PER)
}
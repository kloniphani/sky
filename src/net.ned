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


simple node
{
    parameters:
        bool sendMsgOnInit = default(false);
    gates:
        inout gate[];
}

simple provider extends node
{
    parameters:
        sendMsgOnInit = false;
        @display("i=block/routing,white");
}

simple head extends node
{
    parameters:
        sendMsgOnInit = false;
        @display("i=,gold");
}

simple member extends node
{
    parameters:
        sendMsgOnInit = true;
        @display("i=,cyan");
}

//
// Two instances (tic and toc) of node connected both ways.
// Tic and toc will pass messages to one another.
//
network rural
{
    types:
        channel primaryChannel extends ned.DelayChannel
        {
            delay = 1ms;
        }
        channel secondaryChannel extends ned.DelayChannel
        {
            delay = 5 ms;
        }
        
    submodules:
        lap: provider;
        ch: head;
        cm: member;
        
    connections:
        lap.gate++ <--> primaryChannel <--> ch.gate++;
        lap.gate++ <--> primaryChannel <--> cm.gate++;
        ch.gate++ <--> secondaryChannel <--> cm.gate++;
}
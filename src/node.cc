/*
 * functionality.cc
 *
 *  Created on: Aug 25, 2018
 *      Author: Kloniphani
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>


using namespace omnetpp;

class node : public cSimpleModule
{
    protected:
        // The following redefined virtual function holds the algorithm.
        virtual void forwardMessage(cMessage *msg);
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(node);

void node::initialize()
{
    // Initialize is called at the beginning of the simulation.
    // To bootstrap the tic-toc-tic-toc process, one of the modules needs

    if (par("sendMsgOnInit").boolValue() == true && getIndex() == 00)
    {
        char msgname[20];
        sprintf(msgname, "%s: %d", getName(), getIndex());
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
}

void node::handleMessage(cMessage *msg)
{
    if(getIndex() == 3)
    {
        EV << "Message " << msg << " arrived.\n";
		delete msg;
    }
    else
    {
        // We need to forward the message.
        forwardMessage(msg);
    }
}

void node::forwardMessage(cMessage *msg)
{
    // In this example, we just pick a random gate to send it on.
    // We draw a random number between 0 and the size of gate `out[]'.
    int n = gateSize("gate");
    int k = intuniform(0, n-1);

    EV << "Forwarding message " << msg << " on port out[" << k << "]\n";
    send(msg, "gate$o", k);
}

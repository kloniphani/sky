/*
 * Node.cc
 *
 *  Created on: Aug 25, 2018
 *      Author: Kloniphani
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

#include ".\simpleMsg_m.h"

class Node : public cSimpleModule
{
    private:
        long numSent;
        long numReceived;
        cLongHistogram hopCountStats;
        cOutVector hopCountVector;

        simsignal_t arrivalSignal;

    protected:
        // The following redefined virtual function holds the algorithm.
        virtual simpleMsg *generateMessage();
        virtual void forwardMessage(simpleMsg *msg);
        virtual void refreshDisplay() const override;
        virtual void finish() override;

        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(Node);

void Node::initialize()
{
    // Initialize variables
    numSent = 0;
    numReceived = 0;
    WATCH(numSent);
    WATCH(numReceived);

    hopCountStats.setName("hopCountStats");
    hopCountStats.setRangeAutoUpper(0, 10, 1.5);
    hopCountVector.setName("HopCount");

    arrivalSignal = registerSignal("arrival");

    // Initialize is called at the beginning of the simulation.
    // To bootstrap the tic-toc-tic-toc process, one of the modules needs
    if (par("sendMsgOnInit").boolValue() == true && getIndex() == 00)
    {
        simpleMsg *msg = generateMessage();
        numSent++;
        scheduleAt(0.0, msg);
    }
}


void Node::handleMessage(cMessage *msg)
{
    simpleMsg *ttmsg = check_and_cast<simpleMsg *>(msg);
    if(ttmsg->getDestination() == getIndex())
    {
        // Message arrived.
        int hopcount = ttmsg->getHopCount();
        EV << "Message " << ttmsg << " arrived after " << ttmsg->getHopCount() << " hops.\n";

        // update statistics.
        numReceived++;
        hopCountVector.record(hopcount);
        hopCountStats.collect(hopcount);

        // send a signal
        emit(arrivalSignal, hopcount);

        if (hasGUI())
        {
            char label[50];
            // Write last hop count to string
            sprintf(label, "last hopCount = %d", hopcount);
            // Get pointer to figure
            cCanvas *canvas = getParentModule()->getCanvas();
            cTextFigure *textFigure = check_and_cast<cTextFigure*>(canvas->getFigure("lasthopcount"));
            // Update figure text
            textFigure->setText(label);
        }

        bubble("ARRIVED, starting new one!");
        delete msg;

        // Generate another one.
        EV << "Generating another message: ";
        simpleMsg *newmsg = generateMessage();
        EV << newmsg << endl;
        forwardMessage(newmsg);
        numSent++;
    }
    else
    {
        // We need to forward the message.
        forwardMessage(ttmsg);
    }
}

simpleMsg *Node::generateMessage()
{
    // Produce source and destination addresses.
    int src = getIndex();  // our module index
    int n = getVectorSize();  // module vector size
    int dest = intuniform(0, n-2);
    if (dest >= src)
        dest++;

    char msgname[20];
    sprintf(msgname, "tic-%d-to-%d", src, dest);

    // Create message object and set source and destination field.
    simpleMsg *msg = new simpleMsg(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Node::forwardMessage(simpleMsg *msg)
{
    // Increment hop count.
    msg->setHopCount(msg->getHopCount()+1);

    // Same routing as before: random gate.
    int n = gateSize("gate");
    int k = intuniform(0, n-1);

    EV << "Forwarding message " << msg << " on gate[" << k << "]\n";
    send(msg, "gate$o", k);
}

void Node::refreshDisplay() const
{
    char buf[40];
    sprintf(buf, "rcvd: %ld sent: %ld", numReceived, numSent);
    getDisplayString().setTagArg("t", 0, buf);
}

void Node::finish()
{
    // This function is called by OMNeT++ at the end of the simulation.
    EV << "Sent:     " << numSent << endl;
    EV << "Received: " << numReceived << endl;
    EV << "Hop count, min:    " << hopCountStats.getMin() << endl;
    EV << "Hop count, max:    " << hopCountStats.getMax() << endl;
    EV << "Hop count, mean:   " << hopCountStats.getMean() << endl;
    EV << "Hop count, stddev: " << hopCountStats.getStddev() << endl;

    recordScalar("#sent", numSent);
    recordScalar("#received", numReceived);

    hopCountStats.recordAs("hop count");
}

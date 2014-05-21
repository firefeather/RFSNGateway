#include "RFEndPoint.h"
#include <Message/Message.h>
#include <Message/Header.h>
#include <Message/SensorData.h>
#include <Routing/RoutingTableElement.h>
#include <Message/RoutingTableElementMessage.h>
#include <Util/Util.h>
#include <Util/ByteBuffer.h>
#include "GWDataTypes/GWSensorData.h"
#include <iostream>
#include "RFSNGateway.h"

namespace RFSNGW {

RFEndPoint::RFEndPoint(uint8_t _cepin, uint8_t _cspin, RFSNGateway* gw) :
		physicalLayer("/dev/ttyS2"), transportProtocol(&physicalLayer, 0), gateway(gw) {
	transportProtocol.begin(this);

}

void RFEndPoint::handleMessage(nRFTP::ByteBuffer& bb, uint8_t type, bool isResponse) {
	nRFTP::Header header(bb);
	std::cout << "HandleMessage, src: " << header.srcAddress << std::endl;
	if (!gateway->isKnownNode(header.srcAddress)) {
		std::cout<<"adding node" << std::endl;
		gateway->addNodeToDB((int)header.srcAddress);
	} else std::cout<<"known node" << std::endl;
	bb.reset();

	switch (type) {
	case nRFTP::Message::TYPE_SENSORDATA:
	{
		nRFTP::SensorData sd(bb);
		GWSensorData* gwsd = new GWSensorData(sd, gateway->getNode(sd.header.srcAddress));
		gateway->sensorDataArrived(gwsd);
		break;
	}

	case nRFTP::Message::TYPE_ROUTING_TABLE:
		if (isResponse){
			nRFTP::RoutingTableElementMessage rtem(bb);
			std::cout << "RFEndPoint::handleMessage/" << __LINE__ << ": rte arrived" << std::endl;
			std::cout << "RFEndPoint::handleMessage/" << __LINE__ << ": dest: " << rtem.routingTableElement.destinationAddress << std::endl;
			std::cout << "RFEndPoint::handleMessage/" << __LINE__ << ": next: " << rtem.routingTableElement.nextHop << std::endl;
			gateway->getWebServiceEndPoint().routingTableElementArrived(rtem.header.srcAddress, rtem.routingTableElement);
		}
		break;
	}
}
void RFEndPoint::pingResponseArrived(uint16_t milis, uint16_t destAddress) {

}

void RFEndPoint::update() {
	transportProtocol.run();
}

RFEndPoint::~RFEndPoint() {
}

} /* namespace RFSNGW */

 nRFTP::nRFTransportProtocol& RFSNGW::RFEndPoint::getTransportProtocol() {
	return transportProtocol;
}

#include "RFEndPoint.h"
#include <Message/Message.h>
#include <Message/SensorData.h>
#include <Routing/RoutingTableElement.h>
#include <Message/RoutingTableElementMessage.h>
#include <Util/Util.h>
#include "GWDataTypes/GWSensorData.h"
#include <iostream>
#include "RFSNGateway.h"

namespace RFSNGW {

RFEndPoint::RFEndPoint(uint8_t _cepin, uint8_t _cspin, RFSNGateway* gw) :
		physicalLayer("/dev/tty????"), transportProtocol(&physicalLayer, 0), gateway(gw) {
	transportProtocol.begin(this);

}

void RFEndPoint::handleMessage(nRFTP::ByteBuffer& bb, uint8_t type, bool isResponse) {

	switch (type) {
	case nRFTP::Message::TYPE_SENSORDATA:
	{
		nRFTP::SensorData sd(bb);
		if (!gateway->isKnownNode(sd.header.srcAddress)) {
			gateway->addNodeToDB((int)sd.header.srcAddress);
		}
		GWSensorData* gwsd = new GWSensorData(sd, gateway->getNode(sd.header.srcAddress));
		gateway->sensorDataArrived(gwsd);
		break;
	}

	case nRFTP::Message::TYPE_ROUTING_TABLE:
		if (isResponse){
			nRFTP::RoutingTableElementMessage rtem(bb);
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

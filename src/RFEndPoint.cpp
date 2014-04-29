#include "RFEndPoint.h"
#include <Message/Message.h>
#include <Message/SensorData.h>
#include <Util/Util.h>
#include "GWDataTypes/GWSensorData.h"
#include <iostream>
#include "RFSNGateway.h"

namespace RFSNGW {

RFEndPoint::RFEndPoint(uint8_t _cepin, uint8_t _cspin, RFSNGateway* gw):
	physicalLayer(_cepin,_cspin),
	transportProtocol(&physicalLayer,0),
	gateway(gw){
	transportProtocol.begin(this);

}

void RFEndPoint::handleMessage(nRFTP::ByteBuffer& bb, uint8_t type, bool isResponse){

	switch (type){
	case nRFTP::Message::TYPE_SENSORDATA:
		nRFTP::SensorData sd(bb);
		GWSensorData* gwsd = new GWSensorData(sd);
		gateway->sensorDataArrived(gwsd);
		break;
	}
}
void RFEndPoint::pingResponseArrived(uint16_t milis, uint16_t destAddress){

}

void RFEndPoint::update(){
	transportProtocol.run();
}

RFEndPoint::~RFEndPoint() {
}

} /* namespace RFSNGW */

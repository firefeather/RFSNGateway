#ifndef RFENDPOINT_H_
#define RFENDPOINT_H_

#include <SerialPhysicalLayer.h>
#include <IMessageHandler.h>
#include <nRFTransportProtocol.h>
#include <stdint.h>

namespace RFSNGW {

class RFSNGateway;

class RFEndPoint : public nRFTP::IMessageHandler{
public:
	RFEndPoint(uint8_t _cepin, uint8_t _cspin, RFSNGateway* gw);
	virtual ~RFEndPoint();

    void handleMessage(nRFTP::ByteBuffer& bb, uint8_t type, bool isResponse);
    void pingResponseArrived(uint16_t milis, uint16_t destAddress);
    void update();

    nRFTP::nRFTransportProtocol& getTransportProtocol();

private:
	nRFTP::Serial_PhysicalLayer physicalLayer;
	nRFTP::nRFTransportProtocol transportProtocol;
	RFSNGateway* gateway;

};

} /* namespace RFSNGW */

#endif /* RFENDPOINT_H_ */

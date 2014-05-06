#ifndef WEBSERVICEENDPOINT_H_
#define WEBSERVICEENDPOINT_H_

#include <pion/http/server.hpp>
#include <boost/make_shared.hpp>
#include "WebServices/GetSensorData.h"
#include "WebServices/GetNode.h"
#include "WebServices/ListNodes.h"

namespace nRFTP {
	class RoutingTableElement;
}

namespace RFSNGW {

class DBHandler;
class RFSNGateway;
class SensorNode;

class WebServiceEndPoint {
public:
	WebServiceEndPoint(DBHandler* dbh, RFSNGateway* gw);
	virtual ~WebServiceEndPoint();

	void start(unsigned int _port);
	void update();
	void stop();
	void addNodeResource(const SensorNode* node);

	void routingTableElementArrived(int stcAddress, nRFTP::RoutingTableElement rte);

private:
	pion::http::server_ptr m_httpServer;
	DBHandler* dataBaseHandler;
	GetSensorData getSensorData;
	GetNode getNode;
    ListNodes listNodes;
	RFSNGateway* gateway;


};
}

#endif /* WEBSERVICEENDPOINT_H_ */

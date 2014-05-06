#ifndef RFSNGATEWAY_H_
#define RFSNGATEWAY_H_

#include "RFEndPoint.h"
#include "WebServiceEndPoint.h"
#include "DBHandler.h"
#include "LastValueCache.h"
#include "SensorNode.h"
#include <Wt/Dbo/Dbo>
#include <map>
#include <string>

namespace dbo = Wt::Dbo;

typedef dbo::collection< dbo::ptr<RFSNGW::SensorNode> > SNCollection;

namespace nRFTP {
	class RoutingTableElement;
}

namespace RFSNGW {

class GWSensorData;

class RFSNGateway {
public:
	RFSNGateway();
	virtual ~RFSNGateway();

	void run(int port);

	void sensorDataArrived(GWSensorData* gwsd);
	void routingTableElementArrived(nRFTP::RoutingTableElement& rte);

	bool isKnownNode(int srcAddress);
	void addNodeToDB(int address);
	dbo::ptr<SensorNode> getNode(int address);
	dbo::ptr<SensorNode> getNode(std::string name);
	std::map<int, dbo::ptr<SensorNode> > getNodes();
	LastValueCache& getLastValueCache();

	RFEndPoint& getEndpointRf();
	WebServiceEndPoint& getWebServiceEndPoint();

private:
	RFEndPoint endpointRF;
	DBHandler dataBaseHandler;
	WebServiceEndPoint webServiceEndPoint;
	std::map<int, dbo::ptr<SensorNode> > nodes;
	std::map<std::string, dbo::ptr<SensorNode> > nodesByName;
	LastValueCache lastValueCache;

	static bool isRunning;
};

}/* namespace RFSNGW */


#endif /* RFSNGATEWAY_H_ */

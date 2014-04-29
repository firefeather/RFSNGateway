#ifndef RFSNGATEWAY_H_
#define RFSNGATEWAY_H_

#include "RFEndPoint.h"
#include "WebServiceEndPoint.h"
#include "DBHandler.h"

namespace RFSNGW {

class GWSensorData;

class RFSNGateway {
public:
	void run();
	void sensorDataArrived(GWSensorData* gwsd);
	RFSNGateway();
	virtual ~RFSNGateway();

private:
	RFEndPoint endpointRF;
	DBHandler dataBaseHandler;
	WebServiceEndPoint webServiceEndPoint;

	static bool isRunning;
};

}/* namespace RFSNGW */


#endif /* RFSNGATEWAY_H_ */

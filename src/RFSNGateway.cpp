#include "RFSNGateway.h"
#include "Util/Util.h"
#include <iostream>

using namespace std;

namespace RFSNGW {

class GWSensorData;

bool RFSNGateway::isRunning;

RFSNGateway::RFSNGateway() :
	endpointRF(8,9, this),
	webServiceEndPoint(&dataBaseHandler){
	webServiceEndPoint.start(4231);
	isRunning = true;

}

RFSNGateway::~RFSNGateway(){
	webServiceEndPoint.stop();
}

void RFSNGateway::run(){
	while (isRunning){
		endpointRF.update();
		RFDELAY(100);
	}
}

void RFSNGateway::sensorDataArrived(GWSensorData* gwsd){
	dataBaseHandler.insertSensorData(gwsd);
}



} /* namespace RFSNGW */

RFSNGW::RFSNGateway gateway;

int main(int argc, const char* argv[] ){
	gateway.run();
}


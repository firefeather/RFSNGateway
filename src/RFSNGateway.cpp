#include "RFSNGateway.h"
#include "Util/Util.h"
#include <iostream>
#include <sstream>
#include <map>

using namespace std;

namespace RFSNGW {

class GWSensorData;

bool RFSNGateway::isRunning;

RFSNGateway::RFSNGateway() :
	endpointRF(8,9, this),
	webServiceEndPoint(&dataBaseHandler,this),
	lastValueCache(this,&dataBaseHandler){

	dbo::Transaction t = dataBaseHandler.newTransaction();
	SNCollection nodesRes  = t.session().find<SensorNode>();

	for (SNCollection::const_iterator i = nodesRes.begin(); i != nodesRes.end(); ++i) {
		nodes[(*i).get()->getAddress()] = *i;
		nodesByName[(*i).get()->getName()] = *i;
	}

	lastValueCache.init();

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

bool RFSNGateway::isKnownNode(int address){
	return nodes.find(address) != nodes.end();
}

void RFSNGateway::addNodeToDB(int address){
	stringstream name;
	name << "Node"<< address;
	SensorNode* node = new SensorNode(address,name.str());
	nodes[address] = dataBaseHandler.insertNode(node);
	nodesByName[name.str()] = nodes[address];
	webServiceEndPoint.addNodeResource(const_cast<const SensorNode*>(node));
}

dbo::ptr<SensorNode> RFSNGateway::getNode(int address){
	return nodes[address];
}

dbo::ptr<SensorNode> RFSNGateway::getNode(std::string name){
	return nodesByName[name];
}

std::map<int, dbo::ptr<SensorNode> > RFSNGateway::getNodes(){
	return nodes;
}

LastValueCache& RFSNGateway::getLastValueCache() {
	return lastValueCache;
}

} /* namespace RFSNGW */

RFSNGW::RFSNGateway gateway;

int main(int argc, const char* argv[] ){
	gateway.run();
}

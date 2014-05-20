#include "WebServiceEndPoint.h"
#include "RFSNGateway.h"
#include <sstream>
#include <map>


namespace RFSNGW {

WebServiceEndPoint::WebServiceEndPoint(DBHandler* dbh, RFSNGateway* gw) :
		dataBaseHandler(dbh),
		getSensorData(dataBaseHandler, gw),
		getNode(dbh,gw),
		listNodes(dbh, gw),
		measureSensorData(dbh, gw),
		gateway(gw){
}

WebServiceEndPoint::~WebServiceEndPoint() {
}

void WebServiceEndPoint::start(unsigned int _port) {
	m_httpServer = boost::make_shared<pion::http::server>(_port);

	std::map<int, dbo::ptr<SensorNode> > nodes = gateway->getNodes();
	std::map<int, dbo::ptr<SensorNode> >::iterator it;
	for (it = nodes.begin(); it != nodes.end(); ++it){
		addNodeResource((*it).second.get());
	}
	m_httpServer->add_resource("/listnodes", boost::bind(&ListNodes::requestHandler, &listNodes, _1, _2));

	m_httpServer->start();
}

void WebServiceEndPoint::stop() {
	if (m_httpServer.get() != NULL) {
		m_httpServer->stop();
	}
}

void WebServiceEndPoint::update() {

}

void WebServiceEndPoint::routingTableElementArrived(int srcAddress, nRFTP::RoutingTableElement rte){
	getNode.routingTableElementArrived(srcAddress, rte);
}

void WebServiceEndPoint::addNodeResource(const SensorNode* node){
	std::stringstream resSDAddress;
	std::stringstream resSDName;
	std::stringstream resLastSDAddress;
	std::stringstream resLastSDName;
	std::stringstream resNodeName;
	std::stringstream resNodeAddress;
	std::stringstream resNodeRTName;
	std::stringstream resNodeRTAddress;
	resSDAddress << "/getsensordata/by-address/" << node->getAddress();
	resSDName << "/getsensordata/by-name/" << node->getName();
	resLastSDAddress << "/getlastsensordata/by-address/" << node->getAddress();
	resLastSDName << "/getlastsensordata/by-name/" << node->getName();
	resNodeAddress << "/getnode/by-address/" << node->getAddress();
	resNodeName << "/getnode/by-name/" << node->getName();
	resNodeRTAddress << "/getroutingtable/by-address/" << node->getAddress();
	resNodeRTName << "/getroutingtable/by-name/" << node->getName();

	m_httpServer->add_resource(resSDAddress.str(), boost::bind(&GetSensorData::byAddress, &getSensorData, _1, _2, node->getAddress(), -1, false));
	m_httpServer->add_resource(resSDName.str(), boost::bind(&GetSensorData::byName, &getSensorData, _1, _2, node->getName(), -1, false));
	m_httpServer->add_resource(resLastSDAddress.str(), boost::bind(&GetSensorData::byAddress, &getSensorData, _1, _2, node->getAddress(), -1, true));
	m_httpServer->add_resource(resLastSDName.str(), boost::bind(&GetSensorData::byAddress, &getSensorData, _1, _2, node->getAddress(), -1, true));

	m_httpServer->add_resource(resNodeName.str(), boost::bind(&GetNode::byName, &getNode, _1, _2, node->getName()));
	m_httpServer->add_resource(resNodeAddress.str(), boost::bind(&GetNode::byAddress, &getNode, _1, _2, node->getAddress()));
	m_httpServer->add_resource(resNodeRTName.str(), boost::bind(&GetNode::routingTableByName, &getNode, _1, _2, node->getName()));
	m_httpServer->add_resource(resNodeRTAddress.str(), boost::bind(&GetNode::routingTableByAddress, &getNode, _1, _2, node->getAddress()));

	for (int i=0; i<3; i++){
		std::stringstream resLastAddressWithType;
		std::stringstream resLastNameWithType;
		std::stringstream resAddressWithType;
		std::stringstream resNameWithType;
		std::stringstream resMeasureSDByName;
		std::stringstream resMeasureSDByAddress;
		resAddressWithType << "/getsensordata/by-address/"<<  node->getAddress() << "/" << i;
		resNameWithType << "/getsensordata/by-name/"<<  node->getName() << "/" << i;
		resLastAddressWithType << "/getlastsensordata/by-address/"<<  node->getAddress() << "/" << i;
		resLastNameWithType << "/getlastsensordata/by-name/"<<  node->getName() << "/" << i;
		resMeasureSDByName << "/measuresensordata/by-name/" << node->getName() << "/" << i;
		resMeasureSDByAddress<< "/measuresensordata/by-address/" << node->getAddress() << "/" << i;
		m_httpServer->add_resource(resAddressWithType.str(), boost::bind(&GetSensorData::byAddress, &getSensorData, _1, _2,  node->getAddress(), i, false));
		m_httpServer->add_resource(resNameWithType.str(), boost::bind(&GetSensorData::byName, &getSensorData, _1, _2,  node->getName(), i, false));
		m_httpServer->add_resource(resLastAddressWithType.str(), boost::bind(&GetSensorData::byAddress, &getSensorData, _1, _2,  node->getAddress(), i, true));
		m_httpServer->add_resource(resLastNameWithType.str(), boost::bind(&GetSensorData::byName, &getSensorData, _1, _2,  node->getName(), i, true));
		m_httpServer->add_resource(resMeasureSDByName.str(), boost::bind(&MeasureSensorData::requestHandler, &measureSensorData, _1, _2,  gateway->getNode(node->getAddress()), i));
		m_httpServer->add_resource(resMeasureSDByAddress.str(), boost::bind(&MeasureSensorData::requestHandler, &measureSensorData, _1, _2,  gateway->getNode(node->getAddress()), i));

	}
}

}

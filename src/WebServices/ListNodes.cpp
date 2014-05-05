/*
 * ListNodes.cpp
 *
 *  Created on: May 3, 2014
 *      Author: gberes
 */

#include "ListNodes.h"
#include <map>

#include "../GWDataTypes/GWSensorData.h"
#include "../SensorNode.h"
#include "../RFSNGateway.h"
#include <iostream>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <pion/http/response_writer.hpp>

namespace RFSNGW {

ListNodes::ListNodes(DBHandler* dbh, RFSNGateway* gw):
				WSBase(dbh, gw) {
	// TODO Auto-generated constructor stub

}

ListNodes::~ListNodes() {
	// TODO Auto-generated destructor stub
}

void ListNodes::requestHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn){
	pion::http::response_writer_ptr writer(pion::http::response_writer::create(tcpConn, *httpRequest, boost::bind(&pion::tcp::connection::finish, tcpConn)));
	pion::http::response& r = writer->get_response();

	r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
	r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);

	boost::property_tree::ptree result;
	boost::property_tree::ptree nodesTree;


	std::map<int, dbo::ptr<SensorNode> > nodes = gateway->getNodes();
	std::cout <<"nodes num: " << nodes.size() << std::endl;
	std::map<int, dbo::ptr<SensorNode> >::iterator it;
	for (it = nodes.begin(); it != nodes.end(); ++it){
		boost::property_tree::ptree nodeTree;
		nodeTree.put("address", (*it).second.get()->getAddress());
		nodeTree.put("name", (*it).second.get()->getName());
		nodesTree.push_back(std::make_pair("", nodeTree));
	}
	result.add_child("nodes", nodesTree);


	std::ostringstream buf;
	boost::property_tree::write_json(buf, result, false);
	writer->write(buf.str());

	writer->send();
}

} /* namespace RFSNGW */

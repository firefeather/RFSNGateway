/*
 * GetNode.cpp
 *
 *  Created on: May 3, 2014
 *      Author: gberes
 */

#include "GetNode.h"
#include "../SensorNode.h"
#include "../RFSNGateway.h"
#include <iostream>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <pion/http/response_writer.hpp>

namespace RFSNGW {

GetNode::GetNode(DBHandler* dbh, RFSNGateway* gw) : WSBase(dbh, gw){

}

GetNode::~GetNode() {
	// TODO Auto-generated destructor stub
}


void GetNode::byAddress(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, int address){
	requestHandler(_httpRequest,  _tcpConn,gateway->getNode(address));
}
void GetNode::byName(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, std::string name){
	requestHandler(_httpRequest,  _tcpConn,gateway->getNode(name));
}

void GetNode::requestHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn, SNptr snptr) {
	pion::http::response_writer_ptr writer(pion::http::response_writer::create(tcpConn, *httpRequest, boost::bind(&pion::tcp::connection::finish, tcpConn)));
	pion::http::response& r = writer->get_response();

	std::stringstream addressStr;
	addressStr << snptr.get()->getAddress();

	r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
	r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);


	boost::property_tree::ptree result;
	result.put(addressStr.str() + ".name", snptr.get()->getName());

	std::ostringstream buf;
	boost::property_tree::write_json(buf, result, false);
	writer->write(buf.str());

	writer->send();
}

} /* namespace RFSNGW */

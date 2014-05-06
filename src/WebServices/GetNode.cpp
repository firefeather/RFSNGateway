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
#include <Routing/RoutingTableElement.h>
#include <Message/RoutingTableElementMessage.h>
#include <Message/Message.h>
#include <Util/ByteBuffer.h>
#include <nRFTransportProtocol.h>

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

void GetNode::routingTableByAddress(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, int address){
	requestRoutingTableHandler(_httpRequest, _tcpConn, gateway->getNode(address));
}

void GetNode::routingTableByName(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, std::string name){
	requestRoutingTableHandler(_httpRequest, _tcpConn, gateway->getNode(name));
}

void GetNode::requestRoutingTableHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn, SNptr snptr){
	nRFTP::RoutingTableElementMessage rtem(0, snptr->getAddress());
	uint8_t buffer[nRFTP::Message::SIZE];
	nRFTP::ByteBuffer bb(buffer);
	rtem.copyToByteBuffer(bb);
	gateway->getEndpointRf().getTransportProtocol().sendMessage(bb,snptr->getAddress() );


	pion::http::response_writer_ptr writer(pion::http::response_writer::create(tcpConn, *httpRequest, boost::bind(&pion::tcp::connection::finish, tcpConn)));
	pion::http::response& r = writer->get_response();

	r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
	r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);

	boost::property_tree::ptree result;
	boost::property_tree::ptree elementsTree;

	std::cout << "GetNode::requestRoutingTableHandler/" << __LINE__ << ": routing table size: " << routingTables.size() << std::endl;
	for ( std::multimap<int , nRFTP::RoutingTableElement >::iterator it = routingTables.begin(); it != routingTables.end(); ++it){
		std::cout << "GetNode::requestRoutingTableHandler/" << __LINE__ << ": in for, it first: "<< (*it).first << ", node address: " << snptr->getAddress() << std::endl;
		if ((*it).first == snptr->getAddress()){
			std::cout << "GetNode::requestRoutingTableHandler/" << __LINE__ << ": adding to json" << std::endl;
			boost::property_tree::ptree elementTree;
			elementTree.put("destAddress", (*it).second.destinationAddress);
			elementTree.put("nextHop", (*it).second.nextHop);
			elementTree.put("lastActivity", (*it).second.lastActivity);
			elementTree.put("reserved", (*it).second.reserved);
			elementTree.put("rtt", (*it).second.rtt);
			elementTree.put("ttl", (*it).second.ttl);
			elementsTree.push_back(std::make_pair("", elementTree));
		}
	}

	result.add_child("elements", elementsTree);

	std::ostringstream buf;
	boost::property_tree::write_json(buf, result, false);
	writer->write(buf.str());

	writer->send();

	routingTables.erase(snptr->getAddress());
}

void GetNode::routingTableElementArrived(int srcAddress, nRFTP::RoutingTableElement rte){
	routingTables.insert(std::pair<int,nRFTP::RoutingTableElement>(srcAddress,rte));
}


} /* namespace RFSNGW */

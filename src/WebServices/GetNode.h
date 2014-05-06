/*
 * GetNode.h
 *
 *  Created on: May 3, 2014
 *      Author: gberes
 */

#ifndef GETNODE_H_
#define GETNODE_H_

#include "WSBase.h"
#include <map>

namespace nRFTP {
	class RoutingTableElement;
}

namespace RFSNGW {

typedef dbo::ptr<RFSNGW::SensorNode>  SNptr;

class GetNode : public WSBase{
public:
	GetNode(DBHandler* dbh, RFSNGateway* gw);
	virtual ~GetNode();

	void byAddress(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, int address);
	void byName(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, std::string name);
	void requestHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn, SNptr snptr);

	void routingTableByAddress(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, int address);
	void routingTableByName(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, std::string name);
	void requestRoutingTableHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn, SNptr snptr);

	void routingTableElementArrived(int stcAddress, nRFTP::RoutingTableElement);

protected:
	std::multimap<int, nRFTP::RoutingTableElement> routingTables;

};

} /* namespace RFSNGW */

#endif /* GETNODE_H_ */

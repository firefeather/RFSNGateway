/*
 * ListNodes.h
 *
 *  Created on: May 3, 2014
 *      Author: gberes
 */

#ifndef LISTNODES_H_
#define LISTNODES_H_

#include "WSBase.h"

namespace RFSNGW {

class ListNodes: public WSBase {
public:
	ListNodes(DBHandler* dbh, RFSNGateway* gw);
	virtual ~ListNodes();

	void requestHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn);
};

} /* namespace RFSNGW */

#endif /* LISTNODES_H_ */

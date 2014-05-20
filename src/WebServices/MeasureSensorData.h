/*
 * MeasureSensorData.h
 *
 *  Created on: May 21, 2014
 *      Author: gberes
 */

#ifndef MEASURESENSORDATA_H_
#define MEASURESENSORDATA_H_

#include "WSBase.h"

namespace RFSNGW {

class MeasureSensorData : public WSBase{
public:
	MeasureSensorData(DBHandler* dbh, RFSNGateway* gw);
	virtual ~MeasureSensorData();

	void requestHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn, SNptr snptr, int type);
	void byAddress(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, int address, int type);
	void byName(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, std::string name, int type);
};

} /* namespace RFSNGW */

#endif /* MEASURESENSORDATA_H_ */

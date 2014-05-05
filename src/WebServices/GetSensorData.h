/*
 * GetSensorData.h
 *
 *  Created on: May 2, 2014
 *      Author: gberes
 */

#ifndef GETSENSORDATA_H_
#define GETSENSORDATA_H_

#include "WSBase.h"


namespace RFSNGW {

typedef dbo::collection< dbo::ptr<RFSNGW::GWSensorData> > SDCollection;
typedef dbo::ptr<RFSNGW::SensorNode> SNptr;

class LastValueCache;

class GetSensorData : public WSBase {
public:
	GetSensorData(DBHandler* dbh, RFSNGateway* gw);
	virtual ~GetSensorData();

	void requestHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn, SNptr snptr, int type, bool lastOnly);
	void byAddress(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, int address, int type, bool lastOnly);
	void byName(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, std::string address, int type, bool lastOnly);

protected:

};

} /* namespace RFSNGW */

#endif /* GETSENSORDATA_H_ */

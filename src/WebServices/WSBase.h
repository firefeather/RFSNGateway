/*
 * WSBase.h
 *
 *  Created on: May 3, 2014
 *      Author: gberes
 */

#ifndef WSBASE_H_
#define WSBASE_H_

#include <Wt/Dbo/Dbo>
#include <pion/http/server.hpp>
#include <string>

namespace RFSNGW {

class GWSensorData;
class SensorNode;
class DBHandler;
class RFSNGateway;


namespace dbo = Wt::Dbo;


using namespace boost::posix_time;

class WSBase {
public:
	WSBase(DBHandler* dbh, RFSNGateway* gw);
	virtual ~WSBase();

protected:
	DBHandler* dataBaseHandler;
	RFSNGateway* gateway;
};

} /* namespace RFSNGW */

#endif /* WSBASE_H_ */

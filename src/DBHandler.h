/*
 * DBHandler.h
 *
 *  Created on: Apr 29, 2014
 *      Author: gberes
 */

#ifndef DBHANDLER_H_
#define DBHANDLER_H_

#include <Wt/Dbo/Dbo>
#include <Wt/Dbo/backend/Sqlite3>
#include "GWDataTypes/GWSensorData.h"
#include "SensorNode.h"

namespace RFSNGW {

typedef dbo::collection< dbo::ptr<RFSNGW::SensorNode> > SNCollection;

class DBHandler {
public:
	DBHandler();
	virtual ~DBHandler();
	void insertSensorData(GWSensorData* gwsd);
	Wt::Dbo::ptr<SensorNode> insertNode( SensorNode* node);

	Wt::Dbo::Transaction newTransaction();

private:

	Wt::Dbo::backend::Sqlite3 sqlite3;
	Wt::Dbo::Session session;
};

} /* namespace RFSNGW */

#endif /* DBHANDLER_H_ */

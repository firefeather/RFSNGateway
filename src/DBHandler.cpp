/*
 * DBHandler.cpp
 *
 *  Created on: Apr 29, 2014
 *      Author: gberes
 */

#include "DBHandler.h"
#include <iostream>

using namespace std;

namespace RFSNGW {

DBHandler::DBHandler() :
		sqlite3("RFSNGW_DB") {

	session.setConnection(sqlite3);

	try {
		session.mapClass<GWSensorData>("gwsensordata");
		session.createTables();
	} catch (...) {

	}
}

DBHandler::~DBHandler() {
	// TODO Auto-generated destructor stub
}

void DBHandler::insertSensorData(GWSensorData* gwsd){
	cout << "Arrived: " << gwsd->getValue() << endl;
	Wt::Dbo::Transaction transaction(session);
	Wt::Dbo::ptr<GWSensorData> userPtr = session.add(gwsd);
	transaction.commit();
}

Wt::Dbo::Transaction  DBHandler::newTransaction(){
	return Wt::Dbo::Transaction(session);
}

} /* namespace RFSNGW */

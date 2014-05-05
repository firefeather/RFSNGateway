/*
 * DBHandler.cpp
 *
 *  Created on: Apr 29, 2014
 *      Author: gberes
 */

#include "DBHandler.h"
#include <iostream>
#include <unistd.h>

using namespace std;

namespace RFSNGW {

DBHandler::DBHandler() :
		sqlite3("RFSNGW_DB") {

	session.setConnection(sqlite3);

	try {
		session.mapClass<GWSensorData>("gwsensordata");
		session.mapClass<SensorNode>("sensornode");
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
	session.add(gwsd);
	transaction.commit();
}

Wt::Dbo::ptr<SensorNode> DBHandler::insertNode(SensorNode* node){
	Wt::Dbo::Transaction transaction(session);
	Wt::Dbo::ptr<SensorNode> ptr = session.add(node);
	transaction.commit();
	return ptr;
}

Wt::Dbo::Transaction  DBHandler::newTransaction(){
	return Wt::Dbo::Transaction(session);
}

} /* namespace RFSNGW */

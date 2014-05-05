/*
 * WSBase.cpp
 *
 *  Created on: May 3, 2014
 *      Author: gberes
 */

#include "WSBase.h"

namespace RFSNGW {

WSBase::WSBase(DBHandler* dbh, RFSNGateway* gw):
	dataBaseHandler(dbh), gateway(gw){

}

WSBase::~WSBase() {
	// TODO Auto-generated destructor stub
}

} /* namespace RFSNGW */

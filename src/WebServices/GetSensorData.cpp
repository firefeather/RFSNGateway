/*
 * GetSensorData.cpp
 *
 *  Created on: May 2, 2014
 *      Author: gberes
 */

#include "GetSensorData.h"
#include "../GWDataTypes/GWSensorData.h"
#include "../DBHandler.h"
#include "../SensorNode.h"
#include "../RFSNGateway.h"
#include "../LastValueCache.h"
#include <iostream>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <pion/http/response_writer.hpp>

namespace RFSNGW {

GetSensorData::GetSensorData(DBHandler* dbh, RFSNGateway* gw) :
		WSBase(dbh, gw){

}

GetSensorData::~GetSensorData() {
}

void GetSensorData::byAddress(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, int address, int type, bool lastOnly ){
	requestHandler(_httpRequest,  _tcpConn,gateway->getNode(address),type, lastOnly);
}
void GetSensorData::byName(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn, std::string name, int type, bool lastOnly ){
	requestHandler(_httpRequest,  _tcpConn,gateway->getNode(name),type, lastOnly);
}

void GetSensorData::requestHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn, SNptr snptr, int type, bool lastOnly) {
	pion::http::response_writer_ptr writer(pion::http::response_writer::create(tcpConn, *httpRequest, boost::bind(&pion::tcp::connection::finish, tcpConn)));
	pion::http::response& r = writer->get_response();

	r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
	r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);

	std::stringstream addressStr;
	addressStr << snptr.get()->getAddress();


	int limit = -1;

	pion::ihash_multimap& params = httpRequest->get_queries();
	pion::ihash_multimap::const_iterator paramIter = params.find("limit");
    if (paramIter != params.end())
    {
    	limit = std::atoi(paramIter->second.c_str());
    }

    if (lastOnly) limit = 1;

	boost::property_tree::ptree result;
	boost::property_tree::ptree result_list;
	std::map<int, boost::property_tree::ptree> sensorResults;


    if (limit == 1){
    	CacheEntryType cacheEntry = gateway->getLastValueCache().getValuesByAddress(snptr.get()->getAddress());
    	for ( CacheEntryType::iterator it = cacheEntry.begin(); it != cacheEntry.end(); ++it){
			boost::property_tree::ptree sensorResult;
			sensorResult.put("timestamp", (*it).second.first);
			sensorResult.put("value", (*it).second.second);
			sensorResults[(*it).first].push_back(std::make_pair("", sensorResult));
    	}
    } else {
    	dbo::Transaction t = dataBaseHandler->newTransaction();
		SDCollection gwdatas;
		if (type >= 0) {
			gwdatas = t.session().find<GWSensorData>().where("sensornode_id = ? and type = ?").bind(snptr.id()).bind(type).orderBy("timestamp DESC");
		} else {
			gwdatas = t.session().find<GWSensorData>().where("sensornode_id = ?").bind(snptr.id()).orderBy("type, timestamp DESC");
		}

		std::map<int, int> outPerType;

		for (SDCollection::const_iterator i = gwdatas.begin(); i != gwdatas.end(); ++i) {
			if ( ( limit > 0 && outPerType[(*i)->getType()] < limit ) || limit == -1){
				boost::property_tree::ptree sensorResult;
				sensorResult.put("timestamp", (*i)->getTimestamp());
				sensorResult.put("value", (*i)->getValue());
				sensorResults[(*i)->getType()].push_back(std::make_pair("", sensorResult));
				outPerType[(*i)->getType()]++;
			}
		}
		t.commit();

    }

	for (std::map<int, boost::property_tree::ptree>::iterator it = sensorResults.begin(); it != sensorResults.end(); ++it){
		boost::property_tree::ptree tmp;
		std::stringstream typeStr;
		typeStr << (*it).first;
		tmp.put("type", typeStr.str());
		tmp.add_child("values", (*it).second);
		result_list.push_back(std::make_pair("", tmp));

	}
	result.add_child("sensordatas", result_list);

	std::ostringstream buf;
	boost::property_tree::write_json(buf, result, false);
	writer->write(buf.str());
	writer->send();
}

} /* namespace RFSNGW */

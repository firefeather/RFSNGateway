/*
 * LastValueCache.cpp
 *
 *  Created on: May 3, 2014
 *      Author: gberes
 */

#include "LastValueCache.h"
#include "SensorNode.h"
#include "RFSNGateway.h"
#include "DBHandler.h"
#include "GWDataTypes/GWSensorData.h"
#include <iostream>

namespace RFSNGW {

LastValueCache::LastValueCache(RFSNGateway* gw, DBHandler* dbh) : gateway(gw), dataBaseHandler(dbh){

}

void LastValueCache::init(){
	dbo::Transaction t = dataBaseHandler->newTransaction();

	std::map<int, dbo::ptr<SensorNode> > nodes = gateway->getNodes();
	std::map<int, dbo::ptr<SensorNode> >::iterator it;

	typedef boost::tuple< int, int, boost::posix_time::ptime > QueryResType;
	typedef dbo::collection< QueryResType > ResultListType;

	for (it = nodes.begin(); it != nodes.end(); ++it){
		ResultListType res = t.session().query< QueryResType >("select value, type, max(timestamp) from gwsensordata").where("sensornode_id = ?").bind((*it).second.id()).groupBy("type").resultList();
		for (ResultListType::const_iterator i = res.begin(); i != res.end(); ++i){
			cache[(*it).second.get()->getAddress()][(*i).get<1>()] = std::make_pair((*i).get<2>(), (*i).get<0>());
		}
	}
}

void LastValueCache::update(GWSensorData* data){
	cache[data->getSensorNode().id()][data->getType()].first = data->getTimestamp();
	cache[data->getSensorNode().id()][data->getType()].second = data->getValue();
}

void LastValueCache::fuk(int address, int type, boost::posix_time::ptime* timestamp, int* value){
	*timestamp = cache[address][type].first;
	*value = cache[address][type].second;
}

CacheEntryType LastValueCache::getValuesByAddress( int address){
	return cache.find(address)->second;
}

LastValueCache::~LastValueCache() {
}

} /* namespace RFSNGW */

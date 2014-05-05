/*
 * LastValueCache.h
 *
 *  Created on: May 3, 2014
 *      Author: gberes
 */

#ifndef LASTVALUECACHE_H_
#define LASTVALUECACHE_H_

#include <map>

#include <Wt/Dbo/Dbo>
#include <boost/tuple/tuple.hpp>
#include <boost/date_time.hpp>

namespace dbo = Wt::Dbo;

namespace RFSNGW {

class RFSNGateway;
class DBHandler;
class GWSensorData;

typedef std::map<int, std::pair<boost::posix_time::ptime, int > > CacheEntryType;
typedef std::map<int, CacheEntryType > CacheType;

class LastValueCache {
public:
	LastValueCache(RFSNGateway* gw, DBHandler* dbh);
	virtual ~LastValueCache();

	void init();
	void update(GWSensorData* data);

	CacheEntryType getValuesByAddress(int address);

	void fuk(int address, int type, boost::posix_time::ptime* timestamp, int* value);


protected:
	RFSNGateway* gateway;
	DBHandler* dataBaseHandler;
	CacheType cache;
};

} /* namespace RFSNGW */

#endif /* LASTVALUECACHE_H_ */

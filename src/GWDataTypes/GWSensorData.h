#ifndef GWSENSORDATA_H_
#define GWSENSORDATA_H_
#include <boost/date_time.hpp>
#include <Wt/Dbo/Dbo>
#include "../SensorNode.h"

namespace nRFTP{
	class SensorData;
}

namespace RFSNGW {

class SensorNode;

class GWSensorData {
public:
	GWSensorData();
	GWSensorData(nRFTP::SensorData& src, Wt::Dbo::ptr<SensorNode> node);
	virtual ~GWSensorData();

	const boost::posix_time::ptime& getTimestamp() const;
	void setTimestamp(const boost::posix_time::ptime& timestamp);
	int getType() const;
	void setType(int type);
	float getValue() const;
	void setValue(float value);
	const Wt::Dbo::ptr<SensorNode>& getSensorNode() const;

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, type,     "type");
		Wt::Dbo::field(a, value, "value");
		Wt::Dbo::field(a, timestamp, "timestamp");
		Wt::Dbo::belongsTo(a, sensorNode, "sensornode");
	}

protected:
	Wt::Dbo::ptr<SensorNode> sensorNode;
	boost::posix_time::ptime timestamp;
	int type;
	float value;
};

} /* namespace RFSNGW */

#endif /* GWSENSORDATA_H_ */

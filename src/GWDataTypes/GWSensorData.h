#ifndef GWSENSORDATA_H_
#define GWSENSORDATA_H_
#include <boost/date_time.hpp>
#include <Wt/Dbo/Dbo>

namespace nRFTP{
	class SensorData;
}

namespace RFSNGW {

class GWSensorData {
public:
	GWSensorData();
	GWSensorData(nRFTP::SensorData& src);
	virtual ~GWSensorData();

	int getSource() const;
	void setSource(int source);
	const boost::posix_time::ptime& getTimestamp() const;
	void setTimestamp(const boost::posix_time::ptime& timestamp);
	int getType() const;
	void setType(int type);
	int getValue() const;
	void setValue(int value);

	template<class Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, type,     "type");
		Wt::Dbo::field(a, value, "value");
		Wt::Dbo::field(a, source,     "source");
		Wt::Dbo::field(a, timestamp, "timestamp");
	}

protected:
	boost::posix_time::ptime timestamp;
	int type;
	int value;
	int source;
};

} /* namespace RFSNGW */

#endif /* GWSENSORDATA_H_ */

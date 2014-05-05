#include "GWSensorData.h"
#include <Message/SensorData.h>
#include <boost/date_time/time_clock.hpp>

namespace RFSNGW {

GWSensorData::GWSensorData(nRFTP::SensorData& src, Wt::Dbo::ptr<SensorNode> node):
	sensorNode(node),timestamp(boost::posix_time::second_clock::local_time()), type(src.sensorType), value(src.sensorData){
}

GWSensorData::GWSensorData() :
	type(-1), value(0){

}

GWSensorData::~GWSensorData() {
}

const boost::posix_time::ptime& GWSensorData::getTimestamp() const {
	return timestamp;
}

void GWSensorData::setTimestamp(const boost::posix_time::ptime& timestamp) {
	this->timestamp = timestamp;
}

int GWSensorData::getType() const {
	return type;
}

void GWSensorData::setType(int type) {
	this->type = type;
}

int GWSensorData::getValue() const {
	return value;
}

void GWSensorData::setValue(int value) {
	this->value = value;
}

const Wt::Dbo::ptr<SensorNode>& RFSNGW::GWSensorData::getSensorNode() const {
	return sensorNode;
}

} /* namespace RFSNGW */


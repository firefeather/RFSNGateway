#include "GWSensorData.h"
#include <Message/SensorData.h>
#include <boost/date_time/time_clock.hpp>

namespace RFSNGW {

GWSensorData::GWSensorData(nRFTP::SensorData& src):
	timestamp(boost::posix_time::second_clock::local_time()), type(src.sensorType), value(src.sensorData), source(src.header.srcAddress){
}

GWSensorData::GWSensorData() :
	type(-1), value(0), source(0){

}

GWSensorData::~GWSensorData() {
}

int GWSensorData::getSource() const {
	return source;
}

void GWSensorData::setSource(int source) {
	this->source = source;
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

} /* namespace RFSNGW */

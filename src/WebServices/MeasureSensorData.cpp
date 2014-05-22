/*
 * MeasureSensorData.cpp
 *
 *  Created on: May 21, 2014
 *      Author: gberes
 */

#include "MeasureSensorData.h"
#include "../GWDataTypes/GWSensorData.h"
#include "../DBHandler.h"
#include "../SensorNode.h"
#include "../RFSNGateway.h"
#include <iostream>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <pion/http/response_writer.hpp>
#include <Message/SensorData.h>
#include <boost/date_time/time_clock.hpp>
#include <boost/date_time.hpp>
#include <Message/Message.h>
#include <Util/ByteBuffer.h>

namespace RFSNGW {

MeasureSensorData::MeasureSensorData(DBHandler* dbh, RFSNGateway* gw) :
				WSBase(dbh, gw) {

}

MeasureSensorData::~MeasureSensorData() {
}

void MeasureSensorData::requestHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn, const SNptr snptr, int type){
	pion::http::response_writer_ptr writer(pion::http::response_writer::create(tcpConn, *httpRequest, boost::bind(&pion::tcp::connection::finish, tcpConn)));
	pion::http::response& r = writer->get_response();

	r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
	r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);

	std::stringstream addressStr;
	addressStr << snptr.get()->getAddress();

	boost::property_tree::ptree result;

	nRFTP::SensorData sd(type, 0, 0, snptr->getAddress());
	uint8_t buffer[nRFTP::Message::SIZE];
	nRFTP::ByteBuffer bb(buffer);
	sd.copyToByteBuffer(bb);
	nRFTP::ByteBuffer res = gateway->getEndpointRf().getTransportProtocol().sendMessageSynchronous(bb,snptr->getAddress() );
	if (res.data != NULL){
		nRFTP::SensorData resultMessage(res);
		res.reset();
		result.put("measure.address", snptr->getAddress());
		result.put("measure.type", type);
		result.put("measure.timestamp", boost::posix_time::second_clock::local_time());
		result.put("measure.value", resultMessage.sensorData);
		gateway->getEndpointRf().handleMessage(res, nRFTP::Message::TYPE_SENSORDATA, true);
	} else {
		result.put("timeout",1);
	}

//	result.add_child("measuereresult", result_list);

	std::ostringstream buf;
	boost::property_tree::write_json(buf, result, false);
	writer->write(buf.str());
	writer->send();

}

} /* namespace RFSNGW */

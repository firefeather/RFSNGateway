#include "WebServiceEndPoint.h"
#include "DBHandler.h"
#include <Wt/Dbo/Dbo>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace RFSNGW {

WebServiceEndPoint::WebServiceEndPoint(DBHandler* dbh) :
		dataBaseHandler(dbh) {
}

WebServiceEndPoint::~WebServiceEndPoint() {
}

void WebServiceEndPoint::start(unsigned int _port) {
	m_httpServer = boost::make_shared<pion::http::server>(_port);
	m_httpServer->add_resource("/node", boost::bind(&WebServiceEndPoint::requestHandler, this, _1, _2));
	m_httpServer->start();
}

void WebServiceEndPoint::stop() {
	if (m_httpServer.get() != NULL) {
		m_httpServer->stop();
	}
}

void WebServiceEndPoint::requestHandler(pion::http::request_ptr& httpRequest, pion::tcp::connection_ptr& tcpConn) {
	pion::http::response_writer_ptr writer(pion::http::response_writer::create(tcpConn, *httpRequest, boost::bind(&pion::tcp::connection::finish, tcpConn)));
	pion::http::response& r = writer->get_response();

	pion::ihash_multimap& params = httpRequest->get_queries();

	pion::ihash_multimap::const_iterator paramIter = params.find("address");
	if (paramIter != params.end()) {

		int address = 0;
		int sensorType = -1;
		bool validParams = true;

		try {
			address = boost::lexical_cast<int>(paramIter->second);
		} catch (boost::bad_lexical_cast const&) {
			r.set_status_code(pion::http::types::RESPONSE_CODE_BAD_REQUEST);
			r.set_status_message(pion::http::types::RESPONSE_MESSAGE_BAD_REQUEST);
			writer->write("Wrong address ");
			validParams = false;
		}

		pion::ihash_multimap::const_iterator sensorParamIter = params.find("sensortype");
		if (sensorParamIter != params.end()) {
			try {
				sensorType = boost::lexical_cast<int>(sensorParamIter->second);
			} catch (boost::bad_lexical_cast const&) {
				r.set_status_code(pion::http::types::RESPONSE_CODE_BAD_REQUEST);
				r.set_status_message(pion::http::types::RESPONSE_MESSAGE_BAD_REQUEST);
				writer->write("Wrong sensor type");
				validParams = false;
			}
		}

		if (validParams) {
			r.set_status_code(pion::http::types::RESPONSE_CODE_OK);
			r.set_status_message(pion::http::types::RESPONSE_MESSAGE_OK);

			dbo::Transaction t = dataBaseHandler->newTransaction();
			SDCollection gwdatas;

			if (sensorType >=0){
				gwdatas = t.session().find<GWSensorData>().where("source = ? and type = ?").bind(address).bind(sensorType).orderBy("timestamp");
			} else {
				gwdatas = t.session().find<GWSensorData>().where("source = ?").bind(address).orderBy("type, timestamp");
			}

			boost::property_tree::ptree result;

			for (SDCollection::const_iterator i = gwdatas.begin(); i != gwdatas.end(); ++i) {
				result.put(paramIter->second + "." + to_simple_string((*i)->getTimestamp()) + "." + lexical_cast<std::string>((*i)->getType()), lexical_cast<std::string>((*i)->getValue()));
			}

			std::ostringstream buf;
			boost::property_tree::write_json(buf, result, false);
			writer->write(buf.str());
		}
	} else {
		r.set_status_code(pion::http::types::RESPONSE_CODE_BAD_REQUEST);
		r.set_status_message(pion::http::types::RESPONSE_MESSAGE_BAD_REQUEST);
	}

	writer->send();
}

}

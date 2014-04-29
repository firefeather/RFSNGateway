#ifndef WEBSERVICEENDPOINT_H_
#define WEBSERVICEENDPOINT_H_

#include <pion/http/server.hpp>
#include <pion/http/response_writer.hpp>
#include <boost/make_shared.hpp>
#include "GWDataTypes/GWSensorData.h"

namespace dbo = Wt::Dbo;
typedef dbo::collection< dbo::ptr<RFSNGW::GWSensorData> > SDCollection;

using boost::lexical_cast;
using namespace boost::posix_time;

namespace RFSNGW {

class DBHandler;

class WebServiceEndPoint {
public:
	WebServiceEndPoint(DBHandler* dbh);
	virtual ~WebServiceEndPoint();

	void start(unsigned int _port);
	void stop();

private:
	void requestHandler(pion::http::request_ptr& _httpRequest, pion::tcp::connection_ptr& _tcpConn);

	pion::http::server_ptr m_httpServer;
	DBHandler* dataBaseHandler;
};
}

#endif /* WEBSERVICEENDPOINT_H_ */

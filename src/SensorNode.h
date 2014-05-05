/*
 * SensorNode.h
 *
 *  Created on: May 1, 2014
 *      Author: gberes
 */

#ifndef SENSORNODE_H_
#define SENSORNODE_H_

#include <string>
#include <Wt/Dbo/Dbo>
#include "GWDataTypes/GWSensorData.h"

namespace dbo = Wt::Dbo;

namespace RFSNGW {

class GWSensorData;

class SensorNode {
public:
	SensorNode();
	SensorNode(uint16_t address, std::string name);
	virtual ~SensorNode();

	int getAddress() const;
	void setAddress(int address);
	const std::string& getName() const;
	void setName(const std::string& name);

	  template<class Action>
	  void persist(Action& a)
	  {
		  dbo::field(a, address, "address");
		  dbo::field(a, name, "name");
	      dbo::hasMany(a, sensorDatas, dbo::ManyToOne, "sensornode");
	  }

protected:
	dbo::collection< dbo::ptr<GWSensorData> > sensorDatas;
	int address;
	std::string name;
};

} /* namespace RFSNGW */

#endif /* SENSORNODE_H_ */

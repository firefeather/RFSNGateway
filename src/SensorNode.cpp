/*
 * SensorNode.cpp
 *
 *  Created on: May 1, 2014
 *      Author: gberes
 */

#include "SensorNode.h"
#include <iostream>

namespace RFSNGW {

SensorNode::SensorNode(){
}

SensorNode::SensorNode(uint16_t address, std::string name) :address(address), name(name){
}

SensorNode::~SensorNode() {
}



int RFSNGW::SensorNode::getAddress() const {
	return address;
}

void RFSNGW::SensorNode::setAddress(int address) {
	this->address = address;
}

const std::string& RFSNGW::SensorNode::getName() const {
	return name;
}

void RFSNGW::SensorNode::setName(const std::string& name) {
	this->name = name;
}

} /* namespace RFSNGW */

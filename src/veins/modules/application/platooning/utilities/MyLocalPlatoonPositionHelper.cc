//
// Copyright (c) 2012-2018 Michele Segata <segata@ccs-labs.org>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "veins/modules/application/platooning/utilities/MyLocalPlatoonPositionHelper.h"

Define_Module(MyLocalPlatoonPositionHelper);

void MyLocalPlatoonPositionHelper::initialize(int stage)
{

    BasePositionHelper::initialize(stage);

    if (stage == 0) {
        nCars = par("nCars").longValue();
        myId = getIdFromExternalId(getExternalId());
        leaderId = getPlatoonLeader(myId, nLanes, platoonSize);
        leader = myId == leaderId;
        frontId = getFrontVehicle(myId, nLanes, platoonSize);
        position = getPositionInPlatoon(myId, nLanes, platoonSize);
        platoonId = getPlatoonNumber(myId, nLanes, platoonSize);
        platoonLane = getPlatoonLane(myId, nLanes);
    }
    if (stage == 1) {
        myId = getIdFromExternalId(getExternalId());
    }
}

int MyLocalPlatoonPositionHelper::getId() const
{
    return myId;
}

int MyLocalPlatoonPositionHelper::getPosition() const
{
    return position;
}

int MyLocalPlatoonPositionHelper::getMemberId(const int position) const
{
    return leaderId + position * nLanes;
//    return position * nLanes;
}

int MyLocalPlatoonPositionHelper::getMemberPosition(const int vehicleId) const
{
    return (vehicleId - leaderId) / nLanes;
//    return vehicleId / nLanes;
}


bool MyLocalPlatoonPositionHelper::isLeader() const
{
    return leader;
}

int MyLocalPlatoonPositionHelper::getFrontId() const
{
    return frontId;
}

int MyLocalPlatoonPositionHelper::getPlatoonId() const
{
    return platoonId;
}

int MyLocalPlatoonPositionHelper::getPlatoonLane() const
{
    return platoonLane;
}

bool MyLocalPlatoonPositionHelper::isInSamePlatoon(const int vehicleId) const
{
    return platoonId == getPlatoonNumber(vehicleId, nLanes, platoonSize);
}

int MyLocalPlatoonPositionHelper::getIdFromExternalId(const std::string externalId)
{
    int dotIndex = externalId.find_last_of('.');
    std::string strId = externalId.substr(dotIndex + 1);
    return strtol(strId.c_str(), 0, 10);
}

bool MyLocalPlatoonPositionHelper::isLeader(const int vehicleId, const int nLanes, const int platoonSize)
{
    return (vehicleId / nLanes) % platoonSize == 0;
}
int MyLocalPlatoonPositionHelper::getPlatoonNumber(const int vehicleId, const int nLanes, const int platoonSize)
{
    return getPlatoonColumn(vehicleId, nLanes, platoonSize) * nLanes + getPlatoonLane(vehicleId, nLanes);
}
int MyLocalPlatoonPositionHelper::getPlatoonLane(const int vehicleId, const int nLanes)
{
    return vehicleId % nLanes;
}
int MyLocalPlatoonPositionHelper::getPlatoonColumn(const int vehicleId, const int nLanes, const int platoonSize)
{
    return vehicleId / (nLanes * platoonSize);
}
int MyLocalPlatoonPositionHelper::getPlatoonLeader(const int vehicleId, const int nLanes, const int platoonSize)
{
    return getPlatoonColumn(vehicleId, nLanes, platoonSize) * nLanes * platoonSize + getPlatoonLane(getPlatoonNumber(vehicleId, nLanes, platoonSize), nLanes);
}
int MyLocalPlatoonPositionHelper::getFrontVehicle(const int vehicleId, const int nLanes, const int platoonSize)
{
    if (getPlatoonLeader(vehicleId, nLanes, platoonSize) == vehicleId)
        return -1;
    else
        return vehicleId - nLanes;
}
bool MyLocalPlatoonPositionHelper::isInSamePlatoon(const int vehicleId, const int myId, const int nLanes, const int platoonSize)
{
    return getPlatoonNumber(vehicleId, nLanes, platoonSize) == getPlatoonNumber(myId, nLanes, platoonSize);
}
bool MyLocalPlatoonPositionHelper::isFrontVehicle(const int vehicleId, const int myId, const int nLanes, const int platoonSize)
{
    return getFrontVehicle(myId, nLanes, platoonSize) == vehicleId;
}
int MyLocalPlatoonPositionHelper::getPositionInPlatoon(const int vehicleId, const int nLanes, const int platoonSize)
{
    return (vehicleId - getPlatoonLeader(vehicleId, nLanes, platoonSize)) / nLanes;
}


int MyLocalPlatoonPositionHelper::getPlatoonSize() const
{
    return formation.size();
}

int MyLocalPlatoonPositionHelper::getLeaderId() const 
{
//    if (getPlatoonSize() > 0) {
//       return formation.at(0);
//    }
//    else {
//       return -99;
//    }
    return 0;
}



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

#ifndef MYLOCALPLATOONPOSITIONHELPER_H_
#define MYLOCALPLATOONPOSITIONHELPER_H_

#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"


class MyLocalPlatoonPositionHelper : public BasePositionHelper {

protected:
    std::vector<int> formation;

public:
    virtual const std::vector<int>& getPlatoonFormation() const override
    {
        return formation;
    }

    virtual void setPlatoonFormation(const std::vector<int>& f) override
    {
        formation = f;
    }

public:
    virtual void initialize(int stage) override;

    virtual int getId() const override;

    /**
     * Returns the position of this vehicle within the platoon
     */
    virtual int getPosition() const override;

    /**
     * Returns the id of the i-th vehicle of the own platoon
     */
    virtual int getMemberId(int position) const override;

    /**
     * Returns the position of a vehicle of the own platoon
     */
    virtual int getMemberPosition(int vehicleId) const override;

    /**
     * Returns the id of the leader of the own platoon
     */
//    virtual int getLeaderId() const override;

    /**
     * Returns whether this vehicle is the leader of the platoon
     */
    virtual bool isLeader() const override;

    /**
     * Returns the id of the vehicle in front of me
     */
    virtual int getFrontId() const override;

    /**
     * Returns the id of the platoon
     */
    virtual int getPlatoonId() const override;

    /**
     * Returns the lane the platoon is traveling on
     */
    virtual int getPlatoonLane() const override;

    /**
     * Returns whether a vehicle is part of my platoon
     */
    virtual bool isInSamePlatoon(int vehicleId) const override;

    virtual int getPlatoonSize() const override;

    virtual int getLeaderId() const override;


public:
    static int getIdFromExternalId(std::string externalId);
    static int getPlatoonColumn(int vehicleId, int nLanes, int platoonSize);
    static int getPlatoonLeader(int vehicleId, int nLanes, int platoonSize);
    static int getPlatoonLane(int vehicleId, int nLanes);
    static int getPlatoonNumber(int vehicleId, int nLanes, int platoonSize);
    static int getFrontVehicle(int vehicleId, int nLanes, int platoonSize);
    static bool isLeader(int vehicleId, int nLanes, int platoonSize);
    static int getPositionInPlatoon(int vehicleId, int nLanes, int platoonSize);
    static bool isFrontVehicle(int vehicleId, int myId, int nLanes, int platoonSize);
    static bool isInSamePlatoon(int vehicleId, int myId, int nLanes, int platoonSize);

public:
    MyLocalPlatoonPositionHelper()
        : BasePositionHelper()
    {
    }
};

#endif

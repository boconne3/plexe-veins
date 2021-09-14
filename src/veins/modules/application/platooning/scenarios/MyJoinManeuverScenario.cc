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

#include "veins/modules/application/platooning/scenarios/MyJoinManeuverScenario.h"

Define_Module(MyJoinManeuverScenario);

void MyJoinManeuverScenario::initialize(int stage)
{
    BaseScenario::initialize(stage);

    if (stage == 1) {
        
        nCars = par("nCars");
        useSinusoidalDisturbance = par("useSinusoidalDisturbance");
        useBrakingDisturbance = par("useBrakingDisturbance");
        leaderSpeed = par("leaderSpeed").doubleValue() / 3.6;

        if(useSinusoidalDisturbance){
            leaderOscillationFrequency = par("leaderOscillationFrequency").doubleValue();
            oscillationAmplitude = par("oscillationAmplitude").doubleValue() / 3.6;
            startOscillating = SimTime(par("startOscillating").doubleValue());
            stopOscillating = SimTime(par("stopOscillating").doubleValue());
        }
        else if(useBrakingDisturbance){
            startBraking = SimTime(par("startBraking").doubleValue());
            brakingDeceleration = par("brakingDeceleration").doubleValue();
        }

        app = FindModule<GeneralPlatooningApp*>::findSubModule(getParentModule());
        prepareManeuverCars(0);

    }
}

void MyJoinManeuverScenario::setupFormation()
{
    std::vector<int> formation;
    for (int i = 0; i < nCars; i++) formation.push_back(i);
    positionHelper->setPlatoonFormation(formation);
}

void MyJoinManeuverScenario::prepareManeuverCars(int platoonLane)
{

    if(positionHelper->getId() == 0){
        // this is the leader
        traciVehicle->setCruiseControlDesiredSpeed(100.0 / 3.6);
        traciVehicle->setActiveController(Plexe::ACC);
        traciVehicle->setFixedLane(platoonLane);

        positionHelper->setIsLeader(true);
        positionHelper->setPlatoonLane(platoonLane);
        positionHelper->setPlatoonSpeed(100 / 3.6);
        positionHelper->setPlatoonId(positionHelper->getId());

        if(useSinusoidalDisturbance){
            changeSpeed = new cMessage();
            scheduleAt(startOscillating, changeSpeed);
            stopChangeSpeed = new cMessage();
            scheduleAt(stopOscillating, stopChangeSpeed);
        }
        else if(useBrakingDisturbance){
            changeSpeedBraking = new cMessage();
            scheduleAt(startBraking, changeSpeedBraking);
        }
        
        setupFormation();
    }

    else if (positionHelper->getId() > 0 && positionHelper->getId() < nCars){
        // these are the followers which are already in the platoon
        traciVehicle->setCruiseControlDesiredSpeed(130.0 / 3.6);
        //traciVehicle->setActiveController(Plexe::CACC);
        traciVehicle->setFixedLane(platoonLane);

        positionHelper->setIsLeader(false);
        positionHelper->setPlatoonLane(platoonLane);
        positionHelper->setPlatoonSpeed(100 / 3.6);
        positionHelper->setPlatoonId(positionHelper->getLeaderId());
        setupFormation();
    }

    else if (positionHelper->getId() == nCars){
        // this is the car which will join
        traciVehicle->setCruiseControlDesiredSpeed(100 / 3.6);
        traciVehicle->setFixedLane(2);

        positionHelper->setPlatoonId(-1);
        positionHelper->setIsLeader(false);
        positionHelper->setPlatoonLane(-1);

        // after 30 seconds of simulation, start the maneuver
        startManeuver = new cMessage();
        scheduleAt(simTime() + SimTime(10), startManeuver);
    }

}

MyJoinManeuverScenario::~MyJoinManeuverScenario()
{
    cancelAndDelete(startManeuver);
    startManeuver = nullptr;

    cancelAndDelete(changeSpeed);
    changeSpeed = nullptr;
}

void MyJoinManeuverScenario::handleSelfMsg(cMessage* msg)
{

    // this takes care of feeding data into CACC and reschedule the self message
    BaseScenario::handleSelfMsg(msg);

    if (msg == startManeuver) {
        app->startJoinManeuver(0, 0, -1);
    }

    if (msg == changeSpeed) {
        traciVehicle->setCruiseControlDesiredSpeed(leaderSpeed + oscillationAmplitude * sin(2 * M_PI * (simTime() - startOscillating).dbl() * leaderOscillationFrequency));
        scheduleAt(simTime() + SimTime(0.1), changeSpeed);
    }

    if (msg == stopChangeSpeed) {
        traciVehicle->setCruiseControlDesiredSpeed(leaderSpeed);
        cancelAndDelete(changeSpeed);
        changeSpeed = nullptr;
    }

    if (msg == changeSpeedBraking) {
        traciVehicle->setFixedAcceleration(1, -brakingDeceleration);
    }
        
}

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

#include "veins/modules/application/platooning/maneuver/JoinAtBack.h"
#include "veins/modules/application/platooning/apps/GeneralPlatooningApp.h"

JoinAtBack::JoinAtBack(GeneralPlatooningApp* app)
    : JoinManeuver(app)
    , joinManeuverState(JoinManeuverState::IDLE)
{
}

JoinAtBack::JoinAtBack(GeneralPlatooningApp* app, const char* _controllerName)
    : JoinManeuver(app)
    , joinManeuverState(JoinManeuverState::IDLE)
{
    if (strcmp(_controllerName, "ACC") == 0) {
        controller = Plexe::ACC;
    }
    else if (strcmp(_controllerName, "CACC") == 0) {
        controller = Plexe::CACC;
    }
    else if (strcmp(_controllerName, "PLOEG") == 0) {
        controller = Plexe::PLOEG;
    }
    else if (strcmp(_controllerName, "CONSENSUS") == 0) {
        controller = Plexe::CONSENSUS;
    }
    else if (strcmp(_controllerName, "FLATBED") == 0) {
        controller = Plexe::FLATBED;
    }
    else {
        throw cRuntimeError("Invalid controller selected");
    }
    controllerName = _controllerName;
}

void JoinAtBack::startManeuver(const void* parameters)
{
    JoinManeuverParameters* pars = (JoinManeuverParameters*) parameters;
    if (joinManeuverState == JoinManeuverState::IDLE) {
        ASSERT(app->getPlatoonRole() == PlatoonRole::NONE);
        ASSERT(!app->isInManeuver());

        app->setInManeuver(true);
        app->setPlatoonRole(PlatoonRole::JOINER);

        // collect information about target Platoon
        targetPlatoonData.reset(new TargetPlatoonData());
        targetPlatoonData->platoonId = pars->platoonId;
        targetPlatoonData->platoonLeader = pars->leaderId;

        // send join request to leader
        JoinPlatoonRequest* req = createJoinPlatoonRequest(positionHelper->getId(), positionHelper->getExternalId(), targetPlatoonData->platoonId, targetPlatoonData->platoonLeader, traciVehicle->getLaneIndex(), mobility->getCurrentPosition().x, mobility->getCurrentPosition().y);
        app->sendUnicast(req, targetPlatoonData->platoonLeader);
        joinManeuverState = JoinManeuverState::J_WAIT_REPLY;
    }
}

void JoinAtBack::abortManeuver()
{
}

void JoinAtBack::onPlatoonBeacon(const PlatooningBeacon* pb)
{
    if (joinManeuverState == JoinManeuverState::J_MOVE_IN_POSITION) {
        // check correct role
        ASSERT(app->getPlatoonRole() == PlatoonRole::JOINER);

        // if the message comes from the leader
        if (pb->getVehicleId() == targetPlatoonData->newFormation.at(0)) {
            traciVehicle->setLeaderVehicleFakeData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed());
        }
        // if the message comes from the front vehicle
        int frontPosition = targetPlatoonData->joinIndex - 1;
        int frontId = targetPlatoonData->newFormation.at(frontPosition);
        if (pb->getVehicleId() == frontId) {
            // get front vehicle position
            Coord frontPosition(pb->getPositionX(), pb->getPositionY(), 0);
            // get my position
            Veins::TraCICoord traciPosition = mobility->getManager()->omnet2traci(mobility->getCurrentPosition());
            Coord position(traciPosition.x, traciPosition.y);
            // compute distance
            double distance = position.distance(frontPosition) - pb->getLength();
            traciVehicle->setFrontVehicleFakeData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed(), distance);
            // if we are in position, tell the leader about that
            if (distance < 16) { // TODO fixed value? make dependent on
                //std::cout << "\nJOINER IN POSITION\n";
                // controller and headway time
                // send move to position response to confirm the parameters
                MoveToPositionAck* ack = createMoveToPositionAck(positionHelper->getId(), positionHelper->getExternalId(), targetPlatoonData->platoonId, targetPlatoonData->platoonLeader, targetPlatoonData->platoonSpeed, targetPlatoonData->platoonLane, targetPlatoonData->newFormation);
                app->sendUnicast(ack, targetPlatoonData->newFormation.at(0));
                joinManeuverState = JoinManeuverState::J_WAIT_JOIN;
            }
            else if (distance < 42 && strcmp(controllerName, "CONSENSUS") == 0) {
                 //std::cout << "\nJOINER IN POSITION\n";
                 MoveToPositionAck* ack = createMoveToPositionAck(positionHelper->getId(), positionHelper->getExternalId(), targetPlatoonData->platoonId, targetPlatoonData->platoonLeader, targetPlatoonData->platoonSpeed, targetPlatoonData->platoonLane, targetPlatoonData->newFormation);
                 app->sendUnicast(ack, targetPlatoonData->newFormation.at(0));
                 joinManeuverState = JoinManeuverState::J_WAIT_JOIN;
             }

        }
    }
}

void JoinAtBack::handleJoinPlatoonRequest(const JoinPlatoonRequest* msg)
{

    if (msg->getPlatoonId() != positionHelper->getPlatoonId()) return;

    if (app->getPlatoonRole() != PlatoonRole::LEADER && app->getPlatoonRole() != PlatoonRole::NONE) return;

    bool permission = app->isJoinAllowed();

    // send response to the joiner
    JoinPlatoonResponse* response = createJoinPlatoonResponse(positionHelper->getId(), positionHelper->getExternalId(), msg->getPlatoonId(), msg->getVehicleId(), permission);
    app->sendUnicast(response, msg->getVehicleId());

    if (!permission) return;

    app->setInManeuver(true);
    app->setPlatoonRole(PlatoonRole::LEADER);

    // disable lane changing during maneuver
    traciVehicle->setFixedLane(traciVehicle->getLaneIndex());
    positionHelper->setPlatoonLane(traciVehicle->getLaneIndex());

    // save some data. who is joining?
    joinerData.reset(new JoinerData());
    joinerData->from(msg);

    // this was only to grant the request
    // now send the data about the platoon to the joiner
    // add the joiner to the end of the platoon
    joinerData->newFormation = positionHelper->getPlatoonFormation();
    joinerData->newFormation.push_back(joinerData->joinerId);

    MoveToPosition* mtp = createMoveToPosition(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), joinerData->joinerId, positionHelper->getPlatoonSpeed(), positionHelper->getPlatoonLane(), joinerData->newFormation);
    app->sendUnicast(mtp, joinerData->joinerId);

    joinManeuverState = JoinManeuverState::L_WAIT_JOINER_IN_POSITION;
}

void JoinAtBack::handleJoinPlatoonResponse(const JoinPlatoonResponse* msg)
{
    if (app->getPlatoonRole() != PlatoonRole::JOINER) return;
    if (joinManeuverState != JoinManeuverState::J_WAIT_REPLY) return;
    if (msg->getPlatoonId() != targetPlatoonData->platoonId) return;
    if (msg->getVehicleId() != targetPlatoonData->platoonLeader) return;

    // evaluate permission
    if (msg->getPermitted()) {
        // wait for information about the join maneuver
        joinManeuverState = JoinManeuverState::J_WAIT_INFORMATION;
        // disable lane changing during maneuver
        traciVehicle->setFixedLane(traciVehicle->getLaneIndex());
    }
    else {
        // abort maneuver
        joinManeuverState = JoinManeuverState::IDLE;
        app->setPlatoonRole(PlatoonRole::NONE);
        app->setInManeuver(false);
    }
}

void JoinAtBack::handleMoveToPosition(const MoveToPosition* msg)
{
    //std::cout << "\nJOINER MOVING TO POSITION, REQUESTING ACK FROM LEADER\n";
    if (app->getPlatoonRole() != PlatoonRole::JOINER) return;
    if (joinManeuverState != JoinManeuverState::J_WAIT_INFORMATION) return;
    if (msg->getPlatoonId() != targetPlatoonData->platoonId) return;
    if (msg->getVehicleId() != targetPlatoonData->platoonLeader) return;

    // the leader told us to move in position, we can start
    // save some data about the platoon
    targetPlatoonData->from(msg);

    // check for correct lane. if not in correct lane, change it
    // if this already is the platoon lane, join at the back (or v.v.)
    // if this is not the plaoon lane, we have to move into longitudinal
    // position
    int currentLane = traciVehicle->getLaneIndex();
    if (currentLane != targetPlatoonData->platoonLane) {
        traciVehicle->setFixedLane(targetPlatoonData->platoonLane);
    }

    // approaching the platoon

    double approachDistance = 15;
    if (strcmp(controllerName, "CONSENSUS") == 0) {
        approachDistance = 40;
    }

    // activate faked CACC. this way we can approach the front car
    // using data obtained through GPS
    traciVehicle->setCACCConstantSpacing(approachDistance);
    // we have no data so far, so for the moment just initialize
    // with some fake data
    traciVehicle->setLeaderVehicleFakeData(0, 0, targetPlatoonData->platoonSpeed);
    traciVehicle->setFrontVehicleFakeData(0, 0, targetPlatoonData->platoonSpeed, approachDistance);
    // set a CC speed higher than the platoon speed to approach it
    traciVehicle->setCruiseControlDesiredSpeed(targetPlatoonData->platoonSpeed + (100 / 3.6));
    traciVehicle->setActiveController(Plexe::FAKED_CACC);

    joinManeuverState = JoinManeuverState::J_MOVE_IN_POSITION;
}

void JoinAtBack::handleMoveToPositionAck(const MoveToPositionAck* msg)
{
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;
    if (joinManeuverState != JoinManeuverState::L_WAIT_JOINER_IN_POSITION) return;
    if (msg->getPlatoonId() != positionHelper->getPlatoonId()) return;
    if (msg->getVehicleId() != joinerData->joinerId) return;

    for (unsigned i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) ASSERT(msg->getNewPlatoonFormation(i) == joinerData->newFormation.at(i));

    // the joiner is now in position and is ready to join

    // tell the joiner to join the platoon
    JoinFormation* jf = createJoinFormation(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), joinerData->joinerId, positionHelper->getPlatoonSpeed(), traciVehicle->getLaneIndex(), joinerData->newFormation);
    app->sendUnicast(jf, joinerData->joinerId);
    joinManeuverState = JoinManeuverState::L_WAIT_JOINER_TO_JOIN;
}

void JoinAtBack::handleJoinFormation(const JoinFormation* msg)
{
    if (app->getPlatoonRole() != PlatoonRole::JOINER) return;
    if (joinManeuverState != JoinManeuverState::J_WAIT_JOIN) return;
    if (msg->getPlatoonId() != targetPlatoonData->platoonId) return;
    if (msg->getVehicleId() != targetPlatoonData->platoonLeader) return;

    for (unsigned i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) ASSERT(msg->getNewPlatoonFormation(i) == targetPlatoonData->newFormation[i]);

    // we got confirmation from the leader
    // switch from faked CACC to real CACC
    traciVehicle->setActiveController(controller);
    if (strcmp(controllerName, "CACC") == 0) {
        traciVehicle->setCACCConstantSpacing(5);
    }
    else if (strcmp(controllerName, "CONSENSUS") == 0) {
//        traciVehicle->setCACCConstantSpacing(35);
        traciVehicle->setParameter(CC_PAR_VEHICLE_POSITION, positionHelper->getPosition());
        traciVehicle->setParameter(CC_PAR_PLATOON_SIZE, positionHelper->getPlatoonSize());
    }

    // update platoon information
    positionHelper->setPlatoonId(msg->getPlatoonId());
    positionHelper->setPlatoonLane(targetPlatoonData->platoonLane);
    positionHelper->setPlatoonSpeed(targetPlatoonData->platoonSpeed);
    std::vector<int> formation;
    for (unsigned i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) formation.push_back(msg->getNewPlatoonFormation(i));
    positionHelper->setPlatoonFormation(formation);

    // tell the leader that we're now in the platoon
    JoinFormationAck* jfa = createJoinFormationAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), targetPlatoonData->platoonLeader, positionHelper->getPlatoonSpeed(), traciVehicle->getLaneIndex(), formation);
    app->sendUnicast(jfa, positionHelper->getLeaderId());

    app->setPlatoonRole(PlatoonRole::FOLLOWER);
    joinManeuverState = JoinManeuverState::IDLE;

    app->setInManeuver(false);
}

// final state for leader
// request update of formation information
void JoinAtBack::handleJoinFormationAck(const JoinFormationAck* msg)
{
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;
    if (joinManeuverState != JoinManeuverState::L_WAIT_JOINER_TO_JOIN) return;
    if (msg->getPlatoonId() != positionHelper->getPlatoonId()) return;
    if (msg->getVehicleId() != joinerData->joinerId) return;
    for (unsigned i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) ASSERT(msg->getNewPlatoonFormation(i) == joinerData->newFormation.at(i));

    // the joiner has joined the platoon
    // add the joiner to the list of vehicles in the platoon
    positionHelper->setPlatoonFormation(joinerData->newFormation);

    // send to all vehicles in Platoon
    for (unsigned int i = 1; i < positionHelper->getPlatoonSize(); i++) {
        UpdatePlatoonFormation* dup = app->createUpdatePlatoonFormation(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), -1, positionHelper->getPlatoonSpeed(), traciVehicle->getLaneIndex(), joinerData->newFormation);
        int dest = positionHelper->getMemberId(i);
        dup->setDestinationId(dest);
        app->sendUnicast(dup, dest);

    }

    joinManeuverState = JoinManeuverState::IDLE;
    app->setInManeuver(false);
}

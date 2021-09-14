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

package org.car2x.veins.modules.application.platooning.scenarios;

import org.car2x.veins.modules.application.platooning.scenarios.BBaseScenario;

simple MyJoinManeuverScenario extends BBaseScenario
{
    parameters:
        int nCars = default(0);

        bool useSinusoidalDisturbance = default(true);
        double leaderOscillationFrequency @unit("Hz") = default(0.2Hz);
        double oscillationAmplitude @unit("kmph") = default(10kmph);
        double startOscillating @unit("s") = default(25s);
        double stopOscillating @unit("s") = default(60s);

        bool useBrakingDisturbance = default(false);
        double brakingDeceleration @unit("mpsps") = default(4mpsps);
        double startBraking @unit("s") = default(5s);


        @display("i=block/app2");
        @class(MyJoinManeuverScenario);
}

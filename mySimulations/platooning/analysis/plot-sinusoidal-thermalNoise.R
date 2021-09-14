#load ggplot for quick and dirty plotting
library(ggplot2)

cntr = c(
   "ACC",
   "CACC",
   "PLOEG",
   "CONSENSUS"
)

#map controller id to name
controller <- function(id, headway, thermalNoise) {
    paste(cntr[id + 1], " noise=", thermalNoise, "dBm", sep="")
    #paste(cntr[id + 1], " noise=", ifelse(thermalNoise %% 1 == 0, paste(thermalNoise, ".0", sep=""), thermalNoise), " dBm", sep="")
}

load('../results/SinusoidalThermalNoise.Rdata')
allData$controllerName <- controller(allData$controller, allData$headway, allData$thermalNoise)

#p.speed <- ggplot(allData, aes(x=time, y=speed*3.6, col=factor(nodeId))) +
#           geom_line() +
#           facet_grid(controllerName~., scales='free_y')
#ggsave('sinusoidal-thermalNoise-speed.png', p.speed, width=16, height=9)

p.distance <- ggplot(subset(allData, distance != -1), aes(x=time, y=distance, col=factor(nodeId))) +
              geom_line() +
              facet_grid(controllerName~., scales='free_y') +
              xlab("Time elapsed (s)") + ylab("Inter-vehicle distance (m)")
ggsave('sinusoidal-thermalNoise-distance.png', p.distance, width=16, height=9)

#p.accel <- ggplot(allData, aes(x=time, y=acceleration, col=factor(nodeId))) +
#           geom_line() +
#           facet_grid(controllerName~., scales='free_y')
#ggsave('sinusoidal-thermalNoise-acceleration.png', p.accel, width=16, height=9)

#p.caccel <- ggplot(allData, aes(x=time, y=controllerAcceleration, col=factor(nodeId))) +
#            geom_line() +
#            facet_grid(controllerName~., scales='free_y')
#ggsave('sinusoidal-thermalNoise-controller-acceleration.png', p.caccel, width=16, height=9)

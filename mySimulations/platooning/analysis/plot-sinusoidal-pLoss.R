#load ggplot for quick and dirty plotting
library(ggplot2)

#cntr = c(
#   "ACC (0.3 s)",
#   "ACC (1.2 s)",
#   "CACC",
#   "PLOEG",
#   "CONSENSUS",
#   "FLATBED"
#)

cntr = c(
   "ACC",
   "CACC",
   "PLOEG",
   "Consensus"
)

#map controller id to name
controller <- function(id, headway, pLoss) {
    #ifelse(id == 0, paste(cntr[1], "  pLoss=", pLoss, sep=""), paste(cntr[2], "  pLoss=", pLoss, sep=""))
    paste(cntr[id + 1], "  pLoss=", pLoss, sep="")
}

load('../results/SinusoidalPLoss.Rdata')
allData$controllerName <- controller(allData$controller, allData$headway, allData$packetLossRate)

p.speed <- ggplot(allData, aes(x=time, y=speed*3.6, col=factor(nodeId))) +
           geom_line() +
           facet_grid(controllerName~., scales='free_y') +
           ggtitle("Sinusoidal Packet Loss Control Experiment") + xlab("Time elapsed (s)") + ylab("Speed (km/h)")
ggsave('sinusoidal-pLoss-speed.png', p.speed, width=16, height=9)

#p.distance <- ggplot(subset(allData, distance != -1), aes(x=time, y=distance, col=factor(nodeId))) +
#              geom_line() +
#              facet_grid(controllerName~., scales='free_y') +
#              xlab("Time elapsed (s)") + ylab("Inter-vehicle distance (m)")
#ggsave('sinusoidal-pLoss-distance.png', p.distance, width=16, height=9)


#p.accel <- ggplot(allData, aes(x=time, y=acceleration, col=factor(nodeId))) +
#           geom_line() +
#           facet_grid(controllerName~., scales='free_y')
#ggsave('sinusoidal-pLoss-acceleration.pdf', p.accel, width=16, height=9)
#print(p.accel)

#p.caccel <- ggplot(allData, aes(x=time, y=controllerAcceleration, col=factor(nodeId))) +
#            geom_line() +
#            facet_grid(controllerName~., scales='free_y')
#ggsave('sinusoidal-pLoss-controller-acceleration.pdf', p.caccel, width=16, height=9)
#print(p.caccel)

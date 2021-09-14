#load ggplot for quick and dirty plotting
library(ggplot2)

cntr = c(
   "ACC",
   "CACC"
)

#map controller id to name
controller <- function(id, headway, repetition) {
    #paste(cntr[id + 1], " actDelay=", repetition, " s", sep="")
    paste(cntr[id + 1], " pLoss=0", sep="")
}

load('../results/Braking.Rdata')
allData$controllerName <- controller(allData$controller, allData$headway, allData$runNumber)

p.speed <- ggplot(allData, aes(x=time, y=speed*3.6, col=factor(nodeId))) +
           geom_line() +
           facet_grid(controllerName~., scales='free_y') +
           xlab("Time elapsed (s)") + ylab("Vehicle Speed (km/h)")
ggsave('sinusoidal-braking-speed.png', p.speed, width=16, height=9)
#print(p.speed)

#p.distance <- ggplot(subset(allData, distance != -1), aes(x=time, y=distance, col=factor(nodeId))) +
#              geom_line() +
#              facet_grid(controllerName~., scales='free_y') +
#              xlab("Time elapsed (s)") + ylab("Inter-vehicle distance (m)")
#ggsave('sinusoidal-braking-distance.png', p.distance, width=16, height=9)
#print(p.distance)

#p.accel <- ggplot(allData, aes(x=time, y=acceleration, col=factor(nodeId))) +
#           geom_line() +
#           facet_grid(controllerName~., scales='free_y')
#ggsave('sinusoidal-braking-acceleration.pdf', p.accel, width=16, height=9)
#print(p.accel)

#p.caccel <- ggplot(allData, aes(x=time, y=controllerAcceleration, col=factor(nodeId))) +
#            geom_line() +
#            facet_grid(controllerName~., scales='free_y') + 
#            xlab("Time elapsed (s)") + ylab("Controller Acceleration (m/s^2)")
#ggsave('sinusoidal-braking-controller-acceleration.png', p.caccel, width=16, height=9)
#print(p.caccel)

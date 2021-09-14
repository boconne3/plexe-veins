#load ggplot for quick and dirty plotting
library(ggplot2)

cntr = c(
   "ACC",
   "CACC",
   "PLOEG",
   "C'SUS"
)

#map controller id to name
controller <- function(id, headway, actDelay) {
    
    paste(cntr[id + 1], " actDelay=", ifelse(actDelay == 0, "0.0", actDelay), "s", sep="")
}

load('../results/SinusoidalActDelay.Rdata')
allData$controllerName <- controller(allData$controller, allData$headway, allData$actDelay)

#p.speed <- ggplot(allData, aes(x=time, y=speed*3.6, col=factor(nodeId))) +
#           geom_line() +
#           facet_grid(controllerName~., scales='free_y')
#ggsave('sinusoidal-actDelay-speed.pdf', p.speed, width=16, height=9)
#print(p.speed)

p.distance <- ggplot(subset(allData, distance != -1), aes(x=time, y=distance, col=factor(nodeId))) +
              geom_line() +
              facet_grid(controllerName~., scales='free_y') + 
              xlab("Time elapsed (s)") + ylab("Inter-vehicle distance (m)")
ggsave('sinusoidal-actDelay-distance.png', p.distance, width=16, height=9)
#print(p.distance)

p.accel <- ggplot(allData, aes(x=time, y=acceleration, col=factor(nodeId))) +
           geom_line() +
           facet_grid(controllerName~., scales='free_y') + 
              xlab("Time elapsed (s)") + ylab("Vehicle Acceleration (m/s^2)")
ggsave('sinusoidal-actDelay-acceleration.png', p.accel, width=16, height=9)
#print(p.accel)

p.caccel <- ggplot(allData, aes(x=time, y=controllerAcceleration, col=factor(nodeId))) +
            geom_line() +
            facet_grid(controllerName~., scales='free_y') + 
              xlab("Time elapsed (s)") + ylab("Controller Acceleration (m/s^2)")
ggsave('sinusoidal-actDelay-controller-acceleration.png', p.caccel, width=16, height=9)
#print(p.caccel)

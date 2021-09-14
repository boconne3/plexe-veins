#load ggplot for quick and dirty plotting
library(ggplot2)

cntr = c(
   "ACC",
   "CACC"
)

#map controller id to name
controller <- function(id, headway, beacInt) {
    ifelse(id == 0, cntr[1], paste(cntr[2], " beacInt=", beacInt, "s" sep=""))
}

load('../results/SinusoidalBeacInt.Rdata')
allData$controllerName <- controller(allData$controller, allData$headway, allData$beaconInterval)

p.speed <- ggplot(allData, aes(x=time, y=speed*3.6, col=factor(nodeId))) +
           geom_line() +
           facet_grid(controllerName~., scales='free_y')
ggsave('sinusoidal-beacInt-speed.pdf', p.speed, width=16, height=9)
#print(p.speed)

p.distance <- ggplot(subset(allData, distance != -1), aes(x=time, y=distance, col=factor(nodeId))) +
              geom_line() +
              facet_grid(controllerName~., scales='free_y')
ggsave('sinusoidal-beacInt-distance.pdf', p.distance, width=16, height=9)
#print(p.distance)

p.accel <- ggplot(allData, aes(x=time, y=acceleration, col=factor(nodeId))) +
           geom_line() +
           facet_grid(controllerName~., scales='free_y')
ggsave('sinusoidal-beacInt-acceleration.pdf', p.accel, width=16, height=9)
#print(p.accel)

p.caccel <- ggplot(allData, aes(x=time, y=controllerAcceleration, col=factor(nodeId))) +
            geom_line() +
            facet_grid(controllerName~., scales='free_y')
ggsave('sinusoidal-beacInt-controller-acceleration.pdf', p.caccel, width=16, height=9)
#print(p.caccel)

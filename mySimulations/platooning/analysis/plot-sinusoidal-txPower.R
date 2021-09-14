#load ggplot for quick and dirty plotting
library(ggplot2)

cntr = c(
   "ACC",
   "CACC",
   "PLOEG",
   "CONSENSUS"
)

#map controller id to name
controller <- function(id, headway, txPower) {
    paste(cntr[id + 1], " txPower=", txPower, " mW", sep="")
}

load('../results/SinusoidalTxPower.Rdata')
allData$controllerName <- controller(allData$controller, allData$headway, allData$txPower)

#p.speed <- ggplot(allData, aes(x=time, y=speed*3.6, col=factor(nodeId))) +
#           geom_line() +
#           facet_grid(controllerName~., scales='free_y')
#ggsave('sinusoidal-txPower-speed.pdf', p.speed, width=16, height=9)
#print(p.speed)

p.distance <- ggplot(subset(allData, distance != -1), aes(x=time, y=distance, col=factor(nodeId))) +
              geom_line() +
              facet_grid(controllerName~., scales='free_y') +
              xlab("Time elapsed (s)") + ylab("Inter-vehicle distance (m)")
ggsave('sinusoidal-txPower-distance.png', p.distance, width=16, height=9)
#print(p.distance)

#p.accel <- ggplot(allData, aes(x=time, y=acceleration, col=factor(nodeId))) +
#           geom_line() +
#           facet_grid(controllerName~., scales='free_y')
#ggsave('sinusoidal-txPower-acceleration.pdf', p.accel, width=16, height=9)
#print(p.accel)

#p.caccel <- ggplot(allData, aes(x=time, y=controllerAcceleration, col=factor(nodeId))) +
#            geom_line() +
#            facet_grid(controllerName~., scales='free_y')
#ggsave('sinusoidal-txPower-controller-acceleration.pdf', p.caccel, width=16, height=9)
#print(p.caccel)

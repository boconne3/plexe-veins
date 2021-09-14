#load omnet helper function read .vec files
source('omnet_helpers.R')
#load ggplot for quick and dirty plotting
library(ggplot2)

cntr = c(
   "ACC",
   "CACC",
   "Ploeg",
   "Consensus",
   "Flatbed"
)

#map controller id to name
controller <- function(id, controlVar) {
    paste(cntr[id + 1], " pLoss=", ifelse(controlVar %% 1 == 0, paste(controlVar, ".0", sep=""), controlVar), sep="")
    #paste(cntr[id + 1], " txPower=", controlVar, " mW", sep="")
}

load('../results/MyJoinManeuver.Rdata')
allData$controllerName <- controller(allData$controller, allData$controlVar)


#p1 <-    ggplot(allData, aes(x=time, y=speed*3.6, col=factor(nodeId))) +
#        geom_line() +
#        xlim(c(0, 120)) +
#        ylim(c(90, 150)) +
#        facet_grid(controllerName~.) + 
#        xlab("Time elapsed (s)") + ylab("Vehicle speed (km/h)")
#ggsave('myJoin-speed.png', p1, width=16, height=9)

ss <-    subset(allData, nodeId != 0)
p2 <-    ggplot(ss, aes(x=time, y=distance, col=factor(nodeId))) +
        geom_line() +
        xlim(c(0, 120)) +
#        ylim(c(0, 50)) +
        facet_grid(controllerName~., scales='free_y') + 
        xlab("Time elapsed (s)") + ylab("Inter-vehicle distance (m)")
ggsave('myJoin-distance.png', p2, width=16, height=9)

#p3 <-    ggplot(allData, aes(x=time, y=acceleration, col=factor(nodeId))) +
#        geom_line() +
#        xlim(c(0, 120)) +
##        ylim(c(-4, 3)) +
#        facet_grid(controllerName~.) + 
#        xlab("Time elapsed (s)") + ylab("Inter-vehicle distance (m)")
#ggsave('myJoin-acceleration.pdf', p3, width=16, height=9)


#p4 <-    ggplot(allData, aes(x=time, y=relativeSpeed, col=factor(nodeId))) +
#        geom_line() +
#        facet_grid(controllerName~.) + 
#        xlab("Time elapsed (s)") + ylab("Inter-vehicle distance (m)")
#ggsave('myJoin-relativeSpeed.png', p4, width=16, height=9)

#p5 <-    ggplot(allData, aes(x=time, y=controllerAcceleration, col=factor(nodeId))) +
#        geom_line() +
#        facet_grid(controllerName~.) + 
#        xlab("Time elapsed (s)") + ylab("Inter-vehicle distance (m)")
#ggsave('myJoin-controllerAcceleration.png', p4, width=16, height=9)

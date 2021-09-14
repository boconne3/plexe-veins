#load omnet helper function read .vec files
source('omnet_helpers.R')
#load ggplot for quick and dirty plotting
library(ggplot2)

cntr = c(
   "ACC",
   "CACC"
)

#map controller id to name
controller <- function(id, pLoss) {
    #ifelse(id == 0, paste(cntr[1], " pLoss=", pLoss, sep=""), paste(cntr[2], " pLoss=", pLoss, sep=""))
    paste(cntr[id + 1], " pLoss=", pLoss, sep="")
}

load('../results/MyJoinManeuverPLoss.Rdata')
allData$controllerName <- controller(allData$controller, allData$pLoss)


#plot speed as function of time for different controller parameters
#p1 <-    ggplot(allData, aes(x=time, y=speed*3.6, col=factor(nodeId))) +
#        geom_line() +
#        xlim(c(0, 100)) +
#        ylim(c(90, 150)) +
#        facet_grid(controllerName~.)
#print(p1)
p1 <-    ggplot(allData, aes(x=time, y=speed*3.6, col=factor(nodeId))) +
        geom_line() +
        facet_grid(controllerName~.)
ggsave('myJoin-pLoss-speed.pdf', p1, width=16, height=9)

#plot distance as function of time for different controller parameters
ss <-    subset(allData, nodeId != 0)
p2 <-    ggplot(ss, aes(x=time, y=distance, col=factor(nodeId))) +
        geom_line() +
        xlim(c(0, 120)) +
        ylim(c(2, 8)) +
        facet_grid(controllerName~.) +
        ggtitle("Sinusoidal Packet Loss Control Experiment") + xlab("Time elapsed (s)") + ylab("Inter-vehicle distance (m)")
#print(p2)
ggsave('myJoin-pLoss-distance.png', p2, width=16, height=9)

#plot acceleration as function of time for different controller parameters
#p3 <-    ggplot(allData, aes(x=time, y=acceleration, col=factor(nodeId))) +
#        geom_line() +
#        xlim(c(0, 100)) +
#        ylim(c(-4, 3)) +
#        facet_grid(controllerName~.)
#print(p3)
p3 <-    ggplot(allData, aes(x=time, y=acceleration, col=factor(nodeId))) +
        geom_line() +
        facet_grid(controllerName~.)
ggsave('myJoin-pLoss-acceleration.pdf', p3, width=16, height=9)

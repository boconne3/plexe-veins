#load omnet helper function read .vec files
source('omnet_helpers.R')
#load ggplot for quick and dirty plotting
library(ggplot2)

cntr = c(
   "ACC",
   "CACC"
)

#map controller id to name
controller <- function(id) {
    ifelse(id == 0, cntr[1], cntr[2])
}

load('../results/MyJoinManeuverBraking.Rdata')
allData$controllerName <- controller(allData$controller)


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
ggsave('myJoin-braking-speed.pdf', p1, width=16, height=9)

#plot distance as function of time for different controller parameters
ss <-    subset(allData, nodeId != 0)
p2 <-    ggplot(ss, aes(x=time, y=distance, col=factor(nodeId))) +
        geom_line() +
        xlim(c(0, 120)) +
        ylim(c(4, 6)) +
        facet_grid(controllerName~.)
#print(p2)
ggsave('myJoin-braking-distance.pdf', p2, width=16, height=9)

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
ggsave('myJoin-braking-acceleration.pdf', p3, width=16, height=9)

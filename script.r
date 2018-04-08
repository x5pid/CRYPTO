#! /usr/bin/env Rscript
d<-read.table('collisions2.txt')
d <- unlist(d, use.names=FALSE)
cat(min(d), max(d), mean(d), sep=' ')
cat('\n')


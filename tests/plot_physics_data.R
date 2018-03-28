require(ggplot2)

setwd("~/Projects/ece-6122-final-project/tests")

df <- read.csv("sphere_60hz_300pts.csv", header = FALSE, col.names = c("data"))
df$x <- seq(1, length(df$data), 1)

ggplot(df) + geom_line(aes(x, data))

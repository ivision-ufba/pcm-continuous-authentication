#!/usr/bin/env Rscript

# open png file
png(file="roc_ecg.png", width = 4.25, height = 4.25, units = "in", res = 600, pointsize = 7)

RANGE = 0.3

# plot labels and axes
plot(c(0:1.0), c(0:1.0), xlab = "FAR", ylab = "1 - FRR", type="n", xlim = range(0.0, RANGE), ylim = range(1.0 - RANGE, 1.0), xaxs="i", yaxs="i")
grid(col = "lightgray", lty = "dotted", lwd = par("lwd"), equilogs = TRUE)
par(new = TRUE)
plot(c(0:1.0), c(0:1.0), xlab = "FAR", ylab = "1 - FRR", type="n", xlim = range(0.0, RANGE), ylim = range(1.0 - RANGE, 1.0), xaxs="i", yaxs="i")

# legend
legend("bottomright", 
       inset=.015, 
       cex = 1.15, 
       c("PCM", "PCM D2", "PCM D3", "PCM ZT", "CDF/LTST"), 
       lty = c(1,1,1,1,1,1), 
       lwd = 2, 
       col = c("red", "brown", "cyan", "deeppink", "darkviolet"), 
       bg="grey96")

# plot Multimodal New
par(new = TRUE)
plot(1-read.table("results/roc/pcm_ecg.roc"), col = "red", pch = 16, type = "l", xaxs = "i", yaxs = "i", ann = FALSE, xlim = range(0.0, RANGE), ylim = range(1.0 - RANGE, 1.0))

par(new = TRUE)
plot(1-read.table("results/roc/pcm_d2_ecg.roc"), col = "brown", pch = 16, type = "l", xaxs = "i", yaxs = "i", ann = FALSE, xlim = range(0.0, RANGE), ylim = range(1.0 - RANGE, 1.0))

par(new = TRUE)
plot(1-read.table("results/roc/pcm_d3_ecg.roc"), col = "cyan", pch = 16, type = "l", xaxs = "i", yaxs = "i", ann = FALSE, xlim = range(0.0, RANGE), ylim = range(1.0 - RANGE, 1.0))

par(new = TRUE)
plot(1-read.table("results/roc/pcm_zt_ecg.roc"), col = "deeppink", pch = 16, type = "l", xaxs = "i", yaxs = "i", ann = FALSE, xlim = range(0.0, RANGE), ylim = range(1.0 - RANGE, 1.0))

par(new = TRUE)
plot(1-read.table("results/roc/cdf_ecg.roc"), col = "darkviolet", pch = 16, type = "l", xaxs = "i", yaxs = "i", ann = FALSE, xlim = range(0.0, RANGE), ylim = range(1.0 - RANGE, 1.0))

dev.off()

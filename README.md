ipv4heatmap
===========

IPv4 Hilbert curve heatmap package for R

Rcpp-backed package for making Hilbert IPv4 space heatmaps in R. The C/C++ code is heavily based on the work of [The Measurement Factory](http://maps.measurement-factory.com/).

(NOTE: you'll need to `brew install boost` or use some other package manager to install or manually install the [boost library](http://www.boost.org/))

### sample usage

    library(ipv4heatmap)
    library(data.table)
    require(ggplot2)

    # read in a list of IOC's (you'll have to find one on your own :-)
    # using fread() and data.table since this particular database has 
    # a ton of them.
    dat <- fread("2014-03-01.csv")
    setkey(dat, observable_value, ttp_category)

    # extract only the C2 IP addresses from them
    ips <- dat[ttp_category=="c2",]$observable_value

    # build the heatmap (not shown)
    hm <- ipv4heatmap(ips)

    # plot the heatmap
    hm$gg


    # read in a list of CIDRs that are in China
    china <- read.table("http://dds.ec/data/chinacidr.txt", header=FALSE)

    # generate the bounding boxes for them
    boxes <- ldply(boundingBoxFromCIDR(china$V1), data.frame)

    # overlay the bounding boxes for China onto the C2 IOCs
    hm$gg + geom_rect(data=boxes, aes(xmin=xmin, ymin=ymin, xmax=xmax, ymax=ymax), fill="white", alpha=0.1)
    
![img](https://farm3.staticflickr.com/2895/14600640420_463624bfc3_o.png)

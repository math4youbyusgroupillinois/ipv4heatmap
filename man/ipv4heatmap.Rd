% Generated by roxygen2 (4.0.1): do not edit by hand
\name{ipv4heatmap}
\alias{ipv4heatmap}
\title{IPv4 Hilbert Curve Heatmap}
\usage{
ipv4heatmap(ips, colors = NA, cb.pal = "PuOr", alpha = FALSE,
  legend = FALSE)
}
\arguments{
\item{ips}{character vector of ip addresses}

\item{colors}{character vector (5 elements) of colors to be used in the plot. Each color maps to the number of IP addresses in the netblock (~log scale). By default, it will use ColorBrewer "PuOr" range}

\item{cbpal}{named RColorBrewer palette to use (using \code{colors} overrides any value used here)}

\item{alpha}{scale pixel alpha along with color}

\item{legend}{if you want the legend, then set this to TRUE.}
}
\value{
list containing a ggplot2 object (\code{gg}) and the x,y point data (\code{dt} - which is a \code{data.table}) which can be ggsave'd or manipulated further
}
\description{
Return a minimally annotated ggplot2 object of a hilbert-space heatmap for a set of ip addresses.
}
\examples{
ips <- read.csv("fileofipaddresses.csv")
hm <- ipv4heatmap(ips)

## plot it
hm$gg

## inspect the points \\code{data.table}:
str(hm$dt)
}


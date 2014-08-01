

#' IPv4 Hilbert Curve Heatmap
#'
#' Return a minimally annotated ggplot2 object of a hilbert-space heatmap for a set of ip addresses.
#'
#' @param ips character vector of ip addresses
#' @param colors character vector (5 elements) of colors to be used in the plot. Each color maps to the number of IP addresses in the netblock (~log scale). By default, it will use ColorBrewer "PuOr" range
#' @param cbpal named RColorBrewer palette to use (using \code{colors} overrides any value used here)
#' @param alpha scale pixel alpha along with color
#' @param legend if you want the legend, then set this to TRUE.
#' @return list containing a ggplot2 object (\code{gg}) and the x,y point data (\code{dt} - which is a \code{data.table}) which can be ggsave'd or manipulated further
#' @examples
#' ips <- read.csv("fileofipaddresses.csv")
#' hm <- ipv4heatmap(ips)
#'
#' ## plot it
#' hm$gg
#'
#' ## inspect the points \code{data.table}:
#' str(hm$dt)
#' @export

ipv4heatmap <- function(ips, colors=NA, cb.pal="PuOr", alpha=FALSE, legend=FALSE) {

  require(data.table)
  require(ggplot2)
  require(grid)
  require(RColorBrewer)

  # only takes valid IPv4 addresses

  mx <- ipv4matrix(grep("^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}$", ips, value=TRUE))
  dt <- data.table(which(mx!=0, arr.ind=TRUE), val=mx[mx!=0])
  setkeyv(dt, c("row", "col"))
  dt$color <- cut(dt$val,
                  breaks=c(0, 5, 15, 40, 100, 255),
                  include.lowest=TRUE,
                  labels=c("1-5", "6-15", "16-40", "41-100", "101-255"))

  if (anyNA(colors)) {
    hilbcols <- brewer.pal(5, cb.pal)
  } else {
    hilbcols <- colors
  }

  gg <- ggplot()

  suppressMessages({

    if (alpha) {
      gg <- gg + geom_point(data=dt, aes(x=row, y=col, color=color, alpha=color), size=1)
    } else {
      gg <- gg + geom_point(data=dt, aes(x=row, y=col, color=color), size=1)
    }

    gg <- gg + xlim(0, 4095) + ylim(0, 4095)
    gg <- gg + labs(x="", y="", title="")
    gg <- gg + scale_color_manual(values=hilbcols)
    gg <- gg + scale_x_continuous(expand=c(0,0))
    gg <- gg + scale_y_reverse(expand=c(0,0))
    gg <- gg + coord_equal()
    gg <- gg + theme_bw()

    if (legend == FALSE) {
      gg <- gg + theme(legend.position="none")
    }

    gg <- gg + theme(panel.grid=element_blank())
    gg <- gg + theme(panel.border=element_blank())
    gg <- gg + theme(axis.ticks=element_blank())
    gg <- gg + theme(axis.text=element_blank())
    gg <- gg + theme(axis.title=element_blank())
    gg <- gg + theme(axis.line=element_blank())
    gg <- gg + theme(panel.background=element_rect(fill="black"))
    gg <- gg + theme(plot.margin=unit(c(0,0,0,0), "null"))
    gg <- gg + theme(axis.ticks.length=unit(0, "null"))
    gg <- gg + theme(axis.ticks.margin=unit(0, "null"))

  })

  return(list(gg=gg, dt=dt))

}

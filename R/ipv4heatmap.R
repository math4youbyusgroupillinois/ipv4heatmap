

#' Return a ggplot2 object of a hilbert-space heatmap for a set of ip addys
#'
#' @param ips character vector of ip addresses
#' @return list containing a ggplot2 object and the point data which can be ggsave'd or manipulated further
#' @export

ipv4heatmap <- function(ips) {

  require(data.table)
  require(ggplot2)
  require(grid)

  mx <- ipv4matrix(ips)
  dt <- data.table(which(mx!=0, arr.ind=TRUE), val=mx[mx!=0])
  setkeyv(dt, c("row", "col"))

  gg <- ggplot(dt, aes(x=row, y=col, alpha=cut(val, 3)))

  suppressMessages({
    gg <- gg + geom_point(size=1, color="#1e90ff")
    gg <- gg + xlim(0, 4095) + ylim(0, 4095)
    gg <- gg + labs(x="", y="", title="")
    gg <- gg + scale_x_continuous(expand=c(0,0))
    gg <- gg + scale_y_reverse(expand=c(0,0))
    gg <- gg + coord_equal()
    gg <- gg + theme_bw()
    gg <- gg + theme(legend.position="none")
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
    gg <- gg + theme(plot.margin=unit(c(0, 0, 0, 0), "lines"))
  })

  return(list(gg=gg, dt=dt))

}

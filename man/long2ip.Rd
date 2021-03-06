% Generated by roxygen2 (4.0.1): do not edit by hand
\name{long2ip}
\alias{long2ip}
\title{Intger IPv4 Address Conversion to Character}
\usage{
long2ip(ip)
}
\arguments{
\item{ip}{input numeric (long integer) vector}
}
\value{
vector of equivalent character (dotted-decimal) IP addresses
}
\description{
Converts IP addresses in long integer format to character (dotted-decimal) notation
}
\examples{
long2ip(402654475)
long2ip(c(402654475, 3540208992))
}


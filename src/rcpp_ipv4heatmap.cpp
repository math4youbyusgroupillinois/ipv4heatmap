#include <Rcpp.h>
#include <boost/asio/ip/address_v4.hpp>

using namespace Rcpp;
using namespace boost::asio::ip;

#include "cidr.h"
#include "bbox.h"
#include "xy_from_ip.h"

extern void set_morton_mode();
extern void set_crop(const char *);
extern void set_bits_per_pixel(int);

unsigned int addr_space_first_addr;
unsigned int addr_space_last_addr;

#ifndef MIN
#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a>b?a:b)
#endif

int morton_flag = 0 ;

//' Convert IP addresses in dotted-decimal notation to long integers
//'
//' @param ip input character vector
//' @return vector of equivalent long integer IP addresses
// [[Rcpp::export]]
NumericVector ip2long (CharacterVector ip) {

  int ipCt = ip.size(); // how many elements in vector

  NumericVector ipInt(ipCt); // allocate new numeric vector

  // CONVERT ALL THE THINGS!
  for (int i=0; i<ipCt; i++) {
    ipInt[i] = address_v4::from_string(ip[i]).to_ulong();
  }

  return(ipInt);
}

//' Convert IP addresses in long integer format to dotted-decimal notation
//'
//' @param ip input numeric (long int) vector
//' @return vector of equivalent dotted-decimal IP addresses
// [[Rcpp::export]]
CharacterVector long2ip (NumericVector ip) {

  int ipCt = ip.size();

  CharacterVector ipStr(ipCt); // allocate new character vector
  // CONVERT ALL THE THINGS!
  for (int i=0; i<ipCt; i++) {
    ipStr[i] = address_v4(ip[i]).to_string();
  }

  return(ipStr);

}

extern int hilbert_curve_order;
extern int addr_space_bits_per_image;  /* /0 */
extern int addr_space_bits_per_pixel;  /* /24 */
extern unsigned int addr_space_first_addr;
extern unsigned int addr_space_last_addr;

extern void (*xy_from_s) (unsigned s, int n, unsigned *xp, unsigned *yp);


/*
 * Translate an IPv4 address (stored as a 32bit int) into
 * output X,Y coordinates.  First check if its within our
 * crop bounds.  Return 0 if out of bounds.
 */
unsigned int xy_from_ip(unsigned ip, unsigned *xp, unsigned *yp) {
    unsigned int s;
    if (ip < addr_space_first_addr)
  return 0;
    if (ip > addr_space_last_addr)
	return 0;
    s = (ip - addr_space_first_addr) >> addr_space_bits_per_pixel;
    xy_from_s(s, hilbert_curve_order, xp, yp);
    return 1;
}

int set_order() {
    hilbert_curve_order = (addr_space_bits_per_image - addr_space_bits_per_pixel) / 2;
    return hilbert_curve_order;
}

// [[Rcpp::export]]
NumericMatrix ipv4matrix(CharacterVector ip) {

unsigned int x;
  unsigned int y;

  set_order() ;

  NumericMatrix ipmap(4096, 4096);

  std::fill(ipmap.begin(), ipmap.end(), 0);

  NumericVector ipl = ip2long(ip) ;

  int ipCt = ipl.size() ;
  for (int i=0; i<ipCt; i++) {

    if (0 != xy_from_ip((unsigned)ipl[i], &x, &y)) {
       ipmap(x, y) = ipmap(x, y) + 1;
       // Rcpp::Rcout << i << " : " << x << " " << y << " " << ipmap(x, y) << std::endl;
    }

  }

  return(ipmap);

}

List boundingBoxFromCIDR(CharacterVector cidr) {

    bbox bbox;
    bbox = bbox_from_cidr(cidr[0]);
    List out(10) ;
    return(out) ;
}


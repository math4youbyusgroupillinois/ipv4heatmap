/*
 *                 ------------------------------------
 *                 iLab Neuromorphic Vision C++ Toolkit
 *                 ------------------------------------
 *
 *      The iLab Neuromorphic Vision C++ Toolkit - Copyright (C)
 *      2001-2005 by the University of Southern California (USC) and
 *      iLab at USC.
 *
 *      Major portions of the iLab Neuromorphic Vision C++ Toolkit are
 *      protected under the U.S. patent ``Computation of Intrinsic
 *      Perceptual Saliency in Visual Environments, and Applications''
 *      by Christof Koch and Laurent Itti, California Institute of
 *      Technology, 2001 (patent pending; application number
 *      09/912,225 filed July 23, 2001; see
 *      http://pair.uspto.gov/cgi-bin/final/home.pl for current
 *      status)
 *
 *      This file is part of the iLab Neuromorphic Vision C++ Toolkit.
 *
 *      The iLab Neuromorphic Vision C++ Toolkit is free software; you
 *      can redistribute it and/or modify it under the terms of the
 *      GNU General Public License as published by the Free Software
 *      Foundation; either version 2 of the License, or (at your
 *      option) any later version.
 *
 *      The iLab Neuromorphic Vision C++ Toolkit is distributed in the
 *      hope that it will be useful, but WITHOUT ANY WARRANTY; without
 *      even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 *      PARTICULAR PURPOSE.  See the GNU General Public License for
 *      more details.
 *
 *      You should have received a copy of the GNU General Public
 *      License along with the iLab Neuromorphic Vision C++ Toolkit
 *      (see the top-level file named COPYING); if not, write to the
 *      Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 *      Boston, MA 02111-1307 USA.
 */

// ######################################################################
//T.Nathan Mundhenk
// mundhenk @ usc.edu
// C / C++ Macro HSV to RGB
#define PIX_HSV_TO_RGB_COMMON(H,S,V,R,G,B)                          \
if( V == 0 )                                                        \
{ R = 0; G = 0; B = 0; }                                            \
else if( S == 0 )                                                   \
{                                                                   \
  R = V;                                                            \
  G = V;                                                            \
  B = V;                                                            \
}                                                                   \
else                                                                \
{                                                                   \
  const double hf = H / 60.0;                                       \
  const int    i  = (int) floor( hf );                              \
  const double f  = hf - i;                                         \
  const double pv  = V * ( 1 - S );                                 \
  const double qv  = V * ( 1 - S * f );                             \
  const double tv  = V * ( 1 - S * ( 1 - f ) );                     \
  switch( i )                                                       \
    {                                                               \
    case 0:                                                         \
      R = V;                                                        \
      G = tv;                                                       \
      B = pv;                                                       \
      break;                                                        \
    case 1:                                                         \
      R = qv;                                                       \
      G = V;                                                        \
      B = pv;                                                       \
      break;                                                        \
    case 2:                                                         \
      R = pv;                                                       \
      G = V;                                                        \
      B = tv;                                                       \
      break;                                                        \
    case 3:                                                         \
      R = pv;                                                       \
      G = qv;                                                       \
      B = V;                                                        \
      break;                                                        \
    case 4:                                                         \
      R = tv;                                                       \
      G = pv;                                                       \
      B = V;                                                        \
      break;                                                        \
    case 5:                                                         \
      R = V;                                                        \
      G = pv;                                                       \
      B = qv;                                                       \
      break;                                                        \
    case 6:                                                         \
      R = V;                                                        \
      G = tv;                                                       \
      B = pv;                                                       \
      break;                                                        \
    case -1:                                                        \
      R = V;                                                        \
      G = pv;                                                       \
      B = qv;                                                       \
      break;                                                        \
    default:                                                        \
      abort();                                                      \
      break;                                                        \
    }                                                               \
}                                                                   \
R *= 255.0F;                                                        \
G *= 255.0F;                                                        \
B *= 255.0F;


/*
 * IPv4 Heatmap
 * (C) 2007 The Measurement Factory, Inc
 * Licensed under the GPL, version 2
 * http://maps.measurement-factory.com/
 */


struct bb {
    int xmin, ymin, xmax, ymax;
};

typedef struct bb bbox;

int cidr_parse(const char *cidr, unsigned int *rfirst, unsigned int *rlast, int *rslash);
void hil_xy_from_s(unsigned s, int order, unsigned *xp, unsigned *yp);
void mor_xy_from_s(unsigned s, int order, unsigned *xp, unsigned *yp);
unsigned int xy_from_ip(unsigned ip, unsigned *xp, unsigned *yp);
bbox bbox_from_cidr(const char *prefix);
void (*xy_from_s) (unsigned s, int n, unsigned *xp, unsigned *yp) = hil_xy_from_s;

#define BBOX_WIDTH(b) (b.xmax-b.xmin)
#define BBOX_HEIGHT(b) (b.ymax-b.ymin)
#define BBOX_CTR_X(b) ((b.xmax+b.xmin)/2)
#define BBOX_CTR_Y(b) ((b.ymax+b.ymin)/2)
#define BBOX_SET(B,W,X,Y,Z) B.xmin=W; B.ymin=X; B.xmax=Y; B.ymax=Z;
#define BBOX_PRINT(B) fprintf(stderr, "%s=%d,%d,%d,%d\n", #B, B.xmin,B.ymin,B.xmax,B.ymax)


unsigned int allones = ~0;
int debug = 0;


/*
 * Parse a CIDR string
 */
int cidr_parse(const char *cidr, unsigned int *rfirst, unsigned int *rlast, int *rslash) {
    char cidr_copy[24];
    char *t;
    int slash;
    unsigned int first;
    unsigned int last;
    strncpy(cidr_copy, cidr, 24);
    t = strchr(cidr_copy, '/');
    if (NULL == t) {
  warnx("missing / on CIDR '%s'\n", cidr_copy);
  return 0;;
    }
    *t++ = '\0';
    slash = atoi(t);
    if (1 != inet_pton(AF_INET, cidr_copy, &first)) {
  warnx("inet_pton failed on '%s'\n", cidr_copy);
	return 0;
    }
    first = ntohl(first);
    if (slash < 32)
	last = first | (allones >> slash);
    else
	last = first;
    *rfirst = first;
    *rlast = last;
    *rslash = slash;
    return 1;
}

/*
 * The default the Hilbert curve order is 12.  This gives a 4096x4096
 * output image covering the entire space where each pixel represents
 * a /24.
 */
int addr_space_bits_per_image = 32;  /* /0 */
int addr_space_bits_per_pixel = 8;  /* /24 */
unsigned int addr_space_first_addr = 0;
unsigned int addr_space_last_addr = ~0;
int hilbert_curve_order = (addr_space_bits_per_image - addr_space_bits_per_pixel) / 2;

void set_morton_mode();
void set_crop(const char *);
void set_bits_per_pixel(int);


#ifndef MIN
#define MIN(a,b) (a<b?a:b)
#define MAX(a,b) (a>b?a:b)
#endif

int morton_flag = 0 ;


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


void set_morton_mode() {
    xy_from_s = mor_xy_from_s;
}

int set_order() {
    hilbert_curve_order = (addr_space_bits_per_image - addr_space_bits_per_pixel) / 2;
    return hilbert_curve_order;
}

void set_crop(const char *cidr) {
    cidr_parse(cidr, &addr_space_first_addr, &addr_space_last_addr, &addr_space_bits_per_image);
    addr_space_bits_per_image = 32 - addr_space_bits_per_image;
    if (1 == (addr_space_bits_per_image % 2))
	errx(1, "Space to render must have even number of CIDR bits");
}

void set_bits_per_pixel(int bpp) {
    addr_space_bits_per_pixel = bpp;
    if (1 == (addr_space_bits_per_pixel % 2))
	errx(1, "CIDR bits per pixel must be even");
}


/*
 * Z_order curve calculation
 * Roy Arends
 * Nominet
 */

void mor_xy_from_s(unsigned s, int order, unsigned *xp, unsigned *yp) {
    int i;
    unsigned x, y;
    x = y = 0;
    for (i = 2 * order - 2; i >= 0; i -= 2) {
  x = (x << 1) | ((s >> i) & 1);
  y = (y << 1) | ((s >> (i + 1)) & 1);
    }
    *xp = x;
    *yp = y;
}


/*
 * Figure 14-5 from Hacker's Delight (by Henry S. Warren, Jr. published by
 * Addison Wesley, 2002)
 *
 * See also http://www.hackersdelight.org/permissions.htm
 */

void hil_xy_from_s(unsigned s, int order, unsigned *xp, unsigned *yp) {

    int i;
    unsigned state, x, y, row;

    state = 0;      /* Initialize. */
    x = y = 0;

    for (i = 2 * order - 2; i >= 0; i -= 2) {	/* Do n times. */
	row = 4 * state | ((s >> i) & 3);	/* Row in table. */
	x = (x << 1) | ((0x936C >> row) & 1);
	y = (y << 1) | ((0x39C6 >> row) & 1);
	state = (0x3E6B94C1 >> 2 * row) & 3;	/* New state. */
    }
    *xp = x;			/* Pass back */
    *yp = y;			/* results. */
}



/*
 * Find the "bounding box" for the IPv4 netblock starting at 'first' and having
 * 'slash' netmask bits.
 *
 * For square areas this is pretty easy.  We know how to find the point diagonally
 * opposite the first value (add 1010..1010). Its a little harder for
 * rectangular areas, so I cheat a little and divide it into the two smaller
 * squares.
 */
static bbox bounding_box(unsigned int first, int slash) {
    bbox box;
    unsigned int diag = 0xAAAAAAAA;
    if (morton_flag) { diag = 0xFFFFFFFF; }
    unsigned int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

    if (slash > 31) {
  /*
	 * treat /32 as a special case
	 */
	xy_from_ip(first, &x1, &y1);
	box.xmin = x1;
	box.ymin = y1;
	box.xmax = x1;
	box.ymax = y1;
    } else if (0 == (slash & 1)) {
	/*
	 * square
	 */
	diag >>= slash;
	xy_from_ip(first, &x1, &y1);
	xy_from_ip(first + diag, &x2, &y2);
	box.xmin = MIN(x1, x2);
	box.ymin = MIN(y1, y2);
	box.xmax = MAX(x1, x2);
	box.ymax = MAX(y1, y2);
    } else {
	/*
	 * rectangle: divide, conquer
	 */
	bbox b1 = bounding_box(first, slash + 1);
	bbox b2 = bounding_box(first + (1 << (32 - (slash + 1))), slash + 1);
	box.xmin = MIN(b1.xmin, b2.xmin);
	box.ymin = MIN(b1.ymin, b2.ymin);
	box.xmax = MAX(b1.xmax, b2.xmax);
	box.ymax = MAX(b1.ymax, b2.ymax);
    }
    return box;
}

/*
 * Calculate the bounding box of a CIDR prefix string
 */
bbox bbox_from_cidr(const char *cidr) {
    int slash;
    unsigned int first;
    unsigned int last;
    bbox bbox;
    cidr_parse(cidr, &first, &last, &slash);
    if (first < addr_space_first_addr || last > addr_space_last_addr) {
      bbox.xmin = bbox.ymin = bbox.xmax = bbox.ymax = -1;
      return bbox;
    }
    memset(&bbox, '\0', sizeof(bbox));
    bbox = bounding_box(first, slash);

    return bbox;
}

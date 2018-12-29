/*!
 * \file      portable_endian.h
 * \license   https://creativecommons.org/publicdomain/zero/1.0/
 *
 * Original author:
 * - Mathias Panzenböck (https://gist.github.com/panzi/6856583)
 * Related resources:
 * - https://en.wikipedia.org/wiki/Endianness
 * - https://sourceforge.net/p/predef/wiki/Endianness/
 * - http://stackoverflow.com/questions/2100331/c-macro-definition-to-determine-big-endian-or-little-endian-machine
 */

#ifndef PORTABLE_ENDIAN_H
#define PORTABLE_ENDIAN_H
/* ************************************************************************** */


#define LITTLE_ENDIAN   1234
#define BIG_ENDIAN      4321
#define PDP_ENDIAN      3412
#define BYTE_ORDER      LITTLE_ENDIAN
#define __BIG_ENDIAN BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN

/* ************************************************************************** */
#endif // PORTABLE_ENDIAN_H

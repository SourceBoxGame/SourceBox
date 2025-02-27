//========= Copyright � 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
// netadr.h
#ifndef NETADR_H
#define NETADR_H
#ifdef _WIN32
#pragma once
#endif

#include "tier0/platform.h"
#undef SetPort

typedef enum
{ 
	NA_NULL = 0,
	NA_LOOPBACK,
	NA_BROADCAST,
	NA_IP,
} netadrtype_t;

struct netadr_t
{
public:
	netadr_t() { SetIP( 0 ); SetPort( 0 ); SetType( NA_IP ); }
	netadr_t( uint unIP, uint16 usPort ) { SetIP( unIP ); SetPort( usPort ); SetType( NA_IP ); }
	netadr_t( const char *pch ) { SetFromString( pch ); }
	void	Clear();	// invalids Address

	void	SetType( netadrtype_t type );
	void	SetPort( unsigned short port );
	bool	SetFromSockadr(const struct sockaddr *s);
	void	SetIP(uint8 b1, uint8 b2, uint8 b3, uint8 b4);
	void	SetIP(uint unIP);									// Sets IP.  unIP is in host order (little-endian)
	void    SetIPAndPort( uint unIP, unsigned short usPort ) { SetIP( unIP ); SetPort( usPort ); }
	void	SetFromString(const char *pch, bool bUseDNS = false ); // if bUseDNS is true then do a DNS lookup if needed
	
	bool	CompareAdr (const netadr_t &a, bool onlyBase = false) const;
	bool	CompareClassBAdr (const netadr_t &a) const;
	bool	CompareClassCAdr (const netadr_t &a) const;

	netadrtype_t	GetType() const;
	unsigned short	GetPort() const;
	const char*		ToString( bool onlyBase = false ) const; // returns xxx.xxx.xxx.xxx:ppppp
	void			ToSockadr(struct sockaddr *s) const;
	unsigned int	GetIP() const;

	bool	IsLocalhost() const; // true, if this is the localhost IP 
	bool	IsLoopback() const;	// true if engine loopback buffers are used
	bool	IsReservedAdr() const; // true, if this is a private LAN IP
	bool	IsValid() const;	// ip & port != 0
	bool	IsBaseAdrValid() const;	// ip != 0

	void    SetFromSocket( int hSocket );

	// These function names are decorated because the Xbox360 defines macros for ntohl and htonl
 	unsigned long addr_ntohl() const;
 	unsigned long addr_htonl() const;
	bool operator==(const netadr_t &netadr) const {return ( CompareAdr( netadr ) );}
	bool operator<(const netadr_t &netadr) const;

public:	// members are public to avoid to much changes

	netadrtype_t	type;
	unsigned char	ip[4];
	unsigned short	port;
};

#endif // NETADR_H

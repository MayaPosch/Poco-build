//
// SocketAddress.cpp
//
// Library: Net
// Package: NetCore
// Module:  SocketAddress
//
// Copyright (c) 2005-2011, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/IPAddress.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/DNS.h"
#include "Poco/NumberFormatter.h"
//#include "Poco/RefCountedObject.h"
#include "Poco/NumberParser.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"
#include <algorithm>
#include <cstring>
#include <iostream>


//using Poco::RefCountedObject;
using Poco::NumberParser;
using Poco::UInt16;
using Poco::InvalidArgumentException;
//using Poco::Net::Impl::SocketAddressImpl;
//using Poco::Net::Impl::IPv4SocketAddressImpl;
#ifdef POCO_HAVE_IPv6
//using Poco::Net::Impl::IPv6SocketAddressImpl;
#endif
#ifdef POCO_OS_FAMILY_UNIX
//using Poco::Net::Impl::LocalSocketAddressImpl;
#endif


namespace Poco {
namespace Net {


struct AFLT {
	bool operator () (const IPAddress& a1, const IPAddress& a2) {
		return a1.af() < a2.af();
	}
};


//
// SocketAddress
//


#if !defined(_MSC_VER) || defined(__STDC__)
// Go home MSVC, you're drunk...
// See http://stackoverflow.com/questions/5899857/multiple-definition-error-for-static-const-class-members
const SocketAddress::Family SocketAddress::IPv4;
#if defined(POCO_HAVE_IPv6)
const SocketAddress::Family SocketAddress::IPv6;
#endif
#if defined(POCO_OS_FAMILY_UNIX)
const SocketAddress::Family SocketAddress::UNIX_LOCAL;
#endif
#endif


SocketAddress::SocketAddress() {
	//newIPv4();
}


SocketAddress::SocketAddress(Family fam) {
		init(IPAddress(fam), 0);
}


SocketAddress::SocketAddress(const IPAddress& hostAddress, Poco::UInt16 portNumber) {
	init(hostAddress, portNumber);
}


SocketAddress::SocketAddress(Poco::UInt16 portNumber) {
	init(IPAddress(), portNumber);
}


SocketAddress::SocketAddress(Family fam, Poco::UInt16 portNumber) {
	init(IPAddress(fam), portNumber);
}


SocketAddress::SocketAddress(const std::string& hostAddress, Poco::UInt16 portNumber) {
	std::cout << "SocketAddress constructor: " << hostAddress << std::endl;
	init(hostAddress, portNumber);
}


SocketAddress::SocketAddress(Family fam, const std::string& hostAddress, Poco::UInt16 portNumber) {
	init(fam, hostAddress, portNumber);
}


SocketAddress::SocketAddress(const std::string& hostAddress, const std::string& portNumber) {
	init(hostAddress, resolveService(portNumber));
}


SocketAddress::SocketAddress(Family fam, const std::string& hostAddress, const std::string& portNumber) {
	init(fam, hostAddress, resolveService(portNumber));
}


SocketAddress::SocketAddress(Family fam, const std::string& addr) {
	init(fam, addr);
}


SocketAddress::SocketAddress(const std::string& hostAndPort) {
	init(hostAndPort);
}


SocketAddress::SocketAddress(const SocketAddress& socketAddress) {
	if (socketAddress.family() == IPv4) {
		//newIPv4(reinterpret_cast<const sockaddr_in*>(socketAddress.addr()));
		sockAddr = *((const SockAddr::SockAddrIPv4*) socketAddress.addr());
	}
#if defined(POCO_HAVE_IPv6)
	else if (socketAddress.family() == IPv6) {
		//newIPv6(reinterpret_cast<const sockaddr_in6*>(socketAddress.addr()));
		sockAddr = *((const SockAddr::SockAddrIPv6*) socketAddress.addr());
	}
#endif
#if defined(POCO_OS_FAMILY_UNIX)
	else if (socketAddress.family() == UNIX_LOCAL) {
		//newLocal(reinterpret_cast<const sockaddr_un*>(socketAddress.addr()));
		sockAddr = *((const SockAddr::SockAddrUnix*) socketAddress.addr());
	}
#endif
}


SocketAddress::SocketAddress(const struct sockaddr* sockAddr, poco_socklen_t length) {
	if (length == sizeof(struct sockaddr_in) && sockAddr->sa_family == AF_INET) {
		//newIPv4(reinterpret_cast<const struct sockaddr_in*>(sockAddr));
		this->sockAddr = *((const SockAddr::SockAddrIPv4*) sockAddr);
	}
#if defined(POCO_HAVE_IPv6)
	else if (length == sizeof(struct sockaddr_in6) && sockAddr->sa_family == AF_INET6) {
		//newIPv6(reinterpret_cast<const struct sockaddr_in6*>(sockAddr));
		this->sockAddr = *((const SockAddr::SockAddrIPv6*) sockAddr);
	}
#endif
#if defined(POCO_OS_FAMILY_UNIX)
	else if (length > 0 && length <= sizeof(struct sockaddr_un) && sockAddr->sa_family == AF_UNIX) {
		//newLocal(reinterpret_cast<const sockaddr_un*>(sockAddr));
		this->sockAddr = *((const SockAddr::SockAddrUnix*) sockAddr);
	}
#endif
}


SocketAddress::~SocketAddress() { }


bool SocketAddress::operator < (const SocketAddress& socketAddress) const {
	if (family() < socketAddress.family()) return true;
	if (family() > socketAddress.family()) return false;
#if defined(POCO_OS_FAMILY_UNIX)
	if (family() == UNIX_LOCAL) return toString() < socketAddress.toString();
#endif
	if (host() < socketAddress.host()) return true;
	if (host() > socketAddress.host()) return false;
	return (port() < socketAddress.port());
}


SocketAddress& SocketAddress::operator = (const SocketAddress& socketAddress) {
	if (&socketAddress != this) {
		if (socketAddress.family() == IPv4) {
			//newIPv4(reinterpret_cast<const sockaddr_in*>(socketAddress.addr()));
			sockAddr = *((const SockAddr::SockAddrIPv4*) socketAddress.addr());
		}
#if defined(POCO_HAVE_IPv6)
		else if (socketAddress.family() == IPv6) {
			//newIPv6(reinterpret_cast<const sockaddr_in6*>(socketAddress.addr()));
			sockAddr = *((const SockAddr::SockAddrIPv6*) socketAddress.addr());
		}
#endif
#if defined(POCO_OS_FAMILY_UNIX)
		else if (socketAddress.family() == UNIX_LOCAL) {
			//newLocal(reinterpret_cast<const sockaddr_un*>(socketAddress.addr()));
			sockAddr = *((SockAddr::SockAddrUnix*) socketAddress.addr());
		}
#endif
	}
	
	return *this;
}


IPAddress SocketAddress::host() const {
	//return pImpl()->host();
	if (sockAddr.sin_family == AF_INET) {
		return IPAddress(&sockAddr.ipv4.sin_addr, sizeof(sockAddr.ipv4.sin_addr));
	}
#if defined(POCO_HAVE_IPv6)
	else if (sockAddr.sin_family == AF_INET6) {
		return IPAddress(&sockAddr.ipv6.sin_addr, sizeof(sockAddr.ipv6.sin_addr), sockAddr.ipv6.sin_scope_id);
	}
#endif
		
	return IPAddress();
 }


Poco::UInt16 SocketAddress::port() const {
	//return ntohs(pImpl()->port());
	if (sockAddr.sin_family == AF_INET) {
		return ntohs(sockAddr.ipv4.sin_port);
	}
#if defined(POCO_HAVE_IPv6)
	else if (sockAddr.sin_family == AF_INET6) {
		return ntohs(sockAddr.ipv6.sin_port);
	}
#endif
		
	return 0;
}


poco_socklen_t SocketAddress::length() const {
	//return pImpl()->length();
	return sizeof(sockAddr);
}


const struct sockaddr* SocketAddress::addr() const {
	//return pImpl()->addr();
	return (const struct sockaddr*) &sockAddr;
}


int SocketAddress::af() const {
	//return pImpl()->af();
	return sockAddr.sin_family;
}


SocketAddress::Family SocketAddress::family() const {
	//return static_cast<Family>(pImpl()->family());
	return (SocketAddress::Family) sockAddr.sin_family;
}


std::string SocketAddress::toString() const {
	//return pImpl()->toString();
#if defined(POCO_OS_FAMILY_UNIX)
	if (sockAddr.sin_family == AF_UNIX) {
		std::string result(sockAddr.unix.sun_path);
		return result;
	}
	
#endif
	if (sockAddr.sin_family == AF_INET) {
		const UInt8* bytes = reinterpret_cast<const UInt8*>(&sockAddr.ipv4.sin_addr);
		std::string result;
		result.reserve(16);
		NumberFormatter::append(result, bytes[0]);
		result.append(".");
		NumberFormatter::append(result, bytes[1]);
		result.append(".");
		NumberFormatter::append(result, bytes[2]);
		result.append(".");
		NumberFormatter::append(result, bytes[3]);
		return result;
	}
#if defined(POCO_HAVE_IPv6)
	else if (sockAddr.sin_family == AF_INET6) {
		return "TODO: SocketAddress::toString(): Implement ipv6";
	}
#endif
	return "SocketAddress::toString(): unhandled sin_family";
}


void SocketAddress::init(const IPAddress& hostAddress, Poco::UInt16 portNumber) {
	std::cout << "SocketAddress Init(IPAddress, uint16_t)." << std::endl;
	if (hostAddress.family() == IPAddress::IPv4) {
		std::cout << "SocketAddress family IPv4." << std::endl;
		//newIPv4(hostAddress, portNumber);
		
		// debug
		std::cout << "SocketAddress newIPv4. Port: " << portNumber << std::endl;
		if (hostAddress.family() == Poco::Net::AddressFamily::IPv4) {
			std::cout << "IPAddress is IPv4." << std::endl;
		}
		
		//_pImpl = new Poco::Net::Impl::IPv4SocketAddressImpl(hostAddress.addr(), htons(portNumber));
		SockAddr::SockAddrIPv4 ad;
		ad.sin_port = htons(portNumber);
		ad.sin_addr = *((const in_addr*) hostAddress.addr());
		sockAddr = ad;
		
		// debug
		std::cout << "After constructor Port: " << sockAddr.ipv4.sin_port << std::endl;
		std::cout << "Socket address: ";
		std::cout << std::ios::hex;
		/* for (int32_t i = 0; i < 16; ++i) { 
			std::cout << (uint16_t) (_pImpl->addr()->sa_data)[i] << " ";
		} */
		
		std::cout << std::ios::dec << std::endl;
	}
#if defined(POCO_HAVE_IPv6)
	else if (hostAddress.family() == IPAddress::IPv6) {
		//newIPv6(hostAddress, portNumber);
		SockAddr::SockAddrIPv6 ad;
		ad.sin_port = htons(portNumber);
		ad.sin_addr = *((const in6_addr*) hostAddress.addr());
		sockAddr = ad;
	}
#endif
	else throw Poco::NotImplementedException("unsupported IP address family");
}


void SocketAddress::init(const std::string& hostAddress, Poco::UInt16 portNumber) {
	std::cout << "SocketAddress init: " << hostAddress << std::endl;
	IPAddress ip;
	if (IPAddress::tryParse(hostAddress, ip)) {
		std::cout << "SocketAddress IPAddress parsed." << std::endl;
		init(ip, portNumber);
	}
	else {
		HostEntry he = DNS::hostByName(hostAddress);
		HostEntry::AddressList addresses = he.addresses();
		if (addresses.size() > 0) {
#if defined(POCO_HAVE_IPv6) && defined(POCO_SOCKETADDRESS_PREFER_IPv4)
			// if we get both IPv4 and IPv6 addresses, prefer IPv4
			std::stable_sort(addresses.begin(), addresses.end(), AFLT());
#endif
			init(addresses[0], portNumber);
		}
		else throw HostNotFoundException("No address found for host", hostAddress);
	}
}


void SocketAddress::init(Family fam, const std::string& hostAddress, Poco::UInt16 portNumber) {
	IPAddress ip;
	if (IPAddress::tryParse(hostAddress, ip)) {
		if (ip.family() != fam) throw AddressFamilyMismatchException(hostAddress);
		init(ip, portNumber);
	}
	else {
		HostEntry he = DNS::hostByName(hostAddress);
		HostEntry::AddressList addresses = he.addresses();
		if (addresses.size() > 0) {
			for (const auto& addr: addresses) {
				if (addr.family() == fam) {
					init(addr, portNumber);
					return;
				}
			}
			
			throw AddressFamilyMismatchException(hostAddress);
		}
		else {
			throw HostNotFoundException("No address found for host", hostAddress);
		}
	}
}


void SocketAddress::init(Family fam, const std::string& address) {
#if defined(POCO_OS_FAMILY_UNIX)
	if (fam == UNIX_LOCAL) {
		//newLocal(address);
		if (address.length() <= sizeof(((SockAddr::SockAddrUnix*) &sockAddr)->sun_path)) {
			SockAddr::SockAddrUnix ad;
			memcpy(&ad.sun_path, address.c_str(), address.length());
			sockAddr = ad;
		}
		
		return;
	}
	
#endif
	std::string host;
	std::string port;
	std::string::const_iterator it  = address.begin();
	std::string::const_iterator end = address.end();

	if (*it == '[') {
		++it;
		while (it != end && *it != ']') { host += *it++; }
		if (it == end) throw InvalidArgumentException("Malformed IPv6 address");
		++it;
	}
	else {
		while (it != end && *it != ':') { host += *it++; }
	}
	
	if (it != end && *it == ':') {
		++it;
		while (it != end) { port += *it++; }
	}
	else throw InvalidArgumentException("Missing port number");
	init(fam, host, resolveService(port));
}


void SocketAddress::init(const std::string& hostAndPort) {
	poco_assert (!hostAndPort.empty());

	std::string host;
	std::string port;
	std::string::const_iterator it  = hostAndPort.begin();
	std::string::const_iterator end = hostAndPort.end();

#if defined(POCO_OS_FAMILY_UNIX)
	if (*it == '/') {
		//newLocal(hostAndPort);
		if (hostAndPort.length() <= sizeof(((SockAddr::SockAddrUnix*) &sockAddr)->sun_path)) {
			SockAddr::SockAddrUnix ad;
			memcpy(&ad.sun_path, hostAndPort.c_str(), hostAndPort.length());
			sockAddr = ad;
		}
		
		return;
	}
	
#endif
	if (*it == '[') {
		++it;
		while (it != end && *it != ']') { host += *it++; }
		if (it == end) {
			throw InvalidArgumentException("Malformed IPv6 address");
		}
		
		++it;
	}
	else {
		while (it != end && *it != ':') { host += *it++; }
	}
	
	if (it != end && *it == ':') {
		++it;
		while (it != end) { port += *it++; }
	}
	else {
		throw InvalidArgumentException("Missing port number");
	}
	
	init(host, resolveService(port));
}


Poco::UInt16 SocketAddress::resolveService(const std::string& service) {
	unsigned port;
	if (NumberParser::tryParseUnsigned(service, port) && port <= 0xFFFF) {
		return (UInt16) port;
	}
	else {
#if defined(POCO_VXWORKS)
		throw ServiceNotFoundException(service);
#else
		struct servent* se = getservbyname(service.c_str(), NULL);
		if (se)
			return ntohs(se->s_port);
		else
			throw ServiceNotFoundException(service);
#endif
	}
}


} } // namespace Poco::Net


Poco::BinaryWriter& operator << (Poco::BinaryWriter& writer, const Poco::Net::SocketAddress& value)
{
	writer << value.host();
	writer << value.port();
	return writer;
}


Poco::BinaryReader& operator >> (Poco::BinaryReader& reader, Poco::Net::SocketAddress& value)
{
	Poco::Net::IPAddress host;
	reader >> host;
	Poco::UInt16 port;
	reader >> port;
	value = Poco::Net::SocketAddress(host, port);
	return reader;
}


std::ostream& operator << (std::ostream& ostr, const Poco::Net::SocketAddress& address)
{
	ostr << address.toString();
	return ostr;
}

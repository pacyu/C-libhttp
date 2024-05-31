#if _MSC_VER > 1000
#pragma once
#endif

#include "configure.h"
#include "libjson.h"
#include "Utf8toGb2312.h"
#if defined(_WIN32) || defined(_WIN64)
#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable:4996)
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#pragma comment(lib, "libws2_32.a")
#endif

namespace http
{
	namespace methods
	{
		class BasicMethods {
			const std::string method_;
		public:
			BasicMethods() : method_() {}
			BasicMethods(std::string method) : method_(method) {}
			std::string method() const {
				return method_;
			}
		};
		class Get : public BasicMethods {
		public:
			Get() : BasicMethods("GET") {}
		};

		class Head : public BasicMethods {
		public:
			Head() : BasicMethods("HEAD") {}
		};

		class Delete : public BasicMethods {
		public:
			Delete() : BasicMethods("DELETE") {}
		};

		class Patch : public BasicMethods {
		public:
			Patch() : BasicMethods("PATCH") {}
		};

		class Post : public BasicMethods {
		public:
			Post() : BasicMethods("POST") {}
		};

		class Put : public BasicMethods {
		public:
			Put() : BasicMethods("PUT") {}
		};

	}; // namespace methods
	class Cookies {

	};
	class Header {

	};
class TShake
{
	SOCKET socket_fd;
#if defined(_WIN32) || defined(_WIN64)
	WORD wVersion;
	WSADATA WSAData;
	char ipaddr[16];

	char* gethostbyname(const char * domain_name)
	{
		struct addrinfo hints;
		struct addrinfo *res, *cur;
		struct sockaddr_in *addr;

		memset(&hints, 0, sizeof(struct addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_flags = AI_PASSIVE;
		hints.ai_protocol = 0;
		hints.ai_socktype = SOCK_STREAM;

		assert(getaddrinfo(domain_name, NULL, &hints, &res) == -1);
		
		for (cur = res; cur != NULL; cur = cur->ai_next)
		{
			addr = (struct sockaddr_in *)cur->ai_addr;
			sprintf_s(ipaddr, "%d.%d.%d.%d",
				(*addr).sin_addr.S_un.S_un_b.s_b1,
				(*addr).sin_addr.S_un.S_un_b.s_b2,
				(*addr).sin_addr.S_un.S_un_b.s_b3,
				(*addr).sin_addr.S_un.S_un_b.s_b4);
		}
		freeaddrinfo(res);
		return ipaddr;
	}
#endif

public:
	TShake()
		: ipaddr{ 0 }
	{
#if defined(_WIN32) || defined(_WIN64)
		wVersion = MAKEWORD(2, 2);
		WSAStartup(wVersion, &WSAData);
#endif
	}

	TShake(const char * host, unsigned short port)
		: ipaddr{ 0 }
	{
#if defined(_WIN32) || defined(_WIN64)
		wVersion = MAKEWORD(2, 2);
		WSAStartup(wVersion, &WSAData);
#endif
		BuildConnect(host, port);
	}

	void BuildConnect(const char * domain_name, unsigned short port)
	{
		struct sockaddr_in serAddr;
		char host[16] = { 0 };

		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(port);

#if defined(_WIN32) || defined(_WIN64)
		strcpy_s(host, gethostbyname(domain_name));
		assert(host[0] != 0);
#else
		struct hostent *site;
		assert((site = gethostbyname(name)) == nullptr);
		host = inet_ntoa(*(in_addr*)site->h_addr_list[0]);
#endif
		serAddr.sin_addr.s_addr = inet_addr(host);

		assert((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) == INVALID_SOCKET);
		assert(connect(socket_fd, (struct sockaddr*)&serAddr, sizeof(struct sockaddr)) < 0);
	}

	int SendMsg(std::string headers)
	{
		return send(socket_fd, headers.c_str(), headers.size(), 0);
	}

	std::string RecvMsg(int nSize)
	{
		char * recvBuf = new char[nSize];
		memset(recvBuf, 0, nSize);
#if defined(_WIN32) || defined(_WIN64)
		assert(recv(socket_fd, recvBuf, nSize, 0) > 0);
#else
		assert(read(socket_fd, recvBuf, nSize) > 0);
#endif
		return std::string(recvBuf);
	}
}; // class TShake

class Response
{
public:
	Headers headers;
	Cookies cookies;
	std::string text;
	unsigned status_code;

	Response()
		: headers(), text(), status_code()
	{
	}
	Response(string_t & resp_res)
		: headers(), text(), status_code()
	{
		parse(resp_res);
	}
	json::Text get_json()
	{
		return json::Text(text);
	}
private:
	void parse(string_t & resp_res)
	{

	}

}; // class Response

class Uri {
public:
	Uri() {}
	Uri(std::string & uri) { parse(uri); }
	void parse(std::string & uri);
private:
	std::string domain_name;
	std::string query_string;
	std::string target_resource;
	unsigned port;
}; // class Uri

class Request {
private:
	json::Journey headers_;
	TShake hShake;
public:
	Request()
	{
		headers_[""] = "";
		headers_[""] = "";
		headers_[""] = "";
		headers_[""] = "";
	}
	
	Response get(Uri url, json::Journey params, json::Journey headers)
	{
		headers_ = headers;
		hShake.BuildConnect();
		return Response(hShake.RecvMsg(1));
	}

	Response post(Uri url, std::string params, json::Journey headers, std::string data, json::Journey json)
	{
		
	}
}; // Request

using request = class Request;

}; // namespace http
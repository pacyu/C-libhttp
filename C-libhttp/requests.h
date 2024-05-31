#pragma once

#include "json.h"
#if defined(_WIN32) || defined(_WIN64)
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <sstream>
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
	class methods
	{
	public:
		methods() : m_name() {}
		methods(std::string m_name)
			: m_name(m_name)
		{
		}

		std::string name() const
		{
			return m_name;
		}
	private:
		std::string m_name;
	};

	class Delete : public methods
	{
	public:
		Delete() : methods("DELETE") {}
	};
	class Get : public methods
	{
	public:
		Get() : methods("GET") {}
	};
	class Head : public methods
	{
	public:
		Head() : methods("HEAD") {}
	};
	class Patch : public methods
	{
	public:
		Patch() : methods("PATCH") {}
	};
	class Post : public methods
	{
	public:
		Post() : methods("POST") {}
	};
	class Put : public methods
	{
	public:
		Put() : methods("PUT") {}
	};

	class form_data
	{
	public:
		form_data() : m_data(json::value::object()) {}
		form_data(std::vector<std::pair<details::string_t, json::value>> fields)
			: m_data(json::value::object(fields))
		{
		}

		json::value& operator[](details::string_t key)
		{
			return m_data[key];
		}
		json::value operator=(details::string_t value)
		{
			return json::value(value);
		}
		details::string_t to_string() const
		{
			details::string_t data_to_string;
			auto m_data = this->m_data.as_object();
			for (auto iter : m_data)
			{
				data_to_string += iter.first + "=" + iter.second.as_string() + "&";
			}
			data_to_string.erase(data_to_string.end() - 1);
			return data_to_string;
		}

	private:
		json::value m_data;
	};

	class parameters
	{
	public:
		parameters() : m_params(json::value::object()) {}
		parameters(std::vector<std::pair<details::string_t, json::value>> fields)
			: m_params(json::value::object(fields))
		{
		}

		json::value& operator[](details::string_t key)
		{
			return m_params[key];
		}
		json::value operator=(details::string_t value)
		{
			return json::value(value);
		}
		details::string_t to_string() const
		{
			details::string_t params_to_string = "?";
			auto m_data = m_params.as_object();
			for (auto iter : m_data)
			{
				params_to_string += iter.first + "=" + iter.second.as_string() + "&";
			}
			params_to_string.erase(params_to_string.end() - 1);
			return params_to_string;
		}
		void parse(details::string_t query)
		{
			std::vector<details::string_t> params = query.split('&');
			for (auto iter : params)
			{
				std::vector<details::string_t> param = iter.split('=');
				m_params[param[0]] = param.size() > 1 ? json::value(param[1]) : json::value("");
			}
		}

	private:
		json::value m_params;
	};

	class cookies
	{
	public:
		json::value& operator[](details::string_t key)
		{
			return m_cookies[key];
		}
		json::value operator=(details::string_t value)
		{
			return json::value(value);
		}
		details::string_t to_string() const
		{
			details::string_t cookies_to_string;
			for (auto iter : m_cookies.as_object())
			{
				cookies_to_string += iter.first + "=" + iter.second.as_string() + "; ";
			}
			return cookies_to_string;
		}

		void parse(details::string_t str_cookie)
		{

		}

	private:
		json::value m_cookies;
	};

	class first_row
	{
	public:
		first_row()
			: status_code()
			, msg()
			, protocol("HTTP")
			, version("1.1")
		{
		}

		details::string_t to_string(methods method, details::string_t resource) const
		{
			details::string_t first_row_to_string;

			first_row_to_string += method.name() + " " + resource + " " + protocol + "/" + version;
			first_row_to_string += "\r\n";

			return first_row_to_string;
		}

		void parse(details::string_t str_first_row)
		{
			assert(str_first_row.substr(0, 4) == "HTTP");

			protocol = str_first_row.substr(0, 4);
			version = str_first_row.substr(5, 3);
			std::size_t pos = str_first_row.find_first_of(' ');
			std::string code = str_first_row.substr(pos + 1, 3);
			status_code = std::stoi(code);
			msg = str_first_row.substr(pos + 5);
			msg.erase(msg.end() - 1);
		}

	public:
		unsigned status_code;
		details::string_t msg;
		details::string_t protocol;
		details::string_t version;
	};

	class body
	{
	public:
		body() : m_body() {}
		body(details::string_t data)
			: m_body(data)
		{
		}

		details::string_t to_string() const
		{
			return m_body;
		}

		void parse(details::string_t str_body)
		{
			m_body = str_body;
		}

	private:
		details::string_t m_body;
	};

	class headers
	{
	public:
		headers() : m_headers(json::value::object()) {}
		headers(std::vector<std::pair<details::string_t, json::value>> fields)
			: m_headers(json::value::object(fields))
		{
		}

		json::value& operator[](details::string_t key)
		{
			return m_headers[key];
		}

		json::value operator=(details::string_t value)
		{
			return json::value(value);
		}
	
		details::string_t to_string() const
		{
			return m_headers.to_string();
		}

		details::string_t to_headers() const
		{
			details::string_t header_to_string;
			for (auto iter : m_headers.as_object())
			{
				header_to_string += iter.first + ": " + iter.second.as_string() + "\r\n";
			}
			header_to_string += "\r\n";
			return header_to_string;
		}

		void parse(details::string_t str_header)
		{
			std::istringstream isstream(str_header.data());
			std::vector<details::string_t> lines;
			std::string line;
			while (std::getline(isstream, line, '\r'))
				lines.push_back(line);

			for (auto line : lines)
			{
				std::size_t pos = line.find(": ");
				if (pos != std::string::npos)
				{
					m_headers[line.substr(0, pos)] = json::value(line.substr(pos + 1));
				}
			}
		}

	private:
		json::value m_headers;
	};

	class Uri
	{
	public:
		Uri() : url(), scheme(), domain_name(), query(), path("/"), port_(80) {}
		Uri(details::string_t uri)
			: url(uri)
			, scheme()
			, domain_name()
			, query()
			, path("/")
			, port_(80)
		{
			parse(uri);
		}

		void parse(details::string_t uri)
		{
			assert(uri.empty() != true);

			std::size_t ques_mark_pos = uri.find('?');
			std::vector<details::string_t> dns_and_query = uri.split('?');

			details::string_t dns = dns_and_query[0];

			std::string scheme = "https";
			std::size_t ssl_mark_pos = dns.find(scheme);

			if (ssl_mark_pos == std::string::npos)
			{
				scheme.erase(scheme.end() - 1);
			}

			dns.replace(0, scheme.size() + 3, "");
			this->scheme = scheme;

			std::vector<details::string_t> split_path = dns.split('/');
			details::string_t dns_and_port = split_path[0];
			std::vector<details::string_t> dns_ = dns_and_port.split(':');
			domain_name = dns_[0];
			if (dns_.size() > 1)
				port_ = std::stoul(dns_[1]);
			else if (ssl_mark_pos != std::string::npos)
				port_ = 443;

			for (std::size_t i = 1; i < split_path.size(); i++)
			{
				path += split_path[i];
			}

			if (ques_mark_pos != std::string::npos)
			{
				details::string_t query = dns_and_query[1];
				this->query.parse(query);
			}
		}

		details::string_t dns() const
		{
			return domain_name;
		}

		std::size_t port() const
		{
			return port_;
		}

		details::string_t locator() const
		{
			return path;
		}

		parameters & query_params()
		{
			return query;
		}

		parameters query_params() const
		{
			return query;
		}

		details::string_t query_string() const
		{
			return query.to_string();
		}

		details::string_t resource() const
		{
			return path + query.to_string();
		}

	private:
		details::string_t url;
		details::string_t scheme;
		details::string_t domain_name;
		details::string_t path;

		parameters query;

		std::size_t port_;
	}; // class Uri

	class http_socket
	{
		SOCKET socket_fd;
#if defined(_WIN32) || defined(_WIN64)
		WORD wVersion;
		WSADATA WSAData;
		char ipaddr[16];

		char* gethostbyname(const char * domain_name)
		{
			struct addrinfo hints;
			struct addrinfo *res;

			memset(&hints, 0, sizeof(struct addrinfo));
			hints.ai_family = AF_INET;
			hints.ai_flags = AI_PASSIVE;
			hints.ai_protocol = 0;
			hints.ai_socktype = SOCK_STREAM;

			assert(getaddrinfo(domain_name, NULL, &hints, &res) != -1);

			for (struct addrinfo *cur = res; cur != NULL; cur = cur->ai_next)
			{
				struct sockaddr_in *addr = (struct sockaddr_in *)cur->ai_addr;
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
		http_socket()
			: ipaddr{ 0 }
		{
#if defined(_WIN32) || defined(_WIN64)
			wVersion = MAKEWORD(2, 2);
			WSAStartup(wVersion, &WSAData);
#endif
		}

		http_socket(details::string_t dns, std::size_t port)
			: ipaddr{ 0 }
		{
#if defined(_WIN32) || defined(_WIN64)
			wVersion = MAKEWORD(2, 2);
			WSAStartup(wVersion, &WSAData);
#endif
			create_connect(dns.c_str(), port);
		}

		void create_connect(const char * domain_name, unsigned short port)
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

			assert((socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) != INVALID_SOCKET);
			assert(connect(socket_fd, (struct sockaddr*)&serAddr, sizeof(struct sockaddr)) != SOCKET_ERROR);
		}

		int send_msg(std::string headers)
		{
			return send(socket_fd, headers.c_str(), headers.size(), 0);
		}

		std::string receive_msg()
		{
			const std::size_t nSize = 65536;
			std::unique_ptr<char*> recvBuf = std::make_unique<char*>(new char[nSize]);
			memset(*recvBuf, 0, nSize);
#if defined(_WIN32) || defined(_WIN64)
			assert(recv(socket_fd, *recvBuf, nSize, 0) > 0);
#else
			assert(read(socket_fd, *recvBuf, nSize) > 0);
#endif
			return std::string(*recvBuf);
		}
	}; // class http_socket

	class Response
	{
	public:
		Response(Uri uri, methods method, http::headers header)
			: response_first_row()
		{
			http_socket phc(uri.dns().c_str(), uri.port());
			phc.send_msg(request_first_row.to_string(method, uri.resource()) + header.to_headers());
			details::string_t response_result = phc.receive_msg();
			parse(response_result);
		}

		template<typename ...Args>
		Response(Uri uri, methods method, http::headers header, Args... args)
			: request_first_row()
		{
			http_socket phc(uri.dns().c_str(), uri.port());
			phc.send_msg(request_first_row.to_string(method, uri.resource()) + header.to_headers() + args.to_string());
			details::string_t response_result = phc.receive_msg();
			parse(response_result);
		}

		json::value json() const
		{
			return json::value::parse(response_body.to_string());
		}

		unsigned status_code() const
		{
			return response_first_row.status_code;
		}

		details::string_t message() const
		{
			return response_first_row.msg;
		}

		details::string_t headers() const
		{
			return response_header.to_string();
		}

		details::string_t html() const
		{
			return response_body.to_string();
		}

	private:
		void parse(details::string_t text)
		{
			std::istringstream isstream(text.data());
			std::string line;
			details::string_t str_first_row;
			details::string_t str_header;
			std::size_t lines = 0;
			while (std::getline(isstream, line, '\n'))
			{
				if (line.substr(0, 5) == "HTTP/")
					str_first_row = line;
				else if (line.find(':') != std::string::npos)
					str_header += line;
				else
					break;
				lines++;
			}

			text = text.replace(0, str_first_row.size() + str_header.size() + lines + 2, "");

			response_first_row.parse(str_first_row);
			response_header.parse(str_header);
			response_body.parse(text);
		}

	private:
		http::headers response_header;
		cookies response_cookie;
		body response_body;
		first_row response_first_row;

		first_row request_first_row;
	}; // class Response

	class request
	{
	public:
		request() {}

		static Response get(Uri uri, parameters params, http::headers header)
		{
			std::unique_ptr<methods> m_method = std::make_unique<Get>();
			uri.query_params().parse(params.to_string());
			return Response(uri, *m_method, header);
		}

		template<typename ... Args>
		static Response post(Uri uri, parameters params, http::headers header, Args ...data)
		{
			std::unique_ptr<methods> m_method = std::make_unique<Post>();
			uri.query_params().parse(params.to_string());
			return Response(uri, *m_method, header, data);
		}
	};

} // namespace http
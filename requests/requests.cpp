#include "pch.h"
#include <iostream>
#include <vector>
#include "C-libhttp/json.h"
#include "C-libhttp/requests.h"

using namespace http;
using namespace http::json;

int main()
{
	//request request;
	headers h;

	//h["Accept"] = value("text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
	h["Accept-Encoding"] = value("gzip, deflate");
	h["Accept-Language"] = value("zh-CN, zh;q=0.9");
	h["Cache-Control"] = value("no-cache");
	h["Connection"] = value("keep-alive");
	h["Host"] = value("latex.codecogs.com");
	//h["User-Agent"] = value("Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36(KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36");
	std::cout << h.to_string();
	//Response r = request.get(Uri{"http://latex.codecogs.com"}, parameters(), h);
	//std::cout << r.status_code() << std::endl;
	//std::cout << r.message() << std::endl;
	//std::cout << r.headers() << std::endl;
	//std::cout << r.html() << std::endl;

	return 0;
}

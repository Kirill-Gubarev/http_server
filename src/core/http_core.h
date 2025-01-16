#ifndef HTTP_CORE_H
#define HTTP_CORE_H

#include <string>
#include "net/session.h"

namespace core{
using std::string;
	void process_request(net::Session& session, string&& request);
}

#endif//HTTP_CORE_H

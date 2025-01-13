#ifndef HTTP_CORE_H
#define HTTP_CORE_H

#include <string>
#include "session.h"

namespace core{
using std::string;
	void process_request(net::Session& session, const string& request);
}

#endif//HTTP_CORE_H

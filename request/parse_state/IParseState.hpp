#ifndef IPARSESTATE_HPP
#define IPARSESTATE_HPP

#include <sstream>

class ParseRequest;

class IParseState
{
	public:
		virtual ~IParseState() = default;
		virtual IParseState* handle(ParseRequest& parser, std::stringstream& ss) = 0;
};

#endif

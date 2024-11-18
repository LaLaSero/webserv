#ifndef HEADERSTATE_HPP
#define HEADERSTATE_HPP

#include "IParseState.hpp"

class HeadersState : public IParseState
{
	public:
		IParseState* handle(ParseRequest& parser, std::stringstream& ss);
};

#endif

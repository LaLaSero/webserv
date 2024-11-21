#ifndef BODYSTATE_HPP
#define BODYSTATE_HPP

#include "IParseState.hpp"

class BodyState : public IParseState
{
	public:
		IParseState* handle(ParseRequest& parser, std::stringstream& ss);
};

#endif

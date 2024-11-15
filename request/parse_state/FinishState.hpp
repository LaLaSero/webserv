#ifndef FINISHSTATE_HPP
#define FINISHSTATE_HPP

#include "IParseState.hpp"

class FinishState : public IParseState
{
	public:
		IParseState* handle(ParseRequest& parser, std::stringstream& ss);
};

#endif

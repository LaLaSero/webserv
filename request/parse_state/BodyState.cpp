#include "BodyState.hpp"
#include "../ParseRequest.hpp"
#include "FinishState.hpp"

IParseState* BodyState::handle(ParseRequest& parser, std::stringstream& ss)
{
	if (!parser.readBody(ss))
		return this;
	return new FinishState();
}

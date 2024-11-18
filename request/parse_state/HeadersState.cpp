#include "HeadersState.hpp"
#include "../ParseRequest.hpp"
#include "BodyState.hpp"
#include "FinishState.hpp"

IParseState* HeadersState::handle(ParseRequest& parser, std::stringstream& ss)
{
	if (!parser.readHeaders(ss))
		return this;

	if (parser.hasBody())
		return new BodyState();
	else
		return new FinishState();
}

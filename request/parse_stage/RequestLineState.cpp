#include "RequestLineState.hpp"
#include "../ParseRequest.hpp"
#include "HeadersState.hpp"
#include "FinishState.hpp"

IParseState* RequestLineState::handle(ParseRequest& parser, std::stringstream& ss) {
	if (!parser.readRequestLine(ss))
		return this;
	if (!parser.getRequest().getMethod().empty())
		return new HeadersState();
	return NULL;
}

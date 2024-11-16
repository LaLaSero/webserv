#include "FinishState.hpp"
#include "../ParseRequest.hpp"

IParseState* FinishState::handle(ParseRequest& parser, std::stringstream& ss)
{
	parser.finalize();
	return NULL;
	(void)ss;
}

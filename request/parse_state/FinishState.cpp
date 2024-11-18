#include "FinishState.hpp"
#include "../ParseRequest.hpp"

IParseState* FinishState::handle(ParseRequest& parser, std::stringstream& ss)
{
	parser.finalize();
	parser.setFinished(true);
	return NULL;
	(void)ss;
}

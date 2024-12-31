#include "FinishState.hpp"
#include "../ParseRequest.hpp"

IParseState* FinishState::handle(ParseRequest& parser, std::stringstream& ss)
{
	parser.finalize();
	// if (parser.getBody().size() < parser.getContentLength())
	// {
	// 	parser.setFinished(false);
	// 	return NULL;
	// }
	// parser.setFinished(true);
	return NULL;
	(void)ss;
}

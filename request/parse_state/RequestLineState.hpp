#ifndef REQUESTLINESTATE_HPP
#define REQUESTLINESTATE_HPP

#include "IParseState.hpp"

class RequestLineState : public IParseState
{
	private:
		int errorno_;
	public:
		virtual ~RequestLineState() {};
		IParseState* handle(ParseRequest& parser, std::stringstream& ss) override;
};

#endif

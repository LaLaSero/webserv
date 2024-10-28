#ifndef REQUESTLINESTATE_HPP
#define REQUESTLINESTATE_HPP

#include "IParseState.hpp"

class RequestLineState : public IParseState
{
    public:
    IParseState* handle(ParseRequest& parser, std::stringstream& ss);
};

#endif

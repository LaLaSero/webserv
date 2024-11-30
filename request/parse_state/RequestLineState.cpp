#include "RequestLineState.hpp"
#include "../ParseRequest.hpp"
#include "HeadersState.hpp"
#include "FinishState.hpp"

IParseState* RequestLineState::handle(ParseRequest& parser, std::stringstream& ss)
{
	if (!parser.readRequestLine(ss))
		return (this);
	if (!parser.getRequest().getMethod().empty())
		return new HeadersState();
	return (NULL);
}

// IParseState* RequestLineState::handle(ParseRequest& parser, std::stringstream& ss) {
//     // 実際の処理を記述する
//     // この中で RequestLineState の状態を処理し、次の状態（IParseState*）を返す
//     return nullptr;  // 仮実装。実際には次の状態に遷移する処理が必要。
// }
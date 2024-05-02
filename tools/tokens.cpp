#include <iostream>
#include <string_view>
#include <string>
#include <vector>

#include "scripts/scripts_ext.h"
#include "scripts/scripts_tools.h"

using namespace std::literals;

std::string to_string(asETokenClass tokenClass)
{
    switch (tokenClass)
    {
        case asTC_UNKNOWN:
            return "unknown"s;
        case asTC_KEYWORD:
            return "keyword"s;
        case asTC_VALUE:
            return "value"s;
        case asTC_IDENTIFIER:
            return "identifier"s;
        case asTC_COMMENT:
            return "comment"s;
        case asTC_WHITESPACE:
            return "whitespace"s;
    }
    return "unknown"s;
}

struct Token
{
    asETokenClass tokenClass;
    std::string token;

    Token(asETokenClass clazz, const char* text, asUINT len)
        : tokenClass{clazz}
        , token{text, len}
    {}

    [[nodiscard]]
    std::string to_string() const
    {
        return token + "<<"s + ::to_string(tokenClass) + ">>"s;
    }

    using list = std::vector<Token>;
};

Token::list parse(asIScriptEngine* engine, const std::string& code)
{
    Token::list tokens;
    asUINT pos = 0, len = 0, size = code.size();
    auto it = code.data();
    while (pos < code.size())
    {
        asUINT rest = size - pos;
        auto type = engine->ParseToken(it, rest, &len);
        tokens.emplace_back(type, it, len);
        it += len;
        pos += len;
    }

    return tokens;
}

const auto script = R"(/**
multiline comment
*/
#pragma // unknown token
import print;
shared class Main // line comment
{
    void main(array<string> args)
    {
        print("hello");
    }
}
)"s;

int main(int argc, char** argv)
{
    auto engine = asCreateScriptEngine();

    const auto tokens = parse(engine, script);
    for (auto& token: tokens)
    {
        std::cout << token.to_string() << std::endl;
    }

    engine->ShutDownAndRelease();
    return EXIT_SUCCESS;
}

#ifndef SCRIPTS_TOOLS_H
#define SCRIPTS_TOOLS_H

#include <angelscript.h>
#include <stdexcept>

namespace scripts::tools
{

struct script_exception: std::runtime_error
{
    script_exception(int code, const char* message) noexcept
            : std::runtime_error(message)
            , code{code}
    {}

    [[nodiscard]]
    int get_code() const noexcept { return code; }

private:
    int code;
};

void check(int r, const char *message);

void not_null(const void* ptr, const char* message);


void on_engine_message(const asSMessageInfo* msg, void* param);

} // namespace scripts::tools

#endif //SCRIPTS_TOOLS_H

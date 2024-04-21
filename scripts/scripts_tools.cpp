#include "scripts_tools.h"
#include <iostream>

namespace scripts::tools
{

void check(int r, const char *message)
{
    if (r < 0)
    {
        throw script_exception{r, message};
    }
}

void not_null(const void *ptr, const char *message)
{
    if (nullptr == ptr)
    {
        throw std::runtime_error{message};
    }
}

void on_engine_message(const asSMessageInfo *msg, void *param)
{
    const char* prefix = "Unk : ";
    switch (msg->type) {
        case asMSGTYPE_ERROR:
            prefix = "Err : ";
            break;
        case asMSGTYPE_WARNING:
            prefix = "Warn: ";
            break;
        case asMSGTYPE_INFORMATION:
            prefix = "Info: ";
            break;
    }

    std::cout << prefix << msg->section << " (" << msg->row << ", " << msg->col << ") : " << msg->message << std::endl;
}

} // namespace scripts::tools

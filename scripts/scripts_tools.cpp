#include "scripts_tools.h"

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

} // namespace scripts::tools

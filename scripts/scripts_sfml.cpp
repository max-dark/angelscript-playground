#include "scripts_sfml.h"
#include "scripts_tools.h"
#include "sfml_time.h"
#include "sfml_clock.h"

namespace scripts::sfml
{
int Register(asIScriptEngine *engine)
{
    using scripts::tools::check;
    int r;
    r = engine->SetDefaultNamespace("sf");
    check(r, "unable select namespace 'sf'");

    system::RegisterTime(engine);
    system::RegisterClock(engine);

    r = engine->SetDefaultNamespace("");
    check(r, "unable select namespace ''");

    return asSUCCESS;
}

} // namespace scripts::sfml

#include <iostream>
#include <cstdint>
#include <atomic>

#include <stdexcept>

#include <angelscript.h>
#include "scripts/scripts_ext.h"

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

void check(int r, const char* message)
{
    if (r < 0)
    {
        throw script_exception{r, message};
    }
}

struct Ref {
    using counter_t = std::uint64_t;
    using ref_counter_t = std::atomic<counter_t>;

    void AddRef() { ++refCount; }

    void Release() {
        if (--refCount == 0) {
            delete this;
        }
    }

    [[nodiscard]]
    counter_t Count() const { return refCount; }

    static Ref* create()
    {
        return new Ref{};
    }

    static int Register(asIScriptEngine* engine)
    {
        try {
            int r;
            r = engine->RegisterObjectType("ref", 0, asOBJ_REF);
        }
        catch (script_exception &e) {
            return e.get_code();
        }
        return 0;
    }

    ~Ref() = default;

private:
    ref_counter_t refCount = 1;
};

int main(int argc, char **argv) {
    auto engine = asCreateScriptEngine();
    scripts::ext::registerExtensions(engine);
    Ref::Register(engine);
    engine->ShutDownAndRelease();
    return 0;
}
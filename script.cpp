#include <iostream>
#include <cstdint>
#include <atomic>

#include <stdexcept>
#include <string_view>

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

void not_null(const void* ptr, const char* message)
{
    if (nullptr == ptr)
    {
        throw std::runtime_error{message};
    }
}

struct Ref {
    using counter_t = std::uint64_t;
    using ref_counter_t = std::atomic<counter_t>;

    static inline constexpr auto Type = "ref";

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

    static int Register(asIScriptEngine* engine) noexcept
    {
        try {
            int r;
            // register ref-counted type
            r = engine->RegisterObjectType(Type, 0, asOBJ_REF);
            check(r, "type register fail");
            // add factory method for object creation
            r = engine->RegisterObjectBehaviour(Type, asBEHAVE_FACTORY, "ref@ f()", asFUNCTION(create), asCALL_CDECL);
            check(r, "factory register fail");
            // add reference increment
            r = engine->RegisterObjectBehaviour(Type, asBEHAVE_ADDREF, "void f()", asMETHOD(Ref, AddRef), asCALL_THISCALL);
            check(r, "AddRef");
            // add reference decrement
            r = engine->RegisterObjectBehaviour(Type, asBEHAVE_RELEASE, "void f()", asMETHOD(Ref, Release), asCALL_THISCALL);
            check(r, "Release");
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

namespace utility
{
    void print(std::string& msg)
    {
        std::cout << msg << std::endl;
    }
}
void RegisterUtility(asIScriptEngine* engine)
{
    using namespace utility;
    int r;
    r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
    check(r, "print");
}

constexpr std::string_view script_code = R"(
void main()
{
    print("Hello");
}
)";

int main(int argc, char **argv) {
    auto engine = asCreateScriptEngine();
    // store scripts in engine memory(?)
    engine->SetEngineProperty(asEP_COPY_SCRIPT_SECTIONS, true);
    // "std" addons
    scripts::ext::registerExtensions(engine);
    // simple class what can be used in scripts
    Ref::Register(engine);

    try
    {
        int r;
        RegisterUtility(engine);
        // program should create module
        auto program = engine->GetModule("program", asGM_ALWAYS_CREATE);
        not_null(program, "unable create module");

        // add source
        program->AddScriptSection("program.as", script_code.data(), script_code.size(), 0);
        // compile
        r = program->Build();
        check(r, "program->build() fail");

        // try find "main"
        auto func = program->GetFunctionByDecl("void main()");
        not_null(func, "no main function in program");

        // create execution context
        auto ctx = engine->CreateContext();
        not_null(ctx, "ctx == nullptr");

        ctx->Prepare(func); // TODO: check ret code
        int exec = ctx->Execute();

        if (exec != asEXECUTION_FINISHED) // ctx can be suspended - see coroutines / debug samples
        {
            if (exec == asEXECUTION_EXCEPTION)
            {
                std::cerr << "Exception: " << ctx->GetExceptionString() << std::endl;
            }
        }
        ctx->Release();
    }
    catch (const std::runtime_error& re)
    {
        std::cerr << re.what() << std::endl;
    }

    engine->ShutDownAndRelease();
    return 0;
}
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

struct Check
{
Check& operator[](const char *msg)
{
    message = msg;
    return *this;
}
Check& operator=(int r)
{
    if (r < 0)
    {
        throw script_exception{r, message};
    }
    return *this;
}
private:
    const char * message = nullptr;
};

void check(int r, const char *message)
{
    Check v;
    v[message] = r;
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

// garbage collector support
struct RefGC
{
    using counter_t = int;
    using ref_counter_t = std::atomic<counter_t>;
    using gc_flag_t = std::atomic_bool;

    static inline constexpr auto Type = "ref_gc";

    // register type
    static void Register(asIScriptEngine *engine)
    {
        RefGC::engine_ptr = engine;

        int r;
        // new gc type
        r = engine->RegisterObjectType(Type, 0, asOBJ_REF | asOBJ_GC);
        check(r, Type);

        // same as Ref
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_FACTORY, "ref_gc@ f()", asFUNCTION(RefGC::create), asCALL_CDECL);
        check(r, "factory");
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_ADDREF, "void f()", asMETHOD(RefGC, AddRef), asCALL_THISCALL);
        check(r, "add ref");
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_RELEASE, "void f()", asMETHOD(RefGC, Release), asCALL_THISCALL);
        check(r, "release");

        // gc support

        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_GETREFCOUNT, "int f()", asMETHOD(RefGC, GetRefCount), asCALL_THISCALL);
        check(r, "get ref");
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_SETGCFLAG, "void f()", asMETHOD(RefGC, SetGCFlag), asCALL_THISCALL);
        check(r, "set flag");
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_GETGCFLAG, "bool f()", asMETHOD(RefGC, GetGCFlag), asCALL_THISCALL);
        check(r, "get flag");
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_ENUMREFS, "void f(int&in)", asMETHOD(RefGC, EnumReferences), asCALL_THISCALL);
        check(r, "enum ref");
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_RELEASEREFS, "void f(int&in)", asMETHOD(RefGC, RemoveAllReferences), asCALL_THISCALL);
        check(r, "release all");

        // cache type info
        typeInfo = engine->GetTypeInfoByName(Type);
    }

    // fabric method
    static RefGC *create()
    {
        not_null(engine_ptr, "engine_ptr == nullptr");
        not_null(typeInfo, "typeInfo == nullptr");

        auto obj = new RefGC{};
        engine_ptr->NotifyGarbageCollectorOfNewObject(obj, typeInfo);
        return obj;
    }

    void AddRef()
    {
        gcFlag = false;
        ++refCount;
    }

    void Release()
    {
        gcFlag = false;
        if (--refCount == 0)
        {
            delete this;
        }
    }

    // mark as garbage
    void SetGCFlag()
    {
        gcFlag = true;
    }

    bool GetGCFlag()
    {
        return gcFlag;
    }

    counter_t GetRefCount()
    {
        return refCount;
    }

    void EnumReferences(asIScriptEngine* engine)
    {
        // see dictionary add-on
        // for each sub-object
        //    engine->GCEnumCallback(object)
    }

    void RemoveAllReferences(asIScriptEngine* engine)
    {
        // release all sub-objects
    }

private:
    static asIScriptEngine *engine_ptr;
    static asITypeInfo *typeInfo;
private:
    ref_counter_t refCount = 1;
    gc_flag_t gcFlag = false;
};

asIScriptEngine* RefGC::engine_ptr = nullptr;
asITypeInfo* RefGC::typeInfo = nullptr;

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
    ref r;
    ref_gc rgc;
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
        RefGC::Register(engine);
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

        const int prepare = ctx->Prepare(func);
        if (prepare == asSUCCESS)
        {
            const int exec = ctx->Execute();

            if (exec != asEXECUTION_FINISHED) // ctx can be suspended - see coroutines / debug samples
            {
                if (exec == asEXECUTION_EXCEPTION)
                {
                    std::cerr << "Exception: " << ctx->GetExceptionString() << std::endl;
                }
            }
        }
        else
        {
            std::cerr << "Prepare fail: " << prepare << std::endl;
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
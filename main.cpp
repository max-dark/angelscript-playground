#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>

#include "scripts/scripts_ext.h"

void on_engine_message(const asSMessageInfo* msg, void* param)
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

void on_line_callback(asIScriptContext* ctx, void* param)
{
    const char* section;
    auto line = ctx->GetLineNumber(0, nullptr, &section);
    auto func = ctx->GetFunction(0);
    auto st = ctx->GetCallstackSize();
    auto v = ctx->GetVarCount(0);
    auto sv = ctx->GetArgsOnStackCount(0);
    std::cout
        << " l:" << std::setw(3) << line
        << " s:" << std::setw(3) << st
        << " v:" << std::setw(3) << v
        << " c:" << std::setw(3) << sv
        << " f:" << func->GetDeclaration(true, true, true)
        << std::endl;
    for (int i = 0; i < v; ++i)
    {
        const char* name;
        auto var = ctx->GetVar(i, 0, &name);
        std::cout
            << std::setw(5) << i
            << std::setw(5) << var
            << " '" << name << "'" << std::endl;
    }
}

void print(const std::string& msg)
{
    std::cout << msg;
}

const char* script = R"(
class Class
{
void demo()
{
    print("Hello, " + name() + "\n");
}
string name()
{
    return "script";
}
};
void main()
{
    auto m = Class();
    m.demo();
}
)";

void ret_check(int r, const char* msg = "error")
{
    if (r < 0)
        throw std::runtime_error(msg);
}

int main() {
    auto engine = asCreateScriptEngine();
    try {
        int r;
        r = engine->SetMessageCallback(asFUNCTION(on_engine_message), nullptr, asCALL_CDECL);
        ret_check(r, "SetMsgCallback");
        scripts::ext::registerExtensions(engine);

        r = engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL);
        ret_check(r, "unable register print");

        CScriptBuilder builder;
        r = builder.StartNewModule(engine, "main");
        ret_check(r, "start module fail");
        r = builder.AddSectionFromMemory("main.as", script);
        ret_check(r, "add section fail");
        r = builder.BuildModule();
        ret_check(r, "build module fail");

        auto mod = engine->GetModule("main");
        auto func = mod->GetFunctionByDecl("void main()");
        if (func == nullptr)
            ret_check(-1, "no void main");

        auto ctx = engine->CreateContext();
        try
        {
            //r = ctx->SetLineCallback(asFUNCTION(on_line_callback), nullptr, asCALL_CDECL);
            //ret_check(r, "ctx set callback");
            r = ctx->Prepare(func);
            std::cout << "Prepare: " << r << std::endl;
            r = ctx->Execute();
            std::cout << "Exec: " << r << std::endl;
            if (r != asEXECUTION_FINISHED) {
                if (r == asEXECUTION_EXCEPTION) {
                    std::cerr << "Exception: " << ctx->GetExceptionString() << std::endl;
                }
            }
        }
        catch (std::runtime_error& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
        ctx->Release();
    }
    catch (std::runtime_error& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    engine->ShutDownAndRelease();
    return 0;
}

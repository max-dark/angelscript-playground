#include "sfml_string.h"
#include "scripts_sfml.h"
#include "scripts_tools.h"

#include <string>
#include <atomic>

namespace scripts::sfml::system
{
using scripts::tools::check;
using scripts::tools::not_null;

struct StringRef
{
    using counter_t = int;
    using ref_count_t = std::atomic<counter_t>;

    StringRef()
        : value{}{}
    explicit StringRef(const std::string& str)
        : value(sf::String::fromUtf8(str.begin(), str.end()))
        {}
    StringRef(const StringRef& str)
        : value(str.value)
    {}

    // factory methods
    static StringRef* create()
    {
        return new StringRef;
    }
    static StringRef* create_std(const std::string& str)
    {
        return new StringRef{str};
    }
    static StringRef* create_sf(const sf::String& str)
    {
        return new StringRef{str};
    }


    void AddRef()
    {
        ++refCount;
    }

    void Release()
    {
        if (--refCount == 0)
        {
            delete this;
        }
    }

    sf::String value;
    ref_count_t refCount = 1;
};

void RegisterString(asIScriptEngine* engine)
{
    const char * Type = "String";
    int r;

    r = engine->RegisterObjectType(Type, 0, asOBJ_REF);
    check(r, "sf::String");

    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_FACTORY, "String@ f()", asFUNCTION(StringRef::create), asCALL_CDECL);
    check(r, "String::create");
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_FACTORY, "String@ f(const string &in)", asFUNCTION(StringRef::create_std), asCALL_CDECL);
    check(r, "String::create_std");
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_FACTORY, "String@ f(const String &in)", asFUNCTION(StringRef::create_sf), asCALL_CDECL);
    check(r, "String::create_std");
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_ADDREF, "void f()", asMETHOD(StringRef, AddRef), asCALL_THISCALL);
    check(r, "String::AddRef");
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_RELEASE, "void f()", asMETHOD(StringRef, Release), asCALL_THISCALL);
    check(r, "String::Release");
}
} // namespace scripts::sfml::system

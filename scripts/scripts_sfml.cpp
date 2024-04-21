#include "scripts_sfml.h"
#include "scripts_tools.h"

namespace scripts::sfml
{
using namespace scripts::tools;
namespace system
{
void Time_ctor(sf::Time* memory)
{
    new(memory) sf::Time{};
}

void Time_copy(sf::Time* memory, const sf::Time& o)
{
    new(memory) sf::Time{o};
}

bool Time_eq(const sf::Time& left, const sf::Time& right)
{
    return left == right;
}

int Time_cmp(const sf::Time& a, const sf::Time& b)
{
    if (a == b) return 0;
    if (a < b) return 1;
    return 1;
}

sf::Time Time_neg(const sf::Time& a)
{
    auto b = -a;
    return b;
}

sf::Time Time_add(const sf::Time& a, const sf::Time& b)
{
    return a + b;
}

sf::Time& Time_addAssign(sf::Time& a, const sf::Time& b)
{
    a +=b;
    return a;
}

sf::Time Time_sub(const sf::Time& a, const sf::Time& b)
{
    return a - b;
}

sf::Time& Time_subAssign(sf::Time& a, const sf::Time& b)
{
    a -=b;
    return a;
}

void Time_dtor(sf::Time* value)
{
    value->~Time();
}

void Register(asIScriptEngine* engine)
{
    // sf::Time
    // register as "Value type"
    {
        const char* Type = "Time";
        int r;
        auto flags = asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<sf::Time>();
        r = engine->RegisterObjectType(Type, sizeof(sf::Time), flags);
        check(r, "class sf::Time");
        // default ctor
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Time_ctor), asCALL_CDECL_OBJLAST);
        check(r, "sf::Time::Time()");
        // copy ctor
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, "void f(const sf::Time& in)", asFUNCTION(Time_copy), asCALL_CDECL_OBJFIRST);
        check(r, "sf::Time::Time(sf::Time)");
        // destructor
        r = engine->RegisterObjectBehaviour(Type, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Time_dtor), asCALL_CDECL_OBJLAST);;
        check(r, "sf::Time::~Time()");

        // methods

        r = engine->RegisterObjectMethod(Type, "float asSeconds() const", asMETHODPR(sf::Time, asSeconds, () const, float), asCALL_THISCALL);
        check(r, "sf::Time::asSeconds()");
        r = engine->RegisterObjectMethod(Type, "int32 asMilliseconds() const", asMETHODPR(sf::Time, asMilliseconds, () const, sf::Int32), asCALL_THISCALL);
        check(r, "sf::Time::asMilliseconds()");
        r = engine->RegisterObjectMethod(Type, "int64 asMicroseconds() const", asMETHODPR(sf::Time, asMicroseconds, () const, sf::Int64), asCALL_THISCALL);
        check(r, "sf::Time::asMicroseconds()");

        // static Members
        r = engine->RegisterGlobalProperty("const sf::Time TimeZero", (void *) &sf::Time::Zero);
        check(r, "static const sf::Time TimeZero;");

        // global functions

        r = engine->RegisterGlobalFunction("sf::Time seconds(float)", asFUNCTIONPR(sf::seconds, (float), sf::Time), asCALL_CDECL);
        check(r, "sf::seconds");
        r = engine->RegisterGlobalFunction("sf::Time milliseconds(int32)", asFUNCTIONPR(sf::milliseconds, (sf::Int32), sf::Time), asCALL_CDECL);
        check(r, "sf::milliseconds");
        r = engine->RegisterGlobalFunction("sf::Time microseconds(int64)", asFUNCTIONPR(sf::microseconds, (sf::Int64), sf::Time), asCALL_CDECL);
        check(r, "sf::microseconds");

        // operators

        // == , !=
        r = engine->RegisterObjectMethod(Type, "bool opEquals(const sf::Time &in) const", asFUNCTIONPR(Time_eq, (const sf::Time&, const sf::Time&), bool), asCALL_CDECL_OBJFIRST);
        check(r, "sf::Time::opEquals");

        // compare- <, >, <=, >=
        r = engine->RegisterObjectMethod(Type, "int opCmp(const sf::Time &in) const", asFUNCTIONPR(Time_cmp, (const sf::Time&, const sf::Time&), int), asCALL_CDECL_OBJFIRST);
        check(r, "sf::Time::opCmp");

        r = engine->RegisterObjectMethod(Type, "sf::Time opNeg() const", asFUNCTION(Time_neg), asCALL_CDECL_OBJFIRST);
        check(r, "sf::Time::opNeg");

        // arithmetic operators
        r = engine->RegisterObjectMethod(Type, "sf::Time opAdd(const sf::Time &in) const", asFUNCTIONPR(Time_add, (const sf::Time&, const sf::Time&), sf::Time), asCALL_CDECL_OBJFIRST);
        check(r, "sf::Time::opAdd");
        r = engine->RegisterObjectMethod(Type, "sf::Time& opAddAssign(const sf::Time &in)", asFUNCTIONPR(Time_addAssign, (sf::Time&, const sf::Time&), sf::Time&), asCALL_CDECL_OBJFIRST);
        check(r, "sf::Time::opAddAssign");
        r = engine->RegisterObjectMethod(Type, "sf::Time opSub(const sf::Time &in) const", asFUNCTIONPR(Time_sub, (const sf::Time&, const sf::Time&), sf::Time), asCALL_CDECL_OBJFIRST);
        check(r, "sf::Time::opSub");
        r = engine->RegisterObjectMethod(Type, "sf::Time& opSubAssign(const sf::Time &in)", asFUNCTIONPR(Time_subAssign, (sf::Time&, const sf::Time&), sf::Time&), asCALL_CDECL_OBJFIRST);
        check(r, "sf::Time::opSubAssign");
    }
}
}

int Register(asIScriptEngine *engine)
{
    int r;
    r = engine->SetDefaultNamespace("sf");
    check(r, "unable select namespace 'sf'");

    system::Register(engine);

    r = engine->SetDefaultNamespace("");
    check(r, "unable select namespace ''");

    return asSUCCESS;
}

} // namespace scripts::sfml

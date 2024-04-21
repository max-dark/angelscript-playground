#include "sfml_vectors.h"
#include "scripts_sfml.h"
#include "scripts_tools.h"

#include <string>

#include <type_traits>

namespace scripts::sfml::system
{
using scripts::tools::check;
using scripts::tools::not_null;
using namespace std::string_literals;

template<typename T>
constexpr auto float_flag_v = std::is_same_v<T, float> ? asOBJ_APP_CLASS_ALLFLOATS : 0;
template<typename T>
constexpr auto int_flag_v = std::is_same_v<T, int> ? asOBJ_APP_CLASS_ALLINTS : 0;
template<typename T>
constexpr auto uint_flag_v = std::is_same_v<T, unsigned int> ? asOBJ_APP_CLASS_ALLINTS : 0;

template<typename T>
constexpr auto elem_flags_v = float_flag_v<T> | int_flag_v<T> | uint_flag_v<T>;


template<class V>
void Vector_ctor(V* memory)
{
    new(memory) V{};
}

template<typename T, class V = sf::Vector2<T>>
void Vector_ctor_2d(V* memory, T x, T y)
{
    new(memory) V{x, y};
}

template<typename T, class V = sf::Vector3<T>>
void Vector_ctor_3d(V* memory, T x, T y, T z)
{
    new(memory) V{x, y, z};
}

template<class V>
void Vector_dtor(V* memory)
{
    memory->~V();
}

// Vector common operators
template<typename Vector, typename T>
void RegisterVectorOps(const char* Type, const char* Elem, asIScriptEngine* engine)
{}

template<typename T>
void RegisterVector2d(const char* Type, const char* Elem, asIScriptEngine* engine)
{
    using Vector = sf::Vector2<T>;

    int r;
    asDWORD flags = asOBJ_VALUE | elem_flags_v<T> | asGetTypeTraits<Vector>();
    r = engine->RegisterObjectType(Type, sizeof(Vector), flags);
    check(r, Type);

    const auto decl = "void f("s + Elem + ", "s + Elem + ")"s; // "void f(T, T)"
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Vector_ctor<Vector>), asCALL_CDECL_OBJFIRST);
    check(r, "Vector::ctor");
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, decl.c_str(), asFUNCTION(Vector_ctor_2d<T>), asCALL_CDECL_OBJFIRST);
    check(r, "Vector::ctor(x, y)");
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Vector_dtor<Vector>), asCALL_CDECL_OBJFIRST);
    check(r, "Vector::dtor");

    RegisterVectorOps<Vector, T>(Type, Elem, engine);

    // fields

    r = engine->RegisterObjectProperty(Type, (Elem + " x"s).c_str(), asOFFSET(Vector, x));
    check(r, "Vector::x");
    r = engine->RegisterObjectProperty(Type, (Elem + " y"s).c_str(), asOFFSET(Vector, y));
    check(r, "Vector::y");
}


template<typename T>
void RegisterVector3d(const char* Type, const char* Elem, asIScriptEngine* engine)
{
    using Vector = sf::Vector3<T>;

    int r;
    asDWORD flags = asOBJ_VALUE | elem_flags_v<T> | asGetTypeTraits<Vector>();
    r = engine->RegisterObjectType(Type, sizeof(Vector), flags);
    check(r, Type);

    const auto decl = "void f("s + Elem + ", "s + Elem + ", "s + Elem + ")"s; // "void f(T, T, T)"
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(Vector_ctor<Vector>), asCALL_CDECL_OBJFIRST);
    check(r, "Vector::ctor");
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_CONSTRUCT, decl.c_str(), asFUNCTION(Vector_ctor_3d<T>), asCALL_CDECL_OBJFIRST);
    check(r, "Vector::ctor");
    r = engine->RegisterObjectBehaviour(Type, asBEHAVE_DESTRUCT, "void f()", asFUNCTION(Vector_dtor<Vector>), asCALL_CDECL_OBJFIRST);
    check(r, "Vector::dtor");

    RegisterVectorOps<Vector, T>(Type, Elem, engine);

    // fields

    r = engine->RegisterObjectProperty(Type, (Elem + " x"s).c_str(), asOFFSET(Vector, x));
    check(r, "Vector::x");
    r = engine->RegisterObjectProperty(Type, (Elem + " y"s).c_str(), asOFFSET(Vector, y));
    check(r, "Vector::y");
    r = engine->RegisterObjectProperty(Type, (Elem + " z"s).c_str(), asOFFSET(Vector, z));
    check(r, "Vector::z");
}

void RegisterVectors(asIScriptEngine* engine)
{
    RegisterVector2d<float>("Vector2f", "float", engine);
    RegisterVector2d<int>("Vector2i", "int", engine);
    RegisterVector2d<unsigned int>("Vector2u", "uint", engine);

    RegisterVector3d<float>("Vector3f", "float", engine);
    RegisterVector3d<int>("Vector3i", "int", engine);
}
} // namespace scripts::sfml::system

// run: script data/sfml_string.as
void main()
{
    sf::String a;
    sf::String b = "string";
    a = b;
    print(a.to_string());
    b += a;
    print(a.to_string());
    print(b.to_string());

    b = "hello";
    a = "world ";
    a += " hello";
    print(a.to_string());
    print(b.to_string());
}
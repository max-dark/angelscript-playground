// run: script data/sfml_string.as
void main()
{
    sf::String a;
    sf::String b = "string";
    a = b;
    print(a.to_string());
}
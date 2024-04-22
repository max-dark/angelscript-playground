
void print(const sf::Time &in value)
{
    print("Time(s/ms/us) == " + value.asSeconds() + " / " + value.asMilliseconds() + " / " + value.asMicroseconds());
}

void print(const sf::Clock &in clock)
{
    sf::Time value = clock.getElapsedTime();
    print("elapsed(s/ms/us) == " + value.asSeconds() + " / " + value.asMilliseconds() + " / " + value.asMicroseconds());
}

void main()
{
    sf::Clock clk;
    print(clk);
    ref r;
    ref_gc rgc;
    print("Hello");
    print(sf::TimeZero);

    sf::Time seconds_1 = sf::seconds(10.0);
    sf::Time seconds_2 = sf::seconds(20.0);
    print(seconds_1);
    print(sf::milliseconds(1000));
    print(sf::microseconds(100000));
    print(clk);
    clk.restart();

    print("a == b : " + (sf::TimeZero == seconds_1));
    print("a != b : " + (sf::TimeZero != seconds_1));
    print("c == b : " + (seconds_2 == seconds_1));

    print("a > b : " + (sf::TimeZero > seconds_1));
    print("a < b : " + (sf::TimeZero < seconds_1));

    print("c > b : " + (seconds_2 > seconds_1));
    print("c < b : " + (seconds_2 < seconds_1));

    print(seconds_1);
    print(seconds_2);
    print(sf::seconds(-10.0));
    sf::Time neg = -seconds_1;
    print(neg);
    print(seconds_1 + seconds_2);
    seconds_1 += sf::seconds(5.0);
    print(seconds_1);

    print(seconds_1 - seconds_2);
    seconds_1 -= sf::seconds(5.0);
    print(seconds_1);
    print(clk);
    print("Vectors:");

    sf::Vector2i v2i(1, 2);
    sf::Vector3i v3i(1, 2, 3);

    print("2d: " + v2i.x + ", " + v2i.y);
    print("3d: " + v3i.x + ", " + v3i.y + ", " + v3i.z);
}

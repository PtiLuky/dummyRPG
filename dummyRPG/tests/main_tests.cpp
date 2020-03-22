#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this in one cpp file

#include "catch2/catch.hpp"

#include "dummyrpg/layer.hpp"

struct tData
{
    uint16_t w;
    uint16_t h;
    uint8_t v;
};

TEST_CASE("Construct Layers", "[layer]")
{
    SECTION("Default constructor")
    {
        Dummy::Layer<uint8_t> l;
        REQUIRE(l.size() == 0);
        REQUIRE(l.width() == 0);
        REQUIRE(l.height() == 0);
    }

    SECTION("Null sized layers")
    {
        auto data = GENERATE(values<tData>({
            {0, 0, 42},
            {10, 0, 42},
            {0, 10, 42},
        }));
        Dummy::Layer<uint8_t> l(data.w, data.h, data.v);
        REQUIRE(l.width() == 0);
        REQUIRE(l.height() == 0);
        REQUIRE(l.size() == 0);
    }

    SECTION("Normal layer")
    {
        Dummy::Layer<uint8_t> l(10, 10, 42);
        REQUIRE(l.width() == 10);
        REQUIRE(l.height() == 10);
        REQUIRE(l.size() == 100);
        REQUIRE(l.at({0, 0}) == 42);
        REQUIRE(l.at({0, 9}) == 42);
        REQUIRE(l.at({9, 0}) == 42);
        REQUIRE(l.at({9, 9}) == 42);
    }

    SECTION("Max size")
    {
        Dummy::Layer<uint8_t> l(1024, 1024, 42);
        REQUIRE(l.width() == 1024);
        REQUIRE(l.height() == 1024);
        REQUIRE(l.size() == 1024 * 1024);
    }

    SECTION("Overflow")
    {
        Dummy::Layer<uint8_t> l(1000, 2500, 42);
        REQUIRE(l.width() == 1000);
        REQUIRE(l.height() == 1024);
        REQUIRE(l.size() == 1'024'000);
    }
}

TEST_CASE("Layers set", "[layer")
{
    Dummy::Layer<uint8_t> l(10, 10, 42);

    SECTION("Check validity")
    {
        REQUIRE(l.areCoordValid({0, 0}));
        REQUIRE(l.areCoordValid({5, 5}));
        REQUIRE(l.areCoordValid({9, 9}));
        REQUIRE_FALSE(l.areCoordValid({0, 10}));
        REQUIRE_FALSE(l.areCoordValid({10, 0}));
        REQUIRE_FALSE(l.areCoordValid({10, 10}));
        REQUIRE_FALSE(l.areCoordValid({static_cast<uint16_t>(-1), 0}));
    }

    SECTION("Set")
    {
        l.set({5, 5}, 0);

        bool areSame = true;
        for (uint16_t x = 0; x < 10; ++x)
            for (uint16_t y = 0; y < 10; ++y)
                if (x == 5 && y == 5) {
                    if (l.at({x, y}) != 0)
                        areSame = false;
                } else {
                    if (l.at({x, y}) != 42)
                        areSame = false;
                }

        REQUIRE(areSame);
    }
}

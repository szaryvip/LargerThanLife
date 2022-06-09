#include <catch2/catch_all.hpp>
#include "random_rules.hpp"


TEST_CASE("Range greater than 0")
{
    REQUIRE(generate_range() > 0);
}

TEST_CASE("Range lower or equal 10")
{
    REQUIRE(generate_range() <= 10);
}

TEST_CASE("Neighbourhood", "[!shouldfail]")
{
    char result = generate_neighbourhood();
    REQUIRE(result == 'm');
    REQUIRE(result == 'n');
}

TEST_CASE("Middle included", "[!shouldfail]")
{
    bool result = generate_middle_included();
    REQUIRE(result);
    REQUIRE(!result);
}

TEST_CASE("Number of states greater or equal 2")
{
    REQUIRE(generate_number_of_states() >= 2);
}

TEST_CASE("Number of states lower or equal 255")
{
    REQUIRE(generate_number_of_states() <= 255);
}

TEST_CASE("Birth or survive cond len greater than 0")
{
    REQUIRE(generate_birth_survive_cond().size() > 0);
}

TEST_CASE("Birth or survive cond len lower or equal 10")
{
    REQUIRE(generate_birth_survive_cond().size() <= 10);
}

TEST_CASE("Birth or survive cond greater or equal 0")
{
    std::vector<int> conds = generate_birth_survive_cond();
    for(auto& i : conds){
        REQUIRE(i >= 0);
    }
}

TEST_CASE("Birth or survive cond lower than 10")
{
    std::vector<int> conds = generate_birth_survive_cond();
    for(auto& i : conds){
        REQUIRE(i < 10);
    }
}

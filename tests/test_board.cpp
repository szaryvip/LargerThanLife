#include <catch2/catch_all.hpp>
#include "../src/board.hpp"


/* ---------  INVALID ARGUMENTS  --------- */


TEST_CASE("Create Board with negative neighborhoodRadius")
{
    BoardArgs args;
    args.neighborhoodRadius = -1;
    args.surviveConds.insert(2);
    args.birthConds.insert(2);
    REQUIRE_THROWS_AS(Board(args), std::invalid_argument);
}

TEST_CASE("Create Board with neighborhoodRadius out of bounds")
{
    BoardArgs args;
    args.neighborhoodRadius = NEIGHBORHOOD_RADIUS_MAX+1;
    args.surviveConds.insert(2);
    args.birthConds.insert(2);
    REQUIRE_THROWS_AS(Board(args), std::invalid_argument);
}

TEST_CASE("Create Board with negative states")
{
    BoardArgs args;
    args.states = -1;
    args.surviveConds.insert(2);
    args.birthConds.insert(2);
    REQUIRE_THROWS_AS(Board(args), std::invalid_argument);
}

TEST_CASE("Create Board with states out of bounds")
{
    BoardArgs args;
    args.states = STATES_MAX+1;
    args.surviveConds.insert(2);
    args.birthConds.insert(2);
    REQUIRE_THROWS_AS(Board(args), std::invalid_argument);
}

TEST_CASE("Create Board with empty birthConds")
{
    BoardArgs args;
    args.neighborhoodRadius = -1;
    args.surviveConds.insert(2);
    REQUIRE_THROWS_AS(Board(args), std::invalid_argument);
}

TEST_CASE("Create Board with empty surviveConds")
{
    BoardArgs args;
    args.neighborhoodRadius = NEIGHBORHOOD_RADIUS_MAX+1;
    args.birthConds.insert(2);
    REQUIRE_THROWS_AS(Board(args), std::invalid_argument);
}

TEST_CASE("Create Board with incorrect cell values")
{
    BoardArgs args;
    args.birthConds.insert(2);
    args.surviveConds.insert(1);

    cells_t cells;
    for (size_t row = 0; row < cells.size(); ++row) {
        for (size_t col = 0; col < cells[0].size(); ++col) {
            cells[row][col] = 3;
        }
    }
    REQUIRE_THROWS_AS(Board(args, cells), std::invalid_argument);
}

/* ---------  INITIALIZATION  --------- */

TEST_CASE("Constructor taking BoardArgs OK")
{
    BoardArgs args;
    args.birthConds.insert(2);
    args.surviveConds.insert(1);
    REQUIRE_NOTHROW(Board(args));
}

TEST_CASE("Constructor taking BoardArgs and cells_t OK")
{
    BoardArgs args;
    args.birthConds.insert(2);
    args.surviveConds.insert(1);
    cells_t cells;
    for (size_t row = 0; row < cells.size(); ++row) {
        for (size_t col = 0; col < cells[0].size(); ++col) {
            cells[row][col] = (int)col % 2;
        }
    }
    REQUIRE_NOTHROW(Board(args));
}

TEST_CASE("getSize returns correct size")
{
    BoardArgs args;
    args.birthConds.insert(2);
    args.surviveConds.insert(3);
    auto board = Board(args);
    REQUIRE(board.getSize() == BOARD_SIZE);
    REQUIRE(board.getSize() == board.getCells().size());
    REQUIRE(board.getSize() == board.getCells()[0].size());
}

TEST_CASE("Correct amount of alive cells initialized")
{
    BoardArgs args;
    args.birthConds.insert(2);
    args.surviveConds.insert(3);
    auto board = Board(args);

    int aliveCells = 0;
    for (auto row : board.getCells()) {
        for (auto cell : row) {
            REQUIRE((cell == 0 || cell == 1));
            if (cell != 0)
                ++aliveCells;
        }
    }

    REQUIRE(aliveCells == START_CELLS_ALIVE);
}


/* ---------  BOARD UPDATES  --------- */

typedef std::tuple<size_t, size_t> coords_t;
typedef std::set<coords_t> coords_vect_t;

int calculateNeighbors(const cells_t &cells, const coords_t &coords) {
    auto row = std::get<0>(coords);
    auto col = std::get<1>(coords);

    int neighbors = 0;

    if (row > 0 && cells[row-1][col] == 1)
        ++neighbors;

    if (col > 0 && cells[row][col-1] == 1)
        ++neighbors;

    if (row < cells.size()-1 && cells[row+1][col] == 1)
        ++neighbors;

    if (col < cells.size()-1 && cells[row][col+1] == 1)
        ++neighbors;

    return neighbors;
}

TEST_CASE("Cells are born after update() is called - birthConds {1}")
{
    BoardArgs args;
    args.birthConds.insert(1);
    args.neighborhoodRadius = 1;
    args.states = 2;
    args.isIncludeCenter = false;
    args.isMooreType = false; // von Neumann type
    args.surviveConds.insert(3); // doesn't matter here
    auto board = Board(args);

    coords_vect_t shouldBeAlive;

    auto cells = board.getCells();

    // find the cells which should be alive after update
    for (size_t row = 0; row < board.getSize(); ++row) {
        for (size_t col = 0; col < board.getSize(); ++col) {
            if (cells[row][col] == 1)
                continue; // not a dead cell

            auto coords = std::make_tuple(row, col);
            if (calculateNeighbors(cells, coords) == 1) // should be exactly one neighbor
                shouldBeAlive.insert(coords);
        }
    }

    // Now call update
    board.update();

    cells = board.getCells();

    // Check if the designated cells are actually alive
    for (auto coords : shouldBeAlive) {
        auto row = std::get<0>(coords);
        auto col = std::get<1>(coords);
        REQUIRE(cells[row][col] == 1);
    }
}

TEST_CASE("Cells are born after update() is called - birthConds {2}")
{
    BoardArgs args;
    args.birthConds.insert(2);
    args.neighborhoodRadius = 1;
    args.states = 2;
    args.isIncludeCenter = false;
    args.isMooreType = false; // von Neumann type
    args.surviveConds.insert(3); // doesn't matter here
    auto board = Board(args);

    coords_vect_t shouldBeAlive;

    auto cells = board.getCells();

    // find the cells which should be alive after update
    for (size_t row = 0; row < board.getSize(); ++row) {
        for (size_t col = 0; col < board.getSize(); ++col) {
            if (cells[row][col] == 1)
                continue; // not a dead cell

            auto coords = std::make_tuple(row, col);
            if (calculateNeighbors(cells, coords) == 2) // should be exactly two neighbors
                shouldBeAlive.insert(coords);
        }
    }

    // Now call update
    board.update();

    cells = board.getCells();

    // Check if the designated cells are actually alive
    for (auto coords : shouldBeAlive) {
        auto row = std::get<0>(coords);
        auto col = std::get<1>(coords);
        REQUIRE(cells[row][col] == 1);
    }
}

TEST_CASE("Cells are born after update() is called - birthConds {1, 2, 3}")
{
    BoardArgs args;
    args.birthConds.insert(1);
    args.birthConds.insert(2);
    args.birthConds.insert(3);
    args.neighborhoodRadius = 1;
    args.states = 2;
    args.isIncludeCenter = false;
    args.isMooreType = false; // von Neumann type
    args.surviveConds.insert(3); // doesn't matter here
    auto board = Board(args);

    coords_vect_t shouldBeAlive;

    auto cells = board.getCells();

    // find the cells which should be alive after update
    for (size_t row = 0; row < board.getSize(); ++row) {
        for (size_t col = 0; col < board.getSize(); ++col) {
            if (cells[row][col] == 1)
                continue; // not a dead cell

            auto coords = std::make_tuple(row, col);
            int neighbors = calculateNeighbors(cells, coords);
            if (neighbors > 0 && neighbors < 4) // 1-3 neighbors OK
                shouldBeAlive.insert(coords);
        }
    }

    // Now call update
    board.update();

    cells = board.getCells();

    // Check if the designated cells are actually alive
    for (auto coords : shouldBeAlive) {
        auto row = std::get<0>(coords);
        auto col = std::get<1>(coords);
        REQUIRE(cells[row][col] == 1);
    }
}

TEST_CASE("Cells are born after update() is called - birthConds {2, 4}")
{
    BoardArgs args;
    args.birthConds.insert(2);
    args.birthConds.insert(4);
    args.neighborhoodRadius = 1;
    args.states = 2;
    args.isIncludeCenter = false;
    args.isMooreType = false; // von Neumann type
    args.surviveConds.insert(1); // doesn't matter here
    auto board = Board(args);

    for (int i = 0; i < 5; ++i)
        board.update(); // update the board a couple of times

    coords_vect_t shouldBeAlive;

    auto cells = board.getCells();

    // find the cells which should be alive after update
    for (size_t row = 0; row < board.getSize(); ++row) {
        for (size_t col = 0; col < board.getSize(); ++col) {
            if (cells[row][col] == 1)
                continue; // not a dead cell

            auto coords = std::make_tuple(row, col);
            int neighbors = calculateNeighbors(cells, coords);
            if (neighbors == 2 || neighbors == 4) // 2 or 4 neighbors OK
                shouldBeAlive.insert(coords);
        }
    }

    // Now call update
    board.update();

    cells = board.getCells();

    // Check if the designated cells are actually alive
    for (auto coords : shouldBeAlive) {
        auto row = std::get<0>(coords);
        auto col = std::get<1>(coords);
        REQUIRE(cells[row][col] == 1);
    }
}

TEST_CASE("Cells are dead or survive after update() is called - surviveConds {1, 2}")
{
    BoardArgs args;
    args.birthConds.insert(2); // doesn't matter here
    args.neighborhoodRadius = 1;
    args.states = 2;
    args.isIncludeCenter = false;
    args.isMooreType = false; // von Neumann type
    args.surviveConds.insert(1);
    args.surviveConds.insert(2);
    auto board = Board(args);

    coords_vect_t shouldBeAlive, shouldBeDead;

    auto cells = board.getCells();

    // find the cells which should be alive after update
    for (size_t row = 0; row < board.getSize(); ++row) {
        for (size_t col = 0; col < board.getSize(); ++col) {
            if (cells[row][col] != 1)
                continue; // not an alive cell

            auto coords = std::make_tuple(row, col);
            int neighbors = calculateNeighbors(cells, coords);
            if (neighbors == 1 || neighbors == 2) // should be exactly two neighbors
                shouldBeAlive.insert(coords);
            else
                shouldBeDead.insert(coords);
        }
    }

    // Now call update
    board.update();

    cells = board.getCells();

    // Check if the designated cells are actually alive
    for (auto coords : shouldBeAlive) {
        auto row = std::get<0>(coords);
        auto col = std::get<1>(coords);
        REQUIRE(cells[row][col] == 1);
    }
}

TEST_CASE("Cells are dead or survive after update() is called - surviveConds {1, 2, 4}")
{
    BoardArgs args;
    args.birthConds.insert(2); // doesn't matter here
    args.neighborhoodRadius = 1;
    args.states = 2;
    args.isIncludeCenter = false;
    args.isMooreType = false; // von Neumann type
    args.surviveConds.insert(1);
    args.surviveConds.insert(2);
    args.surviveConds.insert(4);
    auto board = Board(args);

    for (int i = 0; i < 5; ++i)
        board.update(); // update the board a couple of times

    coords_vect_t shouldBeAlive, shouldBeDead;

    auto cells = board.getCells();

    // find the cells which should be alive after update
    for (size_t row = 0; row < board.getSize(); ++row) {
        for (size_t col = 0; col < board.getSize(); ++col) {
            if (cells[row][col] != 1)
                continue; // not an alive cell

            auto coords = std::make_tuple(row, col);
            int neighbors = calculateNeighbors(cells, coords);
            if (neighbors == 1 || neighbors == 2 || neighbors == 4) // should be exactly two neighbors
                shouldBeAlive.insert(coords);
            else
                shouldBeDead.insert(coords);
        }
    }

    // Now call update
    board.update();

    cells = board.getCells();

    // Check if the designated cells are actually alive
    for (auto coords : shouldBeAlive) {
        auto row = std::get<0>(coords);
        auto col = std::get<1>(coords);
        REQUIRE(cells[row][col] == 1);
    }
}

TEST_CASE("Cells state increases after update() is called - 5 states")
{
    BoardArgs args;
    args.birthConds.insert(2); // doesn't matter here
    args.neighborhoodRadius = 1;
    args.states = 5;
    args.isIncludeCenter = false;
    args.isMooreType = false; // von Neumann type
    args.surviveConds.insert(2);
    auto board = Board(args);

    coords_vect_t shouldDecrease;

    auto cells = board.getCells();

    // find the cells which should be alive after update
    for (size_t row = 0; row < board.getSize(); ++row) {
        for (size_t col = 0; col < board.getSize(); ++col) {
            if (cells[row][col] != 1)
                continue; // not an alive cell

            auto coords = std::make_tuple(row, col);
            int neighbors = calculateNeighbors(cells, coords);
            if (neighbors != 2) // should be exactly two neighbors
                shouldDecrease.insert(coords);
        }
    }

    // Update through states and see if the values increase
    for (int state = 2; state <= args.states; ++state) {
        // Now call update
        board.update();

        cells = board.getCells();

        // Check if the designated cells are actually alive
        for (auto coords: shouldDecrease) {
            auto row = std::get<0>(coords);
            auto col = std::get<1>(coords);
            if (state != args.states)   // should increase
                REQUIRE(cells[row][col] == state);
            else                        // now should be dead
                REQUIRE(cells[row][col] == 0);
        }
    }
}

TEST_CASE("Cells state increases after update() is called - 256 states")
{
    BoardArgs args;
    args.birthConds.insert(2); // doesn't matter here
    args.neighborhoodRadius = 1;
    args.states = 256;
    args.isIncludeCenter = false;
    args.isMooreType = false; // von Neumann type
    args.surviveConds.insert(2);
    auto board = Board(args);

    coords_vect_t shouldDecrease;

    auto cells = board.getCells();

    // find the cells which should be alive after update
    for (size_t row = 0; row < board.getSize(); ++row) {
        for (size_t col = 0; col < board.getSize(); ++col) {
            if (cells[row][col] != 1)
                continue; // not an alive cell

            auto coords = std::make_tuple(row, col);
            int neighbors = calculateNeighbors(cells, coords);
            if (neighbors != 2) // should be exactly two neighbors
                shouldDecrease.insert(coords);
        }
    }

    // Update through states and see if the values increase
    for (int state = 2; state <= args.states; ++state) {
        // Now call update
        board.update();

        cells = board.getCells();

        // Check if the designated cells are actually alive
        for (auto coords: shouldDecrease) {
            auto row = std::get<0>(coords);
            auto col = std::get<1>(coords);
            if (state != args.states)   // should increase
                REQUIRE(cells[row][col] == state);
            else                        // now should be dead
                REQUIRE(cells[row][col] == 0);
        }
    }
}
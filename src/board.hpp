#pragma once
#include <set>
#include <array>
#include <vector>
#include <cstddef>

const int NEIGHBORHOOD_RADIUS_MIN = 1;
const int NEIGHBORHOOD_RADIUS_MAX = 10;

const int STATES_MIN = 2;
const int STATES_MAX = 256;

const int START_CELLS_ALIVE = 150;
const size_t BOARD_SIZE = 60;

typedef std::set<int> conds_t;

typedef int cell_t;
typedef std::array<cell_t, BOARD_SIZE> row_t;
typedef std::array<row_t, BOARD_SIZE> cells_t;

/* Helper structure for passing game rules
 * as arguments to the Board class constructor */
struct BoardArgs {
    int neighborhoodRadius = 1; // Rr
    int states = 2; // Cc
    conds_t surviveConds = conds_t(); // Ss
    conds_t birthConds = conds_t(); // Bb
    bool isIncludeCenter = false; // Mm
    bool isMooreType = true; // Nn
};


/* Class representing a board with cells. Implements
 * core functionality of the game. */
class Board {

    const BoardArgs args;   // arguments passed from the user
    cells_t cells{};        // all cells in a board
    cells_t snapshot{};     // snapshot of the board state

public:

    /* Creates Board  with random alive cells at start,
     * game rules on each update are defined by the
     * boardArgs argument. */
    explicit Board(BoardArgs boardArgs);


    /* Creates Board with predefined start cell state, game
     * rules on each update are defined by the boardArgs
     * argument. */
    Board(BoardArgs boardArgs, const cells_t &startState);


    /* Handles regular updates of the cell states,
     * in accordance with game conditions. */
    void update();

    /* Returns const-reference to a 2-dimensional
     * container with all cell values. */
    const cells_t &getCells() const;

    /* Returns the size of the board. */
    size_t getSize() const;

private:

    /* Checks if arguments saved in 'args' variable are
     * logically correct. */
    void checkArgsCorrect() const;

    /* Tests birth conditions for a dead cell (returns true if
     * the conditions are met and false otherwise). For a fully
     * or partially alive cell, false is returned. */
    bool testBirthConditions(size_t row, size_t col) const;

    /* Tests survival conditions for a fully alive cell (returns true if
     * the conditions are met and false otherwise). For a dead
     * or partially alive cell, false is returned. */
    bool testSurvivalConditions(size_t row, size_t col) const;

    /* Tests any conditions for neighbor count. */
    bool testConditions(size_t row, size_t col, const conds_t &conds) const;

    /* Returns count of neighbors (state 1 cells)
     * in a row relative to the center point. */
    int getNeighborsInRow(size_t centerRow, size_t centerCol, int offset) const;

    /* Fills the container passed as an argument
     * with random cells designated to become alive.
     * The count of cells is defined by an appropriate
     * argument from 'args' variable. */
    void getRandomStartCells(std::set<cell_t> &cellSet) const;
};


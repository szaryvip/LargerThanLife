#include "board.hpp"
#include <stdexcept>
#include <random>
#include <utility>

Board::Board(BoardArgs boardArgs) : args(std::move(boardArgs)) {
    checkArgsCorrect();

    std::set<cell_t> aliveCells;
    getRandomStartCells(aliveCells);

    for (size_t row = 0, cellId = 0; row < cells.size(); ++row) {
        cells[row] = row_t();
        for (size_t col = 0; col < cells[0].size(); ++col, ++cellId) {
            if (aliveCells.find(cellId) != aliveCells.cend())
                cells[row][col] = 1; // make cell fully alive
            else
                cells[row][col] = 0; // make cell dead
        }
    }
}

Board::Board(BoardArgs boardArgs, const cells_t &startState)
        : args(std::move(boardArgs)), cells(startState) {
    checkArgsCorrect();
}

void Board::update() {
    snapshot = cells; // save a snapshot of the current cell states
    for (size_t row = 0; row < cells.size(); ++row) {
        for (size_t col = 0; col < cells[0].size(); ++col) {
            cell_t state = cells[row][col];
            if (state == 0) {
                // cell is dead
                if (testBirthConditions(row, col))
                    cells[row][col] = 1;
            }
            else if (state > 1 || !testSurvivalConditions(row, col)) {
                // cell is alive, but aging
                if (++cells[row][col] == args.states) {
                    // cell has lived a full lifetime
                    cells[row][col] = 0;
                }
            }
        }
    }
}

const cells_t & Board::getCells() const {
    return cells;
}

size_t Board::getSize() const {
    return cells.size();
}

void Board::checkArgsCorrect() const {
    if (args.neighborhoodRadius > NEIGHBORHOOD_RADIUS_MAX
    || args.neighborhoodRadius < NEIGHBORHOOD_RADIUS_MIN)
        throw std::invalid_argument("Neighborhood radius out of range");

    if (args.states > STATES_MAX || args.states < STATES_MIN)
        throw std::invalid_argument("States count out of range");

    if (args.birthConds.empty())
        throw std::invalid_argument("No birth conditions specified");

    if (args.surviveConds.empty())
        throw std::invalid_argument("No survival conditions specified");

    for (auto row : cells) {
        for (auto cell : row) {
            if (cell != 0 && cell != 1)
                throw std::invalid_argument("Start cell values in the array can only be 0 or 1");
        }
    }
}

void Board::getRandomStartCells(std::set<cell_t> &cellSet) const {
    std::random_device dev;
    std::mt19937 generator(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, (cells.size()*cells.size())-1);

    while (cellSet.size() != START_CELLS_ALIVE) {
        cellSet.insert(dist(generator));
    }
}

/* ---------- Helpers for calculating offset ---------- */

inline bool isCorrectOffset(size_t coord, int offset) {
    return (offset > -1 || coord >= (size_t) (-offset));
}

inline size_t addOffset(size_t coord, int offset) {
    if (offset < 0) {
        if (coord < (size_t) (-offset))
            throw std::invalid_argument("Negative offset greater than coordinate");

        return coord - (size_t) (-offset);
    }
    return coord + (size_t) offset;
}

/* --------------------------------------------------- */

bool Board::testBirthConditions(size_t row, size_t col) const {
    if (snapshot[row][col] != 0) return false; // cell not dead
    return testConditions(row, col, args.birthConds);
}

bool Board::testSurvivalConditions(size_t row, size_t col) const {
    if (snapshot[row][col] != 1) return false; // cell not fully alive
    return testConditions(row, col, args.surviveConds);
}

bool Board::testConditions(size_t row, size_t col, const conds_t &conds) const {
    int maxNeighbors = *conds.crbegin();

    int neighbors = 0;
    for (int offset = -args.neighborhoodRadius; offset <= args.neighborhoodRadius; ++offset) {
        if (isCorrectOffset(row, offset)) {
            // Add neighbors in a row
            neighbors += getNeighborsInRow(row, col, offset);

            // Is the count already too large?
            if (neighbors > maxNeighbors) return false;
        }
    }

    return (conds.find(neighbors) != conds.cend());
}

int Board::getNeighborsInRow(size_t centerRow, size_t centerCol, int offset) const {
    auto row = addOffset(centerRow, offset);
    if (row >= snapshot.size())
        return 0; // no neighbors in a non-existent row

    const int start = (args.isMooreType ?
                       -args.neighborhoodRadius :
                       -args.neighborhoodRadius + std::abs(offset));
    const int end = -start;

    int count = 0;

    for (int i = start; i <= end; ++i) {
        if ((i > -1 || centerCol >= (size_t) (-i))) {
            auto col = addOffset(centerCol, i);
            if (col < snapshot.size()
                && snapshot[row][col] == 1) {
                    ++count; // neighbor cell found
            }
        }

    }

    // Subtract center point, if it was unnecessarily counted
    if (offset == 0 && !args.isIncludeCenter && snapshot[centerRow][centerCol] == 1)
        --count;

    return count;
}
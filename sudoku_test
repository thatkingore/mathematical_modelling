#include "catch.hpp"
#include <vector>

using Part = std::vector<int>;
using Grid = std::vector<Part>;

static const auto Test = Grid{
	Part{ 11, 12, 13, 14, 15, 16, 17, 18, 19 },
	Part{ 21, 22, 23, 24, 25, 26, 27, 28, 29 },
	Part{ 31, 32, 33, 34, 35, 36, 37, 38, 39 },
	Part{ 41, 42, 43, 44, 45, 46, 47, 48, 49 },
	Part{ 51, 52, 53, 54, 55, 56, 57, 58, 59 },
	Part{ 61, 62, 63, 64, 65, 66, 67, 68, 69 },
	Part{ 71, 72, 73, 74, 75, 76, 77, 78, 79 },
	Part{ 81, 82, 83, 84, 85, 86, 87, 88, 89 },
	Part{ 91, 92, 93, 94, 95, 96, 97, 98, 99 },
};

class Sudoku
{
	Grid entries;

public:
	Sudoku(const Grid& new_entries) : entries{ new_entries } {}

public:
	Part row(int row) const {
		return entries.at(row);
	}

	Part column(int column) const {
		Part result;
		for (const auto entry_row : entries)
			result.push_back(entry_row.at(column));
		return result;
	}

	Part box(int the_box) const {
		int x = (the_box / 3) * 3;
		int y = (the_box % 3) * 3;
		Part result;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				result.push_back(entries.at(x + i).at(y + j));
			}
		}
		return result;
	}

};

TEST_CASE("Sudoku::row") {
	REQUIRE((Part{ 21, 22, 23, 24, 25, 26, 27, 28, 29 }) == Sudoku{ Test }.row(1));
}

TEST_CASE("Sudoku::column") {
	REQUIRE((Part{ 11, 21, 31, 41, 51, 61, 71, 81, 91 }) == Sudoku{ Test }.column(0));
}

TEST_CASE("Quotients") {
	auto expected = Part{ 0, 0, 0, 3, 3, 3, 6, 6, 6 };
	Part actual;
	for (int i = 0; i < 9; ++i) {
		actual.push_back((i / 3) * 3);
	}
	REQUIRE(expected == actual);
}

TEST_CASE("Remainders") {
	auto expected = Part{ 0, 3, 6, 0, 3, 6, 0, 3, 6 };
	Part actual;
	for (int i = 0; i < 9; ++i) {
		actual.push_back((i % 3) * 3);
	}
	REQUIRE(expected == actual);
}

TEST_CASE("Sudoku::box") {
	REQUIRE((Part{ 11, 12, 13, 21, 22, 23, 31, 32, 33 }) == Sudoku{ Test }.box(0));
}

bool duplicates(const Part& p) 
{
	auto counter = Part{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (auto e : p) {
		if (e != 0) {
			++counter[e - 1];
			if (counter[e - 1] > 1)
				return true;
		}
	}
	return false;
}

TEST_CASE("Part contains no duplicates") {
	REQUIRE(!duplicates(Part{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }));
	REQUIRE(!duplicates(Part{ 0, 2, 0, 4, 0, 6, 7, 0, 9 }));
	REQUIRE(duplicates(Part{ 0, 2, 2, 4, 0, 6, 7, 0, 9 }));
}

static const auto Solved = Grid{
	Part{ 1, 4, 8, 6, 5, 9, 7, 3, 2 },
	Part{ 7, 2, 5, 8, 1, 3, 9, 6, 4 },
	Part{ 9, 6, 3, 4, 7, 2, 5, 1, 8 },
	Part{ 3, 7, 2, 5, 8, 6, 4, 9, 1 },
	Part{ 5, 1, 6, 9, 2, 4, 3, 8, 7 },
	Part{ 8, 9, 4, 1, 3, 7, 2, 5, 6 },
	Part{ 4, 3, 1, 7, 6, 5, 8, 2, 9 },
	Part{ 6, 5, 7, 2, 9, 8, 1, 4, 3 },
	Part{ 2, 8, 9, 3, 4, 1, 6, 7, 5 },
};

static const auto Partial = Grid{
	Part{ 1, 4, 0, 0, 0, 9, 0, 3, 0 },
	Part{ 0, 0, 5, 8, 0, 3, 0, 6, 0 },
	Part{ 9, 0, 0, 0, 0, 0, 5, 0, 0 },
	Part{ 3, 7, 0, 5, 0, 0, 4, 0, 0 },
	Part{ 0, 1, 6, 0, 0, 0, 3, 8, 0 },
	Part{ 0, 0, 4, 0, 0, 7, 0, 5, 6 },
	Part{ 0, 0, 1, 0, 0, 0, 0, 0, 9 },
	Part{ 0, 5, 0, 2, 0, 8, 1, 0, 0 },
	Part{ 0, 8, 0, 3, 0, 0, 0, 7, 5 },
};

static const auto Invalid = Grid{
	Part{ 1, 4, 2, 6, 5, 9, 7, 3, 2 },
	Part{ 7, 2, 5, 8, 1, 3, 9, 6, 4 },
	Part{ 9, 6, 3, 4, 7, 2, 5, 1, 8 },
	Part{ 3, 7, 2, 5, 8, 6, 4, 9, 1 },
	Part{ 5, 1, 6, 9, 2, 4, 3, 8, 7 },
	Part{ 8, 9, 4, 1, 3, 7, 2, 5, 6 },
	Part{ 4, 3, 1, 7, 6, 5, 8, 2, 9 },
	Part{ 6, 5, 7, 2, 9, 8, 1, 4, 3 },
	Part{ 2, 8, 9, 3, 4, 1, 6, 7, 5 },
};

bool sudoku_valid(const Grid& g) {
	Sudoku s{ g };
	for (int i = 0; i < 9; ++i) {
		if	(
				duplicates(s.row(i)) 
				|| duplicates(s.column(i)) 
				|| duplicates(s.box(i))
			)
			return false;
	}
	return true;
}

TEST_CASE("(partial) Grid contains no duplicates") {
	REQUIRE(sudoku_valid(Solved));
	REQUIRE(sudoku_valid(Partial));
	REQUIRE(!sudoku_valid(Invalid));
}

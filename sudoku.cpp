#include "catch.hpp"

// #include <array>
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
	Part row(int the_row) {
		return entries.at(the_row);
	}

	Part column(int the_column) {
		Part result;
		for (const auto entry_row : entries)
			result.push_back(entry_row.at(the_column));
		return result;
	}

	Part box(int the_box) {
		int y = (the_box / 3) * 3;
		int x = (the_box % 3) * 3;
		Part result;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				result.push_back(entries.at(y + i).at(x + j));
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

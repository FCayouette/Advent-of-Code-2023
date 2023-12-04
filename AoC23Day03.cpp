import std.core;

using u64 = unsigned long long;

struct Number
{
	Number(int v, int m, int M) : value(v), colMin(m), colMax(M) {}
	int value, colMin, colMax;
	bool counts = false;

	int Check(int col)
	{
		if (col + 1 >= colMin && col - 1 <= colMax)
		{
			counts = true;
			return value;
		}
		return 0;
	}
};

struct Symbol
{
	Symbol(int _x, int _y, bool g) : x(_x), y(_y), isGear(g) {}
	int x, y;
	bool isGear;
};

constexpr bool IsDigit(char c) { return c >= '0' && c <= '9'; }

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day03.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	u64 part1 = 0, part2 = 0;
	std::string line;
	std::vector<std::vector<Number>> numbers;
	std::vector<Symbol> symbols;
	int row = 0;
	while (in >> line)
	{
		numbers.emplace_back();

		for (int i = 0, cur; i < line.size(); ++i)
		{
			if (IsDigit(line[i]))
			{
				cur = line[i] - '0';
				int start = i++;
				while (i <= line.size() && IsDigit(line[i]))
					cur = cur * 10 + line[i++] - '0';
				numbers.back().emplace_back(cur, start, i - 1);
			}
			if (i < line.size() && line[i] != '.')
				symbols.emplace_back(row, i, line[i]=='*');
		}
		++row;
	}

	for (const Symbol& s : symbols)
	{
		std::vector<int> gears;
		for (int k = std::max(0, s.x - 1); k < std::min<int>(s.x + 2, numbers.size()); ++k)
			for (Number& n : numbers[k])
				if (int value = n.Check(s.y);
					value > 1 && s.isGear)
					gears.push_back(value);
		if (gears.size() == 2)
			part2 += gears[0] * gears[1];

	}

	for (const auto& r : numbers)
		for (const Number& n : r)
			if (n.counts)
				part1 += n.value;
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
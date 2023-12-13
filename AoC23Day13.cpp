import std.core;

int FindColumnMirror(const std::vector<std::string>& m, int ignore = 0)
{
	for (int i = 1; i < m.front().size(); ++i)
	{
		if (i == ignore) continue;
		bool valid = true;
		for (const std::string& s : m)
		{
			for (int x = i - 1, y = i; x >= 0 && y < s.size() && valid; --x, ++y)
				valid = (valid && s[x] == s[y]);
			if (!valid)
				break;
		}
		if (valid)
			return i;
	}

	return 0;
}

int FindRowMirror(const std::vector<std::string>& m, int ignore = 0)
{
	for (int i = 1; i < m.size(); ++i)
	{
		if (i == ignore) continue;
		bool valid = true;
		for (int j = 0; j < m.front().size() && valid; ++j)
			for (int x = i - 1, y = i; x >= 0 && y < m.size() && valid; --x, ++y)
				valid = (valid && m[x][j] == m[y][j]);
		if (valid)
			return i;
	}

	return 0;
}

std::pair<int, int> SmudgeFind(std::vector<std::string>& m)
{
	int origCol = FindColumnMirror(m), origRow = FindRowMirror(m), first = origCol + 100 * origRow;

	for (int x = 0; x < m.size(); ++x)
		for (int y = 0; y < m.front().size(); ++y)
		{
			m[x][y] = (m[x][y] == '.' ? '#' : '.');
			if (int i = FindColumnMirror(m, origCol);
				i != 0)
				return { first, i };
			if (int i = FindRowMirror(m, origRow);
				i != 0)
				return { first, 100 * i };
			
			m[x][y] = (m[x][y] == '.' ? '#' : '.');
		}

	return { first, 0 }; // Error !
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day13.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	int part1 = 0, part2 = 0;
	std::string line;

	std::vector<std::string> pattern;
	while (std::getline(in, line))
	{
		if (line.empty())
		{
			auto [a, b] = SmudgeFind(pattern);
			part1 += a; part2 += b;
			pattern.clear();
		}
		else
			pattern.push_back(line);
	}

	auto [a, b] = SmudgeFind(pattern);
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1 + a, part2 + b);
}
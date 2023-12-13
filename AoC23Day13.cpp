import std.core;

std::pair<int, int> FindMirrors(std::vector<std::string>& m)
{
	std::pair<int, int> results;

	for (int i = 1, count = 0; i < m.front().size() && !(results.first & results.second); ++i, count = 0)
	{
		for (const std::string& s : m)
			for (int x = i - 1, y = i; x >= 0 && y < s.size() && count < 2; --x, ++y)
				count += (s[x] != s[y]);

		if (count == 0)
			results.first = i;
		else if (count == 1)
			results.second = i;
	}
	
	for (int i = 1, count = 0; i < m.size() && !(results.first & results.second); ++i, count = 0)
	{
		for (int j = 0; j < m.front().size() && count < 2; ++j)
			for (int x = i - 1, y = i; x >= 0 && y < m.size() && count < 2; --x, ++y)
				count += (m[x][j] != m[y][j]);

		if (count == 0)
			results.first = i*100;
		else if (count == 1)
			results.second = i*100;
	}

	return results;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day13.exe inputFilename\n";
		return -1;
	}
	auto start = std::chrono::high_resolution_clock::now();
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
			auto [a, b] = FindMirrors(pattern);
			part1 += a; part2 += b;
			pattern.clear();
		}
		else
			pattern.push_back(line);
	}

	auto [a, b] = FindMirrors(pattern);
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
	std::cout << std::format("Part 1: {}\nPart 2: {}\nDuration: {}\n", part1 + a, part2 + b, duration);
}
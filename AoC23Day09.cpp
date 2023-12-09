import std.core;

using i64 = long long;

std::pair<i64, i64> Bothpolate(const std::vector<i64>& v)
{
	std::vector<i64> next;
	for (int i = 1; i < v.size(); ++i)
		next.push_back(v[i] - v[i - 1]);
	if (std::any_of(next.cbegin(), next.cend(), [](i64 a) { return a != 0; }))
	{
		auto [f, b] = Bothpolate(next);;
		return { f + v.back(), v.front() - b };
	}
	return { v.back(), v.front() };
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day09.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	i64 part1 = 0, part2 = 0;
	std::string line;

	while (std::getline(in, line))
	{
		std::stringstream ss(line);
		i64 value;
		std::vector<i64> values;
		while (ss >> value)
			values.push_back(value);

		auto [p1, p2] = Bothpolate(values);
		part1 += p1;
		part2 += p2;
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
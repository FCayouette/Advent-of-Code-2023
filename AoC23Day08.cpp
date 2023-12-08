import std.core;

using u64 = unsigned long long;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23DayXX.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	u64 part1 = 0;
	std::string moveSequence, base, left, right, pos = "AAA";
	std::map<std::string, std::pair<std::string, std::string>> map;
	std::vector<std::string> positions;
	std::vector<u64> cycles;

	in >> moveSequence;
	while (in >> base >> left >> left >> right)
	{
		left = left.substr(1, left.size() - 2);
		right = right.substr(0, right.size() - 1);
		map[base] = std::make_pair(left, right);
		if (base.ends_with('A'))
			positions.emplace_back(base);
	}

	while (pos != "ZZZ")
		pos = moveSequence[part1++ % moveSequence.size()] == 'L' ? map[pos].first : map[pos].second;
	
	for (std::string s : positions)
	{
		u64 count = 0;
		while (!s.ends_with('Z'))
			s = moveSequence[count++ % moveSequence.size()] == 'L' ? map[s].first : map[s].second;
		cycles.push_back(count);
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, std::accumulate(cycles.cbegin(), cycles.cend(), 1ull, [](u64 a, u64 b) {return std::lcm(a, b); }));
}
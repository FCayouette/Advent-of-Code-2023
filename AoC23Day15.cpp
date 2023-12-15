import std.core;

constexpr int Hash(const std::string& s)
{
	return std::accumulate(s.cbegin(), s.cend(), (unsigned char)0, [](unsigned char a, unsigned char b) { return (a + b) * 17; });
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day15.exe inputFilename\n";
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
	std::array<std::list<std::pair<std::string, int>>, 256> lenses;

	while (std::getline(in, line, ','))
	{
		part1 += Hash(line);

		if (line.back() == '-')
		{
			std::string label = line.substr(0, line.size() - 1);
			int box = Hash(label);
			for (auto iter = lenses[box].begin(); iter != lenses[box].end(); ++iter)
				if (iter->first == label)
				{
					lenses[box].erase(iter);
					break;
				}
		}
		else
		{
			std::string label = line.substr(0, line.size() - 2);
			int box = Hash(label), power = line.back() - '0';
			auto iter = lenses[box].begin();
			while (iter != lenses[box].end())
			{
				if (iter->first == label)
				{
					iter->second = power;
					break;
				}
				++iter;
			}
			if (iter == lenses[box].end())
				lenses[box].push_back({ label, power });
		}
	}

	int boxMul = 1;
	for (const auto& box : lenses)
	{
		int mul = 1;
		for (const auto& p : box)
			part2 += boxMul * mul++ * p.second;
		++boxMul;
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
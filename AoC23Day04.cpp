import std.core;

using u64 = unsigned long long;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day04.exe inputFilename\n";
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
	std::vector<u64> copies, counts;
	while (in >> line >> line)
	{
		std::vector<int> winning;
		while (in >> line)
		{
			if (line[0] == '|')
				break;
			winning.push_back(stoi(line));
		}
		std::sort(winning.begin(), winning.end());
		int cardVal = 0, n;
		std::getline(in, line);
		std::stringstream ss(line);
		copies.push_back(1);
		counts.push_back(0);
		while (ss >> n)
			if (std::binary_search(winning.cbegin(), winning.cend(), n))
			{
				cardVal = (!cardVal ? 1 : cardVal * 2);
				++counts.back();
			}
		part1 += cardVal;
	}

	for (int i = 0; i < copies.size(); ++i)
	{
		part2 += copies[i];
		for (int j = i + 1; j < counts.size() && j <= i + counts[i]; ++j)
			copies[j] += copies[i];
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
import std.core;

using u64 = unsigned long long;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day01.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	constexpr auto IsDigit = [](char c) { return c >= '1' && c <= '9'; };	
	constexpr std::array<std::string_view, 9> names= { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
	u64 part1 = 0, part2 = 0;
	std::string line;

	while (in >> line)
	{
		auto ToInt = [&line](auto i) { return line[i] - '0'; };
		part1 += 10 * ToInt(line.find_first_of("123456789")) + ToInt(line.find_last_of("123456789"));

		std::vector<int> p2;
		for (size_t i = 0; i < line.size(); ++i)
		{
			if (std::string_view sv(line.cbegin() + i, line.end()); 
				IsDigit(line[i]))
				p2.push_back(ToInt(i));
			else
				for (int j = 0; j < names.size(); ++j)
					if (sv.starts_with(names[j]))
					{
						p2.push_back(j + 1);
						i += names[j].size() - 2;
						break;
					}
		}
		part2 += 10 * p2.front() + p2.back();
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
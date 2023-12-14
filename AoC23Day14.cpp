import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()
using u64 = unsigned long long;

u64 Load(const std::vector<std::string>& m)
{
	u64 result = 0;
	for (int i = 0, value = m.size(); i < m.size(); ++i, --value)
		result += std::count(ALLc(m[i]), 'O') * value;
	return result;
}

u64 Spin(std::vector<std::string>& map)
{
	// North
	for (int i = 1; i < map.size(); ++i)
		for (int j = 0; j < map.front().size(); ++j)
			if (map[i][j] == 'O')
			{
				int cur = i, target = i - 1;
				while (target >= 0 && map[target][j] == '.')
					std::swap(map[cur--][j], map[target--][j]);
			}
	//West
	for (std::string& s : map)
		for (int i = 1; i < s.size(); ++i)
			if (s[i] == 'O')
			{
				int cur = i, target = i - 1;
				while (target >= 0 && s[target] == '.')
					std::swap(s[cur--], s[target--]);
			}
	//South
	for (int i = map.size() - 2; i >= 0; --i)
		for (int j = 0; j < map.front().size(); ++j)
			if (map[i][j] == 'O')
			{
				int cur = i, target = i + 1;
				while (target < map.size() && map[target][j] == '.')
					std::swap(map[cur++][j], map[target++][j]);
			}
	//East
	for (std::string& s : map)
		for (int i = s.size() - 2; i >= 0; --i)
			if (s[i] == 'O')
			{
				int cur = i, target = i + 1;
				while (target < s.size() && s[target] == '.')
					std::swap(s[cur++], s[target++]);
			}
	return Load(map);
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day14.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	std::string line;
	std::vector<std::string> map, copy;
	std::vector<u64> values;

	while (in >> line)
		map.emplace_back(std::move(line));

	copy = map;
	
	for (int i = 1; i < map.size(); ++i)
		for (int j = 0; j < map.front().size(); ++j)
			if (map[i][j] == 'O')
			{
				int cur = i, target = i - 1;
				while (target >=0 && map[target][j] == '.')
					std::swap(map[cur--][j], map[target--][j]);
			}

	for (int i = 0; i < 500; ++i)
		values.push_back(Spin(copy));
	int cycle = 1;
	while (values[values.size() - ++cycle] != values.back());
	--cycle;
	
	int down = cycle - (1'000'000'000 - values.size()) % cycle;
	auto iter = values.cend() - 1;
	while (down-- > 0) --iter;
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", Load(map), *iter);
}
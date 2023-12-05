import std.core;

#define ALL(x) (x).begin(),(x).end()

using u64 = unsigned long long;
using Map = std::array<u64, 3>;
using Ranges = std::vector<std::pair<u64, u64>>;

u64 Convert(const std::vector<Map> maps, u64 v)
{
	for (const auto& m : maps)
		if (v >= m[1] && v < m[1] + m[2])
			return m[0] + (v - m[1]);
	return v;
}

Ranges SplitRanges(std::vector<Map> maps, const Ranges& ranges)
{
	std::sort(ALL(maps), [](const Map& l, const Map& r) { return l[1] < r[1]; });
	Ranges result;

	auto Emplace = [&result](u64 a, u64 b) { if (a != b) result.emplace_back(a, b); }; // avoid empty range

	int currMap = 0;
	for (const std::pair<u64, u64>& r : ranges)
	{
		while (currMap < maps.size() && (maps[currMap][1]+maps[currMap][2] < r.first))
			++currMap;
		if (currMap == maps.size())
			result.push_back(r);
		else
		{
			bool inside = r.first > maps[currMap][1];
			u64 from = r.first, to = r.second;
			while (from != to)
			{
				if (inside)
				{
					u64 rangeMax = maps[currMap][1] + maps[currMap][2], m = std::min(to, rangeMax);
					Emplace(maps[currMap][0] + (from - maps[currMap][1]), maps[currMap][0] + (m - maps[currMap][1]));
					from = m;
					if (m != to)
						inside = (++currMap < maps.size() && (from >= maps[currMap][1]));
				}
				else
				{
					if (currMap == maps.size())
					{
						Emplace(from, to);
						from = to;
					}
					else
					{
						u64 m = std::min(to, maps[currMap][1]);
						Emplace(from, m);
						from = m;
						inside = m != to;
					}
				}
			}
		}
	}

	std::sort(ALL(result));
	return result;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day05.exe inputFilename\n";
		return -1;
	}
	
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	std::string line;

	std::getline(in, line);
	std::stringstream seedstream(line.substr(line.find(':') + 2));
	std::vector<u64> seeds;
	u64 val;
	while (seedstream >> val)
		seeds.push_back(val);
		
	std::vector<std::vector<Map>> maps;
	while (std::getline(in, line))
	{
		if (line.empty()) continue;
		if (!std::isdigit(line[0]))
			maps.push_back({});
		else
		{
			std::stringstream ss(line);
			Map m;
			ss >> m[0] >> m[1] >> m[2];
			maps.back().push_back(m);
		}
	} 

	std::vector<u64> results;
	for (u64 seed : seeds)
	{
		for (const auto& m : maps)
			seed = Convert(m, seed);
		results.push_back(seed);
	}

	Ranges ranges;
	for (int i = 0; i < seeds.size(); i += 2)
		ranges.emplace_back(seeds[i], seeds[i] + seeds[i+1]);
	std::sort(ALL(ranges));
	
	for (const auto& m : maps)
		ranges = SplitRanges(m, ranges);
	
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", *std::min_element(results.cbegin(), results.cend()), ranges.front().first);
}
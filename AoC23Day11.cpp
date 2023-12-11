import std.core;
using u64 = unsigned long long;

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	T x, y;
};
using Point = Coord<int>;

template<typename T>
T ManhattanDistance(const Coord<T>& c1, const Coord<T>& c2)
{
	if constexpr (std::is_signed_v<T>)
		return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
	else
		return std::max(c1.x, c2.x) - std::min(c1.x, c2.x) + std::max(c1.y, c2.y) - std::min(c1.y, c2.y);
}

template <typename T>
constexpr bool Within(T a, T val, T b)
{
	return std::min(a,b) < val && val < std::max(a,b);
}

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

	u64 part1 = 0, part2 = 0;
	std::string line;
	std::vector<std::string> map;
	std::vector<int> emptyRows, emptyCols;
	std::vector<Point> galaxies;

	while (in >> line)
	{
		for (int i = 0; i < line.size(); ++i)
			if (line[i] == '#')
				galaxies.push_back(Point(map.size(), i));
		
		if (line.find('#') == std::string::npos)
			emptyRows.push_back(map.size());

		map.emplace_back(line);
	}

	for (int y = 0; y < map.front().size(); ++y)
	{
		bool empty = true;
		for (int x = 0; x < map.size() && empty; ++x)
			empty = empty && (map[x][y] != '#');

		if (empty)
			emptyCols.push_back(y);
	}

	for (int i = 0; i < galaxies.size() - 1; ++i)
	{
		const Point& a = galaxies[i];
		for (int j = i + 1; j < galaxies.size(); ++j)
		{
			const Point& b = galaxies[j];
			int dist = ManhattanDistance(a, b), dist2 = dist;
			for (int r : emptyRows)
				if (Within(a.x, r, b.x))
				{
					++dist;
					dist2 += 1000000 -1;
				}
			for (int c : emptyCols)
				if (Within(a.y, c, b.y))
				{
					++dist;
					dist2 += 1000000 -1;
				}
			
			part1 += dist;
			part2 += dist2;
		}
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
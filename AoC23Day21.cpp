import std.core;

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	T x, y;
};

using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

using u64 = unsigned long long;
using i64 = long long;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day21.exe inputFilename\n";
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
	Point start;
	while (in >> line)
	{
		auto p = line.find('S');
		if (p != std::string::npos)
		{
			start = Point(map.size(), p);
			line[p] = '.';
		}
		map.emplace_back(line);
	}

	std::set<Point> cur, next;
	int xs = map.size(), ys = map.front().size(), offset = 26501365 % xs, iter = offset;
	std::vector<i64> counts;
	cur.insert(start);
	for (int i = 1; i <= 2*xs + offset; ++i)
	{
		for (const Point& p : cur)
			for (const Point& delta : directions)
			{
				Point np = p + delta;
				int x = np.x, y = np.y;
				if (x >= 0)
					x %= xs;
				else
					x = (xs - ((-x) % xs)) % xs;
				if (y >= 0)
					y %= ys;
				else
					y = (ys - ((-y) % ys)) % ys;

				if (map[x][y] == '.')
					next.insert(np);
			}
		std::swap(cur, next);

		if (i == 64)
			part1 = cur.size();
	
		if (i %xs == offset)
			counts.push_back(cur.size());
	}
	part2 = counts.front(); ;
	u64 f = (counts[2] - 2 * counts[1] + counts[0]), g = (counts[1] - counts[0])-f;
	for (i64 x = 1; iter< 26501365; ++x, iter+=xs)
		part2 += x * f + g;
	
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
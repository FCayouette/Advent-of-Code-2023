import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr bool operator==(const Coord& p) const { return x == p.x && y == p.y; }
	T x, y;
};

using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };
constexpr std::array<char, 4> validSlopes = { 'v', '>', '^', '<' };

template <typename T>
class y_combinator {
	T lambda;
public:
	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}
	template <typename...Args>
	constexpr decltype(auto) operator()(Args&&...args) const { return lambda(std::move(*this), std::forward<Args>(args)...); }
};

struct PathLength
{
	Point start, end;
	int cost;
	bool operator<(const PathLength& pl) const { if (start < pl.start) return true; if (pl.start < start) return false; return end < pl.end; }
};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day23.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	std::string line;
	std::vector<std::string> map;
	while (in >> line)
		map.emplace_back(std::move(line));
	
	std::vector<PathLength> pathSegments, bothDir;
	Point startPoint(0, 1), targetPoint(map.size() - 1, map.front().size() - 2);
	std::set<Point> toEval = {startPoint};
	
	while (!toEval.empty())
	{
		Point pathStart = *toEval.cbegin();
		std::set<Point> cur = { pathStart }, next, repeatGuard = cur;
		toEval.erase(pathStart);
		int pathCost = 1;
		
		while (!cur.empty())
		{
			for (Point p : cur)
			{
				char slope = map[p.x][p.y];
				for (int i = 0; i < 4; ++i)
				{
					if (slope != '.' && slope != validSlopes[i])
						continue;
					if (Point np = p + directions[i];
						np.x >= 0 && np.y >= 0 && np.x < map.size() && np.y < map.front().size())
						if (char c = map[np.x][np.y];
							c != '#')
						{
							if (c == '.')
							{
								if (repeatGuard.insert(np).second)
									next.insert(np);
								if (np == targetPoint)
								{
									PathLength pl;
									pl.start = pathStart;
									pl.end = np;
									pl.cost = pathCost;
									pathSegments.push_back(pl);
								}
							}
							else if (c == validSlopes[i] )
							{
								PathLength pl;
								pl.start = pathStart;
								pl.end = np;
								pl.cost = pathCost;
								pathSegments.push_back(pl);
								toEval.insert(np);
							}
						}
				}
			}
			std::swap(next, cur);
			next.clear();
			++pathCost;
		}
	}

	auto FindLongest = y_combinator([&](auto FindLongest, const Point& p) ->int
	{
		int maxSubCost = 0;
		for (const PathLength& pl : pathSegments)
			if (pl.start == p)
			{
				if (pl.end != targetPoint)
				{
					int cost = FindLongest(pl.end);
					maxSubCost = std::max(maxSubCost, cost+pl.cost);
				}
				else
				{
					maxSubCost = std::max(maxSubCost, pl.cost);
				}
			}

		return maxSubCost;
	});
	
	int part1 = FindLongest(startPoint), part2 = 0;
	
	auto IsSlope = [&](const Point& p) {
		char c = map[p.x][p.y];
		return std::find(ALLc(validSlopes), c) != validSlopes.cend();
	};

	std::vector<Point> crossings = { startPoint, targetPoint }, path = {startPoint};
	for (int i = 1; i < map.size() -1; ++i)
		for (int j = 1; j < map.front().size()-1; ++j)
			if (map[i][j] == '.')
			{
				std::array<Point, 4> dir = { Point(i - 1, j), Point(i + 1, j) , Point(i, j - 1) , Point(i, j + 1) };
				if (int count = IsSlope(dir[0]) + IsSlope(dir[1]) + IsSlope(dir[2]) + IsSlope(dir[3]);
					count > 2)
				{
					crossings.emplace_back(i, j);
				}
			}

	for (const Point& toEval : crossings)
	{
		if (toEval == targetPoint) continue;
		std::set<Point> cur = { toEval }, next, guard = cur;
		int cost = 1;
		while (!cur.empty())
		{
			for (const Point& p : cur)
				for (const Point& d  : directions)
					if (Point np = p + d;
						np.x >= 0 && np.y >= 0 && np.x < map.size() && np.y < map.front().size())
			{
				if (char c = map[np.x][np.y];
					c != '#' && guard.insert(np).second)
				{
					if (std::find(ALLc(crossings), np) != crossings.cend())
					{
						if (np == startPoint) continue;
						PathLength pl;
						pl.start = toEval;
						pl.end = np;
						pl.cost = cost;
						bothDir.push_back(pl);
					}
					else
						next.insert(np);
				}
				
			}
			std::swap(cur, next);
			next.clear();
			++cost;
		}
	}

	std::sort(ALL(bothDir));
	y_combinator([&](auto FindLongest2, std::vector<Point>& path, int runningCost) -> void
		{
			int i = 0;
			const Point& p = path.back();
			while (bothDir[i].start != p)
				++i;

			for (; i < bothDir.size() && bothDir[i].start == p; ++i)
			{
				const PathLength& pl = bothDir[i];
				if (pl.end != targetPoint)
				{
					if (std::find(ALLc(path), pl.end) == path.cend())
					{
						path.push_back(pl.end);
						FindLongest2(path, runningCost + pl.cost);
						path.pop_back();
					}
				}
				else
					part2 = std::max(runningCost + pl.cost, part2);
			}
		}) (path, 0);
	
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
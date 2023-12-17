import std.core;

template <typename Info_t, typename Cost_t, bool Unique>
struct AStarInfo
{
	constexpr AStarInfo(const Info_t& i, Cost_t c) : info(i), cost_plus_Eval{}, cost(c) {}
	constexpr AStarInfo(const Info_t& i, Cost_t c, Cost_t eval) : info(i), cost_plus_Eval(c + eval), cost(c) {}

	constexpr bool operator > (const AStarInfo& right) const { return cost_plus_Eval > right.cost_plus_Eval || (cost_plus_Eval == right.cost_plus_Eval && info > right.info); }
	constexpr bool RepeatSetCheck(const AStarInfo& right) const
	{
		if constexpr (Unique) return info < right.info;
		else return info < right.info || (info == right.info && cost < right.cost);
	}
	Info_t info;
	Cost_t cost_plus_Eval;
	Cost_t cost;
};

template <typename ASI, typename Expand, typename Estimate, typename TargetEquality>
class AStar
{
public:
	using Cost_t = decltype(ASI::cost);

	AStar(const ASI& start, const Expand& ex, const Estimate& es, const TargetEquality& eq) : expand(ex), estimate(es), equality(eq)
	{
		Initialize(start);
	}

	void Initialize(const ASI& start)
	{
		std::priority_queue<ASI, std::vector<ASI>, std::greater<ASI>> empty;
		std::swap(toEvaluate, empty);
		repeatGuard.clear();
		toEvaluate.push(start);
	}

	std::optional<Cost_t> Evaluate()
	{
		while (!toEvaluate.empty())
		{
			ASI current = toEvaluate.top();
			if (equality(current))
				return current.cost;

			toEvaluate.pop();
			if (repeatGuard.insert(current).second)
			{
				auto toAdd = expand(current);
				for (ASI& a : toAdd)
				{
					a.cost_plus_Eval = a.cost + estimate(a);
					toEvaluate.push(a);
				}
			}
		}
		return {};
	}

protected:
	struct Comparator { bool operator()(const ASI& l, const ASI& r) const { return l.RepeatSetCheck(r); } };
	std::set<ASI, Comparator> repeatGuard;
	std::priority_queue<ASI, std::vector<ASI>, std::greater<ASI>> toEvaluate;
	Expand expand;
	Estimate estimate;
	TargetEquality equality;
};

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr Coord& operator+=(const Coord& p) { x += p.x; y += p.y; return *this; }
	constexpr bool operator==(const Coord& p) const { return x == p.x && y == p.y; }
	constexpr bool operator!=(const Coord& p) const { return x != p.x || y != p.y; }
	T x, y;
};

template<typename T>
T ManhattanDistance(const Coord<T>& c1, const Coord<T>& c2)
{
	if constexpr (std::is_signed_v<T>)
		return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y);
	else
		return std::max(c1.x, c2.x) - std::min(c1.x, c2.x) + std::max(c1.y, c2.y) - std::min(c1.y, c2.y);
}

using Point = Coord<int>;

constexpr std::array<char, 4> coords = { 'N', 'E', 'S', 'W' };
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

struct PathInfo
{
	Point p = {};
	int lastDir = 0;
	int moveCount = 0;

	constexpr bool operator <  (const PathInfo& pi) const {
		if (p < pi.p) return true;
		if (p == pi.p) return std::make_pair(lastDir, moveCount) < std::make_pair(pi.lastDir, pi.moveCount);
		return false;
	}
	constexpr bool operator >  (const PathInfo& pi) const {
		if (pi.p < p) return true;
		if (p == pi.p) return std::make_pair(lastDir, moveCount) > std::make_pair(pi.lastDir, pi.moveCount);
		return false;
	}
};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day17.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	std::string line;
	std::vector<std::vector<int>> heatMap;

	while (in >> line)
	{
		std::vector<int> values;
		for (char c : line)
			values.push_back(c - '0');
		heatMap.emplace_back(std::move(values));
	}

	using ASI = AStarInfo<PathInfo, int, true>;
	Point targetPoint(heatMap.size() - 1, heatMap.front().size() - 1);
	auto target = [targetPoint](const ASI& asi) { return asi.info.p == targetPoint; };

	auto NotReverse = [](int a, int b)
	{
		int min = std::min(a, b), max = std::max(a, b);
		return min + 2 != max;
	};

	auto expand = [&heatMap, &NotReverse](const ASI& asi)
	{
		std::vector<ASI> newPoints;
		for (int i = 0; i < directions.size(); ++i)
		{
			Point np = asi.info.p + directions[i];
			if (np.x >= 0 && np.y >= 0 && np.x < heatMap.size() && np.y < heatMap.front().size())
			{
				PathInfo info;
				if (asi.info.lastDir < 0)
				{
					info.p = np;
					info.lastDir = i;
					info.moveCount = 1;
					newPoints.emplace_back(info, asi.cost + heatMap[np.x][np.y]);
				}
				else if (i != asi.info.lastDir)
				{
					if (NotReverse(asi.info.lastDir, i))
					{
						info.p = np;
						info.lastDir = i;
						info.moveCount = 1;
						newPoints.emplace_back(info, asi.cost + heatMap[np.x][np.y]);
					}
				}
				else if (asi.info.moveCount < 3)
				{
					info.p = np;
					info.lastDir = i;
					info.moveCount = asi.info.moveCount + 1;
					newPoints.emplace_back(info, asi.cost + heatMap[np.x][np.y]);
				}
			}
		}
		return newPoints;
	};

	auto estimate = [targetPoint](const ASI& asi) { return ManhattanDistance(asi.info.p, targetPoint); };

	auto expand2 = [&heatMap, &NotReverse](const ASI& asi)
	{
		std::vector<ASI> newPoints;
		for (int i = 0; i < directions.size(); ++i)
		{
			Point np = asi.info.p + directions[i];
			if (np.x >= 0 && np.y >= 0 && np.x < heatMap.size() && np.y < heatMap.front().size())
			{
				PathInfo info;
				if (asi.info.lastDir < 0)
				{
					info.p = np;
					info.lastDir = i;
					info.moveCount = 1;
					newPoints.emplace_back(info, asi.cost + heatMap[np.x][np.y]);
				}
				else if (i != asi.info.lastDir)
				{
					if (asi.info.moveCount >= 4 && NotReverse(asi.info.lastDir, i))
					{
						info.p = np;
						info.lastDir = i;
						info.moveCount = 1;
						newPoints.emplace_back(info, asi.cost + heatMap[np.x][np.y]);
					}
				}
				else if (asi.info.moveCount < 10)
				{
					info.p = np;
					info.lastDir = i;
					info.moveCount = asi.info.moveCount + 1;
					newPoints.emplace_back(info, asi.cost + heatMap[np.x][np.y]);
				}
			}
		}
		return newPoints;
	};
	
	PathInfo info;
	info.p = Point(0, 0);
	info.lastDir = -1;
	info.moveCount = 0;
	ASI start(info, 0, heatMap.size() + heatMap.front().size());

	AStar aStar(start, expand, estimate, target);
	AStar aStar2(start, expand2, estimate, target);

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", *aStar.Evaluate(), *aStar2.Evaluate());
}
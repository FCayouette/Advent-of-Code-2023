import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr Coord& operator+=(const Coord& p) { x += p.x; y += p.y; return *this; }
	T x, y;
};

using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };

template <typename T>
class y_combinator {
	T lambda;
public:
	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}
	template <typename...Args>
	constexpr decltype(auto) operator()(Args&&...args) const { return lambda(std::move(*this), std::forward<Args>(args)...); }
};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day16.exe inputFilename\n";
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
	std::vector<std::string> map;
	std::vector<std::vector<char>> cycleDetector;
	while (in >> line)
	{
		cycleDetector.emplace_back(line.size(), 0);
		map.emplace_back(std::move(line));
	}

	auto GetChar = [&map](const Point& p) -> std::optional<char> { 
		if (p.x >= 0 && p.y >= 0 && p.x < map.size() && p.y < map.front().size())
			return map[p.x][p.y];
		return {};
	};

	auto Trace = y_combinator([&](auto Trace, Point p, int dir)->int
		{
			constexpr std::array<char, 4> Masks = { 1, 2, 4, 8 };
			int result = 0;
			while (true)
			{
				auto opt = GetChar(p);
				if (!opt || cycleDetector[p.x][p.y] & Masks[dir])
					return result;
				if (cycleDetector[p.x][p.y] == 0)
					++result;
				cycleDetector[p.x][p.y] |= Masks[dir];
				switch (*opt)
				{
				case '\\':
				{
					if (dir == 0) dir = 1;
					else if (dir == 1) dir = 0;
					else if (dir == 2) dir = 3;
					else dir = 2;
					break;
				}
				case '/':
				{
					if (dir == 0) dir = 3;
					else if (dir == 1) dir = 2;
					else if (dir == 2) dir = 1;
					else dir = 0;
					break;
				}
				case '-':
					if (dir == 0 || dir == 2)
					{
						result += Trace(p + directions[1], 1);
						dir = 3;
					}
					break;

				case '|':
					if (dir == 1 || dir == 3)
					{
						result += Trace(p + directions[0], 0);
						dir = 2;
					}
					break;
				}
				p += directions[dir];
			}
		});

	auto Clear = [&cycleDetector]()
	{
		for (auto& l : cycleDetector)
			for (char& x : l)
				x = 0;
	};

	Point p;
	for (int i = 0; i < map.size(); ++i)
	{	
		p.x = i; p.y = 0;
		if (int result = Trace(p, 1); !i)
			part2 = part1 = result;
		else
			part2 = std::max(part2, result);

		Clear();
		p.x = i, p.y = map.front().size()-1;
		part2 = std::max(part2, Trace(p, 3));
		Clear();
	}

	for (int i = 0; i < map.front().size(); ++i)
	{
		p.x = 0; p.y = i;
		part2 = std::max(part2, Trace(p, 0));

		Clear();
		p.x = map.size()-1, p.y = i;
		part2 = std::max(part2, Trace(p, 2));
		Clear();
	}


	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
import std.core;

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr Coord& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }
	constexpr bool operator==(const Coord& p) const { return x == p.x && y == p.y; }
	T x, y;
};
using Point = Coord<int>;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day10.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	int part2 = 0, dir;
	std::string line, fill;
	std::vector<std::string> map, loopcheck;
	Point start, pos;
	while (in >> line)
	{
		if (fill.empty())
		{
			for (char c : line)
				fill += "..";
		}
		else
			loopcheck.push_back(fill);
		loopcheck.push_back(fill);
		if (auto c = line.find('S');
			c != std::string::npos)
		{
			start = Point(map.size(), c);
			loopcheck[map.size() * 2][c * 2] = 'x';
		}
		map.emplace_back(std::move(line));
	}

	constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };
	std::vector<Point> loop;
	loop.push_back(start);
	
	for (int i = 0; i < 4; ++i)
	{
		Point np = loop.back() + directions[i];
		if (np.x >= 0 && np.x < map.size() && np.y >= 0 && np.y < map.front().size())
		{
			char t = map[np.x][np.y];
			if (i == 0 && (t == '|' || t == 'J' || t == 'L'))
			{
				dir = (t == '|' ? 0 : t == 'J' ? 3 : 1);
				loop.push_back(np);
				break;
			}
			if (i == 1 && (t == '-' || t == '7' || t == 'J'))
			{
				dir = (t == '-' ? 1 : t == '7' ? 0 : 2);
				loop.push_back(np);
				break;
			}
			if (i == 2 && (t == '|' || t == 'F' || t == '7'))
			{
				dir = (t == '|' ? 2 : t == '7' ? 3 : 1);
				loop.push_back(np);
				break;
			}
			if (i == 3 && (t == '-' || t == 'F' || t == 'L'))
			{
				dir = (t == '-' ? 0 : t == 'F' ? 0 : 2);
				loop.push_back(np);
			}
		}
	}

	while (loop.back() != start)
	{
		Point np = loop.back() + directions[dir];
		char t = map[np.x][np.y];
		loop.push_back(np);
		if (dir == 0)
		{
			if (t != '|')
				dir = (t == 'J' ? 3 : 1);
		}
		else if (dir == 1)
		{
			if (t != '-')
				dir = (t == '7' ? 0 : 2);
		}
		else if (dir == 2)
		{
			if (t != '|')
				dir = (t == '7' ? 3 : 1);
		}
		else
		{
			if (t != '-')
				dir = (t == 'F' ? 0 : 2);
		}
	}
	

	for (int i = 1; i < loop.size(); ++i)
	{
		Point prev = loop[i - 1], cur = loop[i], b = prev + cur;
		cur *= 2;
		loopcheck[b.x][b.y] = loopcheck[cur.x][cur.y] = 'x';
	}
	std::set<Point> fillCur, fillNext;
	for (int i = 0; i < loopcheck.front().size(); ++i)
	{
		if (loopcheck.front()[i] == '.')
			fillCur.insert(Point(0, i));
		if (loopcheck.back()[i] == '.')
			fillCur.insert(Point(loopcheck.size()-1, i));
	}
	for (int i = 1; i < loopcheck.size() - 1; ++i)
	{
		if (loopcheck[i].front() == '.')
			fillCur.insert(Point(i, 0));
		if (loopcheck[i].back() == '.')
			fillCur.insert(Point(i, loopcheck[i].size()-1));
	}
	while (!fillCur.empty())
	{
		for (const Point& p : fillCur)
			for (const Point& d : directions)
				if (Point n = d + p; 
					n.x >= 0 && n.x < loopcheck.size() && n.y >= 0 && n.y < loopcheck.front().size() && loopcheck[n.x][n.y] == '.')
				{
					loopcheck[n.x][n.y] = 'O';
					fillNext.insert(n);
				}
		std::swap(fillCur, fillNext);
		fillNext.clear();
	}

	for (int x = 0; x < loopcheck.size(); x += 2)
		for (int y = 0; y < loopcheck.front().size(); y += 2)
			part2 += (loopcheck[x][y] == '.');

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", loop.size()/2, part2);
}
import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

template<typename T>
struct Coord3D
{
	constexpr Coord3D(T X = 0, T Y = 0, T Z = 0) : x(X), y(Y), z(Z) {}
	constexpr bool operator <  (const Coord3D& p) const { if (z < p.z) return true; if (p.z < z) return false; if (x < p.x) return true; if (p.x < x) return false; return y < p.y; } // z is more important for ordering
	T x, y, z;
};
using Point = Coord3D<int>;

struct Line
{
	Point a, b;
	std::set<int> supportedBy, supports;

	Line(const Point& p1, const Point& p2)
	{
		if (p1 < p2) { a = p1; b = p2; }
		else { a = p2; b = p1; }
	}

	void Drop(int by) { a.z -= by; b.z -= by; }
	constexpr bool operator <(const Line& l) { if (a < l.a) return true; if (l.a < a) return false; return b < l.b; }
	constexpr bool Intersects(const Line& l) const { return !(l.b.x < a.x || l.a.x > b.x || l.b.y < a.y || l.a.y > b.y); }
};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day22.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	int part1 = 0, part2 = 0, x1, x2, y1, y2, z1, z2;
	std::string line;
	std::vector<Line> lines, settled;
	std::vector<int> safe;

	while (in >> line)
	{
		sscanf_s(line.c_str(), "%d,%d,%d~%d,%d,%d", &x1, &y1, &z1, &x2, &y2, &z2);
		lines.emplace_back(Point(x1, y1, z1), Point(x2, y2, z2));
	}

	std::sort(ALL(lines));
	
	for (Line l : lines)
	{
		int highestZ = 0;
		std::set<int> support;
		for (int i = 0; i < settled.size(); ++i)
			if (settled[i].Intersects(l))
			{
				if (settled[i].b.z > highestZ)
				{
					highestZ = settled[i].b.z;
					support = { i };
				}
				else if (settled[i].b.z == highestZ)
					support.insert(i);
			}

		int delta = l.a.z - highestZ-1;
		l.Drop(delta);
		for (int i : support)
			settled[i].supports.insert(settled.size());
		l.supportedBy = support;
		settled.push_back(l);
		safe.push_back(1);
	}
	
	for (int i = 0; i < settled.size(); ++i)
		if (settled[i].supportedBy.size() == 1)
			safe[*settled[i].supportedBy.cbegin()] = 0;

	for (int i = 0; i < settled.size(); ++i)
	{
		std::set<int> falling = { i }, toEvaluate = settled[i].supports;
		int toRemove = -1;
		while (true)
		{
			for (int j : toEvaluate)
				if (int count = std::count_if(ALLc(settled[j].supportedBy), [&falling](int a) { return falling.find(a) != falling.cend(); }); 
					count == settled[j].supportedBy.size())
				{
					toRemove = j;
					break;
				}
			
			if (toRemove >= 0)
			{
				toEvaluate.erase(toRemove);
				falling.insert(toRemove);
				for (int j : settled[toRemove].supports)
					toEvaluate.insert(j);
				toRemove = -1;
			}
			else break;
		}
	
		part2 += falling.size() - 1;
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", std::accumulate(ALLc(safe), 0), part2);
}
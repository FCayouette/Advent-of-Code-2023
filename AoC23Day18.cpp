import std.core;

using i64 = long long;

template<typename T>
struct Coord
{
	constexpr Coord(T X = 0, T Y = 0) : x(X), y(Y) {}
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr Coord operator * (T scalar) const { return Coord(x * scalar, y * scalar); }
	T x, y;
};

using Point = Coord<i64>;

constexpr std::array<char, 4> coords = { 'D', 'R', 'U', 'L' };
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0, -1) };
constexpr std::array<Point, 4> digDir = { Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0) };

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day18.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	i64 part1 = 0, part2 = 0, trench = 0, longTrench = 0;
	std::string line;
	char d;
	i64 length;

	Point s(0, 0), s2(0, 0);

	while (in >> d >> length >> line)
	{
		int dir = std::distance(coords.cbegin(), std::find(coords.cbegin(), coords.cend(), d));
		Point e = s + (directions[dir]*length);		
		part1 += s.x * e.y - s.y * e.x;
		trench += length;
		s = e;
		
		i64 edgeLength = stoi(line.substr(2, 5), nullptr, 16);
		Point e2 = s2 + (digDir[line[7] - '0'] * edgeLength);
		part2 += s2.x * e2.y - s2.y * e2.x;
		longTrench += edgeLength;
		s2 = e2;
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", std::abs(part1) / 2 + trench / 2 + 1, std::abs(part2) / 2 + longTrench / 2 + 1);
}
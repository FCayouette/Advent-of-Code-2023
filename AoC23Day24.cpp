import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()

using i64 = long long;
template<typename T>
struct Coord3D
{
	constexpr Coord3D(T X = 0, T Y = 0, T Z = 0) : x(X), y(Y), z(Z) {}
	constexpr Coord3D operator + (const Coord3D& p) const { return Coord3D(x + p.x, y + p.y, z + p.z); }
	constexpr Coord3D operator - (const Coord3D& p) { return Coord3D(x - p.x, y - p.y, z - p.z); };
	constexpr Coord3D operator * (T scalar) const { return Coord3D(x * scalar, y * scalar, z * scalar); }
	T x, y, z;
};

using Point3D = Coord3D<i64>;
using PosVel = std::array<i64, 6>;

struct VelSort {
	bool operator()(const PosVel& a, const PosVel& b) const {
		if (a[SortIndex] < b[SortIndex]) return true;
		if (b[SortIndex] < a[SortIndex]) return false;
		return a < b;
	}
	static int SortIndex;
};
int VelSort::SortIndex = 0;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day24.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	i64 part1 = 0;
	std::string line;
	std::vector<Point3D> positions, velocities;
	std::vector<PosVel> dataPoints;
	while (std::getline(in, line))
	{
		Point3D p, v;
		sscanf_s(line.c_str(), "%lld, %lld, %lld @ %lld, %lld, %lld", &p.x, &p.y, &p.z, &v.x, &v.y, &v.z);

		positions.push_back(p);
		velocities.push_back(v);
		dataPoints.push_back({ p.x, p.y, p.z, v.x, v.y, v.z });
	}

	double testMin = positions.size() < 6 ? 7 : 200000000000000, testMax = positions.size() < 6 ? 27 : 400000000000000;
	
	for (i64 a = 0; a < positions.size()-1; ++a)
		for (i64 b = a + 1, pax = positions[a].x, pay = positions[a].y, vax = velocities[a].x, vay = velocities[a].y; b < positions.size(); ++b)
		{
			i64 pbx = positions[b].x, pby = positions[b].y, vbx = velocities[b].x, vby = velocities[b].y;
			double cx = pax - pbx, ax = -vax, bx = vbx;
			double cy = pay - pby, ay = -vay, by = vby;
			double determinant = ax * by  - ay * bx;
			if (std::abs(determinant) > 0.00001)
				if (double x = (cx * by - cy * bx) / determinant, y = (ax * cy - ay * cx) / determinant; 
					x >= 0 && y >= 0)
					if (double ix = pax + x * vax, iy = pay + x * vay; 
						ix >= testMin && ix <= testMax && iy >= testMin && iy <= testMax)
						++part1;
		}

	std::array<i64, 3> hailStoneSpeed = {-3,1,2};
	if (positions.size() > 10)
		for (int axis = 0; axis < 3; ++axis)
		{
			VelSort::SortIndex = 3 + axis;
			std::sort(ALL(dataPoints), VelSort());

			std::vector<std::pair<i64, i64>> constraints;
			for (int i = 1; i < dataPoints.size(); ++i)
				if (dataPoints[i - 1][VelSort::SortIndex] == dataPoints[i][VelSort::SortIndex])
					constraints.push_back({ dataPoints[i][VelSort::SortIndex], dataPoints[i][VelSort::SortIndex - 3] - dataPoints[i - 1][VelSort::SortIndex - 3] });

			int i = 0;
			while (true)
			{
				if (int x = (i + 1) / 2 * (i % 2 == 0 ? 1 : -1); 
					std::all_of(ALLc(constraints), [x](const std::pair<i64, i64>& p) { return (x - p.first) != 0 && p.second % (x - p.first) == 0; }))
				{
					hailStoneSpeed[axis] = x;
					break;
				}
				++i;
			}
		}

	// intersect first 2 in hailstone frame of reference
	i64 a = hailStoneSpeed[0] - velocities[0].x, b = hailStoneSpeed[0] - velocities[1].x;
	i64 c = hailStoneSpeed[1] - velocities[0].y, d = hailStoneSpeed[1] - velocities[1].y;
	i64 x = (b*(positions[1].y - positions[0].y) - (positions[1].x - positions[0].x) * d) / (a * d - b * c);
	
	Point3D origin = positions[0] + (velocities[0]- Point3D(hailStoneSpeed[0], hailStoneSpeed[1], hailStoneSpeed[2]))*x;
	
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, origin.x + origin.y + origin.z);
}
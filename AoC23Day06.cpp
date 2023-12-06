import std.core;

using u64 = unsigned long long;

// From : https://stackoverflow.com/questions/4930307/fastest-way-to-get-the-integer-part-of-sqrtn
// Upgrade to C++20 and to accept signed integers
template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
[[nodiscard]] constexpr T sqrti(T n)
{
	if (n < 2) return n;
	unsigned char shift = 0;
	if constexpr (std::is_unsigned_v<T>)
		shift = std::bit_width(n);
	else
	{
		T work = n;
		do 
		{
			++shift;
			work >>= 1;
		} while (work);
	}
	shift += shift & 1;
	T result = 0;
	do
	{
		shift -= 2;
		result = (result << 1) | 1;
		result ^= result * result > (n >> shift);
	} while (shift);

	return result;
}

constexpr u64 Solve(u64 b, u64 c)
{
	u64 delta = b * b - (c << 2), s = sqrti(delta), result = b - s + 1;
	if (s*s == delta) result += 2;
	return result & (std::numeric_limits<u64>::max() - 1);
}

std::string GetLong(const std::string& s)
{
	std::string r;
	for (char c : s)
		if (std::isdigit(c))
			r += c;
	return r;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day06.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	u64 v, part1 = 1, count = 0;
	std::string line;
	in >> line;
	std::getline(in, line);
	std::string longTime = GetLong(line);
	std::stringstream timestream(line);
	std::vector<u64> time;
	while (timestream >> v)
		time.push_back(v);
	in >> line;
	std::getline(in, line);
	std::string longDist = GetLong(line);
	std::stringstream diststream(line);
	while (diststream >> v)
	{
		part1 *= time[count] + 1 - Solve(time[count], v);
		++count;
	}
	
	u64 t = stoull(longTime);
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, t + 1 - Solve(t, stoull(longDist)));
}
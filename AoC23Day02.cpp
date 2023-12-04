import std.core;

using u64 = unsigned long long;
using Game = std::array<int, 3>;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day02.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	u64 part1 = 0, part2 = 0, val;
	std::string color;

	while (in >> color >> color)
	{
		int num = stoi(color.substr(0, color.size() - 1));
		Game game = { 0,0,0 };
		do
		{
			Game run = { 0,0,0 };
			do
			{
				in >> val >> color;
				switch (color[0]) {
				case 'r': run[0] = val; break;
				case 'g': run[1] = val; break;
				default:  run[2] = val;
				}
			} while (color.back() == ',');
			for (int i = 0; i < 3; ++i)
				game[i] = std::max(game[i], run[i]);
		} while (color.back() == ';');

		part2 += std::accumulate(game.cbegin(), game.cend(), 1, std::multiplies<>());
		if (game[0] <= 12 && game[1] <= 13 && game[2] <= 14)
			part1 += num;
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
import std.core;

#define ALL(x) (x).begin(),(x).end()

using u64 = unsigned long long;
using HandValue = std::array<char, 7>;
using Histogram = std::array<int, 13>;
using Game = std::vector<std::pair<HandValue, u64>>;

Histogram GetHistogram(const HandValue& hv)
{
	Histogram h;
	h.fill(0);
	for (int i = 0; i < h.size(); ++i)
		h[i] = std::count(hv.cbegin() + 2, hv.cend(), i);
	return h;
}

void GetHandScore(Histogram& h, char& a, char& b)
{
	std::sort(ALL(h), std::greater<>());
	a = h[0]; b = h[1];
}

HandValue ToHandValue(const std::string& s)
{
	HandValue hv;
	for (int i = 0; i < 5; ++i)
	{
		if (char c = s[i];
			std::isdigit(c))
			hv[i + 2] = c - '2';
		else switch(c)
		{
		case 'T': hv[i + 2] = 8; break;
		case 'J': hv[i + 2] = 9; break;
		case 'Q': hv[i + 2] = 10; break;
		case 'K': hv[i + 2] = 11; break;
		case 'A': hv[i + 2] = 12; break;
		}
	}
	Histogram h = GetHistogram(hv);
	GetHandScore(h, hv[0], hv[1]);
	return hv;
}

HandValue ToHandValueJokers(const std::string& s)
{
	HandValue hv;
	for (int i = 0; i < 5; ++i)
	{
		if (char c = s[i];
			std::isdigit(c))
			hv[i + 2] = c - '1';
		else switch (c)
		{
		case 'T': hv[i + 2] = 9; break;
		case 'J': hv[i + 2] = 0; break;
		case 'Q': hv[i + 2] = 10; break;
		case 'K': hv[i + 2] = 11; break;
		case 'A': hv[i + 2] = 12; break;
		}
	}

	Histogram h = GetHistogram(hv);
	int jokers = h[0];
	h[0] = 0;
	GetHandScore(h, hv[0], hv[1]);
	hv[0] += jokers;
	return hv;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day07.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	Game game1, game2;
	std::string hand;
	u64 bet;

	while (in >> hand >> bet)
	{
		game1.push_back(std::make_pair(ToHandValue(hand), bet));
		game2.push_back(std::make_pair(ToHandValueJokers(hand), bet));
	}

	auto GameScore = [](Game& g)
	{
		std::sort(ALL(g), std::greater<>());
		u64 count = g.size(), score = 0;
		for (const auto& [_, v] : g)
			score += v * count--;
		return score;
	};
	
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", GameScore(game1), GameScore(game2));
}
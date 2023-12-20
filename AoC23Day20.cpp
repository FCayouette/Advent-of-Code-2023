import std.core;

#define ALL(x) (x).begin(),(x).end()
#define ALLc(x) (x).cbegin(),(x).cend()
using u64 = unsigned long long;

enum class ModuleType : char
{
	None,
	FlipFlop,
	Conjunction
};

struct Module
{
	ModuleType type = ModuleType::None;
	
	bool flipflopOn;
	std::vector<std::string> destinations;
	std::map<std::string, bool> inputs;
};

struct Pulse
{
	std::string from, to;
	bool high;
};

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day20.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	std::string line, rxInput;
	std::map<std::string, Module> modules;
	std::vector<std::string> conjuctions;
	while (in >> line)
	{
		std::string name = line, out;
		in >> line;
		std::getline(in, line);
		std::stringstream ss(line);

		Module m;

		while (std::getline(ss, out, ','))
			m.destinations.push_back(out.substr(1));
		
		if (name[0] == '%')
		{
			name = name.substr(1);
			m.type = ModuleType::FlipFlop;
			m.flipflopOn = false;
		}
		else if (name[0] == '&')
		{
			name = name.substr(1);
			m.type = ModuleType::Conjunction;
			conjuctions.push_back(name);
		}
		if (std::find(ALLc(m.destinations), "rx") != m.destinations.cend())
			rxInput = name;
		modules[name] = m;
	}

	// Process conjunctions
	std::sort(ALL(conjuctions));
	for (auto& [name, m] : modules)
		for (const std::string& s : m.destinations)
			if (std::binary_search(ALLc(conjuctions), s))
				modules[s].inputs[name] = false;
	
	u64 part1 = 0, part2 = 1, lows = 0, highs = 0, pulseCount = 0;
	std::deque<Pulse> pulseQueue;
	std::map<std::string, u64> search;
	for (auto& [input, _] : modules[rxInput].inputs)
		search[input] = 0;
	
	auto DoPulse = [&]() ->void {
		pulseQueue.emplace_back("button", "broadcaster", false); // false low, true high
		while (!pulseQueue.empty())
		{
			Pulse& p = pulseQueue.front();
			if (p.high)
				++highs;
			else
				++lows;
			if (auto iter = modules.find(p.to);
				iter != modules.cend())
			{
				Module& m = iter->second;
				switch (m.type)
				{
				case ModuleType::None:
					for (auto& d : m.destinations)
						pulseQueue.emplace_back(p.to, d, p.high);
					break;
				case ModuleType::FlipFlop:
					if (!p.high)
					{
						m.flipflopOn = !m.flipflopOn;
						for (auto& d : m.destinations)
							pulseQueue.emplace_back(p.to, d, m.flipflopOn);
					}
					break;
				default:
				{
					if (auto iter = m.inputs.find(p.from);
						iter != m.inputs.cend())
						iter->second = p.high;
					
					bool allHigh = true;
					for (auto& [_, high] : m.inputs)
						allHigh = allHigh && high;

					if (!allHigh)
						if (auto iter = search.find(p.to);
							iter != search.cend() && iter->second == 0)
						iter->second = pulseCount;
					
					for (auto& d : m.destinations)
						pulseQueue.emplace_back(p.to, d, !allHigh);
				}
				}
			}
			pulseQueue.pop_front();
		}
	};

	bool hasZero;
	do 
	{
		++pulseCount;
		DoPulse();
		if (pulseCount == 1000)
			part1 = lows * highs;
		
		hasZero = false;
		for (auto& [_, cycle] : search)
			hasZero = hasZero || !cycle;
	} while (hasZero);
	
	for (auto& [_, cycle] : search)
		part2 = std::lcm(part2, cycle);

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
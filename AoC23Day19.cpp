import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()

using i64 = long long;
using Part = std::array<int, 4>;
using Range = std::pair<i64, i64>;
using ValueRange = std::array<Range, 4>;

constexpr std::array<char, 4> ruleIndex{ 'x','m','a','s' };

struct Rule
{
	int index, value;
	bool less;
	std::string target;

	bool Evaluate(const Part& p) const { return less ? p[index] < value : p[index] > value; }
};

struct WorkFlow
{
	std::vector<Rule> rules;
	std::string allFalse;

	void ProcessRule(const std::string s)
	{
		std::stringstream ss(s);
		std::string line;
		while (std::getline(ss, line, ','))
		{
			auto p = line.find(':');
			if (p == std::string::npos)
			{
				allFalse = line;
				break;
			}
			Rule r;
			r.target = line.substr(p + 1);
			line = line.substr(0, p);
			
			r.index = std::distance(ruleIndex.cbegin(), std::find(ALLc(ruleIndex), line[0]));
			r.less = line[1] == '<';
			r.value = stoi(line.substr(2));

			rules.push_back(r);
		}
	}

	const std::string& GetNext(const Part& p) const
	{
		for (const auto& r : rules)
			if (r.Evaluate(p))
				return r.target;
		return allFalse;
	}	
};

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
		std::cout << "Usage: AoC23Day19.exe inputFilename\n";
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
	std::map<std::string, WorkFlow> workflows;

	while (std::getline(in, line))
	{
		if (line.empty())
			break;

		WorkFlow w;
		auto p = line.find('{');
		std::string name = line.substr(0, p);
		line = line.substr(p+1, line.size() - p - 2);
		w.ProcessRule(line);
		workflows[name] = w;
	}

	while (std::getline(in, line))
	{
		line = line.substr(1, line.size() - 2);
		std::stringstream ss(line);
		Part p = { 0,0,0,0 };
		for (int& i: p)
		{
			std::string s;
			std::getline(ss, s, ',');
			i = stoi(s.substr(2));
		}

		std::string at = "in";
		while (at != "A" && at != "R")
			at = workflows.find(at)->second.GetNext(p);
		
		if (at == "A")
			for (int i : p)
				part1 += i;
	}

	auto GetCombinations = y_combinator([&workflows](auto GetCombinations, const std::string& ruleName, ValueRange vr) -> i64
		{
			if (ruleName == "R")
				return 0;
			if (ruleName == "A")
				return std::accumulate(ALLc(vr), 1ll, [](i64 a, const Range& r) {return a * (r.second - r.first + 1); });
			
			i64 result = 0;
			const WorkFlow& w = workflows.find(ruleName)->second;

			for (const Rule& r : w.rules)
			{
				ValueRange split = vr;
				if (r.less)
				{
					split[r.index].second = r.value - 1;
					vr[r.index].first = r.value;
					result += GetCombinations(r.target, split);
				}
				else
				{
					split[r.index].first = r.value + 1;
					vr[r.index].second = r.value;
					result += GetCombinations(r.target, split);
				}
			}

			return result + GetCombinations(w.allFalse, vr);
		}
	);

	constexpr ValueRange initial = { std::make_pair(1ll, 4000ll), std::make_pair(1ll, 4000ll), std::make_pair(1ll, 4000ll), std::make_pair(1ll, 4000ll)};
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, GetCombinations("in", initial));
}
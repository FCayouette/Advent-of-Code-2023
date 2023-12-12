import std.core;

#define ALLc(x) (x).cbegin(),(x).cend()
using u64 = unsigned long long;

template <typename T>
class y_combinator {
	T lambda;
public:
	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}
	template <typename...Args>
	constexpr decltype(auto) operator()(Args&&...args) const { return lambda(std::move(*this), std::forward<Args>(args)...); }
};

u64 MemoizedSolution(std::string_view sv, const std::vector<int>& num)
{
	std::vector<std::vector<std::vector<u64>>> dynamic(sv.size(), std::vector<std::vector<u64>>(num.size() + 1, std::vector<u64>(*std::max_element(ALLc(num)) + 1, -1)));

	return y_combinator([&](auto Process, int stringIndex, int valueIndex, int numberLength) -> u64 
		{
			if (stringIndex >= sv.size()) //reached end of string
				return valueIndex == num.size();
			if (dynamic[stringIndex][valueIndex][numberLength] != -1) // already seen case?
				return dynamic[stringIndex][valueIndex][numberLength];
			u64 result = 0;

			if (!numberLength && sv[stringIndex] != '#') // Can skip over current character?
				result += Process(stringIndex + 1, valueIndex, 0); // check next string position
			if (sv[stringIndex] != '.' && valueIndex < num.size()) // match current number
			{
				if (numberLength + 1 == num[valueIndex]) 
				{
					// Completed number
					if (stringIndex + 1 == sv.size() || sv[stringIndex + 1] != '#') //can stop at position?
						result += Process(stringIndex + 2, valueIndex + 1, 0); // found, check for next
				}
				else //Continue checking for current number
					result += Process(stringIndex + 1, valueIndex, numberLength + 1);
			}
			return dynamic[stringIndex][valueIndex][numberLength] = result;
		}) (0, 0, 0);
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: AoC23Day12.exe inputFilename\n";
		return -1;
	}
	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open inputFilename {}\n", argv[1]);
		return -1;
	}

	u64 part1 = 0, part2 = 0;
	std::string line, numbers;

	while (in >> line >> numbers)
	{
		std::vector<int> num, longnum;
		auto pos = numbers.find(',');
		while (pos != std::string::npos)
		{
			num.push_back(stoi(numbers.substr(0, pos)));
			numbers = numbers.substr(pos + 1);
			pos = numbers.find(',');
		}
		num.push_back(stoi(numbers));
		
		part1 += MemoizedSolution(line, num);

		std::string longline = line + '?' + line + '?' + line + '?' + line + '?' + line;
		for (int i = 0; i < 5; ++i)
			longnum.insert(longnum.cend(), ALLc(num));
		
		part2 += MemoizedSolution(longline, longnum);
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
}
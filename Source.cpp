#include "Header.h"
#include<iostream>

using namespace hanoi;
int main()
{
	solveHillClimbing(3, 3);

	//for (auto& it : solMoves)
	//{
	//	std::cout << it.first << " " << it.second << "\n";
	//	std::cin.get();
	//}
	std::cout << "\n\nSOLUTION\n\n";

	for (int i = 0; i < st.size(); ++i)
	{
		State state = *st[i];
		std::vector<std::stack<int> > lol = state.a;
		for (int i = 0; i < lol.size(); ++i)
		{
			std::cout << "stack " << i + 1 <<": ";
			std::stack<int> s = lol[i];
			std::vector<int> sol;
			while (!s.empty())
			{
				sol.push_back(s.top());
				s.pop();
			}

			std::reverse(sol.begin(), sol.end());
			for (auto it : sol)
			{
				std::cout << it << " ";
			}
			std::cout << "\n";
		}
		std::cout << "\n\n";
		std::cin.get();
	}

	return 0;
}
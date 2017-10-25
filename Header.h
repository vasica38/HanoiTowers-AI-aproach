#pragma once
#include<iostream>
#include<vector>
#include<stack>
#include<queue>

#include<math.h>
#include<time.h>


namespace hanoi {

	typedef struct State {
		std::vector<std::stack<int> > a; /// arragement
		unsigned int nrOfTowers;
		unsigned int nrOfDisks; 

		State(int nrOfTowers, int nrOfDisks, std::vector<std::stack<int> >& a)
		{
			this->nrOfDisks = nrOfDisks;
			this->nrOfTowers = nrOfTowers;
			this->a = a;
		}

		State(int nrOfTowers, int nrOfDisks)
		{
			this->nrOfDisks = nrOfDisks;
			this->nrOfTowers = nrOfTowers;
			this->a.resize(nrOfDisks);
			std::stack<int> st;
			for (int i = nrOfDisks; i >= 1; --i)
			{
				st.push(i);
			}
			this->a[0] = st;
		}

		bool isValid(int from, int where)
		{
			int x{ -1 };
			int y{ -1 };
			if (a[from].size() > 0) {
				x = a[from].top();
				if (!a[where].size() || a[where].top() > x)
				{
					return true;
				}
			}
			return false;
		}

		State* newMove(int from, int where)
		{
			if (isValid(from, where))
			{
				State* newState = new State(this->nrOfTowers, this->nrOfDisks, this->a);
				int x{ newState->a[from].top() };
				newState->a[from].pop();
				newState->a[where].push(x);
				return newState;
			}

			return nullptr;
		}

		bool isDone()
		{
			if (a[a.size() - 1].size() == nrOfDisks)
			{
				return true;
			}

			return false;
		}

		bool operator==(State& state)
		{
			if (state.a.size() != this->a.size())
			{
				return false;
			}

			for (int i = 0; i < this->a.size(); ++i)
			{
				if (this->a[i] != state.a[i])
				{
					return false;
				}
			}

			return true;

		}
	};

	std::vector<State*> st;
	std::vector<std::pair<int, int> > allMoves;
	std::vector<std::pair<int, int> > solMoves;
	std::vector<std::vector< std::pair<int, State*> > > g;

	bool isGoodRecursive{ true };

	void bt(int k, State* currentState)
	{
		if (currentState->isDone())
		{
			isGoodRecursive = false;
		}

		for (int i = 0; i < allMoves.size(); ++i)
		{
			int x{ allMoves[i].first };
			int y{ allMoves[i].second };
			State* newState = currentState->newMove(x, y);
			if (!newState) continue;
			bool isOk{ true };
			for (int j = 0; j < st.size(); ++j)
			{
				if (*st[j] == *newState)
				{
					isOk = false;
					break;
				}
			}

			if (isGoodRecursive) {

				if (isOk)
				{
					st.push_back(newState);
					solMoves.push_back({ x, y });
					bt(k + 1, newState);
				}
			}
		}
	}


	void solverBt(int nrOfTowers, int nrOfTiles)
	{
		State* startState = new State(nrOfTowers, nrOfTiles);
		st.push_back(startState);

		for (int i = 0; i < nrOfTowers; ++i)
		{
			for (int j = 0; j < nrOfTowers; ++j)
			{
				if (i != j)
				{
					allMoves.push_back({ i, j });
				}
			}
		}

		bt(0, startState);
	}

	int fitness(State* state)
	{
		int score{ 0 };
		for (int i = 0; i < state->a.size(); ++i)
		{
			score += state->a[i].size() * i;
		}

		return score;
	}

	std::vector<State*> getNeighbours(State* state)
	{
		std::vector<State*> v;
		int nrOfTowers = state->nrOfTowers;
		int nrOfDisk = state->nrOfDisks;
		for (int i = 0; i < nrOfTowers; ++i)
		{
			for (int j = 0; j < nrOfTowers; ++j)
			{
				if (i == j) continue;
				if (state->isValid(i, j))
				{
					State* newState = state->newMove(i, j);
					if (newState) {
						v.push_back(newState);
					}
				}
			}
		}

		return v;
	}

	int sumFromStack(std::stack<int> st)
	{
		int s{ 0 };
		while (st.size())
		{
			s += st.top();
			st.pop();
		}

		return s;
	}

	double functionOne(State* state)
	{
		double pow = state->nrOfDisks ;
		double sol{ 0 };

		for (int i = 0; i < state->a.size(); ++i)
		{
			sol = sol * pow + sumFromStack(state->a[i]);
		}

		return sol;
	}


	double functioneTwo(State* state)
	{
		double sol{ 0 };
		for (int i = 0; i < state->a.size(); ++i)
		{
			sol += sumFromStack(state->a[i]) * (state->a.size() - i);
		}

		return sol;
	}

	double function(State* state1)
	{
		return functionOne(state1) + functioneTwo(state1);
	}

	void printState(State* state)
	{
		for (int i = 0; i < state->a.size(); ++i) {
			std::cout << "stack " << i + 1 << ": ";
			std::stack<int> s = state->a[i];
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

		std::cout << "\n\n\n";
	}

	void generateGraph(int nrOfTowers, int nrOfTiles)
	{
		State* startState = new State(nrOfTowers, nrOfTiles);
		g.resize(100000);
		std::queue<std::pair<int, State*> > q;
		q.push({ 1, startState });
		g[1].push_back({ 1, startState });
		int nr{ 1 };
		bool isDone{ false };

		while (!q.empty() && !isDone)
		{
			int x{ q.front().first };
			State* currState{q.front().second};
			printState(currState);
			q.pop();
			std::vector<State*> v{ getNeighbours(currState) };
			
			State* toSave{ nullptr };
			double mx{ 0.0 };
			int idx{ 0 };
			std::sort(v.begin(), v.end(), [](State* s1, State* s2)->bool 
			{
				return function(s1) > function(s2);
			});
			for (int i = 0; i < v.size(); ++i)
			{
				g[x].push_back({ ++nr, v[i] });
				
				if (function(v[i]) > mx) {
					mx = function(v[i]);
					toSave = v[i];
					idx = nr;
				}

				if (v[i]->isDone())
				{
					isDone = true;
					printState(v[i]);
				}

				q.push({ nr, v[i] });
			}
		}

	}

	void solveHillClimbing(int nrOfTowers, int nrOfTiles)
	{
		State* startState = new State(nrOfTowers, nrOfTiles);
		st.push_back(startState);

		while (true)
		{
			State* top = st[st.size() - 1];
			std::vector<State*> neighbours = getNeighbours(top);

			State* mx{ nullptr };
			int currFitness{ fitness(top) };
			int value{ 0 };
			for (int i = 0; i < neighbours.size(); ++i)
			{
				State* nState = neighbours[i];
				int x{ fitness(nState) };
				if (x > value)
				{
					value = x;
					mx = neighbours[i];
				}
			}

			if (mx && value >= currFitness - 1)
			{
				st.push_back(mx);

				printState(mx);

				if (mx->isDone())
				{
					break;
				}
			}

		}
	}

	void solverRandom(int nrOfTowers, int nrOfTiles)
	{
		State* startState = new State(nrOfTowers, nrOfTiles);
		st.push_back(startState);

		bool ok{ true };
		srand(time(NULL));
		while (ok)
		{
			int x{ rand() % nrOfTowers };
			int y{ rand() % nrOfTowers };
			if (x == y) continue;

			State* newState = st[st.size()-1]->newMove(x, y);
			if (!newState) continue;

			//bool isOk{ true };
			//for (int i = 0; i < st.size(); ++i)
			//{
			//	if (*st[i] == *newState)
			//	{
			//		isOk = false;
			//		break;
			//	}
			//}

			if (true)
			{
				st.push_back(newState);
				printState(newState);
			}
		}
	}


} // namespace hanoi
#include <queue>
#include <set>
#include <vector>

using namespace std;

class LowestCommonAncestor
{
public:
	static const int maxn = 1000000;
	static const int maxh = 20;

	int n;
	vector<int> E[maxn];
	int dph[maxn];
	int dad[maxn];
	int gpaf[maxn][maxh];

	LowestCommonAncestor(void) : n(0)
	{

	}

	LowestCommonAncestor(const int & n) : n(n)
	{

	}

	void resize(const int & n)
	{
		for (int i = 0; i < this->n; i++)
			E[i].clear();
		this->n = n;
	}

	void add_edge(const int & u, const int & v)
	{
		E[u].push_back(v);
		E[v].push_back(u);
	}

	void build(const int & root = 0)
	{
		memset(dph, 0x7f, sizeof dph[0] * n);
		dad[root] = -1;
		dph[root] = 0;
		queue<int> que;
		for (que.push(root); !que.empty(); que.pop())
		{
			int u = que.front();
			for (const auto & v : E[u])
			{
				if (dph[v] > dph[u] + 1)
				{
					dad[v] = u;
					dph[v] = dph[u] + 1;
					que.push(v);
				}
			}
		}
		for (int i = 0; i < n; i++)
			gpaf[i][0] = dad[i];
		for (int k = 1; k < maxh; k++)
		{
			for (int i = 0; i < n; i++)
				gpaf[i][k] = dph[i] >= (1 << k) ? gpaf[gpaf[i][k - 1]][k - 1] : -1;
		}
	}

	int gpa(int u, int h) const
	{
		for (int i = 0; h > 0; h >>= 1, i++)
		{
			if (h & 1)
				u = gpaf[u][i];
		}
		return u;
	}

	int lca(int u, int v) const
	{
		if (dph[u] > dph[v])
			u = gpa(u, dph[u] - dph[v]);
		else if (dph[u] < dph[v])
			v = gpa(v, dph[v] - dph[u]);
		if (u == v)
			return u;
		for (int k = maxh - 1; k >= 0; k--)
		{
			if (gpaf[u][k] != gpaf[v][k])
			{
				u = gpaf[u][k];
				v = gpaf[v][k];
			}
		}
		return dad[u];
	}

	int distance(const int & u, const int & v) const
	{
		return dph[u] + dph[v] - dph[lca(u, v)] * 2;
	}

	bool on_path(const int & x, const int & u, const int & v)
	{
		int l = lca(u, v);
		return dph[l] <= dph[x] && dph[x] <= dph[u] && gpa(u, dph[u] - dph[x]) == x ||
			   dph[l] <= dph[x] && dph[x] <= dph[v] && gpa(v, dph[v] - dph[x]) == x;
	}

	pair<int, int> intersection(const int & u, const int & v, const int & x, const int & y)
	{
		set<int> s;
		s.insert(u);
		s.insert(v);
		s.insert(x);
		s.insert(y);
		s.insert(lca(u, v));
		s.insert(lca(x, y));
		s.insert(lca(u, x));
		s.insert(lca(u, y));
		s.insert(lca(v, x));
		s.insert(lca(v, y));
		vector<int> vtx;
		for (const auto & p : s)
		{
			if (on_path(p, u, v) && on_path(p, x, y))
				vtx.push_back(p);
		}
		if (vtx.empty())
			return make_pair(-1, -1);
		vector<int> d;
		d.resize(vtx.size());
		for (int i = 0; i < d.size(); i++)
			d[i] = distance(u, vtx[i]);
		return make_pair(vtx[min_element(d.begin(), d.end()) - d.begin()], vtx[max_element(d.begin(), d.end()) - d.begin()]);
	}
};

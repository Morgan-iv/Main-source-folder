// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end

#include<iostream>
#include<string>
#include<vector>

using namespace std;

struct node
{
	int rev;
	string value;
	vector<node *> childs;
	node(int _rev = 0, string _value = "") :
		rev(_rev), value(_value), childs() { }
};

typedef node* tree;

void reverse(tree t)
{
	if (!t->childs.empty())
	{
		tree tmp;
		int sz = t->childs.size();
		for (int i = 0; i < sz / 2; ++i)
		{
			tmp = t->childs[i];
			t->childs[i] = t->childs[sz - i - 1];
			t->childs[sz - i - 1] = tmp;
		}
		for (int i = 0; i < sz; ++i)
			reverse(t->childs[i]);
	}
	else
		t->rev = !(t->rev);
}

int revstr(string & str)
{
	int l = str.length();
	char c;
	for (int i = 0; i < l / 2; ++i)
	{
		c = str[i];
		str[i] = str[l - i - 1];
		str[l - i - 1] = c;
	}
	return 0;
}

tree readtree(string & str, int & i, int n)
{
	tree res = new node();
	if (str[++i] == '(')
	{
		tree tmp = readtree(str, i, n);
		while (str[i++] == ',')
		{
			res->childs.push_back(tmp);
			tmp = readtree(str, i, n);
		}
		res->childs.push_back(tmp);
		if (i < n && str[i] == 'R') ++i, reverse(res);
	}
	else
	{
		while (str[i] != ')')
			res->value += str[i++];
		if (++i < n && str[i] == 'R') ++i, reverse(res);
	}
	return res;
}

string comp(tree t)
{
	if (!t->childs.empty())
	{
		string res = "";
		int sz = t->childs.size();
		for (int i = 0; i < sz; ++i)
			res += comp(t->childs[i]);
		return res;
	}
	else
		return ((t->rev) ? revstr(t->value) : 0), (t->value);
}

int main()
{
	string str;
	int i = 0;
	tree t;
	cin >> str;
	t = readtree(str, i, str.length());
	cout << comp(t);
	return 0;
}

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings

// PVS-settings end

#include<iostream>
#include<map>
#include<string>

using namespace std;

int main()
{
	int n;
	string act, user, num;
	cin >> n;
	map<string, string> users;
	for (int i = 0; i < n; ++i)
	{
		cin >> act >> user;
		if (act[0] == 'A')
		{
			cin >> num;
			if (users.find(user) != users.end())
				cout << "ERROR\n";
			else
				users[user] = num;
		}
		if (act[0] == 'D')
		{
			if (users.find(user) != users.end())
				users.erase(user);
			else
				cout << "ERROR\n";
		}
		if (act[0] == 'E')
		{
			cin >> num;
			if (users.find(user) != users.end())
				users[user] = num;
			else
				cout << "ERROR\n";
		}
		if (act[0] == 'P')
		{
			if (users.find(user) != users.end())
				cout << user << ' ' << users[user] << '\n';
			else
				cout << "ERROR\n";
		}
	}
	return 0;
}
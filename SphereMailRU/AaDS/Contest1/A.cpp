#include<iostream>
#include<stack>

using namespace std;

int main()
{
	char a;
	stack<char> st;
	while (cin >> a)
	{
		if (st.empty()) st.push(a);
		else if (st.top() == a) st.push(a);
		else st.pop();
	}
	cout << ((st.empty()) ? "YES" : "NO");
	return 0;
}
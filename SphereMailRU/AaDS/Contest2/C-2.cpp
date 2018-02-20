#include<vector>
#include<string>
#include<iostream>

using namespace std;

typedef vector<unsigned int> BInt;

const unsigned base    = 0x80000000; // = 2^31 = 2147483648 = MAX_INT + 1
const unsigned basem1  = 0x7FFFFFFF; // = base - 1
const int      basestr = 1000000000; // для перевода из строки и в строку
const int      bsespow = 9         ; // = log(10, basestr)
/*
IsZero     (BInt & a)           - проверка на 0 и приведение лидирующих нулей
OpMinus    (BInt & a)           - унарный минус
OpMinusEqu (BInt & a)           - унарный минус с тем же объектом
OpPlus     (BInt & a)           - унарный плюс, возвращает копию объекта
OpPlusEqu  (BInt & a)           - унарный плюс с тем же объектом, вообще ничего не делает
OpPlus     (BInt & a, BInt & b) - бинарный плюс

*/
bool            IsZero      (BInt & a)
{
	while (a.size() > 0 && a.back() == 0) a.pop_back(); // убираем лидирующие нули
	return !(a.size());
}
BInt            OpMinus     (BInt & a)
{
	int sz = a.size();
	BInt res(sz);
	for (int i = 0; i < sz; ++i) res[i] = -a[i];
	return res;
}
void            OpMinusEqu  (BInt & a)
{
	int sz = a.size();
	for (int i = 0; i < sz; ++i) a[i] = -a[i];
}
BInt            OpPlus      (BInt & a)
{
	BInt res(a);
	return res;
}
void            OpPlusEqu   (BInt & a)
{

}
BInt            OpPlus      (BInt & a, BInt & b)
{
	int carry = 0, sgn1 = a[0] >> 31, sgn2 = b[0] >> 31;
	size_t sz1 = a.size(), sz2 = b.size(), sz = (sz1 > sz2) ? sz1 : sz2;
	BInt res(sz);
	for (size_t i = 0; i < sz; ++i)
	{
		if (i < sz1) res[i] += a[i];
		if (i < sz2) res[i] += b[i];
	}
	if (IsZero(res)) return res;
	sz = res.size();
	int sgn = (sgn1 ^ sgn2) ? (res[sz - 1] >> 31) : sgn1, nsgn = sgn ^ sgn1 ^ sgn2;
	for (size_t i = 0; i < sz; ++i)
	{
		if (nsgn) res[i] -= carry; // прибавляем или вычитаем carry в зависимости от знака
		else      res[i] += carry;
		carry = res[i] >> 31; // считаем carry как знаковый бит числа для 0,
		if (res[i] & basem1) carry ^= sgn; // если не 0 нужно сделать еще xor со знаком
		if (carry) res[i] ^= base; //Ну а тут убираем лишний разряд
	}
	IsZero(res);
	if (carry) res.push_back((sgn) ? (unsigned)-1 : 1);
	return res;
}
void            OpPlusEqu   (BInt & a, BInt & b)
{
	int carry = 0, sgn1 = a[0] >> 31, sgn2 = b[0] >> 31;
	size_t sz1 = a.size(), sz2 = b.size(), sz = (sz1 > sz2) ? sz1 : sz2;
	for (size_t i = 0; i < sz; ++i)
	{
		if (i == sz1) a.push_back(0);
		if (i < sz2) a[i] += b[i];
	}
	if (IsZero(a)) return;
	sz = a.size();
	int sgn = (sgn1 ^ sgn2) ? (a[sz - 1] >> 31) : sgn1, nsgn = sgn ^ sgn1 ^ sgn2;
	for (size_t i = 0; i < sz; ++i)
	{
		if (nsgn) a[i] -= carry; // прибавляем или вычитаем carry в зависимости от знака
		else      a[i] += carry;
		carry = a[i] >> 31; // считаем carry как знаковый бит числа для 0,
		if (a[i] & basem1) carry ^= sgn; // если не 0 нужно сделать еще xor со знаком
		if (carry) a[i] ^= base; //Ну а тут убираем лишний разряд
	}
	IsZero(a);
	if (carry) a.push_back((sgn) ? (unsigned)-1 : 1);
}
BInt            OpMulInt    (BInt & a, int    b)
{
	BInt res(a);
	int carry = 0;
	long long cur;
	for (size_t i = 0; i < res.size() || carry; ++i) {
		if (i == res.size()) res.push_back (0);
		cur = carry + res[i] * 1ll * b;
		res[i] = int (cur % base);
		carry = int (cur / base);
	}
	IsZero(res);
	return res;
}
void            OpMulIntEqu (BInt & a, int    b)
{
	int carry = 0;
	long long cur;
	for (size_t i = 0; i < a.size() || carry; ++i) {
		if (i == a.size()) a.push_back (0);
		cur = carry + a[i] * 1ll * b;
		a[i] = int (cur % base);
		carry = int (cur / base);
	}
	IsZero(a);
}
pair<BInt, int> OpDivInt    (BInt & a, int    b)
{
	int carry = 0;
	BInt res(a);
	for (int i = (int)res.size()-1; i >= 0; --i) {
		long long cur = res[i] + carry * 1ll * base;
		res[i] = int (cur / b);
		carry = int (cur % b);
	}
	IsZero(res);
	return make_pair(res, carry);
}
int             OpDivIntEqu (BInt & a, int    b)
{
	int carry = 0;
	for (int i = (int)a.size() - 1; i >= 0; --i)
	{
		long long cur = a[i] + carry * 1ll * base;
		a[i] = int(cur / b);
		carry = int(cur % b);
	}
	IsZero(a);
	return carry;
}
string          ToString    (BInt & a)
{
	BInt temp(a);
	bool sign = a[0] >> 31;
	string res = "", c; 
	while(!IsZero(temp))
	{
		c = to_string(OpDivIntEqu(temp, basestr));
		if (sign) c = c.substr(1, c.length() - 1);
		res = c + res;
		if (c.length() - bsespow) for(int i = c.length(); i < bsespow; ++i) res = "0" + res;
	}
	while (res.length() > 1 && res[0] == '0') res = res.substr(1, res.length() - 1);
	if (sign) res = "-" + res;
	return res;
}
BInt            ToBInt      (string str)
{
	BInt res(0);
	int carry, sign = 0, a;
	if (str[0] < '0' || str[0] > '9')
	{
		sign = str[0] == '-';
		str = str.substr(1, str.length() - 1);
	}
	a = str.length();
	if (a % 9) for (size_t i = 0; i < 9 - a % 9; ++i) str = "0" + str;
	for (size_t i = 0; i < str.length(); i += bsespow)
	{
		OpMulIntEqu(res, basestr);
		a = atoi(str.substr(i, bsespow).c_str());
		if (sign) a = -a;
		if (!res.size()) res.push_back(0);
		res[0] += a;
		carry = res[0] >> 31;
		if (res[0] & basem1) carry ^= sign;
		if (carry) res[0] ^= base;
		if (res.size() > 1) res[1] += (sign) ? -carry : carry;
		else res.push_back((sign) ? -carry : carry);
		IsZero(res);
	}
	return res;
}

void EMaxxPlus(BInt & a, BInt & b)
{
	int carry = 0;
	size_t sz1 = a.size(), sz2 = b.size(), sz = (sz1 > sz2) ? sz1 : sz2;
	for (size_t i = 0; i < sz || carry; ++i)
	{
		if (i == a.size())
			a.push_back(0);
		a[i] += carry + (i < b.size() ? b[i] : 0);
		carry = a[i] >= base;
		if (carry)  a[i] -= base;
	}
}

int main()
{
	int n, k;
	cin >> n;
	k = n / 2;
	BInt a = ToBInt("1"), b = ToBInt("1");
	for (int i = 0; i < k; ++i)
	{
		OpPlusEqu(a, b);
		OpPlusEqu(b, a);
	}
	cout << ((n % 2) ? ToString(b) : ToString(a));
	return 0;
}
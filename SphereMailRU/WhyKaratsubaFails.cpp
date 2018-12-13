#include<vector>
#include<string>
#include<iostream>
#include<ctime>
#include<cstdio>

using namespace std;

typedef vector<unsigned int> BInt;

const unsigned base = 0x80000000; // = 2^31 = 2147483648 = MAX_INT + 1
const unsigned basem1 = 0x7FFFFFFF; // = base - 1
const int      basestr = 1000000000; // для перевода из строки и в строку
const int      bsespow = 9; // = log(10, basestr)
/*
IsZero     (BInt & a)           - проверка на 0 и приведение лидирующих нулей
OpMinus    (BInt & a)           - унарный минус
OpMinusEqu (BInt & a)           - унарный минус с тем же объектом
OpPlus     (BInt & a)           - унарный плюс, возвращает копию объекта
OpPlusEqu  (BInt & a)           - унарный плюс с тем же объектом, вообще ничего не делает
OpPlus     (BInt & a, BInt & b) - бинарный плюс

*/
bool            IsZero     (BInt & a)
{
	while (a.size() > 0 && a.back() == 0) a.pop_back(); // убираем лидирующие нули
	return !(a.size());
}
BInt            OpMinus    (BInt & a)
{
	int sz = a.size();
	BInt res(sz);
	for (int i = 0; i < sz; ++i) res[i] = -a[i];
	return res;
}
void            OpMinusEqu (BInt & a)
{
	int sz = a.size();
	for (int i = 0; i < sz; ++i) a[i] = -a[i];
}
BInt            OpPlus     (BInt & a)
{
	BInt res(a);
	return res;
}
void            OpPlusEqu  (BInt & a)
{

}
BInt            OpPlus     (BInt & a, BInt & b)
{
	size_t sz1 = a.size(), sz2 = b.size(), sz = (sz1 > sz2) ? sz1 : sz2;
	int carry = 0, sgn1 = (sz1) ? a[sz1 - 1] >> 31 : 0, sgn2 = (sz2) ? b[sz2 - 1] >> 31 : 0;
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
void            OpPlusEqu  (BInt & a, BInt & b)
{
	size_t sz1 = a.size(), sz2 = b.size(), sz = (sz1 > sz2) ? sz1 : sz2;
	if (!sz1)
	{
		a = b;
		return;
	}
	if (!sz2)
		return;
	int carry = 0, sgn1 = a[sz1 - 1] >> 31, sgn2 = b[sz2 - 1] >> 31;
	for (size_t i = 0; i < sz; ++i)
	{
		if (i >= sz1) a.push_back(0);
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
BInt            OpMulInt   (BInt & a, int    b)
{
	BInt res(a);
	int carry = 0;
	long long cur;
	for (size_t i = 0; i < res.size() || carry; ++i) {
		if (i == res.size()) res.push_back(0);
		cur = carry + ((int) res[i]) * 1ll * b;
		res[i] = int(cur % base);
		carry = int(cur / base);
	}
	IsZero(res);
	return res;
}
void            OpMulIntEqu(BInt & a, int    b)
{
	int carry = 0;
	long long cur;
	for (size_t i = 0; i < a.size() || carry; ++i) {
		if (i == a.size()) a.push_back(0);
		cur = carry + ((int)a[i]) * 1ll * b;
		a[i] = int(cur % base);
		carry = int(cur / base);
	}
	IsZero(a);
}
pair<BInt, int> OpDivInt   (BInt & a, int    b)
{
	int carry = 0;
	BInt res(a);
	for (int i = (int)res.size() - 1; i >= 0; --i) {
		long long cur = ((int) res[i]) + carry * 1ll * base;
		res[i] = int(cur / b);
		carry = int(cur % b);
	}
	IsZero(res);
	return make_pair(res, carry);
}
int             OpDivIntEqu(BInt & a, int    b)
{
	int carry = 0;
	for (int i = (int)a.size() - 1; i >= 0; --i)
	{
		long long cur = ((int) a[i]) + carry * 1ll * base;
		a[i] = int(cur / b);
		carry = int(cur % b);
	}
	IsZero(a);
	return carry;
}
string          ToString   (BInt & a)
{
	BInt temp(a);
	if (!a.size()) return "0";
	bool sign = a[a.size() - 1] >> 31;
	string res = "", c;
	while (!IsZero(temp))
	{
		c = to_string(OpDivIntEqu(temp, basestr));
		if (sign) c = c.substr(1, c.length() - 1);
		res = c + res;
		if (c.length() - bsespow) for (int i = c.length(); i < bsespow; ++i) res = "0" + res;
	}
	while (res.length() > 1 && res[0] == '0') res = res.substr(1, res.length() - 1);
	if (sign) res = "-" + res;
	return res;
}
BInt            ToBInt     (string str)
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
BInt            OpMulNat   (BInt & a, BInt & b)
{
	size_t sz1 = a.size(), sz2 = b.size();
	BInt res(0);
	int sw = 0;
	if (sz2 > sz1)
	{
		a.swap(b);
		swap(sz1, sz2);
		sw = 1;
	}
	for (int i = ((int)sz2) - 1; i >= 0; --i)
	{
		res.insert(res.begin(), 0);
		IsZero(res);
		OpPlusEqu(res, OpMulInt(a, (int) b[i]));
	}
	IsZero(res);
	if (sw) a.swap(b);
	return res;
}
BInt            OpMul      (BInt & a, BInt & b)
{
	size_t sz1 = a.size(), sz2 = b.size();
	int sw = 0;
	if (sz1 < 33 || sz2 < 33)
	{
		return OpMulNat(a, b);
	}
	if (sz1 < sz2)
	{
		a.swap(b);
		swap(sz1, sz2);
		sw = 1;
	}
	if (sz1 / 2 >= sz2)
	{
		BInt a1(a.begin(), a.begin() + (sz1 - 1) / 2 + 1);
		BInt a2(a.begin() + (sz1 - 1) / 2 + 1, a.end());
		a1 = OpMul(a1, b);
		a2 = OpMul(a2, b);
		a2.insert(a2.begin(), (size_t) ((sz1 - 1) / 2 + 1), 0);
		OpPlusEqu(a2, a1);
		return a2;
	}
	BInt a1(a.begin(), a.begin() + (sz1 - 1) / 2 + 1);
	BInt a2(a.begin() + (sz1 - 1) / 2 + 1, a.end());
	BInt b1(b.begin(), b.begin() + (sz1 - 1) / 2 + 1);
	BInt b2(b.begin() + (sz1 - 1) / 2 + 1, b.end());
	BInt ac(OpMul(a1, b1));
	BInt bd(OpMul(a2, b2));
	OpPlusEqu(a1, a2);
	OpPlusEqu(b1, b2);
	BInt ().swap(a2);
	BInt ().swap(b2);
	BInt res = OpMul(a1, b1);
	OpPlusEqu(res, OpMinus(ac));
	OpPlusEqu(res, OpMinus(bd));
	res.insert(res.begin(), (size_t) ((sz1 - 1) / 2 + 1), 0);
	bd.insert(bd.begin(), (size_t) (sz1 + (sz1 % 2)), 0);
	OpPlusEqu(res, ac);
	OpPlusEqu(res, bd);
	return res;
}

int main()
{
	string str = "11111111111111111111111111111111";
	for (int i = 0; i < 9; ++i)
	{
		str += str;
	}
	BInt a(ToBInt(str));
	BInt b(ToBInt("-" + str));
	FILE * out = fopen("out.txt", "w");
	unsigned time1 = clock();
	string res1 = ToString(OpMulNat(a, b));
	unsigned time2 = clock();
	string res2 = ToString(OpMul(a, b));
	unsigned time3 = clock();
	fprintf(out, "%s\n%s", res1.c_str(), res2.c_str());
	cout << time2 - time1 << '\n' << time3 - time2;
	fclose(out);
	return 0;
}

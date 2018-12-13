#include<iostream>
#include<string>
#include<vector>

using namespace std;

struct BigInteger
{
	vector<int> value; // каждый элемент - цифра числа в системе счисления с основанием base,
					   //от младших к старшим, т.е. в value[0] хранится число единиц, для 0 пустой вектор.
					   //для отрицательных храним все цифры умноженными на -1
	static const int base = 1000000000;
	static const int bpow = 9;
	int sign; //хранится в формате 1 или -1, для нуля всегда 1
	BigInteger(int sign_, const vector<int>& value_);
	BigInteger() : BigInteger(1, vector<int>()) {}
	BigInteger(string str);  // по строке
	BigInteger(long long v); // по числу (маленькие и для приведения)
	size_t       Size(); // количество цифр (размер массива)
	string       ToString();
	void         Swap(BigInteger & another);
	bool         IsZero(); // еще и приводит лидирующие нули
	BigInteger   MulNative(BigInteger & another); // школьное умножение
	BigInteger   operator +  (); // унарный + не делает ничего
	BigInteger   operator -  (); // унарный -
	BigInteger   operator +  (BigInteger & another);// бинарный + работает для чисел обоих знаков
	BigInteger   operator -  (BigInteger & another);// бинарный - реализован через унарный - и бинарный +
	BigInteger & operator += (BigInteger & another);// присваивание со сложением, любой знак, без нового объекта
};

BigInteger::BigInteger(int sign_, const vector<int>& value_)
{
	sign = sign_;
	value = vector<int>(value_);
	IsZero();
}
BigInteger::BigInteger(string str)
{
	value = vector<int>();
	sign = (str[0] >= '0' && str[0] <= '9') ? 1 : (str[0] == '-') ? -1 : 1;  // определяем знак у числа в строке по 1 символу
	if (str[0] < '0' || str[0] > '9') str = str.substr(1, str.length() - 1); // и удаляем его из начала строки если есть
	for (int i = str.length(); i > 0; i -= bpow)
	{
		if (i < bpow) value.push_back(sign * atoi(str.substr(0, i).c_str()));  //разбивая по bpow цифр переводим
		else value.push_back(sign * atoi(str.substr(i - bpow, bpow).c_str()));
	}
	IsZero(); // приводим лидирующие нули
}
BigInteger::BigInteger(long long v)
{
	sign = (v >= 0) ? 1 : -1;
	value = vector<int>();
	while (v != 0)
	{
		value.push_back(v % base);
		v /= base;
	}
}
BigInteger   BigInteger::operator +  ()
{
	return *this;
}
BigInteger   BigInteger::operator -  ()
{
	BigInteger res = BigInteger();
	size_t sz = this->Size();
	res.value.reserve(sz);
	for (size_t i = 0; i < sz; ++i) res.value.push_back(-value[i]); //не забываем инвертнуть и весь массив тоже
	res.sign = -sign;
	return res;
}
BigInteger   BigInteger::operator +  (BigInteger & another)
{
	int carry = 0;
	size_t sz1 = value.size(), sz2 = another.value.size(), sz = (sz1 > sz2) ? sz1 : sz2; //определяем максимальный размер
	BigInteger res(1, vector<int>(sz));
	for (size_t i = 0; i < sz; ++i) res.value[sz] = ((i < sz1) ? value[i] : 0) + ((i < sz2) ? another.value[i] : 0); // суммируем без carry
	if (sign + another.sign) //считаем и приводим carry для одинаковых знаков
	{
		res.sign = sign;
		for (size_t i = 0; i < sz; ++i)
		{
			res.value[i] += carry * sign; //нужен res.sign, но они равны и доступ быстрее
			carry = sign * res.value[i] >= base;
			if (carry) res.value[i] -= base * sign;
		}
		if (carry) res.value.push_back(1);
	}
	else //считаем и приводим carry для разных знаков
	{
		if (res.IsZero()) return res;
		sz = res.Size();
		res.sign = (res.value[sz - 1] > 0) ? 1 : -1;
		for (size_t i = 0; i < sz; ++i)
		{
			res.value[i] -= carry * res.sign;
			carry = res.sign * res.value[i] < 0;
			if (carry) res.value[i] += base * res.sign;
		}
		res.IsZero();
	}
	return res;
}
BigInteger   BigInteger::operator -  (BigInteger & another)
{
	BigInteger res = -another;
	res = *this + res;
	return res;
}
BigInteger & BigInteger::operator += (BigInteger & another)
{
	int carry = 0;
	size_t sz2 = another.value.size(), sz = (value.size() > sz2) ? value.size() : sz2; //определяем максимальный размер
	for (size_t i = 0; i < sz; ++i) // суммируем без carry
	{
		if (i == value.size()) value.push_back(0);
		value[i] += (i < sz2) ? another.value[i] : 0;
	}
	if (sign + another.sign) //считаем и приводим carry для одинаковых знаков
	{
		for (size_t i = 0; i < sz; ++i)
		{
			value[i] += carry * sign;
			carry = sign * value[i] >= base;
			if (carry) value[i] -= base * sign;
		}
		if (carry) value.push_back(1);
	}
	else //считаем и приводим carry для разных знаков
	{
		if (IsZero()) return *this;
		sz = value.size();
		sign = (value[sz - 1] > 0) ? 1 : -1;
		for (size_t i = 0; i < sz; ++i)
		{
			value[i] -= carry * sign;
			carry = sign * value[i] < 0;
			if (carry) value[i] += base * sign;
		}
		IsZero();
	}
	return *this;
}
size_t       BigInteger::Size()
{
	return value.size();
}
bool         BigInteger::IsZero()
{
	while (value.size() > 0 && value.back() == 0) value.pop_back(); // убираем лидирующие нули
	if (value.size() == 0)
	{
		sign = 1;
		return 1;
	}
	return 0;
}
string       BigInteger::ToString()
{
	string res = (sign > 0) ? "" : "-", s;
	size_t sz = Size();
	for (int i = sz - 1; i >= 0; --i) // собираем строковое представление, не забываем про - и нули
	{
		s = to_string(sign * value[i]);
		if (i < sz - 1) for (int j = 0; j < bpow - s.length(); ++j) res += "0";
		res += s;
	}
	if (res == "") res = "0";
	return res;
}
void         BigInteger::Swap(BigInteger & another)
{
	swap(sign, another.sign);
	value.swap(another.value);
}
BigInteger   BigInteger::MulNative(BigInteger & another)
{
	if (IsZero() || another.IsZero()) return BigInteger(0);
	size_t sz1 = value.size(), sz2 = another.value.size(), sz = (sz1 * sz2); // определяем размеры
	long long cur;
	BigInteger c(1, vector<int>(sz));
	for (size_t i = 0; i < sz1; ++i)
		for (size_t j = 0, carry = 0; j < sz2 || carry; ++j)
		{
			cur = c.value[i + j] + value[i] * 1ll * (j < sz2 ? another.value[j] : 0) + carry;
			c.value[i + j] = int(cur % base);
			carry = int(cur / base);
		}
	c.IsZero();
	return c;
}

typedef BigInteger BInt;

int main()
{
	
	int n, k;
	cin >> n;
	k = n / 2;
	BInt a(1), b(1);
	unsigned int end2_time, end_time, start_time = clock();
	for (int i = 0; i < k; ++i)
	{
		a += b;
		b += a;
	}
	end_time = clock();
	cout << ((n % 2) ? b.ToString() : a.ToString());
	end2_time = clock();
	cout << '\n' << end_time - start_time << '\n' << end2_time - end_time;
	return 0;
}

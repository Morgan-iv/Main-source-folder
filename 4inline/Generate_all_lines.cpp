#include <algorithm>			//Для swapа

const int sz = 4;		//размер поля
const int msr = 3;			//Количество измерений

enum coor{
	x = 0,
	y = 1,
	z = 2
};

struct coord {
	int mas[msr];
	coord() {
		for (int i = 0; i < msr; i++) {
			mas[i] = -1;
		}
	}

	coord(int xx, int yy, int zz) {
		mas[coor.x] = xx;
		mas[coor.y] = yy;
		mas[coor.z] = zz;
	}
};

struct line {
	coord mas[sz];
	line() {
		for (int i = 0; i < sz; i++) {
			mas[i] = coord();
		}
	}

};

void AllLines(line *allL) {
	//нужна константа sz = 4
	coord c;
	int t1, t2, ind = 0, last_ind = sz - 1;
	//Генерация простых линий(т.е. по осям координат)
	for (int i = 0; i < msr; i++) {
		t1 = (i + 1) % msr;
		for (c.mas[t1] = 0; c.mas[t1] < sz; c.mas[t1]++) {
			t2 = (i + 2) % msr;
			for (c.mas[t2] = 0; c.mas[t2] < sz; c.mas[t2]++) {
				for (c.mas[i] = 0; c.mas[i] < sz; c.mas[i]++) {
					allL[ind].mas[c.mas[i]/*можно вместо этого прикрутить ещё один индекс, но мне лень(#1)*/] = c;
				}
				ind++;
			}
		}

	}
	//Генерация плоских диагоналей
	for (int i = 0; i < msr; i++) {
		for (c.mas[i] = 0; c.mas[i] < sz; c.mas[i]++) {
			t1 = (i + 1) % msr;
			t2 = (i + 2) % msr;
			for (c.mas[t1] = c.mas[t2] = 0; c.mas[t1] < sz; c.mas[t2]++, c.mas[t1]++) {
				allL[ind].mas[c.mas[t1]/*(#1)*/] = c;
			}
			ind++;
			for (c.mas[t1] = 0, c.mas[t2] = last_ind; c.mas[t1] < sz; c.mas[t2]--, c.mas[t1]++) {
				allL[ind].mas[c.mas[t1]/*(#1)*/] = c;
			}
			ind++;
		}
	}
	//Генерация главных диагоналей
	c = coord(0, 0, 0);
	for (int z[5] = { 1, 0, 0, 0, 0 }, i = 0; i < 4; i++) {						//4 - количество главных диагоналей
		for (int j = 0, l; j < msr; j++) {
			if (z[j + 1]) {
				c.mas[j] = 0;
				l = 1;
			}
			else {
				l = -1;
				c.mas[j] = last_ind;
			}
			for (int k = 0; k < sz; c.mas[j] += l, k++) {
				allL[ind].mas[k].mas[j] = c.mas[j];
			}
		}
		ind++;
		std :: swap(z[i + 1], z[i]);
	}
}

void main() {
	line allL[msr * sz * (sz + 2) + 4];
	AllLines(allL);
}
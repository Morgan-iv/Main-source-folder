/*
 * Создано в SharpDevelop.
 * Пользователь: Admin
 * Дата: 23.12.2015
 * Время: 23:20
 * 
 * Для изменения этого шаблона используйте меню "Инструменты | Параметры | Кодирование | Стандартные заголовки".
 */
using System;
using System.Collections;
using System.Collections.Generic;

namespace ШТА
{

public class Program
{
	public static void Main()
	{
		NewGame(true);
		NextTurn(0, 0, 0);
		TurnType = !TurnType;
		NextTurn(0, 0, 2);
		TurnType = !TurnType;
		NextTurn(1, 0, 0);
		TurnType = !TurnType;
		NextTurn(0, 0, 3);
		TurnType = !TurnType;
		NextTurn(2, 0, 0);
		TurnType = !TurnType;
		PlayBot();
		int p;
	}
	public static Token[,,] ChipArr;
	public Line WinLine = new Line();
	public const int sz = 4;
	public const int msr = 3;
	public const int cntlines = msr * sz * (sz + 2) + 4;
	public static bool TurnType = true;
	public static bool Player1Bot = false;
	public static bool Player2Bot = false;
	public static bool GameWin = false;
	public static bool PlayerWin = false;
	public bool ClickNeeded = true;
	public int  LastLeftCyl;
	public static Coord EurChoosed = new Coord();
	public static Line[] AllLines = new Line[cntlines];
	public static Line[] EurLines = new Line[cntlines];
	public static int[,] MaxY;
	public enum Token
	{
		space = 0,  // не занятое место
		white = -1,
		black = 1
	}
	public enum Dngr
	{
		Token_4 = 1000,
		Token_3 = 100,
		Token_2_2 = 0,
		Token_2 = 0,
		Token_1 = 1
	}
	public enum coor
	{
		x = (int)0,
		y = (int)1,
		z = (int)2
	}
	public class Coord
	{
		public int[] arr = new int[msr];
		public void Set(int xx, int yy, int zz)
		{
			arr[0] = xx;
			arr[1] = yy;
			arr[2] = zz;
		}
		public void Set(Coord c)
		{
			for (int i = 0; i < msr; ++i) arr[i] = c.arr[i];
		}
		public bool IsPole()
		{
			for (int i = 0; i < msr; i++)
				if ((arr[i] < 0) || (arr[i] >= sz))
					return false;
			return true;
		}
	}
	public class Line
	{
		public Coord[] arr = new Coord[sz];
		public Line()
		{
			for (int i = 0; i < sz; ++i) arr[i] = new Coord();
		}
		public Coord[] Get()
		{
			return arr;
		}
		public byte c_b;
		public byte c_w;
		public bool m_b;
		public bool m_w;
	}
	public static void SetToZero() {
		for (int x = 0; x < sz; ++x)
			for (int y = 0; y < sz; ++y)
				for (int z = 0; z < sz; ++z)
					ChipArr [x, y, z] = Token.space;
		return;
	}
	public static int EuristicCount()
	{
		int result = 0;
		int mx = 0;
		for (int i = 0; i < cntlines; i++)
		{
			mx = result;
			if ((TurnType) ? EurLines[i].m_b : EurLines[i].m_w)
			{
				for (int j = 0; j < sz; j++)
				{
					if (ChipArr[EurLines[i].arr[j].arr[0], EurLines[i].arr[j].arr[1], EurLines[i].arr[j].arr[2]] == ((TurnType) ? Token.white : Token.black))
					{
						//if (TurnType) EurLines[i].m_b = false;
						//else EurLines[i].m_w = false;
						result = mx;
						break;
					}
					if (ChipArr[EurLines[i].arr[j].arr[0], EurLines[i].arr[j].arr[1], EurLines[i].arr[j].arr[2]] == ((TurnType) ? Token.black : Token.white))
						result++;
				}
				if ((result-mx)==4) return -10000;
			}
		}
		return result;
	}
	public static int Euristic(int deep)
	{
		Coord s = new Coord();
		int EurLast = 0;
		int result = 100000;
		TurnType = !TurnType;
		if (deep==0)
		{
			EurLast = EuristicCount();
			TurnType = !TurnType;
			return EurLast;
		}
		for (int i = 0; i < sz; i++)
		{
			for (int j = 0; j < sz; j++)
			{
				if (MaxY[i, j] == sz) continue;
				ChipArr[i, MaxY[i, j], j] = (TurnType) ? Token.black : Token.white;
				MaxY[i, j]++;
				EurLast = Euristic(deep - 1);
				MaxY[i, j]--;
				ChipArr[i, MaxY[i, j], j] = Token.space;
				if (result>EurLast)
				{
					result = EurLast;
					EurChoosed.Set(i, MaxY[i, j], j);
				}
			}
		}
		TurnType = !TurnType;
		return result;
	}
	public static void NewGame(bool ST)
	{
		for (int i = 0; i < cntlines; ++i) 
		{
			AllLines[i] = new Line();
			EurLines[i] = new Line();
		}
		MaxY = new int[sz,sz];
		CreateAllLines(ref AllLines);
		CreateAllLines(ref EurLines);
		ChipArr = new Token[sz,sz,sz];
		
		SetToZero();
		
		TurnType = true;
		GameWin = false;
		PlayerWin = false;
		//LastChip.Pos = new Coord();
		return;
	}
	public bool CheckLine (Line Cline) {
		for (int i = 0; i < sz - 1; ++i)
		{
			if ((ChipArr[Cline.arr[i+1].arr[0], Cline.arr[i+1].arr[1], Cline.arr[i+1].arr[2]]!=ChipArr[Cline.arr[i].arr[0], Cline.arr[i].arr[1], Cline.arr[i].arr[2]])||(ChipArr[Cline.arr[i+1].arr[0], Cline.arr[i+1].arr[1], Cline.arr[i+1].arr[2]]==Token.space)) return false;
		}
		return true;
		//ЭТО НЕ ОЧЕНЬ ПЛОХО НО ПЛОХО
	}
	public static void CreateAllLines(ref Line[] LN)
	{
		Coord c = new Coord();
		int t1, t2, ind = 0, last_ind = sz - 1;
		//Генерация простых линий(т.е. по осям координат)
		for (int i = 0; i < msr; i++)
		{
			t1 = (i + 1) % msr;
			for (c.arr[t1] = 0; c.arr[t1] < sz; c.arr[t1]++)
			{
				t2 = (i + 2) % msr;
				for (c.arr[t2] = 0; c.arr[t2] < sz; c.arr[t2]++)
				{
					for (c.arr[i] = 0; c.arr[i] < sz; c.arr[i]++)
					{
						LN[ind].arr[c.arr[i]].Set(c);
					}
					++ind;
				}
			}
		}
		//Генерация плоских диагоналей
		for (int i = 0; i < msr; i++)
		{
			for (c.arr[i] = 0; c.arr[i] < sz; c.arr[i]++)
			{
				t1 = (i + 1) % msr;
				t2 = (i + 2) % msr;
				for (c.arr[t1] = c.arr[t2] = 0; c.arr[t1] < sz; c.arr[t2]++, c.arr[t1]++)
				{
					LN[ind].arr[c.arr[t1]].Set(c);
				}
				ind++;
				for (c.arr[t1] = 0, c.arr[t2] = last_ind; c.arr[t1] < sz; c.arr[t2]--, c.arr[t1]++)
				{
					LN[ind].arr[c.arr[t1]].Set(c);
				}
				++ind;
			}
		}
		//Генерация главных диагоналей
		c.Set(0, 0, 0);
		bool[] z = { true, false, false, false, false };
		for (int i = 0; i < 4; i++) {	//4 - количество главных диагоналей
			for (int j = 0, l; j < msr; j++) {
				if (z[j + 1]) {
					c.arr[j] = 0;
					l = 1;
				}
				else {
					l = -1;
					c.arr[j] = last_ind;
				}
				for (int k = 0; k < sz; c.arr[j] += l, k++) {
					LN[ind].arr[k].arr[j] = c.arr[j];
				}
			}
			++ind;
			z[i+1] = true;
			z[i] = false;
		}
	}
	public bool CheckPosition()
	{
		for (int i = 0; i < cntlines; ++i)
		{
			if (CheckLine(AllLines[i]))
			{
				WinLine = AllLines[i];
				return true;
			}
		}
		return false;
	}
	static void NextTurn(int PosX, int PosY, int PosZ)
	{
		if (TurnType) ChipArr[PosX, PosY, PosZ] = Token.white;
		else ChipArr[PosX, PosY, PosZ] = Token.black;
		MaxY[PosX, PosZ]++;
	}
	static void PlayBot()
	{
		for (int i = 0; i < cntlines; ++i)
		{
			EurLines[i].m_b = true;
			EurLines[i].m_w = true;
		}
		
		Euristic(1);
		
		Coord crd = new Coord();
		crd.Set(EurChoosed);
		NextTurn(crd.arr[0], crd.arr[1], crd.arr[2]);
		if (GameWin) return;
		TurnType = !TurnType;
		if ((TurnType&&Player1Bot)||(!TurnType&&Player2Bot)) PlayBot();
	}
}

}

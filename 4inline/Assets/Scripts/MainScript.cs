using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class MainScript : MonoBehaviour {

	public GameObject CameraObject;
	public GameObject LastCyl;
	public GameObject PrefabChip;
	public Material ChipWhite;
	public Material ChipBlack;
	public Material ChipWhiteLight;
	public Material ChipBlackLight;
	public PosList ListOfPos;
	public PosList LastChip;
	public static Token[,,] ChipArr;
	public Line WinLine = new Line();
	public const int sz = 4;
	public const int msr = 3;
	public const int cntlines = msr * sz * (sz + 2) + 4;
	public static bool TurnType = true;
	public static bool Player1Bot = false;
	public static bool Player2Bot = false;
	public bool GameWin = false;
	public bool PlayerWin = false;
	public bool ClickNeeded = true;
	public int  LastLeftCyl;
	public Coord EurChoosed = new Coord();
	public Line[] AllLines = new Line[cntlines];
	public Line[] EurLines = new Line[cntlines];
	public int[,] MaxY;
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
	public class PosList
	{
		public Coord Pos = new Coord();
		public PosList Next;
		public PosList Prev;
		public GameObject Chip;
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
	public void SetToZero() {
		for (int x = 0; x < sz; ++x)
			for (int y = 0; y < sz; ++y)
				for (int z = 0; z < sz; ++z)
					ChipArr [x, y, z] = Token.space;
		return;
	}
	public int EuristicCount()
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
	public int Euristic(int deep)
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
	public void NewGame(bool ST)
	{
		for (int i = 0; i < cntlines; ++i) 
		{
			AllLines[i] = new Line();
			EurLines[i] = new Line();
		}
		MaxY = new int[sz,sz];
		CreateAllLines(ref AllLines);
		CreateAllLines(ref EurLines);
		if (ST) CameraObject.GetComponent<CameraRotate>().CameraReturn();
		ChipArr = new Token[sz,sz,sz];
		
		SetToZero();
		
		TurnType = true;
		GameWin = false;
		PlayerWin = false;
		if ((ListOfPos!=null)&&(LastChip!=null)) {
			while (LastChip!=ListOfPos)
			{
				Destroy(LastChip.Chip, 0);
				LastChip = LastChip.Prev;
			}
			
			Destroy(ListOfPos.Chip, 0);
		}
		ListOfPos = new PosList();
		LastChip = ListOfPos;
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
	public void CreateAllLines(ref Line[] LN)
	{
		Coord c = new Coord();
		int t1, t2, ind = 0, last_ind = sz - 1;
		//Генерация простых линий(т.е. по осям координат)
		for (int i = 0; i < msr; i++) {
			t1 = (i + 1) % msr;
			for (c.arr[t1] = 0; c.arr[t1] < sz; c.arr[t1]++) {
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
		for (int i = 0; i < msr; i++) {
			for (c.arr[i] = 0; c.arr[i] < sz; c.arr[i]++) {
				t1 = (i + 1) % msr;
				t2 = (i + 2) % msr;
				for (c.arr[t1] = c.arr[t2] = 0; c.arr[t1] < sz; c.arr[t2]++, c.arr[t1]++) {
					LN[ind].arr[c.arr[t1]].Set(c);
				}
				ind++;
				for (c.arr[t1] = 0, c.arr[t2] = last_ind; c.arr[t1] < sz; c.arr[t2]--, c.arr[t1]++) {
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
	void Update()
	{
		if (Input.GetKey(KeyCode.Escape))
		{
			Application.Quit();
		}
	}
	void Start()
	{
		NewGame(false);
	}
	void OnGUI()
	{
		if (GUI.Button(new Rect (Screen.width-120, Screen.height-150, 100, 30), "Сделать ход"))
		{
   			HumanTurn();
  		}
		if (GUI.Button(new Rect (Screen.width-120, Screen.height-100, 100, 30), "Новая игра"))
		{
			NewGame(true);
  		}
		if (GUI.Button(new Rect (Screen.width-120, Screen.height-200, 100, 30), "Отменить ход"))
		{
			LastChip = LastChip.Prev;
			ChipArr[LastChip.Pos.arr[0], LastChip.Pos.arr[1], LastChip.Pos.arr[2]] = Token.space;
			MaxY[LastChip.Pos.arr[0], LastChip.Pos.arr[2]]--;
			TurnType = !TurnType;
			Destroy(LastChip.Chip, 0);
			LastChip.Prev.Next = LastChip.Next;
			LastChip.Next.Prev = LastChip.Prev;
			LastChip = LastChip.Next;
  		}
		ClickNeeded = GUI.Toggle(new Rect (Screen.width-150, Screen.height-230, 150, 20), ClickNeeded, "Подтверждение хода");
		Player1Bot = GUI.Toggle(new Rect (Screen.width-150, Screen.height-260, 150, 20), Player1Bot, "Белый бот");
		Player2Bot = GUI.Toggle(new Rect (Screen.width-150, Screen.height-290, 150, 20), Player2Bot, "Черный бот");
		GUI.Box(new Rect (Screen.width-130, Screen.height-50, 120, 25), (GameWin) ? (PlayerWin) ? "Победил белый" : "Победил черный" : (TurnType) ? "Ходит белый" : "Ходит черный");
	}
	void NextTurn(int PosX, int PosY, int PosZ)
	{
		Vector3 ChipPos = new Vector3();
		PosList u;
		ChipPos.x = PosX*2-3;
		ChipPos.z = PosZ*2-3;
		ChipPos.y = PosY+(float)2.5;
		LastChip.Chip = (GameObject)Instantiate(PrefabChip, ChipPos, Quaternion.Euler(90, 0, 0));
		LastChip.Pos.arr[0] = PosX;
		LastChip.Pos.arr[1] = PosY;
		LastChip.Pos.arr[2] = PosZ;
		if (TurnType) LastChip.Chip.GetComponent<Renderer>().material = ChipWhite;
		else LastChip.Chip.GetComponent<Renderer>().material = ChipBlack;
		if (TurnType) ChipArr[PosX, PosY, PosZ] = Token.white;
		else ChipArr[PosX, PosY, PosZ] = Token.black;
		MaxY[PosX, PosZ]++;
		LastChip.Next = new PosList();
		u = LastChip;
		LastChip = LastChip.Next;
		LastChip.Prev = u;
		if (CheckPosition()) {
			GameWin = true;
			PlayerWin = ChipArr[WinLine.arr[0].arr[0], WinLine.arr[0].arr[1], WinLine.arr[0].arr[2]]==Token.white;
			PosList P = ListOfPos;
			while (P!=LastChip)
			{
				for (int i = 0; i<sz; ++i) {
					if ((WinLine.arr[i].arr[0]==P.Pos.arr[0])&&(WinLine.arr[i].arr[1]==P.Pos.arr[1])&&(WinLine.arr[i].arr[2]==P.Pos.arr[2]))
					{
						P.Chip.GetComponent<Renderer>().material = (ChipArr[P.Pos.arr[0], P.Pos.arr[1], P.Pos.arr[2]]==Token.white) ? ChipWhiteLight : ChipBlackLight;
						break;
					}	
				}
				P = P.Next;
			}
		}
	}
	public void HumanTurn() {
		if (GameWin) return;
		if (LastCyl == null) {
			Debug.Log("LastCyl is NULL");
			return;
		}
		int PosY = 0;
		while ((PosY<sz)&&(ChipArr[(((int)LastCyl.transform.position.x)+3)/2, PosY, (((int)LastCyl.transform.position.z)+3)/2]!=Token.space)) ++PosY;
		if (PosY>=sz) {
			Debug.Log("LastCyl PosY>3");
			return;
		}
		NextTurn((((int)LastCyl.transform.position.x)+3)/2, PosY, (((int)LastCyl.transform.position.z)+3)/2);
		if (GameWin) return;
		TurnType = !TurnType;
		if ((TurnType&&Player1Bot)||(!TurnType&&Player2Bot)) PlayBot();
	}
	void PlayBot()
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
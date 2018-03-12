using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//16 16 + 16 + 16 + 2 * 4 + 2 * 4 + 2 * 4 + 2 + 76 количество диагоналей


namespace Four_in_row
{ 
    class Program
    {
		enum error{
			Ok,					//Всё Ок
			Out_of_index_error,  //выход за границы поля
			Vsyo_zanyato		//Всё занято
			 
		}
		enum token
		{
			space = '@',  // не занятое место
			white = 'o',
			black = '*'
		}
        const int sz = 4;

		//вроде как работает нормально
		class vector
		{
			public int[] mas;
			public int size, capacity;
			public vector(int size_mas = 0)
			{
				if (size_mas != 0)
					capacity <<= 1;
				else
					capacity = 1;
				mas = new int[capacity];
				size = size_mas;
			}
			private void grow_up()
			{
				if (size >= capacity)
				{
					capacity <<= 1;
					int[] mas2 = new int[capacity];
					for (int i = 0; i < size; i++)
						mas2[i] = mas[i];
					mas = mas2;
				}
			}

			private void grow_down()
			{
				if ((size > 0) && (size <= (capacity >> 1)))
				{
					capacity >>= 1;
					int[] mas2 = new int[capacity];
					for (int i = 0; i < size; i++)
						mas2[i] = mas[i];
					mas = mas2;
				}
			}
			public void push_back(int x)
			{
				mas[size++] = x;
				grow_up();
			}
			public void pop_back(int zamena = 0)
			{
				mas[--size] = zamena;
				grow_down();
			}
			public void erase(int index, int count = 1)
			{
				for (int i = index; i < index + count; i++) {` }
			}
		}

		class coord
		{
			public int x, y, z;
			public void Set(int xx, int yy, int zz) {
				x = xx;
				y = yy;
				z = zz;
			}
			public void Set(coord c)
			{
				x = c.x;
				y = c.y;
				z = c.z;
			}
		}
		class line
		{
			public coord[] mas = new coord[sz];
			//Генерирует строку по двум точкам
			public void Set(coord c1, coord c2)
			{
				bool x, x2, y, y2, z, z2;
				x = c1.x == c2.x;
				x2 = c1.x + c2.x == sz - 1;
                y = c1.y == c2.y;
				y2 = c1.y + c2.y == sz - 1;
				z = c1.z == c2.z;
				z2 = c1.z + c2.z == sz - 1;
				if (x && y && z) { Console.WriteLine("Points_sovpadayut"); return; }
				//Генерация линии
				for (int i = 0; i < sz; i++)
				{
					mas[i] = new coord();
					if (x) { mas[i].x = c1.x; }
					else if (x2) { mas[i].x = sz - 1 - i; }	
					else{ mas[i].x = i; }

					if (y) { mas[i].y = c1.y; }
					else if (y2) { mas[i].y = sz - 1 - i; }
					else { mas[i].y = i; }

					if (z) { mas[i].z = c1.z; }
					else if (z2) { mas[i].z = sz - 1 - i; }
					else { mas[i].z = i; }
				}
			}
			public coord[] Get()
			{
				return mas;
			}

		}

		class tok                   //не придумал нормального названия
		{
			public token color;
			public ArrayList tokens = new ArrayList();
			//public ArrayList lines = new ArrayList();
			public void Print_tokens()
			{
				if (tokens.Count == 0)
					Console.WriteLine("NoElements");
				else
				{
					for (int i = 0; i < tokens.Count; i++)
						Console.Write("{0} ", tokens[i]);
					Console.WriteLine();
				}
			}
		}
		

		class state
		{
			private token[,,] mas = new token[sz, sz, sz];      //надо описать
			private byte[,] lv = new byte[sz, sz];
			public tok Human = new tok(), Bot = new tok();
			public state(state Where = null)
			{
				if (Where != null)
				{
					for (int x = 0; x < sz; x++)
						for (int y = 0; y < sz; y++)
							for (int z = 0; z < sz; z++)
								mas[x, y, z] = Where.mas[x, y, z];
					Human = Where.Human;
					Bot = Where.Bot;
				}
				else
				{
					for (int x = 0; x < sz; x++)
						for (int y = 0; y < sz; y++)
							for (int z = 0; z < sz; z++)
								mas[x, y, z] = token.space;
					Human.color = token.white;      //получение цвета
					Bot.color = token.black;		//==||==
				}				
			}

			//private void Update_lines(int x, int z, int y, bool Bot_or_not = true) { }
			
			public line[] GetLines(coord central, token color, ref int size)
			{
				line[] lines = new line[13]; //максимум диагоналей + 3
				coord c = new coord();
				size = 0;
				c.Set(central);  //Иначе присваивал в с указатель на central(c = central;)

				c.x = (c.x + 1) % sz;
				lines[size] = new line();
				lines[size++].Set(central, c);
				c.x = central.x;

				c.y = (c.y + 1) % sz;
				lines[size] = new line();
				lines[size++].Set(central, c);
				c.y = central.y;

				c.z = (c.z + 1) % sz;
				lines[size] = new line();
				lines[size++].Set(central, c);
				c.z = central.z;


				bool xy, x_y, xz, x_z, zy, z_y;
				xy = c.x == c.y;
				x_y = c.x == sz - 1 - c.y;
				xz = c.x == c.z;
				x_z = c.x == sz - 1 - c.z;
                zy = c.y == c.z;
				z_y = c.z == sz - 1 - c.y;

				if (xy || x_y)
				{
					c.x = (c.x + 1) % sz;								//Для куба с нечётной стороной нужно добавить if(c.y == (size - 1) / 2){выполни и для 1 и для 2}
					if (x_y) { c.y = (size - 1 + c.y) % sz; }			//1
					else { c.y = (c.y + 1) % sz; }                      //2
					lines[size] = new line();
					lines[size++].Set(central, c);
					c.Set(central);
				}

				if (xz || x_z)
				{
					c.x = (c.x + 1) % sz;
					if (x_z) { c.z = (size - 1 + c.z) % sz; }
					else{ c.z = (c.z + 1) % sz; }
					lines[size] = new line();
					lines[size++].Set(central, c);
					c.Set(central);
				}

				if (zy || z_y)
				{
					c.z = (c.z + 1) % sz;
					if (z_y) { c.y = (size - 1 + c.y) % sz; }
					else { c.y = (c.y + 1) % sz; }
					lines[size] = new line();
					lines[size++].Set(central, c);
					c.Set(central);
				}

				if (xy && xz)
				{
					c.x = c.y = c.z = (c.x + 1) % sz;
					lines[size] = new line();
					lines[size++].Set(central, c);
					c.Set(central);
				}
				
				if (x_y && xz){
					c.z = c.x = (c.x + 1) % sz;
					c.y = (sz - 1 + c.y) % sz;
					lines[size] = new line();
					lines[size++].Set(central, c);
					c.Set(central);
				}
				
				if (x_z && zy)
				{
					c.z = c.y = (sz - 1 + c.z) %sz;
					c.x = (c.x + 1) % sz;
					lines[size] = new line();
					lines[size++].Set(central, c);
					c.Set(central);
				}
				
				if (z_y && xy)
				{
					c.x = c.y = (sz - 1 + c.x) % sz;
					c.z = (c.z + 1) % sz;
					lines[size] = new line();
					lines[size++].Set(central, c);
					c.Set(central);
				}
				return lines;
			}

			public error Put_token(int x, int z, bool Bot_or_not = true)
			{
				if (((x > 0) && (x <= sz)) && ((z > 0) && (z <= sz)))
				{
					for (int y = 0; y < sz; y++)
						if (mas[x, y, z] == token.space)
						{
							coord c = new coord();
							c.Set(x, y, z);
							if (Bot_or_not)
							{
								mas[x, y, z] = Bot.color;
								Bot.tokens.Add(c);
							}
							else
							{
								mas[x, y, z] = Human.color;
								Human.tokens.Add(c);
							}
							return error.Ok;
						}
					return error.Vsyo_zanyato;
				}
				return error.Out_of_index_error;
			}
			//Подсчёт эвристики

			public int CountE(coord c)
			{
				int size = 0;
				line [] lines = 
			}

			public void Print()
			{
				for (int y = 0; y < sz; y++)
				{
					for (int z = 0; z < sz; z++)
					{
						for (int x = 0; x < sz; x++)
							Console.Write((char)mas[x, y, z]);
						Console.WriteLine();
					}
					Console.WriteLine();
				}
			}
			

			public void Hod(state start, bool Bot_or_not = false)
			{

			}
		}
        static void Main(string[] args)
        {                                                   //Бот играет за чёрных
			
		}
	}
}


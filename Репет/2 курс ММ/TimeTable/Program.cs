// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// PVS-settings
// PVS-settings end
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace TimetableK
{
    class Route
    {
        public int num;
        public int starttime;
        public int endtime;
        public int interval;
        public List<int> stations;
        public Route(int n, int s, int e, int i, List<int> st)
        {
            num = n;
            starttime = s;
            endtime = e;
            interval = i;
            stations = st;
        }
    }
    class Schedule : IComparable<Schedule>
    {
        public int num;
        public int dest;
        public int time;
        public Schedule(int n, int d, int t)
        {
            num = n;
            dest = d;
            time = t;
        }
        public int CompareTo(Schedule another)
        {
            return this.time - another.time;
        }
    }
    class Program
    {
        static Dictionary<string, int> stkeyByName;
        static List<string> stnameByKey;
        static List<Dictionary<int, int>> stDist;
        static Dictionary<int, Route> routesByNum;
        static List<Route> routesStorage;
        static List<List<Route>> routesByStkey;

        static void ReadEx(int line)
        {
            Console.WriteLine("Incorrect format at {0} line", line);
            Console.WriteLine("Press any key to exit . . .");
            Console.ReadKey(true);
            Environment.Exit(1);
        }
        static List<Schedule> GetScheduleBySt(int station, DateTime time)
        {
            List<Schedule> result = new List<Schedule>();
            List<int> st;
            Route rt;
            for (int i = 0; i < routesByStkey[station].Count; ++i)
            {
                int t = time.Hour * 60 + time.Minute;
                rt = routesByStkey[station][i];
                st = rt.stations;
                int pos = st.FindIndex(x => x == station);
                int t1 = 0, t2 = 0;
                for (int j = 0; j < pos; ++j)
                    t1 += stDist[st[j]][st[j + 1]];
                for (int j = st.Count - 1; j > pos; --j)
                    t2 += stDist[st[j]][st[j - 1]];
                int tt = t - t1;
                #region
                if (pos < st.Count - 1)
                {
                    if (tt >= rt.starttime && tt <= rt.endtime)
                    {
                        result.Add(new Schedule(rt.num, st[st.Count - 1], rt.interval - (tt - rt.starttime + rt.interval - 1) % rt.interval - 1));
                    }
                    else if ((tt += 24 * 60) >= rt.starttime && tt <= rt.endtime)
                    {
                        result.Add(new Schedule(rt.num, st[st.Count - 1], rt.interval - (tt - rt.starttime + rt.interval - 1) % rt.interval - 1));
                    }
                    else
                    {
                        tt -= 24 * 60;
                        if (tt > rt.starttime)
                            result.Add(new Schedule(rt.num, st[st.Count - 1], rt.starttime + 24 * 60 - tt));
                        else
                            result.Add(new Schedule(rt.num, st[st.Count - 1], rt.starttime - tt));
                    }
                }
                #endregion
                tt = t - t2;
                #region
                if (pos > 0)
                {
                    if (tt >= rt.starttime && tt <= rt.endtime)
                    {
                        result.Add(new Schedule(rt.num, st[0], rt.interval - (tt - rt.starttime + rt.interval - 1) % rt.interval - 1));
                    }
                    else if ((tt += 24 * 60) >= rt.starttime && tt <= rt.endtime)
                    {
                        result.Add(new Schedule(rt.num, st[0], rt.interval - (tt - rt.starttime + rt.interval - 1) % rt.interval - 1));
                    }
                    else
                    {
                        tt -= 24 * 60;
                        if (tt > rt.starttime)
                            result.Add(new Schedule(rt.num, st[0], rt.starttime + 24 * 60 - tt));
                        else
                            result.Add(new Schedule(rt.num, st[0], rt.starttime - tt));
                    }
                }
                #endregion
            }
            result.Sort();
            return result;
        }
        static int GetData()
        {
            StreamReader filestream = new StreamReader( "../../routes.txt");

            stkeyByName = new Dictionary<string, int>();
            stnameByKey = new List<string>();
            stDist = new List<Dictionary<int, int>>();
            routesByNum = new Dictionary<int, Route>();
            routesStorage = new List<Route>();
            routesByStkey = new List<List<Route>>();

            string line;
            int l = 1;
            while ((line = filestream.ReadLine()) != null && line.StartsWith("//")) ;
            try
            {
                string[] parse = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                int m = Convert.ToInt32(parse[0]);
                int r = Convert.ToInt32(parse[1]);
                ++l;
                #region
                for (int i = 0; i < m; ++i, ++l)
                {
                    line = filestream.ReadLine();
                    if (line.StartsWith("//"))
                    {
                        --i;
                        --l;
                        continue;
                    }
                    parse = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                    if (parse.Length != 3)
                        ReadEx(l);
                    if (!stkeyByName.ContainsKey(parse[0]))
                    {
                        stnameByKey.Add(parse[0]);
                        stkeyByName.Add(parse[0], stnameByKey.Count - 1);
                        stDist.Add(new Dictionary<int, int>());
                        routesByStkey.Add(new List<Route>());
                    }
                    if (!stkeyByName.ContainsKey(parse[1]))
                    {
                        stnameByKey.Add(parse[1]);
                        stkeyByName.Add(parse[1], stnameByKey.Count - 1);
                        stDist.Add(new Dictionary<int, int>());
                        routesByStkey.Add(new List<Route>());
                    }
                    int st1 = stkeyByName[parse[0]], st2 = stkeyByName[parse[1]];
                    int dist = Convert.ToInt32(parse[2]);
                    stDist[st1].Add(st2, dist);
                    stDist[st2].Add(st1, dist);
                }
                #endregion

                #region
                
                for (int i = 0; i < r; ++i, ++l)
                {
                    line = filestream.ReadLine();
                    if (line.StartsWith("//"))
                    {
                        --i;
                        --l;
                        continue;
                    }
                    parse = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                    if (parse.Length != 4)
                        ReadEx(l);
                    int rn = Convert.ToInt32(parse[0]);
                    string[] rt = parse[1].Split(new char[] { ':' });
                    int rs = Convert.ToInt32(rt[0]) * 60 + Convert.ToInt32(rt[1]);
                    rt = parse[2].Split(new char[] { ':' });
                    int re = Convert.ToInt32(rt[0]) * 60 + Convert.ToInt32(rt[1]);
                    if (re < rs) re += 24 * 60;
                    int ri = Convert.ToInt32(parse[3]);
                    ++l;
                    while ((line = filestream.ReadLine()).StartsWith("//")) ;
                    parse = line.Split(new char[] { ' ' }, StringSplitOptions.RemoveEmptyEntries);
                    List<int> stList = new List<int>();
                    for (int j = 0; j < parse.Length; ++j)
                        stList.Add(stkeyByName[parse[j]]); //TODO check nonexist station
                    Route nroute = new Route(rn, rs, re, ri, stList);
                    routesStorage.Add(nroute);
                    routesByNum.Add(rn, nroute);
                    foreach (int item in nroute.stations)
                        routesByStkey[item].Add(nroute);
                }
                #endregion
                
            }
            catch (Exception)
            {
                ReadEx(l);
            }
            return 0;
        }
        static void UI()
        {
            Console.Write("Enter station: ");
            string line;
            while ((line = Console.ReadLine()) != "")
            {
                DateTime time = DateTime.Now;
                if (!stkeyByName.ContainsKey(line))
                {
                    Console.WriteLine("No such station!");
                    Console.Write("Enter station: ");
                    continue;
                }
                Console.WriteLine("Current time: {0:00}:{1:00}", time.Hour, time.Minute);
                Console.WriteLine("Schedule:");
                var sch = GetScheduleBySt(stkeyByName[line], time);
                foreach (var item in sch)
                {
                    Console.WriteLine("{0}, destination {1}, {2} min", item.num, stnameByKey[item.dest], item.time);
                }
                Console.Write("Enter station: ");
            }
        }
        static void Main(string[] args)
        {
            GetData();
            //GetScheduleBySt(1, new DateTime(1, 1, 1, 5, 9, 0));
            UI();
        }
    }
}

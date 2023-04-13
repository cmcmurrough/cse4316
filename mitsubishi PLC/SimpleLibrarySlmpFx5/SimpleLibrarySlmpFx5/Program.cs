using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimpleLibrarySlmpFx5
{
    class Program
    {
        static void Main(string[] args)
        {
            SimpleLibSlmpFx5.SimpleSlmp connection1 = new SimpleLibSlmpFx5.SimpleSlmp();
            byte[] newIpAdress = new byte[4];
            newIpAdress[0] = 192; newIpAdress[1] = 168; newIpAdress[2] = 0; newIpAdress[3] = 250;
            string ans = connection1.Open(newIpAdress, 2000);
            Console.WriteLine("Result from opening connection: " + ans);
            if (ans == string.Empty)
            {
                /*
                bool m10Value = false;
                ans = connection1.ReadBit("M", 10, ref m10Value);
                Console.WriteLine("Ans: " + ans + ", Read bit of M10 before invert: " + m10Value.ToString());
                //Console.ReadKey();
                bool m10ValueSet = !m10Value;
                ans = connection1.WriteBit("M", 10, m10ValueSet);
                Console.WriteLine("Write bit M10 Ans: " + ans);
                //Console.ReadKey();
                ans = connection1.ReadBit("M", 10, ref m10Value);
                Console.WriteLine("Ans: " + ans + ",Read bit of M10 after invert: " + m10Value.ToString());
                //Console.ReadKey();
                */
                Int16 d10Value = 0;
                ans = connection1.ReadWord("D", 200, ref d10Value);
                Console.WriteLine("Ans: " + ans + ",Read value of D10 before increment: " + d10Value.ToString());
                Console.ReadKey();
                d10Value++;
                ans = connection1.WriteWord("D", 10, d10Value);
                Console.WriteLine("Write value D10 Ans: " + ans);
                Console.ReadKey();
                ans = connection1.ReadWord("D", 10, ref d10Value);
                Console.WriteLine("Ans: " + ans + ",Read value of D10 after increment: " + d10Value.ToString());
                Console.ReadKey();
            }
            connection1.Close();
            Console.ReadKey();
        }
    }
}

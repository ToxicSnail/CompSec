using System;

namespace RaiseToPower
{
    class Program
    {
        static void Main()
        {
            Console.WriteLine("Enter the number (a): ");
            double a = double.Parse(Console.ReadLine());
            
            Console.WriteLine("Enter the power (k): ");
            int k = int.Parse(Console.ReadLine());
            
            double result = 0;
            
            if (a == 0)
            {
                Console.WriteLine("{0} raised to the power of {1} is 0", a, k);
            }
            else if (k == 0)
            {
            Console.WriteLine("{0} raised to the power of {1} is 1", a, k);
            }
            else if (a < 0)
            {
                a = (-1) * a;
                if (k % 2 == 0)
                {
                    result = RaiseToPower(a,k);
                    Console.WriteLine("{0} raised to the power of {1} is {2}", a, k, result);
                }
                else 
                {
                    result = (-1) * RaiseToPower(a,k);
                    Console.WriteLine("{0} raised to the power of {1} is {2}", a, k, result);
                }
            }
            else
            {
                result += RaiseToPower(a, k);
                Console.WriteLine("{0} raised to the power of {1} is {2}", a, k, result);
            }
        }

        static double RaiseToPower(double a, int k)
        {
            double result = 1;
            for (int i = 0; i < k; i++)
            {
                result *= a;
            }
            return result;
        }
    }
}

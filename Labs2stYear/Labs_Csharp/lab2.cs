using System;

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("Enter a string:");        
        string S = Console.ReadLine();

        Console.WriteLine("Enter a character:");
        char c = Console.ReadLine()[0];

        int count = 0;        // Counting the number of times the c appears in the string
        foreach (char i in S)
        {
            if (i == c)
            {
                count++;
            }
        }
        Console.WriteLine($"The c '{c}' appears {count} times in the string '{S}'.");

        string stringWithoutDigits = "";        // Removing all digits from the string
        foreach (char i in S)
        {
            if (!char.IsDigit(i))
            {
                stringWithoutDigits += i;
            }
        }
        Console.WriteLine($"The string without digits is: {stringWithoutDigits}");

        bool hasK = false, hasE = false, hasY = false;        // Checking if all letters from the word "key" occur in the string
        foreach (char c in S.ToLower())
        {
            if (i == 'k')
            {
                hasK = true;
            }
            else if (i == 'e')
            {
                hasE = true;
            }
            else if (i == 'y')
            {
                hasY = true;
            }
        }
        if (hasK && hasE && hasY)
        {
            Console.WriteLine("The string contains all letters from the word 'key'.");
        }
        else
        {
            Console.WriteLine("The string does not contain all letters from the word 'key'.");
        }

        int countOpenParentheses = 0, countClosedParentheses = 0;        // Checking if parentheses are placed correctly in the string
        foreach (char i in S)
        {
            if (i == '(')
            {
                countOpenParentheses++;
            }
            else if (i == ')')
            {
                countClosedParentheses++;
            }
        }
        if (countOpenParentheses == countClosedParentheses)
        {
            Console.WriteLine("The parentheses are placed correctly in the string.");
        }
        else
        {
            Console.WriteLine("The parentheses are not placed correctly in the string.");
        }
    }
}

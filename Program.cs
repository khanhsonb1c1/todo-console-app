using System;

class Program
{
    static void Main()
    {
        while (true)
        {
            Console.WriteLine("Danh sách lệnh:");
            Console.WriteLine("1. add [<title>] [<description>] [<time>]");
            Console.WriteLine("2. edit #<num>");
            Console.WriteLine("3. show #<num>");
            Console.WriteLine("4. show all");
            Console.WriteLine("5. quit");

            Console.Write("Nhập lệnh: ");
            string input = Console.ReadLine();

            string commandType = GetCommandType(input);
            Console.WriteLine("Command Type: " + commandType);

            if (commandType.Equals("QUIT", StringComparison.OrdinalIgnoreCase))
            {
                Console.WriteLine("Thoát khỏi chương trình.");
                break;
            }
        }
    }

    static string GetCommandType(string input)
    {
        string[] parts = input.Trim().Split(' ');

        if (parts.Length == 0)
        {
            return "INVALID";
        }

        string firstWord = parts[0].ToUpper();

        switch (firstWord)
        {
            case "ADD":
                if (parts.Length <= 4)
                    return "ADD";
                break;

            case "EDIT":
                if (parts.Length == 2 && parts[1].StartsWith("#"))
                    return "EDIT";
                break;

            case "SHOW":
                if (
                    parts.Length == 1
                    || (parts.Length == 2 && parts[1].StartsWith("#"))
                    || (parts.Length == 2 && parts[1].ToUpper() == "ALL")
                )
                    return "SHOW";
                break;

            case "QUIT":
                if (parts.Length == 1)
                    return "QUIT";
                break;
        }

        return "INVALID";
    }
}

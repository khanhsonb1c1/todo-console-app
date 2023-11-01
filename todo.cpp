#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH_COMMAND 300
#define MAX_NO_TASKS 100
#define MAX_LENGTH_TITLE 100
#define MAX_LENGTH_DESCRIPTION 200
#define MAX_LENGTH_TIME 33
#define WEEK_CELL_FIRST_COL_WIDTH 10
#define WEEK_CELL_OTHER_COL_WIDTH 20

enum Status
{
    IN_PROGRESS,
    DONE,
    ARCHIVED
};
char *status_name[] = {"In Progress", "Done", "Archived"};
enum CommandType
{
    ADD,
    EDIT,
    SHOW,
    DELETE,
    QUIT,
    INVALID
};
char *command_name[] = {"ADD", "EDIT", "SHOW", "DELETE", "QUIT", "INVALID"};

struct Task
{
    int num;
    char title[MAX_LENGTH_TITLE + 1];
    char description[MAX_LENGTH_DESCRIPTION + 1];
    char time[MAX_LENGTH_TIME + 1];
    enum Status status;
};

// Global variable to store tasks
struct Task tasks[MAX_NO_TASKS];
int numTasks = 0;

void printTask(struct Task *task)
{
    printf("--------------------------------------------\n");
    printf("Num: #%d. Title: %s\n", task->num, task->title);
    printf("Description: %s\n", task->description);
    printf("Status: %s\n", status_name[task->status]);
    printf("--------------------------------------------\n");
}

void printUnsupportedTime(struct Task *task)
{
    printf("----- Show week view -----\n");
    printf("Error: Unsupported time with non-zero minutes: %s\n", task->time);
    printf("In Task:\n");
    printTask(task);
}
// Hàm này dùng để kiểm tra khoảng trắng
bool isSpace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

bool isValidDateTime(const char *datetime)
{
    int hour, minute, day, month, year;

    if (sscanf(datetime, "%2d:%2d|%2d/%2d/%4d", &hour, &minute, &day, &month, &year) != 5)
    {
        printf("Định dạng datetime không đúng\n");

        return false; // Định dạng không đúng
    }

    // Kiểm tra giờ và phút
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
    {
        printf("Giờ/phút không đúng\n");

        return false; // Giờ hoặc phút không hợp lệ
    }

    // Kiểm tra ngày và tháng
    if (month < 1 || month > 12 || day < 1 || day > 31)
    {
        printf("Ngày/tháng không đúng\n");

        return false; // Ngày hoặc tháng không hợp lệ
    }

    // Kiểm tra năm (ở đây chúng ta chấp nhận bất kỳ năm nào)
    if (year < 1)
    {
        printf("Năm không đúng\n");

        return false; // Năm không hợp lệ
    }

    return true;
}

// Function to validate the date format
bool checkTime(const char *date)
{
    // Kiểm tra độ dài của chuỗi
    if (strlen(date) != 33)
    {
        printf("Độ dài không đúng\n");
        return false;
    }

    // Lấy datetime1 và datetime2 từ chuỗi
    char datetime1[17];
    char datetime2[17];
    if (sscanf(date, "%16[^-]-%16s", datetime1, datetime2) != 2)
    {
        printf("Định dạng thời gian không hợp lệ.\n");
        return false;
    }

    // Kiểm tra datetime1
    if (!isValidDateTime(datetime1))
    {
        // printf("Datetime1 không hợp lệ.\n");
        return false;
    }

    // Kiểm tra datetime2
    if (!isValidDateTime(datetime2))
    {
        // printf("Datetime2 không hợp lệ.\n");
        return false;
    }

    return true;
}

bool checkTitle(const char *title)
{
    // Kiểm tra title không được bắt đầu hoặc kết thúc bằng khoảng trắng
    if (isSpace(title[0]) || isSpace(title[strlen(title) - 1]))
    {
        printf("Title không được bắt đầu hoặc kết thúc bằng khoảng trắng.\n");
        return false;
    }

    // Kiểm tra độ dài title không vượt quá giới hạn
    if (strlen(title) > MAX_LENGTH_TITLE)
    {
        printf("Title vượt quá độ dài cho phép.\n");
        return false;
    }

    return true;
}

bool checkDescription(const char *description)
{
    // Kiểm tra description không được bắt đầu hoặc kết thúc bằng khoảng trắng
    if (isSpace(description[0]) || isSpace(description[strlen(description) - 1]))
    {
        printf("Description không được bắt đầu hoặc kết thúc bằng khoảng trắng.\n");
        return false;
    }

    // Kiểm tra độ dài description không vượt quá giới hạn
    if (strlen(description) > MAX_LENGTH_DESCRIPTION)
    {
        printf("Description vượt quá độ dài cho phép.\n");
        return false;
    }

    return true;
}

// ------ Begin: Student Answer ------
enum CommandType getCommandType(const char *command)
{
    if (strncmp(command, "add ", 4) == 0)
    {
        return ADD;
    }
    if (strncmp(command, "show ", 5) == 0)
    {
        return SHOW;
    }
    else if (strcmp(command, "quit") == 0)
    {
        return QUIT;
    }
    else
    {
        return INVALID;
    }
}

void addTask(const char *command)
{
    // Check if the tasks array is full
    if (numTasks >= MAX_NO_TASKS)
    {
        printf("Danh sách task đã đầy, không thể thêm nữa.\n");
        return;
    }

    // Parse the command to extract title, description, and time
    char title[MAX_LENGTH_TITLE + 1] = "";
    char description[MAX_LENGTH_DESCRIPTION + 1] = "";
    char time[MAX_LENGTH_TIME + 1] = "";

    if (sscanf(command, "add [%[^]]] [%[^]]] [%[^]]]", title, description, time) != 3)
    {
        printf("Lệnh 'add' không hợp lệ.\n");
        return;
    }

    // Validate title
    if (!checkTitle(title))
    {
        // printf("Định dạng title không hợp lệ.\n");
        return;
    }
    // Validate description
    if (!checkTitle(description))
    {
        // printf("Định dạng description không hợp lệ.\n");
        return;
    }

    // Validate the time format (you can implement date validation logic here)
    if (!checkTime(time))
    {
        // printf("Định dạng thời gian không hợp lệ.\n");
        return;
    }

    // Create a new task
    struct Task newTask;
    newTask.num = numTasks + 1;
    strncpy(newTask.title, title, MAX_LENGTH_TITLE);
    strncpy(newTask.description, description, MAX_LENGTH_DESCRIPTION);
    strncpy(newTask.time, time, MAX_LENGTH_TIME);

    // Add the new task to the tasks array
    tasks[numTasks] = newTask;
    numTasks++;

    printf("Task '%s' đã được thêm vào danh sách.\n", title);
}

// void printAllTasks()
// {
//     if (numTasks == 0)
//     {
//         printf("Danh sách task hiện đang trống.\n");
//         return;
//     }

//     printf("Danh sách task:\n");

//     for (int i = 0; i < numTasks; i++)
//     {
//         struct Task *taskDisplay = &tasks[numTasks];
//         printTask(taskDisplay);
        
//         // printf("Task #%d:\n", tasks[i].num);
//         // printf("Title: %s\n", tasks[i].title);
//         // printf("Description: %s\n", tasks[i].description);
//         // printf("Time: %s\n", tasks[i].time);
//         // printf("Status: %s\n", status_name[tasks[i].status]);
//         // printf("------------------------------\n");
//     }
// }

void printAllTasks()
{

    for (int i = 0; i < numTasks; i++)
    {
        struct Task *taskToDisplay = &tasks[i];
        printTask(taskToDisplay);
    }
}

void showTask(int num)
{
    if (num >= 0 && num < numTasks)
    {
        struct Task *taskToDisplay = &tasks[num-1];
        printTask(taskToDisplay);
    }
    else
    {
        printf("Số thứ tự task không hợp lệ.\n");
    }
}

// ------ End: Student Answer ------

void runTodoApp()
{

    printf("Input your command:");
    // Example of command Add
    char command[MAX_LENGTH_COMMAND + 1];

    while (true)
    {
        fgets(command, MAX_LENGTH_COMMAND + 1, stdin);
        command[strlen(command) - 1] = '\0';

        enum CommandType commandType = getCommandType(command);
        printf("Command : %s\n", command);
        printf("Command type: %s\n", command_name[commandType]);

        if (commandType == ADD)
        {
            // If the command is ADD, call the addTask function
            addTask(command);

            runTodoApp();
        }
        if (commandType == SHOW)
        {
            // If the command is ADD, call the addTask function

            if (strncmp(command, "show #", 6) == 0)
            {
                // printTask(command);
                // Xử lý lệnh "show #[number]" và hiển thị task theo số thứ tự
                int taskNumber;
                if (sscanf(command + 6, "%d", &taskNumber) == 1)
                {
                    if (taskNumber >= 1 && taskNumber <= numTasks)
                    {
                        showTask(taskNumber);
                    }
                    else
                    {
                        printf("Số thứ tự task không hợp lệ.\n");
                    }
                }
                else
                {
                    printf("Lệnh không hợp lệ.\n");
                }
            }
            else if (strcmp(command, "show all") == 0)
            {
                printAllTasks();
            }
            else
            {
                printf("Lệnh show không hợp lệ.\n");
            }

            runTodoApp();
        }
        else if (commandType == QUIT)
        {
            printf("Thoát chương trình.\n");

            // If the command is QUIT, exit the loop
            break;
        }
        else
        {
            // Handle other commands
            printf("Lệnh không được hỗ trợ.\n");
            runTodoApp();
        }

        break;
    }
}

int main()
{
    // test thêm dữ liệu mẫu
    addTask("add [Example Title1] [Example Description1] [10:00|01/01/2023-11:00|01/01/2023]");
    addTask("add [Example Title2] [Example Description2] [10:00|01/01/2023-11:00|01/01/2023]");
    addTask("add [Example Title3] [Example Description3] [10:00|01/01/2023-11:00|01/01/2023]");

    // test khoảng trắng
    //  addTask("add [Example Title ] [Example Description1] [10:00|01/01/2023-11:00|01/01/2023]");

    // test thời gian - sai định dạng
    // addTask("add [Example Title] [Example Description1] [10:00001/01/2023-11:00|01/01/2023]");
    // sai độ dài
    // addTask("add [Example Title] [Example Description1] [1/01/2023-11:00|01/01/2023]");

    // test thời gian - ngày tháng k hợp lệ
    // addTask("add [Example Title] [Example Description1] [10:00|01/01/2023-11:00|01/01/2023]");


    // test show task 1
    // showTask(1);
    // runTodoApp();

    printAllTasks();

    return 0;
}
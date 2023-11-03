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
char *status_name[] = {"In Progress","Done","Archived"};
enum CommandType
{
    ADD,
    EDIT,
    SHOW,
    DELETE,
    QUIT,
    INVALID
};
char *command_name[] = {"ADD","EDIT","SHOW","DELETE","QUIT","INVALID"};

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





// ============== Y/c 1=========================
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


// =============== Y/c 2===============



void getTitleFromAdd(char *command, char *out_title)
{
    char title[MAX_LENGTH_TITLE+ 1] = "";
    char description[MAX_LENGTH_DESCRIPTION+ 1] = "";
    char time[MAX_LENGTH_TIME+ 1] = "";

    if (sscanf(command, "add [%[^]]] [%[^]]] [%[^]]]", title, description, time) == 3)
    {
        // Sao chép title vào out_title
        strncpy(out_title, title, MAX_LENGTH_TITLE);
        out_title[MAX_LENGTH_TITLE] = '\0';
        printf("title: %s, des: %s, time: %s \n", title, description, time);
    }
    else
    {
        printf("sai....");
        // Không tìm thấy title, gán chuỗi rỗng cho out_title
        out_title[0] = '\0';
    }
}

void getTimeFromAdd(char *command, char *out_time)
{
    char title[MAX_LENGTH_TITLE + 1] = "";
    char description[MAX_LENGTH_DESCRIPTION + 1] = "";
    char time[MAX_LENGTH_TIME + 1] = "";

    if (sscanf(command, "add [%[^]]] [%[^]]] [%[^]]]", title, description, time) == 3)
    {
        // Sao chép time vào out_time

        strncpy(out_time, time, MAX_LENGTH_TIME);
        out_time[MAX_LENGTH_TIME] = '\0';
    }
    else
    {
        // Không tìm thấy time, gán chuỗi rỗng cho out_time
        out_time[0] = '\0';
    }
}

void getDescriptionFromAdd(char *command, char *out_description)
{
    char title[MAX_LENGTH_TITLE+ 1] = "";
    char description[MAX_LENGTH_DESCRIPTION+ 1] = "";
    char time[MAX_LENGTH_TIME+ 1] = "";

    if (sscanf(command, "add [%[^]]] [%[^]]] [%[^]]]", title, description, time) == 3)
    {
        // Sao chép mô tả vào out_description
        strncpy(out_description, description, MAX_LENGTH_DESCRIPTION);
        out_description[MAX_LENGTH_DESCRIPTION] = '\0';
    }
    else
    {
        out_description[0] = '\0';
    }
}




// ===================== Y/c 3 ========================

int checkTitle(const char *title)
{
    // Kiểm tra title không được bắt đầu hoặc kết thúc bằng khoảng trắng
    if (isSpace(title[0]) || isSpace(title[strlen(title) - 1]))
    {
        printf("Title không được bắt đầu hoặc kết thúc bằng khoảng trắng.\n");
        return 0;
    }

    // Kiểm tra độ dài title không vượt quá giới hạn
    if (strlen(title) > MAX_LENGTH_TITLE)
    {
        printf("Title vượt quá độ dài cho phép.\n");
        return strlen(title);
    }

    return -1;
}


// ================= Y/c 4 =========================================

int checkDescription(const char *description)
{
    // Kiểm tra description không được bắt đầu hoặc kết thúc bằng khoảng trắng
    if (isSpace(description[0]) || isSpace(description[strlen(description) - 1]))
    {
        printf("Description không được bắt đầu hoặc kết thúc bằng khoảng trắng.\n");
        return 0;
    }

    // Kiểm tra độ dài description không vượt quá giới hạn
    if (strlen(description) > MAX_LENGTH_DESCRIPTION)
    {
        printf("Description vượt quá độ dài cho phép.\n");
        return strlen(description);
    }

    return -1;
}







// ==================== Y/c 5 ========================

// Function to validate the date format
bool checkTime(const char *raw_time)
{
    // Kiểm tra độ dài của chuỗi
    if (strlen(raw_time) != 33)
    {
        printf("Độ dài không đúng\n");
        return false;
    }

    // Lấy datetime1 và datetime2 từ chuỗi
    char datetime1[17];
    char datetime2[17];
    if (sscanf(raw_time, "%16[^-]-%16s", datetime1, datetime2) != 2)
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






// =========== Y/c 6 ======================


void getTitleFromEdit(char *command, char *out_title)
{
    // Tìm vị trí của "title:[" trong lệnh edit
    char *title_start = strstr(command, " title:[");

    if (title_start)
    {
        // Di chuyển con trỏ đến sau "title:["
        title_start += strlen(" title:[");

        // Tìm vị trí kết thúc của tiêu đề (thường là ký tự ']')
        char *title_end = strchr(title_start, ']');

        if (title_end)
        {
            // Tính độ dài của tiêu đề
            int title_length = title_end - title_start;

            // Sao chép tiêu đề vào out_title
            strncpy(out_title, title_start, title_length);
            out_title[title_length] = '\0';
        }
    }
    else
    {
        // Không tìm thấy tiêu đề, gán chuỗi rỗng cho out_title
        out_title[0] = '\0';
    }
}

void getDescriptionFromEdit(char *command, char *out_description)
{
    // Tìm vị trí của "description:[" trong lệnh edit
    char *description_start = strstr(command, " description:[");

    if (description_start)
    {
        // Di chuyển con trỏ đến sau "description:["
        description_start += strlen(" description:[");

        // Tìm vị trí kết thúc của mô tả (thường là ký tự ']')
        char *description_end = strchr(description_start, ']');

        if (description_end)
        {
            // Tính độ dài của mô tả
            int description_length = description_end - description_start;

            // Sao chép mô tả vào out_description
            strncpy(out_description, description_start, description_length);
            out_description[description_length] = '\0';
        }
    }
    else
    {
        // Không tìm thấy mô tả, gán chuỗi rỗng cho out_description
        out_description[0] = '\0';
    }
}

void getTimeFromEdit(char *command, char *out_time)
{
    // Tìm vị trí của "time:[" trong lệnh edit
    char *time_start = strstr(command, " time:[");

    if (time_start)
    {
        // Di chuyển con trỏ đến sau "time:["
        time_start += strlen(" time:[");

        // Tìm vị trí kết thúc của thời gian (thường là ký tự ']')
        char *time_end = strchr(time_start, ']');

        if (time_end)
        {
            // Tính độ dài của thời gian
            int time_length = time_end - time_start;

            // Sao chép thời gian vào out_time
            strncpy(out_time, time_start, time_length);
            out_time[time_length] = '\0';
        }
    }
    else
    {
        // Không tìm thấy thời gian, gán chuỗi rỗng cho out_time
        out_time[0] = '\0';
    }
}



// ==================== Y/c 7 ========================


int getNumFromCommand(char *command)
{
    int num = -1; // Giá trị mặc định nếu không tìm thấy

    // Sử dụng sscanf để tìm và trích xuất giá trị <num>
    if (sscanf(command, "%*[^#]#%d", &num) == 1)
    {
        if (num < 0 || num >= numTasks)
        {
            printf("Lỗi: Số thứ tự không hợp lệ. (Phải nằm trong khoảng từ 0 đến %d)\n", numTasks - 1);
            return -1;
        }
        return num;
    }

    printf("Lỗi: Không tìm thấy số thứ tự trong câu lệnh.\n");
    return -1; // Trả về -1 nếu không tìm thấy hoặc lỗi
}





// ================== Y/c 8 ====================================

int getFieldFromEdit(char *edit_cmd)
{
    char *edit_title = strstr(edit_cmd, " title:[");
    char *edit_description = strstr(edit_cmd, " description:[");
    char *edit_time = strstr(edit_cmd, " time:[");
    char *edit_status = strstr(edit_cmd, " status:[");

    if (edit_title)
    {
        printf("1");
        return 1; // Thay đổi title
    }
    else if (edit_description)
    {
        return 2; // Thay đổi description
    }
    else if (edit_time)
    {
        return 3; // Thay đổi time
    }
    else if (edit_status)
    {
        return 4; // Thay đổi status
    }
    else
    {
        return 0; // Không hợp lệ
    }
}





// ================ Y/c 9 ================================


enum Status getStatusFromEdit(char *edit_cmd)
{
    enum Status status = IN_PROGRESS; // Giá trị mặc định

    // Tìm vị trí của "status:" trong lệnh edit
    char *status_start = strstr(edit_cmd, "status:");
    if (status_start)
    {
        // Di chuyển con trỏ đến sau "status:"
        status_start += strlen("status:");

        // Sử dụng sscanf để đọc trạng thái từ chuỗi
        if (sscanf(status_start, "[%d]", &status) != 1)
        {
            printf("Lỗi: Không thể trích xuất trạng thái từ lệnh edit.\n");
            // Trong trường hợp lỗi, sử dụng giá trị mặc định (IN_PROGRESS) hoặc bạn có thể xử lý lỗi khác theo nhu cầu.
        }
    }
    else
    {
        printf("Lỗi: Lệnh edit không chứa trạng thái.\n");
        // Trong trường hợp lỗi, sử dụng giá trị mặc định (IN_PROGRESS) hoặc bạn có thể xử lý lỗi khác theo nhu cầu.
    }

    return status;
}


// ================ Y/c 10 =================

void printAllTasks(struct Task *array_tasks, int no_tasks)
 // no_tasks: so luong phan tu trong mảng <=> numTasks đc khai báo ở dòng 43
 // array_tasks: mang <=> tasks được khai báo ở dòng 42
{

    for (int i = 0; i < no_tasks; i++)
    {
        struct Task *taskToDisplay = &array_tasks[i];
        printTask(taskToDisplay);
    }
}


// =================== Y/C 11 ====================================

void printTaskByNum(struct Task *array_tasks, int no_tasks, int quan)
{
    if (quan >= 0 && quan < no_tasks)
    {
        struct Task *taskToDisplay = &array_tasks[quan - 1];
        printTask(taskToDisplay);
    }
    else
    {
        printf("Số thứ tự task không hợp lệ.\n");
    }
}


// ===================== Y/c 12========================================
void printHeadTask(struct Task *array_tasks, int numTasks, int num)
{
    int printCount = (num < numTasks) ? num : numTasks; // Số lượng task cần hiển thị
    for (int i = 0; i < printCount; i++)
    {
        printf("%d. %s\n", i + 1, array_tasks[i].title);
    }
}


// =================== Y/c 13 ========================================

void printTailTask(struct Task *array_tasks, int no_tasks, int quan)
{
    int startIdx = (no_tasks > quan) ? (no_tasks - quan) : 0; // Vị trí bắt đầu
    int endIdx = no_tasks;                                  // Vị trí kết thúc

    for (int i = startIdx; i < endIdx; i++)
    {
        printf("%d. %s\n", i + 1, array_tasks[i].title);
    }
}


// =================== Y/c 14 =============================

void printFilteredTaskByTitle(struct Task *array_tasks, int no_tasks, const char *filter_title) {
    printf("Các nhiệm vụ có tiêu đề chứa \"%s\":\n", filter_title);
    for (int i = 0; i < no_tasks; i++) {
        if (strstr(array_tasks[i].title, filter_title)) {
            printTask(&array_tasks[i]);
        }
    }
}

// ==================== Y/c 15 ===========================

void printFilteredTaskByDescription(struct Task *array_tasks, int no_tasks, const char *filter_description) {
    printf("Các nhiệm vụ có mô tả chứa \"%s\":\n", filter_description);
    for (int i = 0; i < no_tasks; i++) {
        if (strstr(array_tasks[i].description, filter_description)) {
            printTask(&array_tasks[i]);
        }
    }
}


// ====================== y/c 16 =========================

void printFilteredTaskByStatus(struct Task *array_tasks, int no_tasks, enum Status filter_status) {
    printf("Các nhiệm vụ có trạng thái \"%s\":\n", status_name[filter_status]);
    for (int i = 0; i < no_tasks; i++) {
        if (array_tasks[i].status == filter_status) {
            printTask(&array_tasks[i]);
        }
    }
}




// ================== Y/c 17 ===============================

bool addTask (struct Task*array_tasks, int no_tasks, char *new_title, char *new_description, char *new_time) {
// Với no_tasks: số lượng các phần tử trong mảng 
// new_title: tiêu đề task
// new_description: mô tả task
// new_time: thời gian thực hiện task
// Yêu cầu: thêm mới task vào cuối mảng
// output: Nếu thành công thì trả về true.

if (no_tasks >= MAX_NO_TASKS) {
        // Kiểm tra xem mảng đã đầy chưa
        printf("Không thể thêm nhiệm vụ mới. Mảng đã đầy.\n");
        return false;
    }

    struct Task new_task;
    new_task.num = no_tasks + 1; // Tính số thứ tự mới cho nhiệm vụ

    if (strlen(new_title) > MAX_LENGTH_TITLE || strlen(new_description) > MAX_LENGTH_DESCRIPTION || strlen(new_time) > MAX_LENGTH_TIME) {
        // Kiểm tra độ dài các trường dữ liệu
        printf("Dữ liệu quá dài. Không thể thêm nhiệm vụ.\n");
        return false;
    }

    strcpy(new_task.title, new_title);
    strcpy(new_task.description, new_description);
    strcpy(new_task.time, new_time);

    // Thiết lập trạng thái mặc định (ví dụ: IN_PROGRESS)
    new_task.status = IN_PROGRESS;

    // Thêm nhiệm vụ mới vào mảng
    array_tasks[no_tasks] = new_task;

    // Cập nhật lại số lượng nhiệm vụ
    numTasks++;

    // printf("Them task thanh cong: %s, #%s",no_tasks, new_task.title);


    return true;
}


// ========================== y/c 18 ==========================

bool deleteTask(struct Task *array_tasks, int *no_tasks, int num)
{
    if (num < 0 || num >= *no_tasks)
    {
        // Vị trí không hợp lệ, không thể xoá
        return false;
    }

    // Di chuyển các task sau vị trí num lên để ghi đè task tại vị trí num
    for (int i = num; i < (*no_tasks - 1); i++)
    {
        array_tasks[i] = array_tasks[i + 1];
    }

    (*no_tasks)--; // Giảm số lượng task đi 1

    return true;
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
            // addTask(command);

            runTodoApp();
        }
        if (commandType == SHOW)
        {
            // If the command is ADD, call the addTask function

            if (strncmp(command, "show #", 6) == 0)
            {
                // Xử lý lệnh "show #[number]" và hiển thị task theo số thứ tự
                int num; // sử dụng getNumFrom để lấy num
                printTaskByNum(tasks, numTasks, num);
                
            }
            else if (strcmp(command, "show all") == 0)
            {
                printAllTasks(tasks, numTasks);
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


// Test hàm addTask
void testAddTask() {

    // char newTitle[MAX_LENGTH_TITLE];
    // char newDescription[MAX_LENGTH_DESCRIPTION];
    // char newTime[MAX_LENGTH_TIME];

    // // Thêm 5 nhiệm vụ
    // for (int i = 0; i < 5; i++) {
    //     snprintf(newTitle, MAX_LENGTH_TITLE, "Task %d", i + 1);
    //     snprintf(newDescription, MAX_LENGTH_DESCRIPTION, "Description for Task %d", i + 1);
    //     snprintf(newTime, MAX_LENGTH_TIME, "10:00|01/01/2023-11:00|01/01/2023");

    //     if (addTask(tasks, numTasks, newTitle, newDescription, newTime)) {
    //         // numTasks++;
    //         printf("Nhiệm vụ %d đã được thêm vào mảng.\n", i + 1);
    //     }
    // }

    
    if (addTask(tasks, numTasks, "tit 1", "des 1", "10:00|01/11/2023-11:00|03/11/2023")) {
            printf("Nhiệm vụ %d đã được thêm vào mảng.\n", numTasks);
    }
    if (addTask(tasks, numTasks, "tit 2", "des 3", "10:00|05/11/2023-11:00|07/11/2023")) {
            printf("Nhiệm vụ %d đã được thêm vào mảng.\n", numTasks);
    }
    if (addTask(tasks, numTasks, "tit 3", "des 3", "10:00|10/11/2023-11:00|13/11/2023")) {
            printf("Nhiệm vụ %d đã được thêm vào mảng.\n", numTasks);
    }
   

}


void testShowWeekTime() {
    char command[] = "show week time:[<date>]";
}

int main()
{

    // runTodoApp();
     // edit #<num> title:[<title2>]
    // edit #<num> description:[<description2>]
    // edit #<num> time:[<time2>]
    // edit #<num> status:[<status2>]


    // test hàm addTask, thêm dữ liệu ban đầu để thực hiện các hàm show, edit, delete
    testAddTask();

    // Test hàm printTask
    // printAllTasks(tasks, numTasks);

    // Test hàm filterTitle Yêu cầu 14
    printFilteredTaskByTitle(tasks, numTasks, "tit 3");


    // test show task 1
    // printTaskByNum(1);

    // printHeadTask(tasks, numTasks, 1);
    // printTailTask(tasks, numTasks, 10 );

    // printAllTasks();

    // test get rdit field

    // char title[100];
    // getTitleFromEdit("edit #1 title:[change title]",title);
    // printf(title);

    // getNumFromCommand("edit #2 title:[change title]");

    // test delete

    // if (deleteTask(tasks, &numTasks, 1))
    // {
    //     printf("Xoá task tại vị trí %d thành công.\n", 2);
    // }
    // else
    // {
    //     printf("Xoá task tại vị trí %d thất bại (vị trí không hợp lệ).\n", 2);
    // }
    // printAllTasks();

    // check getNum

    //    int num1 = getNumFromCommand("edit #2 title:[New Title]");
    //     if (num1 >= 0)
    //     {
    //         printf("Kết quả: %d\n", num1);
    //     }

    // test getTitleFromAdd
    // char command[] = "add [Example Title1] [Example Description1] [10:00|01/01/2023-11:00|01/01/2023]";

    // char title[MAX_LENGTH_TITLE + 1] = "";
    // char description[MAX_LENGTH_DESCRIPTION + 1] = "";
    // char time[MAX_LENGTH_TIME + 1] = "";

    // getTitleFromAdd(command, title);
    // getDescriptionFromAdd(command, description);
    // getTimeFromAdd(command, time);

    // printf("Title: %s\n", title);
    // printf("Description: %s\n", description);
    // printf("Time: %s\n", time);

    return 0;
}
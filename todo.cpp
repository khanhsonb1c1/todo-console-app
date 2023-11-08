#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

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
    printf("Time: %s\n", task->time);
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
        printf("Format time fail\n");

        return false; // Định dạng không đúng
    }

    // Kiểm tra giờ và phút
    if (hour < 0 || hour > 23 || minute < 0 || minute > 59)
    {
        printf("Hour/minutes fail\n");

        return false; // Giờ hoặc phút không hợp lệ
    }

    // Kiểm tra ngày và tháng
    if (month < 1 || month > 12 || day < 1 || day > 31)
    {
        printf("Day/month fail\n");

        return false; // Ngày hoặc tháng không hợp lệ
    }

    // Kiểm tra năm (ở đây chúng ta chấp nhận bất kỳ năm nào)
    if (year < 1)
    {
        printf("year fail\n");

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
    else if (strncmp(command, "delete", 6) == 0)
    {
        return DELETE;
    }
    else if (strncmp(command, "edit", 4) == 0)
    {
        return EDIT;
    }
    else
    {
        return INVALID;
    }
}

// =============== Y/c 2===============

void getTitleFromAdd(char *command, char *out_title)
{
    char title[MAX_LENGTH_TITLE + 1] = "";
    char description[MAX_LENGTH_DESCRIPTION + 1] = "";
    char time[MAX_LENGTH_TIME + 1] = "";

    if (sscanf(command, "add [%[^]]] [%[^]]] [%[^]]]", title, description, time) == 3)
    {
        // Sao chép title vào out_title
        strncpy(out_title, title, MAX_LENGTH_TITLE);
        out_title[MAX_LENGTH_TITLE] = '\0';
        // printf("title: %s, des: %s, time: %s \n", title, description, time);
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
    char title[MAX_LENGTH_TITLE + 1] = "";
    char description[MAX_LENGTH_DESCRIPTION + 1] = "";
    char time[MAX_LENGTH_TIME + 1] = "";

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
        printf("Title dont end with space.\n");
        return 0;
    }

    // Kiểm tra độ dài title không vượt quá giới hạn
    if (strlen(title) > MAX_LENGTH_TITLE)
    {
        printf("Title length so large.\n");
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
        printf("Description dont end with space.\n");
        return 0;
    }

    // Kiểm tra độ dài description không vượt quá giới hạn
    if (strlen(description) > MAX_LENGTH_DESCRIPTION)
    {
        printf("Description length so large.\n");
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
        // printf("Định dạng thời gian không hợp lệ.\n");
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
    int num; // Giá trị mặc định nếu không tìm thấy

    // Sử dụng sscanf để tìm và trích xuất giá trị <num>
    if (sscanf(command, "%*[^#]#%d", &num) == 1)
    {
        if (num < 0 || num > numTasks + 1)
        {
            printf("Lỗi: Số thứ tự không hợp lệ. (Phải nằm trong khoảng từ 0 đến %d)\n", numTasks + 1);
            return -1;
        }
        return num;
    }

    printf("Lỗi: Không tìm thấy số thứ tự trong câu lệnh.\n");
    return -1; // Trả về -1 nếu không tìm thấy hoặc lỗi
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
            printf("cannot get status\n");
            // Trong trường hợp lỗi, sử dụng giá trị mặc định (IN_PROGRESS) hoặc bạn có thể xử lý lỗi khác theo nhu cầu.
        }
    }
    else
    {
        printf("Command have not status.\n");
        // Trong trường hợp lỗi, sử dụng giá trị mặc định (IN_PROGRESS) hoặc bạn có thể xử lý lỗi khác theo nhu cầu.
    }

    return status;
}


// ================== Y/c 8 ====================================

int getFieldFromEdit(char *edit_cmd)
{
    char *edit_title = strstr(edit_cmd, " title:[");
    char *edit_description = strstr(edit_cmd, " description:[");
    char *edit_time = strstr(edit_cmd, " time:[");
    char *edit_status = strstr(edit_cmd, " status:[");

    char *editOutline;

    if (edit_title)
    {
        getTitleFromEdit(edit_cmd, editOutline);
        return 1; // Thay đổi title
    }
    else if (edit_description)
    {
        getDescriptionFromEdit(edit_cmd, editOutline);
        return 2; // Thay đổi description
    }
    else if (edit_time)
    {   getTitleFromEdit(edit_cmd, editOutline);
        return 3; // Thay đổi time
    }
    else if (edit_status)
    {
        getStatusFromEdit(edit_cmd);
        return 4; // Thay đổi status
    }
    else
    {
        return 0; // Không hợp lệ
    }
}

// ================ Y/c 10 =================

void printAllTasks(struct Task *array_tasks, int no_tasks)
// no_tasks: so luong phan tu trong mảng <=> numTasks đc khai báo ở dòng 43
// array_tasks: mang <=> tasks được khai báo ở dòng 42
{
    int i;
    for ( i = 0; i < no_tasks; i++)
    {
        struct Task *taskToDisplay = &array_tasks[i];
        printTask(taskToDisplay);
    }
}

// =================== Y/C 11 ====================================

void printTaskByNum(struct Task *array_tasks, int no_tasks, int quan)
{
    printf("No task: %d \n", no_tasks);
    printf("Quantity: %d \n", quan);

    if (quan > 0 && quan <= no_tasks)
    {
        struct Task *taskToDisplay = &array_tasks[quan - 1];
        printTask(taskToDisplay);
    }
    else
    {
        printf("Number in task not found \n");
    }
}

// ===================== Y/c 12========================================
void printHeadTask(struct Task *array_tasks, int numTasks, int num)
{
    int printCount = (num < numTasks) ? num : numTasks; // Số lượng task cần hiển thị
    int i;
    for (i = 0; i < printCount; i++)
    {
        printf("%d. %s\n", i + 1, array_tasks[i].title);
    }
}

// =================== Y/c 13 ========================================

void printTailTask(struct Task *array_tasks, int no_tasks, int quan)
{
    int startIdx = (no_tasks > quan) ? (no_tasks - quan) : 0; // Vị trí bắt đầu
    int endIdx = no_tasks;                                    // Vị trí kết thúc
    int i;
    for (i = startIdx; i < endIdx; i++)
    {
        printf("%d. %s\n", i + 1, array_tasks[i].title);
    }
}

// =================== Y/c 14 =============================

void printFilteredTaskByTitle(struct Task *array_tasks, int no_tasks, const char *filter_title)
{
    printf("task has title: \"%s\":\n", filter_title);
    int i;
    for (i = 0; i < no_tasks; i++)
    {
        if (strstr(array_tasks[i].title, filter_title))
        {
            printTask(&array_tasks[i]);
        }
    }
}

// ==================== Y/c 15 ===========================

void printFilteredTaskByDescription(struct Task *array_tasks, int no_tasks, const char *filter_description)
{
    printf("task has description: \"%s\":\n", filter_description);
    int i;
    for (i = 0; i < no_tasks; i++)
    {
        if (strstr(array_tasks[i].description, filter_description))
        {
            printTask(&array_tasks[i]);
        }
    }
}

// ====================== y/c 16 =========================

void printFilteredTaskByStatus(struct Task *array_tasks, int no_tasks, enum Status filter_status)
{
    printf("task has status: \"%s\":\n", status_name[filter_status]);
    int i;
    for (i = 0; i < no_tasks; i++)
    {
        if (array_tasks[i].status == filter_status)
        {
            printTask(&array_tasks[i]);
        }
    }
}

// ================== Y/c 17 ===============================

bool addTask(struct Task *array_tasks, int no_tasks, char *new_title, char *new_description, char *new_time)
{
    // Với no_tasks: số lượng các phần tử trong mảng
    // new_title: tiêu đề task
    // new_description: mô tả task
    // new_time: thời gian thực hiện task
    // Yêu cầu: thêm mới task vào cuối mảng
    // output: Nếu thành công thì trả về true.

    if (no_tasks >= MAX_NO_TASKS)
    {
        // Kiểm tra xem mảng đã đầy chưa
        printf("List size is max.\n");
        return false;
    }

    struct Task new_task;
    new_task.num = no_tasks + 1; // Tính số thứ tự mới cho nhiệm vụ

    if (strlen(new_title) > MAX_LENGTH_TITLE || strlen(new_description) > MAX_LENGTH_DESCRIPTION || strlen(new_time) > MAX_LENGTH_TIME)
    {
        // Kiểm tra độ dài các trường dữ liệu
        printf("Data so length.\n");
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
    int i;
    for ( i = num; i < (*no_tasks - 1); i++)
    {
        array_tasks[i] = array_tasks[i + 1];
    }

    (*no_tasks)--; // Giảm số lượng task đi 1

    return true;
}

// ------ End: Student Answer ------

// ================== Y/c 19 ==================

int dayOfWeekIndex(int day, int month, int year, const char *daysOfWeek[])
{
    if (month < 3)
    {
        month += 12;
        year--;
    }
    int k = year % 100;
    int j = year / 100;
    int dayIndex = (day + 13 * (month + 1)) / 5 + k + k / 4 + j / 4 + 5 * j;
    dayIndex %= 7;

    return dayIndex;
}

void printWeekCalendar(struct Task *list_task_in_week, int count)
{
    // Các ngày trong tuần
    const char *daysOfWeek[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
    int numDays = 7;

    // Tính toán khoảng trống trước và sau chuỗi "Time"
    int timePadding = (WEEK_CELL_FIRST_COL_WIDTH - strlen("Time")) / 2;

    if ((WEEK_CELL_FIRST_COL_WIDTH - strlen("Time")) % 2 != 0)
    {
        timePadding--; // Trừ đi nếu có số lẻ để làm tròn xuống (floor)
    }

    // In phần tiêu đề hàng đầu tiên
    printf("%*sTime%*s", timePadding, "", timePadding, "");

    int i;
    for ( i = 0; i < numDays; i++)
    {
        int padding = (WEEK_CELL_OTHER_COL_WIDTH - strlen(daysOfWeek[i])) / 2;
        printf("%*s%s%*s", padding, "", daysOfWeek[i], padding, "");
    }

    printf("\n");

    int printedTitles[MAX_NO_TASKS] = {0};

    int hour;
    for (hour = 1; hour <= 24; hour++)
    {
        // Chuyển đổi giờ thành chuỗi và thêm "0" nếu cần
        char hourStr[3];
        if (hour < 10)
        {
            hourStr[0] = '0';
            hourStr[1] = hour + '0';
            hourStr[2] = '\0';
        }
        else
        {
            hourStr[0] = hour / 10 + '0';
            hourStr[1] = hour % 10 + '0';
            hourStr[2] = '\0';
        }

        // In thời gian
        int firstColSpaces = (WEEK_CELL_FIRST_COL_WIDTH - 2) / 2;
        printf("%*s%s%*s", firstColSpaces, "", hourStr, firstColSpaces, "");
        int i;
        for (i = 0; i < numDays; i++)
        {
            int otherColSpaces = (WEEK_CELL_OTHER_COL_WIDTH - 1) / 2;
            printf("%*s", otherColSpaces, "");

            int isTaskPrinted = 0;
            int j;
            for (j = 0; j < count; j++)
            {
                int taskHourStart, taskMinStart, taskHourEnd, taskMinEnd;
                int dayStart, monthStart, yearStart, dayEnd, monthEnd, yearEnd;

                if (sscanf(list_task_in_week[j].time, "%d:%d|%*d/%*d/%*d-%d:%d|%*d/%*d/%*d",
                           &taskHourStart, &taskMinStart, &taskHourEnd, &taskMinEnd) == 4)
                {

                    if ((hour == taskHourStart && taskMinStart == 0) || (hour == taskHourEnd && taskMinEnd == 0))
                    {
                        if (sscanf(list_task_in_week[j].time, "%*d:%*d|%d/%d/%d-%*d:%*d|%d/%d/%d", &dayStart, &monthStart, &yearStart, &dayEnd, &monthEnd, &yearEnd) == 6)
                        {
                            // Kiểm tra ngày bắt đầu và ngày kết thúc để xác định cột thích hợp
                            if (i == dayOfWeekIndex(dayStart, monthStart, yearStart, daysOfWeek) ||
                                i == dayOfWeekIndex(dayEnd, monthEnd, yearEnd, daysOfWeek))
                            {
                                // In title và đánh dấu là đã in
                                if (!printedTitles[j])
                                {
                                    int titleLength = strlen(list_task_in_week[j].title);
                                    if (titleLength > WEEK_CELL_OTHER_COL_WIDTH)
                                    {
                                        printf("%.17s...", list_task_in_week[j].title);
                                    }
                                    else
                                    {
                                        int spacing = WEEK_CELL_OTHER_COL_WIDTH - titleLength;
                                        int leftSpaces = spacing / 2;
                                        int rightSpaces = spacing - leftSpaces;
                                        printf("%*s%s%*s", leftSpaces, "", list_task_in_week[j].title, rightSpaces, "");
                                    }
                                    printedTitles[j] = 1;
                                }
                                isTaskPrinted = 1;
                            }
                        }
                    }
                }
            }

            if (!isTaskPrinted)
            {
                int spacing = WEEK_CELL_OTHER_COL_WIDTH;
                printf("%*s", spacing, "");
            }
        }

        printf("\n");
    }
}

void printWeekTime(struct Task *tasks, int no_tasks, char *date)
{
    // Lấy ngày trong tuần từ chuỗi date (ví dụ: "tus-15/11/2023")
    char weekday[4];
    int day, month, year;
    if (sscanf(date, "%3s-%d/%d/%d", weekday, &day, &month, &year) != 4)
    {
        printf("Chuoi ngay khong hop le.\n");
        return;
    }

    // Xác định thứ trong tuần dựa vào chuỗi weekday (ví dụ: "tus" cho thứ Ba)
    const char *daysOfWeek[] = {"mon", "tus", "wed", "thu", "fri", "sat", "sun"};
    int dayOfWeek = -1;
    int i;
    for (i = 0; i < 7; i++)
    {
        if (strcmp(weekday, daysOfWeek[i]) == 0)
        {
            dayOfWeek = i;
            break;
        }
    }
    if (dayOfWeek == -1)
    {
        printf("Thu tu khong hop le.\n");
        return;
    }

    // Tính toán ngày bắt đầu và kết thúc của tuần
    int startOfWeek = day - dayOfWeek;
    int endOfWeek = startOfWeek + 6;

    // Khai báo mảng listTaskInWeek để lưu trữ các công việc trong tuần
    struct Task listTaskInWeek[MAX_NO_TASKS];
    int count = 0;

    // Duyệt qua danh sách công việc
    int k;
    for ( k = 0; i < no_tasks; i++)
    {
        int taskDayStart, taskMonthStart, taskYearStart;
        int taskDayEnd, taskMonthEnd, taskYearEnd;
        int taskHourStart, taskMinStart, taskHourEnd, taskMinEnd;

        if (sscanf(tasks[k].time, "%d:%d|%d/%d/%d-%d:%d|%d/%d/%d",
                   &taskHourStart, &taskMinStart, &taskDayStart, &taskMonthStart, &taskYearStart,
                   &taskHourEnd, &taskMinEnd, &taskDayEnd, &taskMonthEnd, &taskYearEnd) != 10)
        {
            printf("Error in format time.\n");
            continue;
        }

        // Kiểm tra xem taskMinStart hoặc taskMinEnd có khác 00 hay không
        if (taskMinStart != 0 || taskMinEnd != 0)
        {
            printf("error: gio phai la so chan (ex: 10:00).\n");
            continue;
        }

        // So sánh ngày công việc với khoảng thời gian của tuần
        if ((taskYearStart == year && taskMonthStart == month && taskDayStart >= startOfWeek && taskDayStart <= endOfWeek) ||
            (taskYearEnd == year && taskMonthEnd == month && taskDayEnd >= startOfWeek && taskDayEnd <= endOfWeek))
        {
            // Nếu công việc nằm trong khoảng thời gian của tuần, thêm vào mảng listTaskInWeek
            listTaskInWeek[count] = tasks[k];
            count++;
        }
    }

    // In ra danh sách công việc trong tuần
    if (count == 0)
    {
        printf("Khong co cong viec trong tuan.\n");
    }
    else
    {
        printf("Danh sach cong viec:\n");
        int i;
        for ( i = 0; i < count; i++)
        {
            printf("Task %d: %s\n", i + 1, listTaskInWeek[i].time);
        }

        // Gọi hàm in lịch tuần
        printWeekCalendar(listTaskInWeek, count);
    }
}

// ================== Y/c 20 ====================================

// Test hàm addTask
void testAddTask()
{

    if (addTask(tasks, numTasks, "title number 1", "description number 1", "10:00|01/11/2023-11:00|18/11/2023"))
    {
        printf("Nhiệm vụ %d đã được thêm vào mảng.\n", numTasks);
    }
    if (addTask(tasks, numTasks, "title number 2", "description number 2`", "09:00|05/11/2023-11:00|07/11/2023"))
    {
        printf("Nhiệm vụ %d đã được thêm vào mảng.\n", numTasks);
    }
    if (addTask(tasks, numTasks, "title number 3", "description number 3", "05:00|14/11/2023-11:00|16/11/2023"))
    {
        printf("Nhiệm vụ %d đã được thêm vào mảng.\n", numTasks);
    }
}

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

            // ex: add [Example Title1] [Example Description1] [10:00|11/11/2023-11:00|12/11/2023]
            char title[MAX_LENGTH_TITLE + 1] = "";
            char description[MAX_LENGTH_DESCRIPTION + 1] = "";
            char time[MAX_LENGTH_TIME + 1] = "";

            int checkTit = checkTitle(title);
            int checkDes = checkTitle(description);
            int checkTim = checkTitle(time);

            getTitleFromAdd(command, title);
            getTimeFromAdd(command, time);
            getDescriptionFromAdd(command, description);

            if (checkTit == -1 && checkDes == -1 && checkTim == -1)
            {

                if (addTask(tasks, numTasks, title, time, description))
                {
                    printf("Add task successfully.\n");
                }
            }
            else
            {
                printf("Invalid task data. Task not added.\n");
            }
            runTodoApp();
            break;
        }
        if (commandType == SHOW)
        {
            // If the command is ADD, call the addTask function

            if (strncmp(command, "show #", 6) == 0)
            {
                // Xử lý lệnh "show #[number]" và hiển thị task theo số thứ tự
                int num = getNumFromCommand(command); // sử dụng getNumFrom để lấy num
                printTaskByNum(tasks, numTasks, num);
            }
            else if (strcmp(command, "show all") == 0)
            {
                printAllTasks(tasks, numTasks);
            }

            else if (strncmp(command, "show head", 9) == 0)
            {
                int num = getNumFromCommand(command);

                printHeadTask(tasks, numTasks, num);
            }

            else if (strncmp(command, "show tail", 9) == 0)
            {
                int num = getNumFromCommand(command);

                printTailTask(tasks, numTasks, num);
            }

            else if (strncmp(command, "show filter description", 23) == 0)
            {
                char description[MAX_LENGTH_DESCRIPTION];
                if (sscanf(command, "show filter description:%[^]]", description) == 1)
                {
                    printFilteredTaskByDescription(tasks, numTasks, description);
                }
                printf("fail get description\n");
            }

            else if (strncmp(command, "show filter status", 18) == 0)
            {
                enum Status status;

                if (sscanf(command, "show filter status:%[^]]", status) == 1)
                {
                    printFilteredTaskByStatus(tasks, numTasks, status);
                }
                printf("fail get status\n");
            }

            else if (strncmp(command, "show week time", 14) == 0)
            {

                // ex: show week time:wed-15/11/2023

                char date[MAX_LENGTH_TIME] = "";

                if (sscanf(command, "show week time:%[^]]", date) == 1)
                {
                    printWeekTime(tasks, numTasks, date);
                    return;
                }
                printf("fail get date \n");
            }
            else
            {
                printf("Command show no found.\n");
            }

            runTodoApp();
        }
        else if (commandType == QUIT)
        {
            printf("Exit program.\n");

            // If the command is QUIT, exit the loop
            break;
        }
        else if (commandType == EDIT)
        {
            getFieldFromEdit(command);

            runTodoApp();
        }
        else if (commandType == DELETE)
        {
            int num = getNumFromCommand(command);

            if (deleteTask(tasks, &numTasks, num))
            {
                printf("Delete successfully. \n");
            }

            // If the command is QUIT, exit the loop
            runTodoApp();
        }
        else
        {
            // Handle other commands
            printf("Command not support zzz.\n");
            runTodoApp();
        }

        break;
    }
}

int main()
{
    testAddTask();
    runTodoApp();

    // edit #<num> title:[<title2>]
    // edit #<num> description:[<description2>]
    // edit #<num> time:[<time2>]
    // edit #<num> status:[<status2>]

    // test printWeekTime
    // printWeekTime(tasks, numTasks, "wed-15/11/2023");

    // Test hàm printTask
    // printAllTasks(tasks, numTasks);

    // Test hàm filterTitle Yêu cầu 14
    // printFilteredTaskByTitle(tasks, numTasks, "tit 3");

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
    //    int num1 = getNumFromCommand("show #0");
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



// show all
// show #1
// show tail #1
// show head #1
// show filter title:3

// delete #2

// add [Example Title1] [Example Description1] [10:00|01/01/2023-11:00|01/01/2023]

// show week time:wed-15/11/2023
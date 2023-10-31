#include <iostream>
#include <string>
#include <sstream>

enum CommandType
{
    QUIT,
    SHOW,
    ADD,
    INVALID
};

// Định nghĩa các hằng số kích thước tối đa cho title, description, và time
const int MAX_LENGTH_TITLE = 50;
const int MAX_LENGTH_DESCRIPTION = 100;
const int MAX_LENGTH_TIME = 20;

enum Status
{
    PENDING,
    COMPLETED
};

struct Task
{
    int num;
    char title[MAX_LENGTH_TITLE + 1];
    char description[MAX_LENGTH_DESCRIPTION + 1];
    char time[MAX_LENGTH_TIME + 1];
    enum Status status;
};

std::vector<Task> tasks; // Danh sách nhiệm vụ (biến toàn cục)


// Hàm kiểm tra xem title hợp lệ hay không
bool checkTitle(const std::string &title) {
    if (title.empty() || title.length() > MAX_LENGTH_TITLE) {
        std::cout << "Title vượt quá độ dài cho phép." << std::endl;
        return false;
    }
    // Kiểm tra xem title bắt đầu hoặc kết thúc bằng ký tự khoảng trắng
    if (!title.empty() && (title[0] == ' ' || title[title.length() - 1] == ' ')) {
        std::cout << "Title không được bắt đầu hoặc kết thúc bằng ký tự khoảng trắng." << std::endl;

        return false;
    }
    return true;
}
// Hàm kiểm tra xem title hợp lệ hay không
bool checkDescription(const std::string &description) {
    if (description.empty() || description.length() > MAX_LENGTH_DESCRIPTION) {
        std::cout << "description vượt quá độ dài cho phép." << std::endl;
        return false;
    }
    // Kiểm tra xem title bắt đầu hoặc kết thúc bằng ký tự khoảng trắng
    if (!description.empty() && (description[0] == ' ' || description[description.length() - 1] == ' ')) {
        std::cout << "description không được bắt đầu hoặc kết thúc bằng ký tự khoảng trắng." << std::endl;

        return false;
    }
    return true;
}

bool isValidDate(const std::string &date) {
    // Định dạng ngày tháng: hh/mm|dd/mo/yyyy

    if (date.length() != 10) {
        return false; // Độ dài phải là 10 ký tự
    }

    if (date[2] != '/' || date[5] != '|' || date[8] != '/') {
        return false; // Các vị trí ký tự / và | phải chính xác
    }

    int hour, minute, day, month, year;

    if (std::istringstream(date.substr(0, 2)) >> hour &&
        std::istringstream(date.substr(3, 2)) >> minute &&
        std::istringstream(date.substr(6, 2)) >> day &&
        std::istringstream(date.substr(9, 4)) >> year) {
        // Kiểm tra giá trị của hour, minute, day, month, year nếu cần
        // Ví dụ: hour phải trong khoảng từ 00 đến 23, month phải trong khoảng từ 01 đến 12, ...

        // Kiểm tra tháng và năm có giá trị hợp lệ
        if (month < 1 || month > 12) {
            return false; // Tháng không hợp lệ
        }

        if (year < 1900 || year > 2100) {
            return false; // Năm không hợp lệ
        }

        // Kiểm tra ngày hợp lệ dựa trên tháng và năm
        bool isLeapYear = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));

        if (day < 1 || (day > 28 && month == 2 && !isLeapYear) ||
            (day > 29 && month == 2 && isLeapYear) ||
            (day > 30 && (month == 4 || month == 6 || month == 9 || month == 11)) ||
            (day > 31)) {
            return false; // Ngày không hợp lệ
        }

        // Kiểm tra giờ và phút nếu cần
        if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
            return false; // Giờ hoặc phút không hợp lệ
        }

        return true; // Ngày tháng hợp lệ
    }

    return false; // Không thể chuyển đổi thành số
}


bool checkTime(const std::string &time) {
    // Kiểm tra định dạng của chuỗi `time`
    if (time.length() < 10 || time[2] != '/' || time[5] != '|' || time[10] != '-' || time[13] != '/' || time[16] != '/' || time.length() != 19) {
        std::cout << "Định dạng thời gian không hợp lệ." << std::endl;
        return false;
    }

    // Tách datetime1 và datetime2 từ chuỗi `time`
    std::string datetime1 = time.substr(0, 10);
    std::string datetime2 = time.substr(11);

    // Kiểm tra định dạng của datetime1 và datetime2
    if (!isValidDate(datetime1) || !isValidDate(datetime2)) {
        std::cout << "Định dạng ngày tháng không hợp lệ." << std::endl;
        return false;
    }

    // Kiểm tra datetime2 luôn lớn hơn datetime1
    if (datetime1 > datetime2) {
        std::cout << "datetime2 phải lớn hơn datetime1." << std::endl;
        return false;
    }

    return true;
}

void AddTask(const std::string &option)
{
    // Kiểm tra xem option có đúng định dạng không
    if (option.find("add [") == 0 && option.find("] [") != std::string::npos)
    {
        size_t titleStart = option.find("[") + 1;
        size_t titleEnd = option.find("]", titleStart);

        size_t descStart = option.find("[", titleEnd) + 1;
        size_t descEnd = option.find("]", descStart);

        size_t timeStart = option.find("[", descEnd) + 1;
        size_t timeEnd = option.find("]", timeStart);

        if (titleEnd != std::string::npos && descEnd != std::string::npos && timeEnd != std::string::npos)
        {
            std::string title = option.substr(titleStart, titleEnd - titleStart);
            std::string description = option.substr(descStart, descEnd - descStart);
            std::string time = option.substr(timeStart, timeEnd - timeStart);

            Task newTask;
            newTask.num = tasks.size() + 1;
            newTask.status = PENDING;

            if (checkTitle(title) == false) {
                return;
            }

            if (checkDescription(description) == false) {
                return;
            }

            // Đảm bảo sao chép không vượt quá giới hạn của các trường
            strncpy(newTask.title, title.c_str(), MAX_LENGTH_TITLE);
            newTask.title[MAX_LENGTH_TITLE] = '\0';

            strncpy(newTask.description, description.c_str(), MAX_LENGTH_DESCRIPTION);
            newTask.description[MAX_LENGTH_DESCRIPTION] = '\0';

            strncpy(newTask.time, time.c_str(), MAX_LENGTH_TIME);
            newTask.time[MAX_LENGTH_TIME] = '\0';

            tasks.push_back(newTask);

            std::cout << newTask.title << std::endl;
        }
        else
        {
            std::cout << "Lệnh 'add' không hợp lệ." << std::endl;
        }
    }
    else
    {
        std::cout << "Lệnh 'add' không hợp lệ." << std::endl;
    }
}

// Hàm để in danh sách các nhiệm vụ
// Hàm để in danh sách các nhiệm vụ hoặc thông tin của một nhiệm vụ tại vị trí được chỉ định
void PrintTasks(const std::string &option)
{
    // Kiểm tra xem option có đúng định dạng "show #n" không
    if (option.find("show #") == 0)
    {
        int taskIndex;
        if (std::istringstream(option.substr(6)) >> taskIndex && taskIndex >= 1 && taskIndex <= tasks.size())
        {
            const Task &task = tasks[taskIndex - 1];
            std::cout << "Task #" << task.num << ": " << task.title << std::endl;
            std::cout << "Description: " << task.description << std::endl;
            std::cout << "Time: " << task.time << std::endl;
            std::cout << "Status: " << (task.status == PENDING ? "Pending" : "Completed") << std::endl;
        }
        else
        {
            std::cout << "Lựa chọn task không hợp lệ." << std::endl;
        }
    }
    else
    {
        std::cout << "Lệnh 'show' không hợp lệ." << std::endl;
    }
}

void showMenu()
{
    std::cout << "1. Add Task" << std::endl;
    std::cout << "2. Show Task" << std::endl;
    std::cout << "3. Show All Tasks" << std::endl;
    std::cout << "4. Quit" << std::endl;
    std::cout << "Enter your choice: ";
}

CommandType getCommandType()
{   
    showMenu();
    std::string option;
    std::getline(std::cin, option);

    if (option == "quit")
    {
        std::cout << "Thoat chuong trinh" << std::endl;
        // return QUIT;
    }
    else if (option.substr(0, 5) == "show ")
    {
        PrintTasks(option);
        getCommandType();
    }
    else if (option.substr(0, 4) == "add ")
    {
        AddTask(option);
        getCommandType();
    }
    else
    {
        std::cout << "Lệnh không hợp lệ" << std::endl;
       getCommandType();
    }
}

int main()
{

    // AddTask(tasks, "Task 1", "Description 1", "2023-10-30");
    // AddTask(tasks, "Task 2", "Description 2", "2023-10-31");

    getCommandType();
    // return 0;
}

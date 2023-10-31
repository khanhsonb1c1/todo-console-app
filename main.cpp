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
const int MAX_LENGTH_TITLE = 100;
const int MAX_LENGTH_DESCRIPTION = 200;
const int MAX_LENGTH_TIME = 33;

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
    if (date.length() != 16) {

        return false; // Độ dài chuỗi không đúng
    }

    // Kiểm tra định dạng của chuỗi date
    if (date[2] != ':' || date[5] != '|' || date[8] != '/' || date[11] != '/') {

        return false; // Định dạng không đúng
    }

    // Chuyển đổi và kiểm tra giờ và phút
    int hour, minute;
    std::istringstream(date.substr(0, 2)) >> hour;
    std::istringstream(date.substr(3, 2)) >> minute;

    // std::cout<<hour<<"___" << minute;

    if (hour < 0 || hour > 23 || minute < 0 || minute > 60 ) {

        return false; // Giờ hoặc phút không hợp lệ
    }


    // Chuyển đổi và kiểm tra ngày, tháng và năm
    int day, month, year;
    std::istringstream(date.substr(6, 2)) >> day;
    std::istringstream(date.substr(9, 2)) >> month;
    std::istringstream(date.substr(12, 4)) >> year;

    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {

        return false; // Ngày, tháng hoặc năm không hợp lệ
    }

    return true; // Chuỗi đúng định dạng
}


bool checkTime(const std::string &time) {
    // Kiểm tra định dạng datetime1 và datetime2
    if (time.length() < 35) {
        std::cout << "Chuỗi quá ngắn để chứa datetime1 và datetime2." << std::endl;
        return false;
    }
    std::string datetime1 = time.substr(0, 16);  // Lấy từ ký tự đầu tiên đến ký tự thứ 16
    std::string datetime2 = time.substr(19);     // Bắt đầu từ ký tự thứ 19

    // if (datetime1.length() != 16 || datetime2.length() != 16) {
    //     std::cout << "Định dạng thời gian không hợp lệ." << std::endl;
    //     return false;
    // }

    // if (datetime1[2] != ':' || datetime1[5] != '|' || datetime1[10] != '/' || datetime1[13] != '/' ||
    //     datetime2[2] != ':' || datetime2[5] != '|' || datetime2[10] != '/' || datetime2[13] != '/') {
    //     std::cout << "Định dạng thời gian không hợp lệ." << std::endl;
    //     return false;
    // }

    // Tách datetime1 và datetime2 từ chuỗi `time`

    // Kiểm tra định dạng của datetime1 và datetime2
    if (isValidDate(datetime1) && isValidDate(datetime2)) {
        // Kiểm tra datetime2 luôn lớn hơn datetime1
        // if (datetime1 > datetime2) {
        //     std::cout << "datetime2 phải lớn hơn datetime1." << std::endl;
        //     return false;
        // }
        return true;
    } else {
        std::cout << "Định dạng ngày tháng không hợp lệ." << std::endl;
        return false;
    }
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

            if (checkTime(time) == false) {
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
            std::cout << "Lệnh 'add' không hợp lệ (1)" << std::endl;
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
    std::cout << "1. add new (add [title] [description] [time])" << std::endl;
    std::cout << "2. Show Task (show #1)" << std::endl;
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


    // checkTime("10:10|10/10/2023 - 15:10|10/10/2023");
    // checkTime("927423");
    // isValidDate("11:01|01/01/2023");

    getCommandType();
    // return 0;
}

#include <iostream>
#include <string>
#include <sstream>

enum CommandType {
    QUIT,
    SHOW,
    ADD,
    INVALID
};


// Định nghĩa các hằng số kích thước tối đa cho title, description, và time
const int MAX_LENGTH_TITLE = 50;
const int MAX_LENGTH_DESCRIPTION = 100;
const int MAX_LENGTH_TIME = 20;

enum Status {
    PENDING,
    COMPLETED
};


struct Task {
    int num;
    char title[MAX_LENGTH_TITLE + 1];
    char description[MAX_LENGTH_DESCRIPTION + 1];
    char time[MAX_LENGTH_TIME + 1];
    enum Status status;
};



// Hàm để thêm một nhiệm vụ mới vào danh sách
void AddTask(Task tasks[], int& numTasks, const char* option) {
    // if (numTasks >= MAX_NUM_TASKS) {
    //     std::cout << "Danh sách nhiệm vụ đã đầy." << std::endl;
    //     return;
    // }

    Task newTask;
    newTask.num = numTasks + 1;
    newTask.status = PENDING; // Mặc định, có thể thay đổi nếu cần

    // Tách các thành phần từ option sử dụng dấu khoảng trắng làm phân cách
    char title[MAX_LENGTH_TITLE + 1];
    char description[MAX_LENGTH_DESCRIPTION + 1];
    char time[MAX_LENGTH_TIME + 1];

    // Sử dụng sscanf để trích xuất thông tin từ option
    if (sscanf(option, "add [%[^]]] [%[^]]] [%[^]]]", title, description, time) == 3) {
        // Đảm bảo sao chép không vượt quá giới hạn của các trường
        strncpy(newTask.title, title, MAX_LENGTH_TITLE);
        newTask.title[MAX_LENGTH_TITLE] = '\0';

        strncpy(newTask.description, description, MAX_LENGTH_DESCRIPTION);
        newTask.description[MAX_LENGTH_DESCRIPTION] = '\0';

        strncpy(newTask.time, time, MAX_LENGTH_TIME);
        newTask.time[MAX_LENGTH_TIME] = '\0';

        tasks[numTasks] = newTask;
        numTasks++;
    } else {
        std::cout << "Lỗi: Cú pháp không hợp lệ. Sử dụng cú pháp 'add [<title>] [<description>] [<time>]'." << std::endl;
    }
}


CommandType getCommandType(const std::string& option) {
    if (option == "quit") {
        return QUIT;
    } else if (option.substr(0, 5) == "show ") {
        return SHOW;
    } else if (option.substr(0, 4) == "add ") {
        return ADD;
    } else {
        return INVALID;
    }
}

int main() {
    std::string option;
    std::cout << "Nhập lệnh: ";
    std::getline(std::cin, option);

    CommandType type = getCommandType(option);

    switch (type) {
        case QUIT:
            std::cout << "Thoát ứng dụng" << std::endl;
            break;
        case SHOW:
            std::cout << "SHOW FUNCTION" << std::endl;
            break;
        case ADD:
            std::cout << "Thêm mới" << std::endl;
            break;
        case INVALID:
            std::cout << "Lệnh không hợp lệ" << std::endl;
            break;
    }

    return 0;
}

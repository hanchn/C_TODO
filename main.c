#include "student.h"

int main() {
    StudentSystem system;
    int choice;
    
    // 初始化系统
    init_system(&system);
    
    // 程序启动时自动加载数据
    printf("欢迎使用学生管理系统！\n");
    load_from_file(&system);
    
    // 主循环
    while (1) {
        display_menu();
        
        if (scanf("%d", &choice) != 1) {
            printf("输入错误，请输入数字！\n");
            clear_input_buffer();
            continue;
        }
        
        switch (choice) {
            case 1:
                add_student(&system);
                break;
            case 2:
                delete_student(&system);
                break;
            case 3:
                modify_student(&system);
                break;
            case 4:
                display_all_students(&system);
                break;
            case 5:
                search_student(&system);
                break;
            case 6:
                sort_students(&system);
                break;
            case 7:
                calculate_statistics(&system);
                break;
            case 8:
                save_to_file(&system);
                break;
            case 9:
                load_from_file(&system);
                break;
            case 0:
                printf("是否保存数据？(y/n): ");
                char save_choice;
                scanf(" %c", &save_choice);
                if (save_choice == 'y' || save_choice == 'Y') {
                    save_to_file(&system);
                }
                printf("感谢使用学生管理系统！再见！\n");
                return 0;
            default:
                printf("无效选择，请重新输入！\n");
                break;
        }
        
        // 暂停，等待用户按键
        printf("\n按回车键继续...");
        clear_input_buffer();
        getchar();
    }
    
    return 0;
}
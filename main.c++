#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

int list_dir()
{
    char buffer[FILENAME_MAX];
    char *current_dir = getcwd(buffer, FILENAME_MAX);

    DIR *directory = opendir(current_dir);

    if (directory == nullptr)
    {
        std::cerr << "Erro ao abrir diretorio atual." << std::endl;
        return 1;
    }

    dirent *entry;
    while ((entry = readdir(directory)) != nullptr)
    {
        std::cout << entry->d_name << std::endl;
    }

    closedir(directory);
    return 0;
}

void change_dir(char *dir)
{
    if (chdir(dir) == 0)
    {
        std::cout << "Diretorio: " << dir << std::endl;
    }
    else
    {
        std::cout << "Erro ao mudar de diretório" << std::endl;
    }
}

void print_dir()
{
    char buffer[FILENAME_MAX];
    char *current_dir = getcwd(buffer, FILENAME_MAX);
    std::cout << current_dir << std::endl;
}

int main(int argc, char *argv[])
{
    if (strcmp(argv[1], "ls") == 0)
    {
        list_dir();
    }

    else if (strcmp(argv[1], "cd") == 0)
    {
        change_dir(argv[2]);
    }
    else if (strcmp(argv[1], "pwd") == 0)
    {
        print_dir();
    }
}
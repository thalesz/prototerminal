#include <windows.h>
#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#define TAM 10000


// Imprime conteúdo de um arquivo
int cat(char *filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo" << filename << std::endl;
        return 1;
    }

    std::string line;
    std::cout << "Conteudo do arquivo: " << std::endl;
    while (std::getline(file, line))
    {
        std::cout << line << std::endl;
    }

    file.close();
    return 0;
}

// Lista pastas e arquivos do diretório atual
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

// Muda diretório
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

// Imprime caminho completo do diretório atual
void print_dir()
{
    char buffer[FILENAME_MAX];
    char *current_dir = getcwd(buffer, FILENAME_MAX);
    std::cout << current_dir << std::endl;
}

int main(/*int argc, char *entrada[]*/)
{

    //setLocale(LC_ALL,"");
    //
    char* entrada = new char[TAM * sizeof(char*)];

    char buffer[MAX_PATH];
    
    //scanf("%c", )
    while(strcmp(entrada, "exit")){

        GetCurrentDirectory(MAX_PATH, buffer);
        std::cout << buffer << "\\" << "aaaa ";
        std::cin.getline(entrada, TAM);
        
        if (strcmp(entrada, "ls") == 0)
        {
            list_dir();
        }else if (strncmp(entrada, "cd", 2) == 0)
        {
            if (!entrada[3])
            {
                std::cerr << "Caminho faltando: cd [CAMINHO]" << std::endl;
            }
            else
            {
                change_dir(&entrada[3]);
            }
        }
        else if (strcmp(&entrada[1], "pwd") == 0)
        {
            print_dir();
        }
        else if (strncmp(entrada, "cat", 3) == 0)
        {
            if (!entrada[4])
            {
                std::cerr << "Nome do arquivo faltando: cat [NOME_ARQUIVO]" << std::endl;
            }
            else
            {

                cat(&entrada[4]);
            }
        }
        else if(strcmp(entrada, "exit"))
        {
            std::cerr << "Comando não encontrado" << std::endl;
        }
        //std::cerr << "\n" << std::endl;
    }
    delete[] entrada;   
}
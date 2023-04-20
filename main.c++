#include <iostream>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <sys/wait.h>
#include <window.h>
#define TAM 10000

void head(char *input) {
    std::istringstream iss(input);
    std::string arg1, arg2, arg3;

    iss >> arg1 >> arg2 >> arg3;

    std::ifstream file;
    int num_lines = 10; // número padrão de linhas

    // Verifica se o segundo argumento é uma opção
    if (arg1 == "-n") {
        // Lê o número de linhas a partir do argumento -n
        num_lines = std::stoi(arg2);
        arg2 = arg3;
    }
    else {
        arg2 = arg1;
    }

    file.open(arg2);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << arg2 << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;

    // Lê as primeiras n linhas do arquivo
    for (int i = 0; i < num_lines && std::getline(file, line); i++) {
        lines.push_back(line);
    }

    // Imprime as linhas lidas
    for (const auto& line : lines) {
        std::cout << line << std::endl;
    }

    file.close();
}




int more(char *filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
        return 0;
    }

    std::string line;
    int line_num = 0;
    while (std::getline(file, line)) {
        if (line_num % 20 == 0 && line_num != 0) {
            std::cout << "-- Mais(Pressione Enter)--" << std::endl;
            std::cin.get();
        }
        std::cout << line << std::endl;
        line_num++;
    }

    file.close();
    return 1;
}

int cat(char *input)
{
    std::istringstream iss(input);

    std::vector<std::string> filenames;
    std::string filename, output_filename;

    bool redirect_output = false;

    // Loop through each token (filename) separated by whitespace
    while (iss >> filename) {
        if (filename == ">") {
            iss >> output_filename;
            redirect_output = true;
        } else {
            filenames.push_back(filename);
        }
    }

    if (redirect_output) {
        // Redireciona a saída padrão para o arquivo de saída
        std::ofstream output_file(output_filename, std::ios::out); // adicionado std::ios::out
        if (!output_file.is_open()) {
            std::cerr << "Erro ao criar o arquivo " << output_filename << std::endl;
            return 1;
        }

        for (const auto& filename : filenames) {
            std::ifstream input_file(filename);
            if (!input_file.is_open()) {
                std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
                return 1;
            }

            std::string line;
            while (std::getline(input_file, line)) {
                output_file << line << std::endl;
            }

            input_file.close();
        }

        output_file.close();
    } else {
        // Exibe o conteúdo dos arquivos de entrada na tela
        for (const auto& filename : filenames) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cerr << "Erro ao abrir o arquivo " << filename << std::endl;
                return 1;
            }

            std::string line;
            std::cout << "Conteudo do arquivo " << filename << ":" << std::endl;
            while (std::getline(file, line)) {
                std::cout << line << std::endl;
            }

            file.close();
        }
    }

    return 0;
}

// Lista pastas e arquivos do diretório atual
int list_dir(char *current_dir)
{
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
    if (chdir(dir) != 0)
    {
        std::cout << "Erro ao mudar de diretório" << std::endl;
    }
}

// Imprime caminho completo do diretório atual
void print_dir(char *current_dir)
{
    std::cout << current_dir << std::endl;
}

// Joga o output do progama para o arquivo saída
int output_to_file(char * entrada){
    char * filename = &entrada[10];
    std::ofstream outfile(filename);
    if (!outfile.is_open())
        {
            std::cerr << "Erro ao abrir o arquivo" << filename << std::endl;
            return 1;
        }
    outfile << "A saída do programa foi colocada aqui" << std::endl;
    outfile.close();   
    return 0;
}

// Recebe como input o arquivo entrada
int get_file_input(char * entrada){
    char * filename = &entrada[10];
    std::ifstream infile(filename);
    if (!infile.is_open())
        {
            std::cerr << "Erro ao abrir o arquivo" << filename << std::endl;
            return 1;
        }
    infile.close();
    std::cout << "Arquivo recebido!"<< std::endl; 
    return 0;
}

void output_to_program(std::string saida_prog1){
    std::cout << "Input recebido de prog1: " << saida_prog1 << std::endl;
}
int main()
{

    char* entrada = new char[TAM * sizeof(char*)];

    char buffer[FILENAME_MAX];
    
    while(strcmp(entrada, "exit") != 0){
        char buffer[FILENAME_MAX];
        char *current_dir = getcwd(buffer, FILENAME_MAX);

        std::cout << ">> " << current_dir  << " ";
        std::cin.getline(entrada, TAM);
        
        if (strcmp(entrada, "ls") == 0)
        {
            list_dir(current_dir);
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
        else if (strcmp(entrada, "pwd") == 0)
        {
            print_dir(current_dir);
            
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
        else if(strncmp(entrada, "more", 4) == 0){
            if (!entrada[5])
            {
                std::cerr << "Nome do arquivo faltando: more [NOME_ARQUIVO]" << std::endl;
            }
            else
            {
                more(&entrada[5]);
            }
        }else if(strncmp(entrada, "head", 4) == 0){
            if (!entrada[5])
            {
                std::cerr << "Nome do arquivo faltando: head [NOME_ARQUIVO]" << std::endl;
            }
            else
            {
                head(&entrada[5]);
            }
        }
        else if(strncmp(entrada, "./prog1",7) == 0){
            pid_t pid = fork();
                if (pid == -1) {
                    std::cerr << "Falha na execução do programa" << std::endl;
                    return 1;
                }
                
                if(strncmp(&entrada[8], ">",1) == 0){
                    output_to_file(entrada);
                }
                else if(strncmp(&entrada[8], "<",1) == 0){
                    get_file_input(entrada);
                }
                else if (strncmp(&entrada[8], "&",1) == 0 || !entrada[8]) {
                     if (pid == 0) {
                        std::cout << "Executando programa..." << std::endl;
                    }
                }
                else if (strncmp(&entrada[8], "|",1) == 0 && &entrada[10]) {
                     if (pid == 0) {
                        std::string saida = "Output do prog1 a ser enviado para o prog2";
                        output_to_program(saida);
                    }
                }
                else if(entrada[8] && entrada[10]){
                    if (pid == 0) {
                    std::cout << "Executando programa com argumentos: " << &entrada[8]<< std::endl;
                    }
                }

                
                int status;
                waitpid(pid, &status, 0);
        }
        else if(strcmp(entrada, "exit")) //implementando a leitura de programa 
        {
            
            if (entrada[0] == '.' && entrada[1] == '/') { // separa se tiver ./
                for (int i = 0; i < strlen(entrada); i++) {
                    entrada[i] = entrada[i + 2];
                }
            }

            //std::cout<<entrada<<std::endl;
            
            std::ifstream file(entrada);
            if (!file.is_open()) {
                std::cerr << "Erro ao abrir o arquivo " << entrada << std::endl;
                //return 1;
            }
            //std::cerr << "Comando não encontrado" << std::endl;
        }
        std::cerr << "\n" << std::endl;
    }
    delete[] entrada;   
}
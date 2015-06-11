#include "installcacicsa.h"
#include <locale.h>

int main(int argc, char* argv[])
{
    setlocale(LC_ALL,"");
    const char *url;
    if (argc > 3) {
        std::cout << "Voce deve fornecer somente uma das seguintes opcoes:" << std::endl <<
                     "-url URL_GERENTE" << std::endl <<
                     "-remove" << std::endl;
        //std::cin.get();
        exit(EXIT_FAILURE);
    } else if (argc <= 1) {
        std::cout << "Voce deve fornecer pelo menos uma das seguintes opcoes:" << std::endl <<
                     "-url URL_GERENTE" << std::endl <<
                     "-remove" << std::endl;
        //std::cin.get();
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i<argc; i++) {
        if (i + 1 != argc || argv[i] == std::string("-remove")) {
            if (argv[i] == std::string("-url")) {
                url = argv[i + 1];
            } else if (argv[i] == std::string("-remove")) {

                std::cout << "Removendo Cacic. Tem certeza? (Ctrl+C para cancelar)" << std::endl;
                std::cin.get();
                InstallCacicSA *icsa = new InstallCacicSA("localhost", "cacic", "cacic123");
                if (icsa->execRemove()) {
                    std::cout << "Remocao realizada com sucesso!" << std::endl << std::endl;

                    // Escreve log para a tela antes de remover
                    FILE* pFile;
                    char buffer[BUFSIZE];
                    pFile = fopen(icsa->createLogFile().c_str(), "r");
                    fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
                    fclose (pFile);

                    // Remove objeto
                    delete icsa;
                    //std::cin.get();
                    exit(EXIT_SUCCESS);
                } else {
                    std::cout << "Nao foi possivel remover o Cacic!" << std::endl <<
                                 "Verifique o log no arquivo = " << icsa->createLogFile() << std::endl;
                    //std::cin.get();
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cout << "Argumentos inválidos! Voce deve fornecer uma das seguintes opções:" << std::endl <<
                             "-url URL_GERENTE" << std::endl <<
                             "-remove" << std::endl;
                //std::cin.get();
                exit(EXIT_FAILURE);
            }
        }
    }

    std::cout << "Comunicando com o Gerente na URL: " << url << std::endl;

    InstallCacicSA *icsa = new InstallCacicSA(url, "cacic", "cacic123");
    if (icsa->exec()) {
        std::cout << "Instalacao realizada com sucesso!" << std::endl <<
                     "Se desejar verifique o log no arquivo = " << icsa->createLogFile() << std::endl;
        // std::cin.get();
    } else {
        std::cout << "Nao foi possivel realizar a instalacao!" << std::endl <<
                     "Verifique o log no arquivo = " << icsa->createLogFile() << std::endl;
        //std::cin.get();
        exit(EXIT_FAILURE);
    }
    return 0;
}

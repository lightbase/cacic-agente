#include "installcacicsa.h"

int main(int argc, char* argv[])
{
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
        if (i + 1 != argc) {
            if (argv[i] == std::string("-url")) {
                url = argv[i + 1];
            } else if (argv[i] == std::string("-remove")) {
                InstallCacicSA *icsa = new InstallCacicSA("localhost", "cacic", "cacic123");
                if (icsa->execRemove()) {
                    std::cout << "Remocao realizada com sucesso!" << std::endl <<
                                 "Se desejar verifique o log no arquivo = " << icsa->createLogFile() << std::endl;
                    //std::cin.get();
                    exit(EXIT_SUCCESS);
                } else {
                    std::cout << "Nao foi possivel remover o Cacic!" << std::endl <<
                                 "Verifique o log no arquivo = " << icsa->createLogFile() << std::endl;
                    //std::cin.get();
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cout << "Argumentos invalidos! Voce deve fornecer uma das seguintes opcoes:" << std::endl <<
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

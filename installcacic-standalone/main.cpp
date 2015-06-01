#include "installcacicsa.h"

int main(int argc, char *argv[])
{
    const char *url;
    if (argc > 2) {
        std::cout << "Você deve fornecer somente uma das seguintes opções:"
                     "-url URL_GERENTE"
                     "-remove" << std::endl;
        std::cin.get();
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i<argc; i++) {
        if (i + 1 != argv) {
            if (argv[i] == "-url") {
                url = argv[i + 1];
            } else if (argv[i] == "-remove") {
                InstallCacicSA icsa = new InstallCacicSA("localhost", "cacic", "cacic123");
                if (icsa.execRemove()) {
                    std::cout << "Remoção realizada com sucesso!"
                                 "Se desejar verifique o log no arquivo = " << icsa.createLogFile() << std::endl;
                    std::cin.get();
                    exit(EXIT_SUCCESS);
                } else {
                    std::cout << "Não foi possível remover o Cacic!"
                                 "Verifique o log no arquivo = " << icsa.createLogFile() << std::endl;
                    std::cin.get();
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cout << "Argumentos inválidos! Você deve fornecer uma das seguintes opções:"
                             "-url URL_GERENTE"
                             "-remove" << std::endl;
                std::cin.get();
                exit(EXIT_FAILURE);
            }
        }
    }

    InstallCacicSA icsa = new InstallCacicSA(url, "cacic", "cacic123");
    if (icsa.exec()) {
        std::cout << "Instalação realizada com sucesso!"
                     "Se desejar verifique o log no arquivo = " << icsa.createLogFile() << std::endl;
        std::cin.get();
    } else {
        std::cout << "Não foi possível realizar a instalação!"
                     "Verifique o log no arquivo = " << icsa.createLogFile() << std::endl;
        std::cin.get();
        exit(EXIT_FAILURE);
    }
    return 0;
}

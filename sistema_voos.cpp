#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

class Astronauta {
public:
    std::string cpf;
    std::string nome;
    int idade;
    bool disponivel;
    bool morto;
    std::vector<int> voosParticipados;

    // Construtor padrão sem argumentos
    Astronauta() : idade(0), disponivel(true), morto(false) {}

    // Construtor com argumentos
    Astronauta(std::string c, std::string n, int i) : cpf(c), nome(n), idade(i), disponivel(true), morto(false) {}
};

class Voo {
public:
    int codigo;
    std::vector<std::string> passageiros;
    bool emPlanejamento;
    bool emCurso;
    bool finalizado;
    bool sucesso;

    // Construtor padrão sem argumentos
    Voo() : codigo(0), emPlanejamento(true), emCurso(false), finalizado(false), sucesso(false) {}

    // Construtor com argumento
    Voo(int c) : codigo(c), emPlanejamento(true), emCurso(false), finalizado(false), sucesso(false) {}
};

class Sistema {
private:
    std::unordered_map<std::string, Astronauta> astronautas;
    std::unordered_map<int, Voo> voos;

public:
    void cadastrarAstronauta(const std::string& cpf, const std::string& nome, int idade) {
        astronautas[cpf] = Astronauta(cpf, nome, idade);
        std::cout << "Astronauta cadastrado: " << nome << std::endl;
    }

    void cadastrarVoo(int codigo) {
        voos[codigo] = Voo(codigo);
        std::cout << "Voo cadastrado: " << codigo << std::endl;
    }

    void adicionarAstronautaEmVoo(const std::string& cpf, int codigo) {
        if (voos.find(codigo) != voos.end() && voos[codigo].emPlanejamento) {
            voos[codigo].passageiros.push_back(cpf);
            astronautas[cpf].voosParticipados.push_back(codigo);
            std::cout << "Astronauta " << cpf << " adicionado ao voo " << codigo << std::endl;
        } else {
            std::cout << "Voo não encontrado ou não está em planejamento." << std::endl;
        }
    }

    void removerAstronautaDeVoo(const std::string& cpf, int codigo) {
        if (voos.find(codigo) != voos.end() && voos[codigo].emPlanejamento) {
            auto& passageiros = voos[codigo].passageiros;
            passageiros.erase(std::remove(passageiros.begin(), passageiros.end(), cpf), passageiros.end());
            auto& voosParticipados = astronautas[cpf].voosParticipados;
            voosParticipados.erase(std::remove(voosParticipados.begin(), voosParticipados.end(), codigo), voosParticipados.end());
            std::cout << "Astronauta " << cpf << " removido do voo " << codigo << std::endl;
        } else {
            std::cout << "Voo não encontrado ou não está em planejamento." << std::endl;
        }
    }

    void lancarVoo(int codigo) {
        if (voos.find(codigo) != voos.end() && voos[codigo].emPlanejamento && voos[codigo].passageiros.size() > 0) {
            bool todosDisponiveis = true;
            for (const auto& cpf : voos[codigo].passageiros) {
                if (!astronautas[cpf].disponivel) {
                    todosDisponiveis = false;
                    break;
                }
            }
            if (todosDisponiveis) {
                voos[codigo].emPlanejamento = false;
                voos[codigo].emCurso = true;
                for (const auto& cpf : voos[codigo].passageiros) {
                    astronautas[cpf].disponivel = false;
                }
                std::cout << "Voo " << codigo << " lançado." << std::endl;
            } else {
                std::cout << "Nem todos os astronautas estão disponíveis." << std::endl;
            }
        } else {
            std::cout << "Voo não encontrado ou não tem astronautas suficientes." << std::endl;
        }
    }

    void explodirVoo(int codigo) {
        if (voos.find(codigo) != voos.end() && voos[codigo].emCurso) {
            for (const auto& cpf : voos[codigo].passageiros) {
                astronautas[cpf].morto = true;
            }
            voos[codigo].emCurso = false;
            voos[codigo].finalizado = true;
            voos[codigo].sucesso = false;
            std::cout << "Voo " << codigo << " explodiu." << std::endl;
        } else {
            std::cout << "Voo não encontrado ou não está em curso." << std::endl;
        }
    }

    void finalizarVoo(int codigo) {
        if (voos.find(codigo) != voos.end() && voos[codigo].emCurso) {
            for (const auto& cpf : voos[codigo].passageiros) {
                astronautas[cpf].disponivel = true;
            }
            voos[codigo].emCurso = false;
            voos[codigo].finalizado = true;
            voos[codigo].sucesso = true;
            std::cout << "Voo " << codigo << " finalizado com sucesso." << std::endl;
        } else {
            std::cout << "Voo não encontrado ou não está em curso." << std::endl;
        }
    }

    void listarVoos() const {
        std::cout << "Voos Planejados:" << std::endl;
        for (auto it = voos.begin(); it != voos.end(); ++it) {
            const auto& codigo = it->first;
            const auto& voo = it->second;
            if (voo.emPlanejamento) {
                std::cout << "Voo " << codigo << ": ";
                for (const auto& cpf : voo.passageiros) {
                    std::cout << cpf << " ";
                }
                std::cout << std::endl;
            }
        }

        std::cout << "\nVoos Em Curso:" << std::endl;
        for (auto it = voos.begin(); it != voos.end(); ++it) {
            const auto& codigo = it->first;
            const auto& voo = it->second;
            if (voo.emCurso) {
                std::cout << "Voo " << codigo << ": ";
                for (const auto& cpf : voo.passageiros) {
                    std::cout << cpf << " ";
                }
                std::cout << std::endl;
            }
        }

        std::cout << "\nVoos Finalizados:" << std::endl;
        for (auto it = voos.begin(); it != voos.end(); ++it) {
            const auto& codigo = it->first;
            const auto& voo = it->second;
            if (voo.finalizado) {
                std::cout << "Voo " << codigo << ": ";
                for (const auto& cpf : voo.passageiros) {
                    std::cout << cpf << " ";
                }
                std::cout << (voo.sucesso ? " (Sucesso)" : " (Explodiu)") << std::endl;
            }
        }
    }

    void listarAstronautasMortos() const {
        std::cout << "Astronautas Mortos:" << std::endl;
        for (auto it = astronautas.begin(); it != astronautas.end(); ++it) {
            const auto& astronauta = it->second;
            if (astronauta.morto) {
                std::cout << "CPF: " << astronauta.cpf << ", Nome: " << astronauta.nome << ", Voos: ";
                for (const auto& voo : astronauta.voosParticipados) {
                    std::cout << voo << " ";
                }
                std::cout << std::endl;
            }
        }
    }
};

int main() {
    Sistema sistema;

    char opcao;
    do {
        std::cout << "\nMenu:\n"
                  << "1. Cadastrar Astronauta\n"
                  << "2. Cadastrar Voo\n"
                  << "3. Adicionar Astronauta em Voo\n"
                  << "4. Lançar Voo\n"
                  << "5. Finalizar Voo\n"
                  << "6. Listar Voos\n"
                  << "7. Listar Astronautas Mortos\n"
                  << "0. Sair\n"
                  << "Escolha uma opção: ";
        std::cin >> opcao;
        std::cin.ignore(); // Limpar o buffer do teclado

        switch (opcao) {
            case '1': {
                std::string cpf, nome;
                int idade;
                std::cout << "CPF: ";
                std::getline(std::cin, cpf);
                std::cout << "Nome: ";
                std::getline(std::cin, nome);
                std::cout << "Idade: ";
                std::cin >> idade;
                sistema.cadastrarAstronauta(cpf, nome, idade);
                break;
            }
            case '2': {
                int codigo;
                std::cout << "Código do Voo: ";
                std::cin >> codigo;
                sistema.cadastrarVoo(codigo);
                break;
            }
            case '3': {
                std::string cpf;
                int codigo;
                std::cout << "CPF do Astronauta: ";
                std::getline(std::cin >> std::ws, cpf);
                std::cout << "Código do Voo: ";
                std::cin >> codigo;
                sistema.adicionarAstronautaEmVoo(cpf, codigo);
                break;
            }
            case '4': {
                int codigo;
                std::cout << "Código do Voo: ";
                std::cin >> codigo;
                sistema.lancarVoo(codigo);
                break;
            }
            case '5': {
                int codigo;
                std::cout << "Código do Voo: ";
                std::cin >> codigo;
                sistema.finalizarVoo(codigo);
                break;
            }
            case '6':
                sistema.listarVoos();
                break;
            case '7':
                sistema.listarAstronautasMortos();
                break;
            case '0':
                std::cout << "Encerrando o programa." << std::endl;
                break;
            default:
                std::cout << "Opção inválida." << std::endl;
                break;
        }

    } while (opcao != '0');

    return 0;
}

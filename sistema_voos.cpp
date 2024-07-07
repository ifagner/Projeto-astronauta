# Projeto-astronauta
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
        for (const auto& [codigo, voo] : voos) {
            if (voo.emPlanejamento) {
                std::cout << "Voo " << codigo << ": ";
                for (const auto& cpf : voo.passageiros) {
                    std::cout << cpf << " ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << "Voos em Curso:" << std::endl;
        for (const auto& [codigo, voo] : voos) {
            if (voo.emCurso) {
                std::cout << "Voo " << codigo << ": ";
                for (const auto& cpf : voo.passageiros) {
                    std::cout << cpf << " ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << "Voos Finalizados:" << std::endl;
        for (const auto& [codigo, voo] : voos) {
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
        for (const auto& [cpf, astronauta] : astronautas) {
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
    
    // Exemplos de uso
    sistema.cadastrarAstronauta("123", "Alice", 30);
    sistema.cadastrarAstronauta("456", "Bob", 35);
    
    sistema.cadastrarVoo(1);
    sistema.adicionarAstronautaEmVoo("123", 1);
    sistema.adicionarAstronautaEmVoo("456", 1);
    
    sistema.lancarVoo(1);
    
    sistema.finalizarVoo(1);
    
    sistema.listarVoos();
    sistema.listarAstronautasMortos();
    
    return 0;
}

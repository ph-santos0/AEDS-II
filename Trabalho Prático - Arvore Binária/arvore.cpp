#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

struct No {
    string dado;
    No* sim;
    No* nao;
};

No* criarNo(const string& dado) {
    No* no = new No();
    no->dado = dado;
    no->sim = nullptr;
    no->nao = nullptr;
    return no;
}

void apagarArvore(No* raiz) {
    if (raiz == nullptr) return;
    apagarArvore(raiz->sim);
    apagarArvore(raiz->nao);
    delete raiz;
}

void salvarArvore(ofstream& arquivo, No* raiz) {
    if (raiz == nullptr) {
        arquivo << "#\n"; // Marcador para um nó vazio
        return;
    }
    arquivo << raiz->dado << endl;
    salvarArvore(arquivo, raiz->sim);
    salvarArvore(arquivo, raiz->nao);
}

No* carregarArvore(ifstream& arquivo) {
    string linha;
    getline(arquivo, linha);

    if (linha == "#" || linha.empty()) {
        return nullptr;
    }

    No* no = criarNo(linha);
    no->sim = carregarArvore(arquivo);
    no->nao = carregarArvore(arquivo);
    return no;
}

void jogar(No* no) {
    string resposta;
    if (!no->sim && !no->nao) { // É uma folha
        cout << "Então você pensou em um(a) " << no->dado << ". Acertei?" << endl;
        cin >> resposta;
        if (resposta == "Sim" || resposta == "sim") {
            cout << "Acertei de novo!" << endl;
        } else {
            cout << "Em qual animal você pensou?" << endl;
            string novoAnimal;
            cin.ignore(); // Limpar o buffer
            getline(cin, novoAnimal);

            cout << "Qual é a diferença entre " << novoAnimal << " e " << no->dado << "?" << endl;
            string novaPergunta;
            getline(cin, novaPergunta);

            // Criação de novos nós para o novo animal e para a nova pergunta
            No* noNovoAnimal = criarNo(novoAnimal);
            No* noAtual = criarNo(no->dado);
            no->dado = novaPergunta;
            no->sim = noNovoAnimal;
            no->nao = noAtual;
        }
    } else {
        cout << "O animal que você está pensando " << no->dado << "?" << endl;
        cin >> resposta;
        if (resposta == "Sim" || resposta == "sim") {
            jogar(no->sim);
        } else {
            jogar(no->nao);
        }
    }
}

int main() {
    UINT CPAGE_UTF8 = 65001;
    UINT CPAGE_DEFAULT = GetConsoleOutputCP();
    SetConsoleOutputCP(CPAGE_UTF8);

    No* raiz = nullptr;

    do {
        ifstream arquivoEntrada("arvore_animais.txt");

        string resposta;
        if (arquivoEntrada) {
            raiz = carregarArvore(arquivoEntrada);
            arquivoEntrada.close();
        } else {
            // Inicialização da árvore com perguntas e animais iniciais
            raiz = criarNo("tem asas");
            raiz->sim = criarNo("pássaro");
            raiz->nao = criarNo("cachorro");
        }

        cout << "Olá! Vou tentar adivinhar o animal que você está pensando." << endl;
        jogar(raiz);

        ofstream arquivoSaida("arvore_animais.txt");
        if (arquivoSaida) {
            salvarArvore(arquivoSaida, raiz);
            arquivoSaida.close();
        } else {
            cout << "Erro ao salvar a árvore no arquivo." << endl;
        }

        cout << "Deseja continuar? (sim/não)" << endl;
        cin >> resposta;

        if(resposta == "nao" || resposta == "não" || resposta == "N" || resposta == "n" || resposta == "Não" || resposta == "Nao")  return 0;

        system("cls");

    } while (true);

    apagarArvore(raiz);

    return 0;
}

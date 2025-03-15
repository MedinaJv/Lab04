#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Livro.h"
#include "Arvore.h"

using namespace std;

void LerDados(const string& arquivo, Arvore& arvore) {
    ifstream file(arquivo);
    ofstream arquivoLivros("livros.dat", ios::binary | ios::trunc);
    ofstream arquivoIndice("indice.dat", ios::binary | ios::trunc);

    if (!file.is_open()) {
        cout << "Erro ao abrir o arquivo CSV!" << endl;
        return;
    } else {
        cout << "Arquivo CSV aberto com sucesso!" << endl;
    }
    if (!arquivoLivros.is_open() || !arquivoIndice.is_open()) {
        cout << "Erro ao abrir arquivos de saída!" << endl;
        return;
    }

    string linha;
    long long posicao = 0;

    while (getline(file, linha)) {
        
        vector<string> campos = Livro::DividirLinhaCSV(linha);
        if (campos.size() < 5) continue;

        int id = stoi(campos[0]);
        int ano = stoi(campos[3]);
        Livro livro(id, campos[1], {campos[2]}, ano, {campos[4]});

        string registro = livro.Pack() + "\n";
        int tamanhoRegistro = registro.size();

        arquivoLivros.seekp(posicao);
        arquivoLivros.write(registro.c_str(), tamanhoRegistro);
        arquivoLivros.flush();
        cout << "Escrevendo no livros.dat (posição " << posicao << "): " << registro << endl;

        arquivoIndice.write(reinterpret_cast<const char*>(&id), sizeof(id));
        arquivoIndice.write(reinterpret_cast<const char*>(&posicao), sizeof(posicao));
        arquivoIndice.flush();
        cout << "Escrevendo no indice.dat: ID " << id << " -> posição " << posicao << endl;

        arvore.Inserir(id, posicao);
        posicao += tamanhoRegistro;
    }

    file.close();
    arquivoLivros.close();
    arquivoIndice.close();

    cout << "Dados do CSV carregados com sucesso!" << endl;
}

void CarregarIndices(Arvore& arvore) {
    ifstream arquivoIndice("indice.dat", ios::binary);
    if (!arquivoIndice.is_open()) {
        cout << "Arquivo de índice não encontrado!" << endl;
        return;
    }

    int id;
    long long posicao;

    while (arquivoIndice.read(reinterpret_cast<char*>(&id), sizeof(id))) {
        arquivoIndice.read(reinterpret_cast<char*>(&posicao), sizeof(posicao));
        arvore.Inserir(id, posicao);
        cout << "Índice carregado: ID " << id << " -> posição " << posicao << endl;
    }

    arquivoIndice.close();
    cout << "Índice carregado com sucesso!" << endl;
}

Livro BuscarLivro(int id, Arvore& arvore) {
    long long posicao = arvore.Buscar(id);
    if (posicao == -1) {
        cout << "ID não encontrado no índice!" << endl;
        return Livro();
    }

    ifstream arquivoLivros("livros.dat", ios::binary);
    if (!arquivoLivros.is_open()) {
        cout << "Erro ao abrir arquivo de livros!" << endl;
        return Livro();
    }

    arquivoLivros.seekg(posicao);
    string dados;
    getline(arquivoLivros, dados);
    arquivoLivros.close();

    cout << "Livro encontrado no arquivo: " << dados << endl;
    return Livro::Unpack(dados);
}

int main() {
    string arquivoCSV = "booksDataset.csv";
    Arvore arvore;

    LerDados(arquivoCSV, arvore);
    CarregarIndices(arvore);

    cout << "Tamanho dos arquivos:" << endl;
    system("ls -lh livros.dat indice.dat");

    int idBusca;
    cout << "Digite o ID do livro para buscar: ";
    cin >> idBusca;

    Livro livroEncontrado = BuscarLivro(idBusca, arvore);
    if (livroEncontrado.id != 0) {
        cout << "Livro encontrado: " << livroEncontrado.titulo << endl;
    } else {
        cout << "Livro não encontrado." << endl;
    }

    return 0;
}
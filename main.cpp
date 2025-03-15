#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include "Livro.h"
#include "Arvore.h"

using namespace std;

// Verifica se uma string representa um número inteiro
bool stoiVeirifier(const string& str) {
    try {
        size_t pos;
        stoi(str, &pos);
        return (pos == str.length());
    } catch(const invalid_argument&) {
        return false;
    }
}

// Função auxiliar para remover espaços em branco do início e fim de uma string
string trim(const string& str) {
    const string whitespace = " \t";
    size_t start = str.find_first_not_of(whitespace);
    if(start == string::npos)
        return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

// Divide a string de autores usando " and " como delimitador.
vector<string> DividirAutores(const string& autores) {
    vector<string> listaAutores;
    size_t inicio = 0;
    size_t pos = autores.find(" and ");
    while (pos != string::npos) {
        string autor = autores.substr(inicio, pos - inicio);
        listaAutores.push_back(trim(autor));
        inicio = pos + 5;
        pos = autores.find(" and ", inicio);
    }
    string ultimoAutor = autores.substr(inicio);
    listaAutores.push_back(trim(ultimoAutor));
    return listaAutores;
}

// Lê os dados do arquivo CSV, grava os registros em um arquivo binário e cria índices pelo ID
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
        if (campos.size() < 5)
            continue;
            
        int id = 0;
        int ano = 0;
        if (stoiVeirifier(campos[0]) && stoiVeirifier(campos[3])) {
            id = stoi(campos[0]);
            ano = stoi(campos[3]);
        }
        
        vector<string> autores = DividirAutores(campos[2]);
        Livro livro(id, campos[1], autores, ano, {campos[4]});

        string registro = livro.Pack() + "\n";
        int tamanhoRegistro = registro.size();

        arquivoLivros.seekp(posicao);
        arquivoLivros.write(registro.c_str(), tamanhoRegistro);
        arquivoLivros.flush();
        
        arquivoIndice.write(reinterpret_cast<const char*>(&id), sizeof(id));
        arquivoIndice.write(reinterpret_cast<const char*>(&posicao), sizeof(posicao));
        arquivoIndice.flush();
        
        arvore.Inserir(id, posicao);
        posicao += tamanhoRegistro;
    }
    file.close();
    arquivoLivros.close();
    arquivoIndice.close();
    cout << "Dados do CSV carregados com sucesso!" << endl;
}

// Recarrega os índices do arquivo de índice para a Arvore
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
    }
    arquivoIndice.close();
    cout << "Índice carregado com sucesso!" << endl;
}

// Busca um livro baseado no ID utilizando o índice da Arvore
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
    return Livro::Unpack(dados);
}

// Processa a query do usuário em linguagem natural
void ProcessarQuery(const string& query, Arvore& arvore) {
    string queryLower = query;
    transform(queryLower.begin(), queryLower.end(), queryLower.begin(), ::tolower);

    size_t posID = queryLower.find("id");
    if (posID != string::npos) {
        size_t posNum = queryLower.find_first_of("0123456789", posID);
        if (posNum != string::npos) {
            size_t posEnd = queryLower.find_first_not_of("0123456789", posNum);
            string numStr = queryLower.substr(posNum, posEnd - posNum);
            int id = stoi(numStr);
            Livro livroEncontrado = BuscarLivro(id, arvore);
            if (livroEncontrado.id == 0) {
                cout << "Livro com ID " << id << " não encontrado." << endl;
            } else {
                if (queryLower.find("autor") != string::npos) {
                    cout << "Autor(es) do livro com ID " << id << ":" << endl;
                    for (const auto& autor : livroEncontrado.autores)
                        cout << autor << " ";
                    cout << endl;
                }
                else if(queryLower.find("ano") != string::npos) {
                    cout << "Ano do livro com ID " << id << ": " << livroEncontrado.ano << endl;
                }
                else if (queryLower.find("título") != string::npos || queryLower.find("titulo") != string::npos) {
                    cout << "Título do livro com ID " << id << ": " << livroEncontrado.titulo << endl;
                }
                else if(queryLower.find("gênero") != string::npos || queryLower.find("genero") != string::npos || queryLower.find("generos") != string::npos || queryLower.find("gêneros") != string::npos) {
                    cout << "Gêneros do livro com ID " << id << ":" << endl;
                    for(const auto& genero : livroEncontrado.generos)
                        cout << genero << endl;
                    cout << endl;
                }
                else {
                    cout << "Livro encontrado:" << endl;
                    cout << "ID: " << livroEncontrado.id << endl;
                    cout << "Título: " << livroEncontrado.titulo << endl;
                    cout << "Ano: " << livroEncontrado.ano << endl;
                    cout << "Autor(es): ";
                    for (const auto& a : livroEncontrado.autores)
                        cout << a << " ";
                    cout << endl;
                }
            }
        } else {
            cout << "Não foi possível extrair um ID." << endl;
        }
    }
    else if(queryLower.compare("sair") == 0) {
        return;
    }
    else{
        cout << "Query não reconhecida!" << endl;
    }
}

// Realiza a adição de um novo Livro ao arquivo de livros e ao índice
void AdicionarRegistro(const Livro& livro, Arvore& arvore){
    ofstream arquivoLivros("livros.dat", ios::binary | ios::app);
    ofstream arquivoIndice("indice.dat", ios::binary | ios::app);

    if (!arquivoLivros.is_open() || !arquivoIndice.is_open()) {
        cout << "Erro ao abrir arquivos de saída!" << endl;
        return;
    }

    string registro = livro.Pack() + "\n";
    int tamanhoRegistro = registro.size();
    arquivoLivros.seekp(0, ios::end);
    long long posicao = arquivoLivros.tellp();

    arquivoLivros.write(registro.c_str(), tamanhoRegistro);
    arquivoLivros.flush();
    arquivoIndice.write(reinterpret_cast<const char*>(&livro.id), sizeof(livro.id));
    arquivoIndice.write(reinterpret_cast<const char*>(&posicao), sizeof(posicao));

    arquivoIndice.flush();
    arvore.Inserir(livro.id, posicao);

    cout << "Registro adicionado com sucesso!" << endl;
}

int main() {
    // Livro exemplo
    Livro livro;
    livro.id = 1;
    livro.titulo = "O Senhor dos Anéis";
    livro.autores = {"J.R.R. Tolkien"};
    livro.ano = 1954;
    livro.generos = {"Fantasia"};

    string arquivoCSV = "booksDataset.csv";
    Arvore arvore;

    // Carrega os dados do CSV e os índices
    LerDados(arquivoCSV, arvore);

    // Acrescenta o livro exemplo
    AdicionarRegistro(livro, arvore);

    // Carrega os indices para a árvore
    CarregarIndices(arvore);
    
    // Realiza a consulta
    string query;
    cout << "Digite sua query: ";
    getline(cin, query);
    ProcessarQuery(query, arvore);

    return 0;
}

#include "Livro.h"
#include <sstream>

// Construtor padrão
Livro::Livro() : id(0), ano(0) {}

// Construtor com parâmetros
Livro::Livro(int id, const string& titulo, const vector<string>& autores, int ano, const vector<string>& generos)
    : id(id), titulo(titulo), autores(autores), ano(ano), generos(generos) {}

// Método para dividir uma linha CSV em campos
vector<string> Livro::DividirLinhaCSV(const string& linha) {
    vector<string> campos;
    stringstream ss(linha);
    string campo;

    while (getline(ss, campo, '|')) { // O delimitador correto é '|'
        campos.push_back(campo);
    }

    return campos;
}

// Método para compactar os dados do livro em uma string
string Livro::Pack() const {
    stringstream ss;
    ss << id << "|"
       << titulo << "|";

    for (size_t i = 0; i < autores.size(); i++) {
        ss << autores[i];
        if (i < autores.size() - 1) ss << ",";
    }
    ss << "|";

    ss << ano << "|";

    for (size_t i = 0; i < generos.size(); i++) {
        ss << generos[i];
        if (i < generos.size() - 1) ss << ",";
    }
    return ss.str();
}

// Método para descompactar uma string em um objeto Livro
    Livro Livro::Unpack(const string& data) {
        stringstream ss(data);
        string token;

        Livro livro;

        // Extrair ID
        getline(ss, token, '|');
        livro.id = stoi(token);

        // Extrair título
        getline(ss, livro.titulo, '|');

        // Extrair autores
        getline(ss, token, '|');
        stringstream autoresStream(token);
        while (getline(autoresStream, token, ',')) {
            livro.autores.push_back(token);
        }

        // Extrair ano
        getline(ss, token, '|');
        livro.ano = stoi(token);

        // Extrair gêneros
        getline(ss, token, '|');
        stringstream generosStream(token);
        while (getline(generosStream, token, ',')) {
            livro.generos.push_back(token);
        }

        return livro;
    }
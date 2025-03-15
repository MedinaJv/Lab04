#ifndef LIVRO_H
#define LIVRO_H
#include <string>
#include <vector>
using namespace std;

class Livro {
public:
    int id;
    string titulo;
    vector<string> autores;
    int ano;
    vector<string> generos;

    Livro();
    Livro(int id, const string& titulo, const vector<string>& autores, int ano, const vector<string>& generos);
    string Pack() const;
    static Livro Unpack(const string& data);
    static vector<string> DividirLinhaCSV(const string& linha);
};

#endif
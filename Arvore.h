#ifndef ARVORE_H
#define ARVORE_H

struct No {
    int chave;
    long long posicao;
    No* esquerda;
    No* direita;
    No(int chave, long long posicao);
};

class Arvore {
private:
    No* raiz;
    void Inserir(No*& raiz, int chave, long long posicao);
    No* Buscar(No* raiz, int chave) const;
    void Limpar(No*& raiz);
public:
    Arvore();
    ~Arvore();
    void Inserir(int chave, long long posicao);
    long long Buscar(int chave) const;
};

#endif
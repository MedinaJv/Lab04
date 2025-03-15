#include "Arvore.h"

// Construtor do nó
No::No(int chave, long long posicao)
    : chave(chave), posicao(posicao), esquerda(nullptr), direita(nullptr) {}

// Construtor da árvore
Arvore::Arvore() : raiz(nullptr) {}

// Destrutor da árvore
Arvore::~Arvore() {
    Limpar(raiz);
}

// Função recursiva para limpar a árvore
void Arvore::Limpar(No*& raiz) {
    if (raiz) {
        Limpar(raiz->esquerda);
        Limpar(raiz->direita);
        delete raiz;
        raiz = nullptr;
    }
}

// Inserção na árvore
void Arvore::Inserir(No*& raiz, int chave, long long posicao) {
    if (!raiz) {
        raiz = new No(chave, posicao);
    } else if (chave < raiz->chave) {
        Inserir(raiz->esquerda, chave, posicao);
    } else if (chave > raiz->chave) {
        Inserir(raiz->direita, chave, posicao);
    }
}

// Inserção pública
void Arvore::Inserir(int chave, long long posicao) {
    Inserir(raiz, chave, posicao);
}

// Busca na árvore
No* Arvore::Buscar(No* raiz, int chave) const {
    if (!raiz || raiz->chave == chave) {
        return raiz;
    } else if (chave < raiz->chave) {
        return Buscar(raiz->esquerda, chave);
    } else {
        return Buscar(raiz->direita, chave);
    }
}

// Busca pública
long long Arvore::Buscar(int chave) const {
    No* resultado = Buscar(raiz, chave);
    return resultado ? resultado->posicao : -1;
}
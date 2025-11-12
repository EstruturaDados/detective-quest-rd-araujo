#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/// estrutura p/ cômodos da mansão
typedef struct Sala {
    char nome[40];
    char pista[80];
    struct Sala *esq, *dir;
} Sala;

/// estrutura p/ árvore de pistas (BST)
typedef struct PistaNode {
    char pista[80];
    struct PistaNode *esq, *dir;
} PistaNode;

/// cria uma nova sala dinamicamente
Sala* criarSala(char nome[], char pista[]) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esq = nova->dir = NULL;
    return nova;
}

/// cria nó da BST de pistas
PistaNode* criarPistaNode(char pista[]) {
    PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
    strcpy(novo->pista, pista);
    novo->esq = novo->dir = NULL;
    return novo;
}

/// insere nova pista na BST (ordem alfabética)
PistaNode* inserirPista(PistaNode *raiz, char pista[]) {
    if (raiz == NULL) return criarPistaNode(pista);

    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (cmp > 0)
        raiz->dir = inserirPista(raiz->dir, pista);
    return raiz;
}

/// mostra pistas em ordem alfabética (in-order)
void exibirPistas(PistaNode *raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esq);
        printf(" - %s\n", raiz->pista);
        exibirPistas(raiz->dir);
    }
}

/// controla exploração dos cômodos e coleta de pistas
void explorarSalasComPistas(Sala *atual, PistaNode **arvPistas) {
    if (atual == NULL) return;

    char op;
    printf("\n=== Local atual: %s ===\n", atual->nome);

    if (strlen(atual->pista) > 0) {
        printf("Pista encontrada: \"%s\"\n", atual->pista);
        *arvPistas = inserirPista(*arvPistas, atual->pista);
    } else {
        printf("Nenhuma pista aqui...\n");
    }

    /// exibe opções de movimento
    printf("\nEscolha o caminho:\n");
    if (atual->esq != NULL) printf(" [e] %s\n", atual->esq->nome);
    if (atual->dir != NULL) printf(" [d] %s\n", atual->dir->nome);
    printf(" [s] Sair da exploração\n> ");
    scanf(" %c", &op);

    switch (op) {
        case 'e': explorarSalasComPistas(atual->esq, arvPistas); break;
        case 'd': explorarSalasComPistas(atual->dir, arvPistas); break;
        case 's': 
            printf("\nExploração encerrada.\n");
            return;
        default:
            printf("\nOpção inválida.\n");
            explorarSalasComPistas(atual, arvPistas);
    }
}

/// libera memória das árvores
void liberarSalas(Sala *r) {
    if (r != NULL) {
        liberarSalas(r->esq);
        liberarSalas(r->dir);
        free(r);
    }
}
void liberarPistas(PistaNode *r) {
    if (r != NULL) {
        liberarPistas(r->esq);
        liberarPistas(r->dir);
        free(r);
    }
}

int main() {
    /// cria mapa fixo da mansão
    Sala *hall = criarSala("Hall de Entrada", "Chave dourada caída no chão");
    Sala *salaEstar = criarSala("Sala de Estar", "Pegadas empoeiradas");
    Sala *cozinha = criarSala("Cozinha", "Anotação rasgada: 'porão'");
    Sala *biblioteca = criarSala("Biblioteca", "Livro aberto sobre venenos");
    Sala *porao = criarSala("Porão", "Colar ensanguentado");
    Sala *jardim = criarSala("Jardim", "");

    /// conexões do mapa (árvore binária)
    hall->esq = salaEstar;
    hall->dir = cozinha;
    salaEstar->esq = biblioteca;
    salaEstar->dir = jardim;
    cozinha->dir = porao;

    PistaNode *arvPistas = NULL;

    printf("=== DETECTIVE QUEST: Coleta de Pistas ===\n");
    printf("Começando a exploração pela mansão...\n");

    explorarSalasComPistas(hall, &arvPistas);

    printf("\n--- Pistas coletadas em ordem alfabética ---\n");
    exibirPistas(arvPistas);

    liberarSalas(hall);
    liberarPistas(arvPistas);

    printf("\nMissão concluída. Até a próxima, detetive!\n");
    return 0;
}

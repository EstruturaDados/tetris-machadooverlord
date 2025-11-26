#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca pecas[5];
    int frente;
    int tras;
    int total;
} Fila;

int idGlobal = 0;

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->total = 0;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

int filaCheia(Fila *f) {
    return f->total == 5;
}

Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = idGlobal++;
    return p;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia!\n");
        return;
    }
    f->tras = (f->tras + 1) % 5;
    f->pecas[f->tras] = p;
    f->total++;
}

Peca dequeue(Fila *f) {
    Peca p = f->pecas[f->frente];
    f->frente = (f->frente + 1) % 5;
    f->total--;
    return p;
}

void mostrarFila(Fila *f) {
    printf("\nFila de pecas: ");
    if (filaVazia(f)) {
        printf("vazia!\n");
        return;
    }
    int i;
    int idx = f->frente;
    for (i = 0; i < f->total; i++) {
        printf("[%c %d] ", f->pecas[idx].nome, f->pecas[idx].id);
        idx = (idx + 1) % 5;
    }
    printf("\n");
}

int main() {
    srand(time(NULL));
    
    printf("\n=== Tetris - MACHADO OVERLORD - LUCIANO ===\n");
    printf("\n=== Tetris Stack ===\n");
    
    Fila fila;
    inicializarFila(&fila);
    
    int i;
    for (i = 0; i < 5; i++) {
        enqueue(&fila, gerarPeca());
    }
    
    int opcao;
    
    while (1) {
        mostrarFila(&fila);
        
        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        
        if (opcao == 0) {
            printf("Saindo...\n");
            break;
        }
        
        if (opcao == 1) {
            if (filaVazia(&fila)) {
                printf("Nao ha pecas para jogar!\n");
            } else {
                Peca jogada = dequeue(&fila);
                printf("\nPeca jogada: [%c %d]\n", jogada.nome, jogada.id);
            }
        } else if (opcao == 2) {
            if (filaCheia(&fila)) {
                printf("Fila cheia! Nao e possivel inserir nova peca.\n");
            } else {
                Peca nova = gerarPeca();
                enqueue(&fila, nova);
                printf("\nNova peca inserida: [%c %d]\n", nova.nome, nova.id);
            }
        } else {
            printf("Opcao invalida!\n");
        }
    }
    
    return 0;
}
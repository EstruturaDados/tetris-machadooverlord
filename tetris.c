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

typedef struct {
    Peca pecas[3];
    int topo;
} Pilha;

int idGlobal = 0;

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->total = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

int filaCheia(Fila *f) {
    return f->total == 5;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == 2;
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

void push(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia!\n");
        return;
    }
    p->topo++;
    p->pecas[p->topo] = peca;
}

Peca pop(Pilha *p) {
    Peca peca = p->pecas[p->topo];
    p->topo--;
    return peca;
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

void mostrarPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("vazia!\n");
        return;
    }
    int i;
    for (i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->pecas[i].nome, p->pecas[i].id);
    }
    printf("\n");
}

int main() {
    srand(time(NULL));
    
    printf("\n=== Tetris - MACHADO OVERLORD - LUCIANO ===\n");
    printf("\n=== Tetris Stack ===\n");
    
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    int i;
    for (i = 0; i < 5; i++) {
        enqueue(&fila, gerarPeca());
    }
    
    int opcao;
    
    while (1) {
        mostrarFila(&fila);
        mostrarPilha(&pilha);
        
        printf("\nOpcoes de acao:\n");
        printf("1 - Jogar peca\n");
        printf("2 - Reservar peca\n");
        printf("3 - Usar peca reservada\n");
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
                enqueue(&fila, gerarPeca());
            }
        } else if (opcao == 2) {
            if (filaVazia(&fila)) {
                printf("Fila vazia! Nao ha pecas para reservar.\n");
            } else if (pilhaCheia(&pilha)) {
                printf("Pilha cheia! Nao e possivel reservar mais pecas.\n");
            } else {
                Peca reservada = dequeue(&fila);
                push(&pilha, reservada);
                printf("\nPeca reservada: [%c %d]\n", reservada.nome, reservada.id);
                enqueue(&fila, gerarPeca());
            }
        } else if (opcao == 3) {
            if (pilhaVazia(&pilha)) {
                printf("Pilha vazia! Nao ha pecas reservadas.\n");
            } else {
                Peca usada = pop(&pilha);
                printf("\nPeca usada da reserva: [%c %d]\n", usada.nome, usada.id);
            }
        } else {
            printf("Opcao invalida!\n");
        }
    }
    
    return 0;
}
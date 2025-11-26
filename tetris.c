#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// struct das pecas
typedef struct {
    char nome;
    int id;
} Peca;

// struct da fila circular
typedef struct {
    Peca pecas[5];
    int frente;
    int tras;
    int total;
} Fila;

// struct da pilha
typedef struct {
    Peca pecas[3];
    int topo;
} Pilha;

int idGlobal = 0;

// variaveis para o desfazer
Peca ultimaJogada;
int temUltimaJogada = 0;

void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->total = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// checa se a fila ta vazia
int filaVazia(Fila *f) {
    if (f->total == 0) {
        return 1;
    } else {
        return 0;
    }
}

int filaCheia(Fila *f) {
    return f->total == 5;
}

int pilhaVazia(Pilha *p) {
    if (p->topo == -1)
        return 1;
    return 0;
}

int pilhaCheia(Pilha *p) {
    return p->topo == 2;
}

// gera uma peca aleatoria
Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    int tipo = rand() % 4;
    p.nome = tipos[tipo];
    p.id = idGlobal;
    idGlobal++;
    return p;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) {
        printf("Fila cheia!\n");
        return;
    }
    f->tras++;
    if (f->tras >= 5) {
        f->tras = 0;
    }
    f->pecas[f->tras] = p;
    f->total = f->total + 1;
}

Peca dequeue(Fila *f) {
    Peca p = f->pecas[f->frente];
    f->frente++;
    if (f->frente >= 5) {
        f->frente = 0;
    }
    f->total--;
    return p;
}

void push(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) {
        printf("Pilha cheia!\n");
        return;
    }
    p->topo = p->topo + 1;
    p->pecas[p->topo] = peca;
}

Peca pop(Pilha *p) {
    Peca peca = p->pecas[p->topo];
    p->topo = p->topo - 1;
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
        idx++;
        if (idx >= 5) {
            idx = 0;
        }
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

// troca a peca da frente da fila com o topo da pilha
void trocarFrenteComTopo(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("Fila vazia! Nao e possivel trocar.\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("Pilha vazia! Nao e possivel trocar.\n");
        return;
    }
    
    Peca temp = f->pecas[f->frente];
    f->pecas[f->frente] = p->pecas[p->topo];
    p->pecas[p->topo] = temp;
    
    printf("\nTroca realizada entre frente da fila e topo da pilha!\n");
}

// troca os 3 primeiros da fila com as 3 pecas da pilha
void trocar3Primeiros(Fila *f, Pilha *p) {
    if (f->total < 3) {
        printf("Fila tem menos de 3 pecas! Nao e possivel trocar.\n");
        return;
    }
    if (p->topo != 2) {
        printf("Pilha nao tem exatamente 3 pecas! Nao e possivel trocar.\n");
        return;
    }
    
    // guarda as 3 primeiras pecas da fila
    Peca temp1, temp2, temp3;
    int idx = f->frente;
    
    temp1 = f->pecas[idx];
    idx = (idx + 1) % 5;
    temp2 = f->pecas[idx];
    idx = (idx + 1) % 5;
    temp3 = f->pecas[idx];
    
    // coloca as pecas da pilha na fila
    idx = f->frente;
    f->pecas[idx] = p->pecas[2];
    idx = (idx + 1) % 5;
    f->pecas[idx] = p->pecas[1];
    idx = (idx + 1) % 5;
    f->pecas[idx] = p->pecas[0];
    
    // coloca as pecas da fila na pilha
    p->pecas[2] = temp1;
    p->pecas[1] = temp2;
    p->pecas[0] = temp3;
    
    printf("\nTroca realizada entre os 3 primeiros da fila e os 3 da pilha!\n");
}

// desfaz a ultima jogada
void desfazerJogada(Fila *f) {
    if (temUltimaJogada == 0) {
        printf("Nenhuma jogada para desfazer!\n");
        return;
    }
    
    // volta a frente da fila
    int novoFrente = f->frente - 1;
    if (novoFrente < 0) {
        novoFrente = 4;
    }
    f->frente = novoFrente;
    f->pecas[f->frente] = ultimaJogada;
    f->total++;
    
    // remove a ultima peca que foi adicionada
    f->tras--;
    if (f->tras < 0) {
        f->tras = 4;
    }
    f->total--;
    
    printf("\nJogada desfeita!\n");
    temUltimaJogada = 0;
}

// inverte a ordem da fila
void inverterFila(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia! Nao e possivel inverter.\n");
        return;
    }
    
    Peca temp[5];
    int count = 0;
    
    // tira todas as pecas da fila
    while (f->total > 0) {
        temp[count] = dequeue(f);
        count++;
    }
    
    // coloca de volta na ordem inversa
    int i;
    for (i = count - 1; i >= 0; i--) {
        enqueue(f, temp[i]);
    }
    
    printf("\nFila invertida!\n");
}

int main() {
    srand(time(NULL));
    
    printf("\n=== Tetris - MACHADO OVERLORD - LUCIANO ===\n");
    printf("\n=== Tetris Stack ===\n");
    
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    // inicializa a fila com 5 pecas
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
        printf("4 - Trocar peca da frente com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com os 3 da pilha\n");
        printf("6 - Desfazer ultima jogada\n");
        printf("7 - Inverter fila\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        
        if (opcao == 0) {
            printf("Saindo...\n");
            break;
        }
        
        // opcao 1: jogar peca
        if (opcao == 1) {
            if (filaVazia(&fila)) {
                printf("Nao ha pecas para jogar!\n");
            } else {
                Peca jogada = dequeue(&fila);
                ultimaJogada = jogada;
                temUltimaJogada = 1;
                printf("\nPeca jogada: [%c %d]\n", jogada.nome, jogada.id);
                enqueue(&fila, gerarPeca());
            }
        // opcao 2: reservar peca
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
                temUltimaJogada = 0;
            }
        // opcao 3: usar peca reservada
        } else if (opcao == 3) {
            if (pilhaVazia(&pilha)) {
                printf("Pilha vazia! Nao ha pecas reservadas.\n");
            } else {
                Peca usada = pop(&pilha);
                printf("\nPeca usada da reserva: [%c %d]\n", usada.nome, usada.id);
                temUltimaJogada = 0;
            }
        // opcao 4: trocar frente com topo
        } else if (opcao == 4) {
            trocarFrenteComTopo(&fila, &pilha);
            temUltimaJogada = 0;
        // opcao 5: trocar 3 primeiros
        } else if (opcao == 5) {
            trocar3Primeiros(&fila, &pilha);
            temUltimaJogada = 0;
        // opcao 6: desfazer
        } else if (opcao == 6) {
            desfazerJogada(&fila);
        // opcao 7: inverter fila
        } else if (opcao == 7) {
            inverterFila(&fila);
            temUltimaJogada = 0;
        } else {
            printf("Opcao invalida!\n");
        }
    }
    
    return 0;
}
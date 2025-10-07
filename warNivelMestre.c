#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;


typedef struct {
    char nome[30];
    char cor[10];
    char *missao;  
} Jogador;


void LimparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void liberarMemoria(Territorio *mapa, Jogador *jogadores, int numJogadores) {
    for (int i = 0; i < numJogadores; i++) {
        free(jogadores[i].missao);
    }
    free(jogadores);
    free(mapa);
    printf("\nMemória liberada com sucesso!\n");
}


void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}


void atacar(Territorio *atacante, Territorio *defensor) {
    printf("\n=== Ataque: %s (Exército %s) ataca %s (Exército %s) ===\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dado_atacante = rand() % 6 + 1;
    int dado_defensor = rand() % 6 + 1;

    printf("Dado do atacante: %d\n", dado_atacante);
    printf("Dado do defensor: %d\n", dado_defensor);

    if (dado_atacante > dado_defensor) {
        printf("O atacante venceu a batalha!\n");

        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
    } else {
        printf("O defensor resistiu ao ataque!\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0)
            atacante->tropas = 0;
    }

    printf("\n--- Estado após o ataque ---\n");
    printf("Atacante: %s (Exército %s, Tropas %d)\n",
           atacante->nome, atacante->cor, atacante->tropas);
    printf("Defensor: %s (Exército %s, Tropas %d)\n\n",
           defensor->nome, defensor->cor, defensor->tropas);
}


int verificarMissao(char *missao, Territorio *mapa, int tamanho, char *corJogador) {
    int contTerritorios = 0;
    int contTropas = 0;

    
    for (int i = 0; i < tamanho; i++) {
        if (strcmp(mapa[i].cor, corJogador) == 0) {
            contTerritorios++;
            contTropas += mapa[i].tropas;
        }
    }

    if (strstr(missao, "3 territorios") && contTerritorios >= 3)
        return 1;
    if (strstr(missao, "5 territorios") && contTerritorios >= 5)
        return 1;
    if (strstr(missao, "20 tropas") && contTropas >= 20)
        return 1;
    if (strstr(missao, "todos vermelhos")) {
        int existeVermelho = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0)
                existeVermelho = 1;
        }
        if (!existeVermelho)
            return 1;
    }

    return 0; // missão não cumprida
}

int main() {
    srand(time(NULL));

    int N;
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &N);
    LimparBufferEntrada();

    Territorio *mapa = (Territorio *) calloc(N, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa!\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        printf("\n=== Cadastro do Território %d ===\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        LimparBufferEntrada();
    }

    
    char *missoes[] = {
        "Conquistar 3 territorios seguidos",
        "Controlar 5 territorios",
        "Eliminar todas as tropas da cor vermelha",
        "Manter pelo menos 20 tropas ativas",
        "Dominar metade do mapa"
    };
    int totalMissoes = 5;

    
    int numJogadores;
    printf("\nQuantos jogadores participarão? ");
    scanf("%d", &numJogadores);
    LimparBufferEntrada();

    Jogador *jogadores = (Jogador *) calloc(numJogadores, sizeof(Jogador));
    if (jogadores == NULL) {
        printf("Erro ao alocar memória para jogadores!\n");
        free(mapa);
        return 1;
    }

    for (int i = 0; i < numJogadores; i++) {
        printf("\n=== Cadastro do Jogador %d ===\n", i + 1);

        printf("Nome do jogador: ");
        fgets(jogadores[i].nome, 30, stdin);
        jogadores[i].nome[strcspn(jogadores[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(jogadores[i].cor, 10, stdin);
        jogadores[i].cor[strcspn(jogadores[i].cor, "\n")] = '\0';

        jogadores[i].missao = (char *) malloc(100 * sizeof(char));
        atribuirMissao(jogadores[i].missao, missoes, totalMissoes);

        printf("Missão atribuída: %s\n", jogadores[i].missao);
    }

    
    char continuar = 's';
    while (continuar == 's' || continuar == 'S') {
        int iAtacante, iDefensor;

        printf("\nEscolha o número do território atacante: ");
        scanf("%d", &iAtacante);
        printf("Escolha o número do território defensor: ");
        scanf("%d", &iDefensor);
        LimparBufferEntrada();

        if (iAtacante < 1 || iAtacante > N || iDefensor < 1 || iDefensor > N) {
            printf("Índices inválidos!\n");
        } else if (iAtacante == iDefensor) {
            printf("Um território não pode atacar a si mesmo!\n");
        } else {
            atacar(&mapa[iAtacante - 1], &mapa[iDefensor - 1]);
        }

       
        for (int j = 0; j < numJogadores; j++) {
            if (verificarMissao(jogadores[j].missao, mapa, N, jogadores[j].cor)) {
                printf("\n🎉 O jogador %s venceu! Cumpriu a missão: %s\n",
                       jogadores[j].nome, jogadores[j].missao);
                liberarMemoria(mapa, jogadores, numJogadores);
                return 0;
            }
        }

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
        LimparBufferEntrada();
    }

    printf("\n=== ESTADO FINAL DO MAPA ===\n");
    for (int i = 0; i < N; i++) {
        printf("%d. %s (Exército %s, Tropas %d)\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }

    liberarMemoria(mapa, jogadores, numJogadores);
    return 0;
}

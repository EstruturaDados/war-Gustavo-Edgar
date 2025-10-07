#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

void LimparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void liberarMemoria(Territorio *mapa) {
    free(mapa);
    printf("\nMemória liberada com sucesso!\n");
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

        // Transferência da cor e metade das tropas para o defensor
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;

        // O atacante mantém a outra metade das tropas
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

int main() {
    srand(time(NULL)); // Inicializa números aleatórios

    int N;
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &N);
    LimparBufferEntrada();

    Territorio *mapa = (Territorio *) calloc(N, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
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

    printf("\n=== MAPA INICIAL ===\n");
    for (int i = 0; i < N; i++) {
        printf("%d. %s (Exército %s, Tropas %d)\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
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

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
        LimparBufferEntrada();
    }

    printf("\n=== ESTADO FINAL DOS TERRITÓRIOS ===\n");
    for (int i = 0; i < N; i++) {
        printf("%d. %s (Exército %s, Tropas %d)\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }

    liberarMemoria(mapa);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LARGURA 1024
#define MAX_ALTURA 768

/* 
   Determina o tipo da região analisada da imagem para ter uma ideia é se a região é uniforme ou segue padrão xadrez.
   Retornos:
     0 → região mista
     1 → branco uniforme
     2 → preto uniforme
     3 → padrão xadrez
*/

int tipo_regiao(int **img, int x_ini, int y_ini, int largura, int altura) {

    int cor0 = img[y_ini][x_ini];   // Cor inicial usada como referência
    int uniforme = 1;               // Assume uniforme até provar o contrário
    int xadrez = 1;                 // Assume padrão xadrez até falhar


       // O padrão xadrez depende da soma dos índices

    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {

            int cor = img[y_ini + y][x_ini + x];

            if (cor != cor0)
                uniforme = 0;

            // Cálculo padrão xadrez
            int esperado = ((x_ini + x) + (y_ini + y)) % 2;

            if (cor != esperado)
                xadrez = 0;
        }
    }

    if (uniforme)
        return (cor0 == 0 ? 1 : 2);

    if (xadrez)
        return 3;

    return 0;
}



// Função recursiva de codificação tendo a subdivisão seguindo quadrantes tipo quadtree
void codificar(int **img, int x_ini, int y_ini, int largura, int altura, int nivel) {

    int tipo = tipo_regiao(img, x_ini, y_ini, largura, altura);

    if (tipo == 1) { printf("B"); return; }
    if (tipo == 2) { printf("P"); return; }

    // Padrão xadrez
    if (tipo == 3 && nivel >= 3) {
        printf("XBPPB");
        return;
    }

    // Região mista
    printf("X");

    // O cálculo das metades usa divisão inteira, respeitando regiões ímpares para evitar perda de pixels.
    int largura_esq = largura / 2 + (largura % 2);
    int largura_dir = largura / 2;
    int altura_sup = altura / 2 + (altura % 2);
    int altura_inf = altura / 2;

    // Quadrantes processados em ordem fixa
    if (largura_esq > 0 && altura_sup > 0)
        codificar(img, x_ini, y_ini, largura_esq, altura_sup, nivel + 1);

    if (largura_dir > 0 && altura_sup > 0)
        codificar(img, x_ini + largura_esq, y_ini, largura_dir, altura_sup, nivel + 1);

    if (largura_esq > 0 && altura_inf > 0)
        codificar(img, x_ini, y_ini + altura_sup, largura_esq, altura_inf, nivel + 1);

    if (largura_dir > 0 && altura_inf > 0)
        codificar(img, x_ini + largura_esq, y_ini + altura_sup, largura_dir, altura_inf, nivel + 1);
}



// Lê imagem digitada manualmente
int **ler_manual(int *largura, int *altura) {

    printf("Digite a largura e a altura da imagem (ex: 4 4):\n");
    scanf("%d %d", largura, altura);

    // Alocação dinâmica da matriz em formato [altura][largura]
    int **img = malloc((*altura) * sizeof(int *));
    for (int i = 0; i < *altura; i++)
        img[i] = malloc((*largura) * sizeof(int));

    printf("Digite a matriz de 0s e 1s:\n");
    for (int y = 0; y < *altura; y++)
        for (int x = 0; x < *largura; x++)
            scanf("%d", &img[y][x]);

    return img;
}

// Função principal.
int main(int argc, char *argv[]) {

    int **imagem = NULL;
    int largura = 0, altura = 0;

    //Caso o usuário execute sem argumentos
    if (argc < 2 || strcmp(argv[1], "-m") != 0) {
        printf("Uso: %s -m\n", argv[0]);
        return 0;
    }

    // Ativa modo manual.
    imagem = ler_manual(&largura, &altura);

    printf("\nResultado da codificação:\n");
    codificar(imagem, 0, 0, largura, altura, 0);
    printf("\n");

    // Liberação de memória da matriz
    for (int i = 0; i < altura; i++)
        free(imagem[i]);
    free(imagem);

    return 0;
}

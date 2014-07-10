#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

#define MAX      256  /* número máximo de endereços (linhas) */
#define MAXLEN    10  /* tamanho máximo de uma expressão */
#define NUM_INST  19  /* nº de instruções distintas no HIPO */
#define MNE_SIZE   3  /* tamanho de uma sigla mnemônica */

const struct
{
    int codigo;
    char sigla[MNE_SIZE+1];
}
  mnemon[NUM_INST] = 
{
    {11, "LDA"}, {12, "STA"}, {21, "ADD"}, {22, "SUB"}, {23, "MUL"},
    {24, "DIV"}, {25, "REM"}, {29, "REV"}, {31, "INN"}, {41, "PRN"},
    {50, "NOP"}, {51, "JMP"}, {52, "JLE"}, {53, "JDZ"}, {54, "JGT"},
    {55, "JEQ"}, {56, "JLT"}, {57, "JGE"}, {70, "STP"}
};

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

/*-------------------------------------------------------------------*
 *
 *  Recebe a lista de instruções HIPO, devolvendo separadamente o
 *  código da instrução (obtido atraveś da sigla mnemônica) e o
 *  endereço, ambos em números de até dois dígitos decimais.
 *
 */
void converte(int n, char linhas[][MAXLEN], int cod[], int end[])
{
    int i, j;

    for (i = 0; i < n; i++) {
        
        char sigla[MNE_SIZE+1] = "";
        strncpy(sigla, linhas[i], 3);
        for (j = 0; j < NUM_INST; j++) {
            if (strcmp(sigla, mnemon[j].sigla) == 0) break;
        }
        if (j == NUM_INST) {
            fprintf(stderr, "ERRO: Instrução \"%s\" não reconhecida\n", sigla);
            exit(EXIT_FAILURE);
        }
        cod[i] = mnemon[j].codigo;
        sscanf(linhas[i] + MNE_SIZE, "%d", &end[i]);
        if (end[i] < 0 || end[i] >= MAX) {
            fprintf(stderr, 
                "ERRO: Endereço %d fora do limite permitido\n", end[i]);
            exit(EXIT_FAILURE);
        }
    }
}

/*-------------------------------------------------------------------*
 *
 *  Lê o arquivo linha por linha, guardando as expressões em um vetor.
 *  Devolve o número de expressões encontradas.
 *
 */
int leArquivo(char nome[], char linhas[][MAXLEN])
{
    int i;
    FILE *arq = fopen(nome, "r");

    if (arq == NULL) {
        fprintf(stderr, "ERRO: Arquivo \"%s\" indisponível\n", nome);
        exit(EXIT_FAILURE);
    }
    /* Lê uma linha de cada vez e armazena no vetor */
    for (i = 0; fscanf(arq, " %[^\n]", linhas[i]) != EOF; i++);

    fclose(arq);
    return i;
}

/*------------------------------------------------------------------*
 *
 * Pula comentários (iniciados com '#') na leitura do arquivo PPM.
 *
 */
void ignoraComentario(FILE *file)
{
    while (getc(file) == '\n');
    fseek(file, -1, SEEK_CUR);

    if (getc(file) == ';') {
        while (getc(file) != '\n');
    }
    else fseek(file, -1, SEEK_CUR);
}

/*-------------------------------------------------------------------*
 *
 *  Grava simultaneamente dois arquivos, um em formato decimal
 *  e outro em hexadecimal. Cada linha corresponde a um 
 *  código de instrução + um endereço.
 *
 */
void
gravaArquivos(int n, char nomeDec[], char nomeHex[], int cod[], int end[])
{
    int i;
    FILE *dec = fopen(nomeDec, "w");
    FILE *hex = fopen(nomeHex, "w");   

    if (dec == NULL || hex == NULL) {
        fprintf(stderr, "ERRO: Não foi possível gravar os arquivos\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < n; i++) {
        fprintf(dec, "+%d%02d\n",  cod[i], end[i]);
        fprintf(hex, "%02X%02X\n", cod[i], end[i]);
    }
    fclose(dec);
    fclose(hex);
}

/*-------------------------------------------------------------------*
 *
 *  Mostra a sintaxe de uso do programa, caso o usuário insira
 *  menos argumentos via linha de comando do que o necessário.
 *
 */
void mostreUso(char *nomePrograma)
{
    fprintf(stderr,
        "Uso: %s [nome-entrada] [nome-dec] [nome-hex]\n"
        "Ex:  %s entrada.txt dec.txt hex.txt\n", 
            nomePrograma, nomePrograma);
    exit(EXIT_FAILURE); 
}

/*-------------------------------------------------------------------*/

/*----------------*
 |   M  A  I  N   |
 *----------------*/

int main(int argc, char **argv)
{
    char linhas[MAX][MAXLEN];
    int n, cod[MAX], end[MAX];

    if (argc < 4) mostreUso(argv[0]);

    n = leArquivo(argv[1], linhas);
    converte(n, linhas, cod, end);
    gravaArquivos(n, argv[2], argv[3], cod, end);

    return EXIT_SUCCESS;
}

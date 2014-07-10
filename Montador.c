#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*-------------------------*
 |   D E F I N I Ç Õ E S   |
 *-------------------------*/

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

/* Arquivos de entrada e saída */
FILE *arqEntrada, *arqSaida;

/*-------------------*
 |   F U N Ç Õ E S   |
 *-------------------*/

void leitura();
bool ignoraComentario();
void verifica();
void converteInstancia();
void converteDado();
void mostreUso();

/*-------------------------------------------------------------------*
 */
void leitura()
{
    int posLida, posAt = 0;

    while (!feof(arqEntrada)) {
        while (ignoraComentario());
        fgetc(arqEntrada);
        fgetc(arqEntrada);
        if (feof(arqEntrada)) break;
        fseek(arqEntrada, -2, SEEK_CUR);
        fscanf(arqEntrada, "%d", &posLida);
        if (posAt < posLida) {
            for (; posAt < posLida; posAt++) {
                fprintf(arqSaida, "0000\n");
            }
        }
        verifica();
        posAt++;
    }
}

/*------------------------------------------------------------------*
 *
 * Pula comentários (iniciados com '#') na leitura do arquivo PPM.
 *
 */
bool ignoraComentario()
{
    int c;

    while (fgetc(arqEntrada) == '\n');
        fseek(arqEntrada, -1, SEEK_CUR);

    c = fgetc(arqEntrada);
    if (c == ';' || c == ' ') {
        while (fgetc(arqEntrada) != '\n');
        return true;
    }
    else fseek(arqEntrada, -1, SEEK_CUR);

    return false;
}

/*-------------------------------------------------------------------*
 */
void verifica()
{
    fgetc(arqEntrada); /* Lê o ' ' */
    fgetc(arqEntrada) == '{' ? converteInstancia()
                             : converteDado();
}

/*-------------------------------------------------------------------*
 *
 *  Recebe a lista de instruções HIPO, devolvendo separadamente o
 *  código da instrução (obtido atraveś da sigla mnemônica) e o
 *  endereço, ambos em números de até dois dígitos decimais.
 *
 */
void converteInstancia()
{
    int codigo, endereco;
    int i;

    /* Lê instrução */
    char sigla[MNE_SIZE+1] = "";
    fgets(sigla, 4, arqEntrada);
    fgetc(arqEntrada); /* Lê o '}' */

    /* Traduz instrução */
    for (i = 0; i < NUM_INST; i++) {
        if (strcmp(sigla, mnemon[i].sigla) == 0) break;
    }
    if (i == NUM_INST) {
        fprintf(stderr, "ERRO: Instrução \"%s\" não reconhecida\n", sigla);
        exit(EXIT_FAILURE);
    }
    codigo = mnemon[i].codigo;
    
    if (codigo != 70) fscanf(arqEntrada, "%d", &endereco);

    fprintf(arqSaida, "%02X", codigo);
    if (codigo != 70) fprintf(arqSaida, "%02X\n", endereco);
    else              fprintf(arqSaida, "00\n");
}

/*-------------------------------------------------------------------*
 *
 *  Recebe a lista de instruções HIPO, devolvendo separadamente o
 *  código da instrução (obtido atraveś da sigla mnemônica) e o
 *  endereço, ambos em números de até dois dígitos decimais.
 *
 */
void converteDado()
{
    int dado;

    fscanf(arqEntrada, "%d", &dado);
    fprintf(arqSaida, "%04X\n", dado & 0xffff);
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
        "Uso: %s [nome-entrada] [nome-hex]\n"
        "Ex:  %s entrada.txt hex.txt\n", 
            nomePrograma, nomePrograma);
    exit(EXIT_FAILURE); 
}

/*-------------------------------------------------------------------*/

/*----------------*
 |   M  A  I  N   |
 *----------------*/

int main(int argc, char **argv)
{
    if (argc < 3) mostreUso(argv[0]);

    arqEntrada = fopen(argv[1], "r");
    arqSaida   = fopen(argv[2], "w");
    leitura();

    fclose(arqEntrada);
    fclose(arqSaida);

    return EXIT_SUCCESS;
}

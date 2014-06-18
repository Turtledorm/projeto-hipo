/*************************************************************************

    >>> MAC0329 (Álgebra Booleana e Aplicações)
    Professor: Junior Barrera

    Grupo:
    > Daniel Toshio Iwamoto   - nº USP: 8516782
    > Gustavo Chicato Wandeur - nº USP: 7557797
    > Leonardo Pereira Macedo - nº USP: 8536065

*************************************************************************/


**************************************
    DESCRIÇÃO DOS CIRCUITOS - EP 3
**************************************


============
    MAIN
============

Está dividido em várias partes:

  - Program Counter (PC): Utiliza um contador de 8 bits para indicar
o próximo endereço de memória a ser usado. Inicia-se com 0 e acrescenta 1
conforme a pulsação de um clock. Trata também os casos JMP (carrega no
contador o endereço a qual se saltará) e STP (trava o contador).

  - Instruction Register (IR): Guarda pares instrução/endereço em um
registrador de 16 bits.

  - Controlador (CTRL): Interpreta a instrução que está no IR. Ver a seção
"Controlador" para maiores detalhes.

  - Acumulador (ACC): Guarda dados de uma posição da memória. Usado com
as instruções LDA e STA.

  - Memória (RAM): Guarda instruções/endereços e dados através do bloco
RAM do Logisim. Por padrão, sua saída é a posição de memória especificada
pelo PC. Trata também os casos LDA e STA ao usar o ACC e o IR.

=============
 CONTROLADOR
=============

ENTRADA:
  - Instrução (1 byte).

SAÍDA:
  - 1 bit indicando a instrução a ser usada (LDA, STA, NOP, JMP ou STP).

DESCRIÇÃO:
  Divide-se a instrução em 2 partes de 4 bits cada. Não é possível usar
o byte todo de uma vez no decodificador, pois este só pode ser usado
para valores de até 5 bits.
  Uma análise das 2 partes através de ANDs determinará qual o bit de
saída que corretamente indica a instrução.

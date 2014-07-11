/*************************************************************************

    >>> MAC0329 (Álgebra Booleana e Aplicações)
    Professor: Junior Barrera

    Grupo:
    > Daniel Toshio Iwamoto   - nº USP: 8516782
    > Gustavo Chicato Wandeur - nº USP: 7557797
    > Leonardo Pereira Macedo - nº USP: 8536065

*************************************************************************/


**************************************
    DESCRIÇÃO DOS CIRCUITOS - EP 4
**************************************


============
    MAIN
============

Está dividido em várias partes:

  - Program Counter (PC): Utiliza um contador de 8 bits para indicar
o próximo endereço de memória a ser usado. Inicia-se com 0 e acrescenta 1
conforme a pulsação de um clock. Trata também casos de saltos (condicionais
ou não) e STP. Foi colocado num subcircuito do main nesse EP 4.

  - Instruction Register (IR): Guarda pares instrução/endereço em um
registrador de 16 bits.

  - Controlador (CTRL): Interpreta a instrução que está no IR. Ver a seção
"Controlador" para maiores detalhes.

  - Acumulador (ACC): Guarda dados de uma posição da memória. Usado com
as instruções LDA, STA e operações do ALU.
  
  - Unidade Lógica e Aritmética (ALU): Realiza operações aritméticas, que
são controladas por um número de 3 bits.

  - Saltos Condicionais: Ver mais abaixo.

  - Memória (RAM): Guarda instruções/endereços e dados através do bloco
RAM do Logisim. Por padrão, sua saída é a posição de memória especificada
pelo PC. Lida com diversos tipos de instruções.

=============
 CONTROLADOR
=============

ENTRADA:
  - Instrução (1 byte).

SAÍDA:
  - 1 bit indicando a instrução a ser usada.
  - 3 bits para indicar a operação aritmética.
  - 3 bits para indicar a operação de salto condicional.

DESCRIÇÃO:
  Divide-se a instrução em 2 partes de 4 bits cada. Não é possível usar
o byte todo de uma vez no decodificador, pois este só pode ser usado
para valores de até 5 bits.
  Uma análise das 2 partes através de ANDs determinará qual o bit de
saída que corretamente indica a instrução.
  Para facilitar as operações aritméticas e de saltos condicionais, cria-se
um número de 3 bits indicando a instrução a ser feita.

=============
     ALU
=============

ENTRADA:
  - Valor do ACC (2 bytes).
  - Valor do endereço XY (2 bytes).
  - Número de 3 bits indicando a operação a ser feita.

SAÍDA:
  - Resultado da operação de ACC com o endereço XY.
  - Bit indicando overflow.
  - Bit indicando divisão por zero.

DESCRIÇÃO:
  A operação indicada pelos 3 bits é realizada entre ACC e XY. Se
houver overflow ou divisão por zero, um bit é ativado para logo em
seguida travar o circuito.

=============
Condicionais
=============

ENTRADA:
  - Valor do ACC (2 bytes).
  - Número de 3 bits indicando a comparação a ser feita.

SAÍDA:
  - Bit indicando se a comparação é verdadeira ou falsa.

DESCRIÇÃO:
  ACC é comparado com 0 segundo o operador indicado pelos 3 bits.
Isso é feito pelo subcircuito "comparator" do Logisim.
  O bit de saída indica a veracidade da comparação.

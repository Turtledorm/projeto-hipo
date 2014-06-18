/*************************************************************************

   >>> MAC0329 (Álgebra Booleana e Aplicações)
   Professor: Junior Barrera

   Grupo:
    > Daniel Toshio Iwamoto   - nº USP: 8516782
    > Gustavo Chicato Wandeur - nº USP: 7557797
    > Leonardo Pereira Macedo - nº USP: 8536065

*************************************************************************/


*******************************
    DESCRIÇÃO DOS CIRCUITOS
*******************************


============
    MAIN
============

ENTRADA:
- Números A e B, com 8 pinos cada um, representando números de 8 bits na forma
  de complemento de dois.
- Três bits, s2, s1 e s0, representando o seletor S da operação.

SAÍDA:
- Resultado R, também um número de 8 bits,
  originado de uma operação entre A e B.
- Um pino "overflow" para indicar se houve a ocorrência do mesmo.

DESCRIÇÃO:
   É a "interface" do programa, que recebe os valores e a operação desejada.


===========
    ALU
===========

ENTRADA:
- Números A e B
- Seletor S
- Clock

SAÍDA:
- Resultado R da operação
- Indicação de overflow

DESCRIÇÃO:
   Contém os circuitos de todas as operações. Devolve como saída o resultado
da operação selecionada, escolhida por meio de multiplexadores.
   O clock é usado apenas nas operações de multiplicação e divisão, que
trabalham com circuitos sequenciais.


============
    SOMA
============

ENTRADA:
- Números A e B

SAÍDA:
- Valor R = A + B
- Indicação do overflow

DESCRIÇÂO:
   Soma-se os bits correspondentes de A e B, começando pelo menos
significativo, através de circuitos "full adder". O "carry out" de cada
circuito é levado para o "carry in" do próximo (o primeiro começa com zero).
   Ocorre overflow se, e somente se, A e B forem de sinais iguais e o
resultado possuir sinal diferente.


==================
   (FULL ADDER)
==================

ENTRADA:
- Dois bits, X e Y
- Um bit "Carry In"

SAÍDA:
- O bit Sum = X + Y
- Um bit "Carry Out"

DESCRIÇÂO:
   É o circuito-chave por trás da soma, por levar em conta o carry in de uma
soma anterior e produzir um carry out para a próxima adição.


=================
    SUBTRAÇÃO
=================

ENTRADA:
- Números A e B.

SAÍDA:
- Valor R = A - B.
- Indicação do overflow.

DESCRIÇÃO:
   A subtração é uma soma de A com o twos complement de B, ou seja:
                        A - B = A + (-B)
   Esta operação dá overflow se A e B possuem sinais diferentes e R não tem
o mesmo sinal de A.


===========================
   (COMPLEMENTO DE DOIS)
===========================

ENTRADA:
- Um número B

SAÍDA:
- O twos complement de B, ou seja, -B

DESCRIÇÃO:
   Usa-se o método para calcular o twos complement de um número: cada bit é
trocado e a esse resultado soma-se 1.


=====================
    MULTIPLICAÇÃO
=====================

ENTRADA:
- Números A e B
- Clock

SAÍDA:
- Valor R = A * B
- Indicação do overflow

DESCRIÇÃO:
   Circuito sequencial onde o número A é somado |B| vezes. É feita a análise
para decidir-se qual será o sinal o resultado final.

   Algumas observações importantes no circuito:

1) Usa-se um contador e um túnel "cont" para verificar o fim desse loop.

2) Caso dê um overflow em alguma soma dentro do loop, o bit do overflow da
   multiplicação é ativado e fixado através de um SR flip-flop.

3) O "reset" serve para reiniciar o loop caso A ou B seja modificado durante
   a operação.


==============
   (MÓDULO)
==============

ENTRADA:
- Um número B de 8 bits
- Um bit "reverse"

SAÍDA:
- O módulo |B| de B

DESCRIÇÃO:
   O bit "reverse" analisa se o módulo será usual ou "inverso".
Este último pode ser explicado como:

   B =  B se B <= 0
   B = -B se B >  0

   Calcula-se o twos complement do número. A resposta dependerá do sinal 
do número e o valor do bit "reverse".


====================
   (BIT * NÚMERO)
====================

ENTRADA:
- Um número de 8 bits
- Um valor "bit"

SAÍDA:
- Um número R de 8 bits

DESCRIÇÃO:
   Devolve o produto de "bit" pelo número; sempre será ou próprio número,
se bit = 1, ou 00000000, se bit = 0.


===============
    DIVISÃO
===============

ENTRADA:
- Números A e B
- Clock

SAÍDA:
- Valor Q = A / B
- Valor R = A % B
- Indicação se houve divisão por zero (que é o overflow)

DESCRIÇÃO:
   A ideia principal é realizar o seguinte loop enquanto A > 0:
(Q = quociente, que começa com 0)
(R = resto, que começa com o valor de A)

R = R - B
Q = Q + 1

   Por ser um circuito sequencial complexo, ele será explicado por partes:

1) Analisar se há divisão por zero é fácil: é só verificar se B é igual a 0.

2) Na inicialização, A passa pelo circuito "módulo" e fica com o mesmo sinal
   de B. Isso facilita a operação mais para frente.

3) O loop principal então acontece, devidamente sincronizado com os clocks: 
   ocorre uma subtração e o contador do quociente é atualizado enquanto o sinal 
   de R for igual ao sinal de B. Os dois circuitos "bit * número" servem para 
   frear a subtração.

4) Quando o loop acaba, o resto pode ou não ser o definitivo, dependendo dos
   sinais dos números envolvidos. Isso é melhor explicado no circuito "resto".

5) O mesmo acontece com o quociente, que também depende dos sinais de A e B.
   A explicação detalhada está no circuito "quociente".

6) Assim como na multiplicação, o circuito é reinicializado caso o usuário
   altere o valor de A ou B. Isso ocorre graças ao valor "reset".


=============
   (RESTO)
=============

ENTRADA:
- Resto "temporário".
- Número B.
- Bit "mesmo sinal".

SAÍDA:
- Resto "definitivo" de A com B.

DESCRIÇÃO:
   Através do bit "mesmo sinal", escolhe-se o resto definitivo:

- Se "mesmo sinal" = 0, então o resto definitivo é o próprio resto temporário.

- Se "mesmo sinal" = 1, então o resto definitivo = B - resto temporário.


=================
   (QUOCIENTE)
=================

ENTRADA:
- Contador
- Resto definitivo
- Bit "acabou a divisão"
- Bit "mesmo sinal"

SAÍDA:
- Quociente da divisão de A com B

DESCRIÇÃO:
   Através do bit "mesmo sinal", escolhe-se o quociente:

- Se "mesmo sinal" = 0, então o quociente é o valor do contador.

- Para "mesmo sinal" = 1, primeiramente o circuito analisa se o resto deu 0.
  Em caso afirmativo, soma-se 1 no contador. O quociente é obtido através do
  complemento de dois desse resultado.


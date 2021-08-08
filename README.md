# Análise de algoritmos para Produtos polinomiais
Aplicação com propósito de analisar o tempo de execução de três algoritmos diferentes para produto polinomiais (***Brute Force***, ***Divide and Conquer com 4 produtos*** e ***Divide and Conquer com 3 produtos***) e compara-los em gráficos.

## Tecnologias

Escolhemos *C* puro como linguagem para aplicação principal por ser compilada e por dar alta liberdade de controle de memória, diminuindo tempo de execução do algoritmo.

Para gerar gráficos utilizamos *Python 3.x* e a biblioteca [_Matplotlib_](https://matplotlib.org/)  devido a facilidade de escrita do código e um resultado gráfico satisfatório.

Todo código foi executado no ambiente *Unix* para termos acesso ao header *<sys/resource.h>* e medirmos tempos de execução da CPU.

## Algoritmos

### Resumo/Informações relevantes
Para que as medições entre os três algoritmos seja justa, rodamos o código seguindo algumas regras:
 
 * Os dois polinômios devem ter o mesmo grau
 * Os graus dos polinômios são n - 1 = 2^k - 1
 * Todos os coeficientes devem ser diferentes de 0 ou 1
 * Cada polinômio deve ser representado por um vetor, onde cada item do vetor é um valor inteiro equivalente a um coeficiente. Por exemplo:
 Se `polinomio = 2 + 3x + 4x^2 +  5x^3`,
 então em C `polinomio = [2, 3, 4, 5]`.

### Brute Force
O algoritmo utiliza duas repetições *"for"* aninhadas para percorrer os dois vetores polinómios e iguala em cada posição "i" do vetor resultado a soma de todos os produtos entre "polinomio1[x]" e "polinomio2[y]" em que `x + y = i`.

A eficiência deste algoritmo é `T(n) = O(n^2)` devido as duas repetições *"for"* aninhadas.

### Soma/subtração de polinómios
Para melhor entendermos a origem do próximo algoritmo, vamos explicar soma/subtração de polinómios. O algoritmo utiliza uma repetição *"for"* para percorrer os dois vetores em uma mesma posição "i" e igualar `polinomioResultado[i] = polinomio1[i] +/- polinomio2[i]`.

A eficiência deste algoritmo é `T(n) = O(n)`.

### Divide and Conquer com 4 produtos
Agora que sabemos que a soma de dois polinômios é mais eficiente que o produto dos mesmo polinômios, podemos discutir Divide and Conquer. 

Vamos dividir um polinômio A em dois polinômios A0 e A1, ambos com metade do tamanho de A, A0 contendo a primeira metade dos coeficientes de A e A1 a segunda metade. Assim podemos reescrever `A =  A0 + A1*x^(n/2)`.

Então
`A * B  = (A0 + A1*x^n/2) * (B0 + B1*x^n/2)`
`A * B = A0*B0 + A0*B1*x^n/2 + A1*B0*x^n/2 + A1*B1*x^n`
`A * B = A0*B0 + (A0*B1 + A1*B0)*x^n/2 + A1*B1*x^n`

Desta forma para atingir o mesmo resultado do produto `A * B` de eficiência `O(n^2)` podemos fazer a soma de quatro produtos porem com `n = n/2`, logo `T(n) = 4 * T(n/2) + O(n) = O(n^2)`.   

### Divide and conquer com 3 produtos
Porém podemos fazer melhor com a mesma ideia. Se analisarmos a equação `A0*B0 + (A0*B1 + A1*B0)*x^n/2 + A1*B1*x^n`, podemos dividi-la em 3 termos:
 
 * `U = A0B0`
 * `Z = A1B1`
 * `Y = (A0+A1)*(B0+B1) = A0B0 + A0B1 + A1B0 + A1B1`

Se fizermos,
`Y-(U+Z) = A0B0-A0B0+A0B1+A1B0+A1B1-A1B1 = A0B1 + A1B0`
teremos exatamente os valores multiplicados por `x^n/2`.

Então não temos mais a necessidade de fazer a soma de quatro produtos, podemos fazer a soma de ***três*** produtos:
* `AO * BO`
* `A1 * B1`
* `(A0 + A1) * (B0 + B1)`

A eficiência deste algoritmo é:
`T(n) = 3 * T(n/2) + O(n)`
`T(n) = O(n^(log2(3))) = O(n^1.58)`

## Como compilar

Precisamos instalar três ferramentas para compilar a aplicação:
* Para executar o script principal "main.c", precisamos do compilador de C `GCC`, para mais informações de como instalar [acesse](https://gcc.gnu.org/).
* Para executar o script em python, precisados do  interpretador de Python 3.x, para mais informações de como instalar [acesse](https://www.python.org/).
* Também precisamos instalar a biblioteca _Matplotlib_, para mais informações de como instalar [acesse](https://matplotlib.org/stable/users/installing.html).

Uma vez vez que todas as ferramentas estiverem instaladas:
1. Para executar o script em C, na mesma pasta dos arquivos abra o terminal e execute o comando

` gcc -o main *.c -lm `

2. Para executar o script gerador de gráficos em python, na mesma pasta dos arquivos abra o terminal e execute o comando

`python plot_script.py`

## Integrantes
Estas aplicações foram desenvolvidas por [Michel Lessa Ziade](https://github.com/mlziade) e [  
Pedro Ivo Terra Bandoli]().

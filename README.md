# Trabalho Prático 2 – Programação Paralela com Pthreads

Este repositório contém a implementação do **Trabalho Prático 2 da disciplina de Sistemas Operacionais**, cujo objetivo é comparar o desempenho entre **implementações sequenciais e paralelas** utilizando a biblioteca **POSIX Threads (pthreads)**.

O trabalho avalia conceitos fundamentais de programação paralela, como criação de threads, divisão de carga de trabalho, sincronização, medição de tempo de execução e análise de desempenho (speedup e eficiência).

---

## Estrutura do Repositório

```
.
├── questao01/
│   ├── sequencial.c
│   └── paralelo.c
│
├── questao02/
│   ├── Sequencial.c
│   └── Paralelo.c
│
├── questao03/
│   ├── visualizador_sequencial.c
│   └── visualizador_paralelo.c
│
└── README.md
```

---

##  Questões Implementadas

###  Questão 1 – Produto Escalar de Vetores

* Implementação **sequencial** do cálculo do produto escalar entre dois vetores.
* Implementação **paralela** utilizando pthreads, com divisão balanceada dos elementos entre as threads.
* Cada thread calcula uma soma parcial, evitando condições de corrida.

---

###  Questão 2 – Multiplicação de Matrizes

* Implementação **sequencial** da multiplicação de duas matrizes quadradas N×N.
* Implementação **paralela** com pthreads, dividindo as linhas da matriz resultado entre as threads.
* Cada thread escreve em regiões exclusivas da matriz, garantindo ausência de race conditions.

---

###  Questão 3 – Visualizador de Imagens (Simulado)

* Implementação **sequencial** que simula o carregamento de imagens de forma sequencial.
* Implementação **paralela** onde cada imagem é carregada por uma thread distinta.
* Uso de `sleep()` para simular tempo de carregamento e `mutex` para sincronização.

---

##  Compilação

### Questão 1 – Produto Escalar

```bash
gcc -O2 -o sequencial sequencial.c

gcc -O2 -pthread -o paralelo paralelo.c
```

### Questão 2 – Multiplicação de Matrizes

```bash
gcc -O2 -o sequencial Sequencial.c

gcc -O2 -pthread -o paralelo Paralelo.c
```

### Questão 3 – Visualizador

```bash
gcc -O2 -o visualizador_seq visualizador_sequencial.c

gcc -O2 -pthread -o visualizador_par visualizador_paralelo.c
```

---

##  Execução

### Questão 1

```bash
./sequencial <tamanho_vetor>
./paralelo <tamanho_vetor> <num_threads>
```

### Questão 2

```bash
./sequencial <ordem_matriz>
./paralelo <ordem_matriz> <num_threads>
```

### Questão 3

```bash
./visualizador_seq
./visualizador_par
```

---

##  Medição de Tempo

O tempo de execução é medido utilizando `clock_gettime(CLOCK_MONOTONIC)`, garantindo maior precisão e evitando interferências de ajustes do sistema.

* **Ts** → tempo de execução sequencial
* **Tp** → tempo de execução paralelo (inclui criação e sincronização das threads)

---

##  Análise de Desempenho

As métricas utilizadas são:

* **Speedup (Sp):**

```
Sp = Ts / Tp
```

* **Eficiência (E):**

```
E = Sp / p
```

onde `p` representa o número de threads utilizadas.

Essas métricas permitem avaliar o ganho real de desempenho e o aproveitamento do paralelismo.

---

##  Observações Importantes

* Caso o número de threads seja maior que o tamanho do problema, o programa ajusta automaticamente para evitar threads ociosas.
* As implementações paralelas foram desenvolvidas de forma a evitar **condições de corrida**.
* O overhead de criação e sincronização das threads está incluído no tempo paralelo, refletindo um cenário real de execução.

---

##  Tecnologias Utilizadas

* Linguagem C
* POSIX Threads (pthreads)
* Sistema Operacional Linux

---

##  Conclusão

O projeto demonstra na prática como o paralelismo pode melhorar o desempenho computacional, ao mesmo tempo em que evidencia os custos associados à criação e sincronização de threads. A comparação entre versões sequenciais e paralelas reforça conceitos essenciais da disciplina de Sistemas Operacionais.

---

 **Trabalho desenvolvido para fins acadêmicos.**



# Trabalho 1 - Persistência Parcial em Árvore Binária de Busca

Disciplina: Estruturas de Dados Avançadas (CKP8077) - 2026.2

Alunos:

- Roberto Sergio Ribeiro de Meneses - 602390
- Erlanio Freire Barros - 603446

## Visão geral

O projeto implementa uma árvore binária de busca não balanceada com persistência parcial em C++20.

As operações de atualização sempre acontecem sobre a versão mais recente da estrutura. Cada inserção ou remoção cria uma nova versão, enquanto consultas podem acessar versões anteriores sem modificá-las.

A versão 0 representa a árvore vazia.

## Compilação e execução

O projeto utiliza `g++` com C++20.

Para compilar:

```bash
make build
```

As opções de compilação utilizadas são:

```text
-std=c++20 -Wall -Wextra -pedantic
```

Para executar:

```bash
make run INPUT=arquivo.in
```

Exemplo:

```bash
make run INPUT=tests/basico.in
```

O executável gerado recebe diretamente o caminho do arquivo de entrada:

```bash
./programa tests/basico.in
```

## Uso do programa

Os arquivos de entrada possuem um comando por linha.

| Comando | Significado |
|---|---|
| `INC x` | Insere uma nova ocorrência de `x` e cria uma versão. |
| `REM x` | Remove uma ocorrência de `x` e cria uma versão. |
| `SUC x v` | Consulta o menor valor estritamente maior que `x` na versão `v`. |
| `IMP v` | Imprime em ordem crescente os elementos da versão `v`. |

Valores repetidos são permitidos.

Uma remoção de valor inexistente não altera a árvore, mas ainda cria uma nova versão.

`SUC` e `IMP` não criam versões.

Quando uma consulta informa uma versão que não existe, a operação é realizada sobre a versão mais recente.

### Impressão de uma versão

Entrada:

```text
IMP 3
```

Saída:

```text
IMP 3
5 10 20
```

Se a versão representar uma árvore vazia, a linha após `IMP` fica vazia.

### Consulta de sucessor

Entrada:

```text
SUC 10 3
```

Saída:

```text
SUC 10 3
20
```

Se não existir valor estritamente maior:

```text
inf
```

## Exemplo completo

Entrada:

```text
INC 10
INC 5
INC 20
IMP 3
SUC 10 3
REM 5
IMP 4
IMP 3
```

Saída:

```text
IMP 3
5 10 20
SUC 10 3
20
IMP 4
10 20
IMP 3
5 10 20
```

Nesse exemplo, a remoção de `5` cria a versão 4, mas a versão 3 continua disponível para consulta.

## Organização do projeto

```text
.
├── Makefile
├── README.md
├── tests/
│   ├── oficiais/
│   ├── basico.in
│   ├── duplicados.in
│   ├── persistencia.in
│   ├── remocao_dois_filhos.in
│   ├── remocao_inexistente.in
│   ├── sucessor.in
│   └── versao_invalida.in
└── src/
    ├── main.cpp
    ├── core/
    │   ├── Node.hpp
    │   ├── PersistenceManager.hpp
    │   ├── PersistenceManager.cpp
    │   ├── PersistentBST.hpp
    │   └── PersistentBST.cpp
    └── io/
        ├── Command.hpp
        ├── InputParser.hpp
        └── InputParser.cpp
```

O código foi separado em três partes principais:

- `core`: implementação da árvore e do mecanismo de persistência;
- `io`: interpretação dos comandos do arquivo;
- `main.cpp`: integração entre entrada, execução das operações e saída.

`PersistentBST` contém as operações próprias da árvore binária de busca.

`PersistenceManager` concentra o controle de versões, modificações e cópia de nós.

`Node` mantém os dados necessários para representar um nó e seu histórico de alterações.

## Estratégia de persistência

A implementação utiliza persistência parcial por cópia de nós (*node copying*).

Cada nó mantém seus ponteiros de filhos originais e espaço para até duas alterações posteriores.

Uma alteração registra:

- a versão em que foi realizada;
- se ocorreu no filho esquerdo ou direito;
- o novo ponteiro de filho.

Enquanto houver espaço disponível, a alteração é armazenada no próprio nó.

Quando o limite de alterações é atingido, é criada uma nova cópia do nó já contendo seu estado mais recente. A ligação do pai é então atualizada para apontar para essa cópia.

Essa atualização também pode causar a cópia do pai, propagando o processo em direção à raiz.

As raízes das versões são mantidas separadamente para permitir acesso direto a estados anteriores.

Na remoção de um nó com dois filhos, é utilizado o menor elemento da subárvore direita. O valor do nó antigo não é alterado, porque esse nó ainda pode fazer parte de versões anteriores.

## Complexidade

Seja `h` a altura da árvore.

Como a árvore não é balanceada, `h` pode chegar a `O(n)` no pior caso.

As operações principais têm custo:

| Operação | Custo |
|---|---|
| Inserção | `O(h)` |
| Remoção | `O(h)` |
| Sucessor | `O(h)` |
| Impressão | `O(n)` |

A leitura de um ponteiro em uma versão possui custo adicional `O(1)`, pois cada nó mantém uma quantidade constante de modificações.

A criação de cópias possui custo amortizado constante por alteração, e o espaço adicional utilizado pelo mecanismo de persistência é linear no número de versões.

## Testes

Os cinco casos fornecidos pela disciplina estão armazenados em:

```text
tests/oficiais/
```

Também foram adicionados casos próprios para verificar:

- preservação de versões anteriores;
- chaves repetidas;
- remoção com dois filhos;
- remoção de valor inexistente;
- sucessor;
- consultas com versão inexistente.

Os testes oficiais podem ser executados com:

```bash
make test
```

O alvo compara a saída do programa com os arquivos `.out` utilizando `diff`.

Para limpar o executável:

```bash
make clean
```
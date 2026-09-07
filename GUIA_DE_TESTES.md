# Guia de testes — CPP Module 09 (STL)

Guia passo a passo para validar os três exercícios contra o **subject** e a
**régua de avaliação**. Cada seção tem:

1. **O que o exercício faz** (linguagem acessível)
2. **Como funciona por dentro** (algoritmo / container escolhido)
3. **Testes** mapeados item a item da régua, com o comando e o resultado esperado

> Convenção: `$>` é o prompt do shell. Saída em bloco é o que você **deve** ver.

---

## 0. Preparação do ambiente (antes de tudo)

A régua manda começar por aqui. Se algo falhar, a avaliação para.

### 0.1 Clonar num diretório vazio

```bash
$> mkdir /tmp/eval && cd /tmp/eval
$> git clone <url-do-repo> .
$> ls
ex00  ex01  ex02  data.csv?  ...
```

- Confirme que o repositório é **do aluno avaliado** e que é o projeto certo.
- `git log --oneline` para ver o histórico.

### 0.2 Checar armadilhas (obrigatório na régua)

```bash
$> alias                 # nenhum alias suspeito (ex.: 'cat', 'make', 'c++' redefinidos)
$> cat ~/.bashrc ~/.zshrc 2>/dev/null | grep -i alias
$> find . -name '*.sh' -o -name 'Makefile' | xargs less   # ler scripts e Makefiles
```

Tanto quem avalia quanto quem é avaliado devem ter lido qualquer script usado.

### 0.3 O arquivo `data.csv` do ex00

O ex00 lê um arquivo `data.csv` (a "tabela de preços do bitcoin") **do diretório
onde o programa roda**. Esse arquivo vem junto com o subject (`cpp_09.tgz`).
Neste repositório ele já está em `ex00/data.csv`. Se não estivesse, bastaria
copiar o do subject para dentro de `ex00/`.

```bash
$> ls -l ex00/data.csv      # deve existir e ter ~1000 linhas "data,taxa"
$> head -3 ex00/data.csv
date,exchange_rate
2009-01-02,0
2009-01-05,0
```

---

## 1. Pré-requisitos técnicos (valem para os 3 exercícios)

A régua: *"Qualquer um destes itens significa NÃO avaliar o exercício"* /
*"significa flag Forbidden Function"*. Rode estas checagens em **cada** pasta.

### 1.1 Compila com as flags exigidas, em C++98

```bash
$> cd ex00 && make && cd ..
$> cd ex01 && make && cd ..
$> cd ex02 && make && cd ..
```

Esperado: compila **sem nenhum warning e sem erro**. O `Makefile` de cada
exercício já usa:

```
CXX      = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
```

Teste reforçado (o subject exige que compile também com `-std=c++98` explícito e
sem quebrar no modo estrito):

```bash
$> c++ -Wall -Wextra -Werror -std=c++98 -pedantic -fsyntax-only ex00/*.cpp
$> c++ -Wall -Wextra -Werror -std=c++98 -pedantic -fsyntax-only ex01/*.cpp
$> c++ -Wall -Wextra -Werror -std=c++98 -pedantic -fsyntax-only ex02/*.cpp
```

Esperado: **nenhuma saída** (tudo ok).

### 1.2 Makefile correto

Cada `Makefile` deve ter as regras `$(NAME) all clean fclean re` e **não
pode relinkar**.

```bash
$> cd ex02
$> make            # compila
$> make            # 2ª vez -> "make: Nothing to be done for 'all'."  (NÃO relinka)
$> make re         # deve APAGAR e RECOMPILAR o binário
$> ls PmergeMe     # o binário existe de novo
$> make fclean     # remove .o e binário
$> ls PmergeMe     # "No such file or directory"
$> cd ..
```

Repita para `ex00` (binário `btc`) e `ex01` (binário `RPN`).

### 1.3 Sem funções C proibidas / sem STL fora do C++98

```bash
$> grep -rnE '\b(printf|fprintf|sprintf|malloc|calloc|realloc|free|strdup)\b' ex0*/*.cpp ex0*/*.hpp
$> grep -rn 'using namespace' ex0*/*.cpp ex0*/*.hpp
$> grep -rn 'friend'          ex0*/*.cpp ex0*/*.hpp
```

Esperado: **nada** nas três buscas.
(`*printf`, `*alloc`, `free` → flag *Forbidden Function*. `using namespace` /
`friend` → nota -42.)

### 1.4 Sem implementação de função em header (exceto templates)

```bash
$> grep -nE '\{' ex0*/*.hpp
```

Esperado: só chaves de **declaração de classe** e, no ex02, a **declaração** (não
a definição) dos métodos template. Nenhum corpo de função `.cpp`-style dentro de
`.hpp`. Caso contrário: nota 0 no exercício.

### 1.5 Include guards e headers independentes

Todo `.hpp` tem `#ifndef / #define / #endif` e inclui o que precisa.

```bash
$> for h in ex0*/*.hpp; do echo "== $h"; grep -E '#ifndef|#define|#endif' $h; done
$> echo '#include "ex00/BitcoinExchange.hpp"' | c++ -std=c++98 -fsyntax-only -x c++ -
```

Esperado: cada header com as 3 diretivas; o header compila sozinho.

### 1.6 Forma Canônica Ortodoxa (OCF)

Da régua do ex02: *"se qualquer classe que não seja de interface não estiver em
OCF, não avalie."* Vale a pena checar nas três.

```bash
$> grep -nE 'BitcoinExchange\(|~BitcoinExchange|operator=' ex00/BitcoinExchange.hpp
$> grep -nE 'RPN\(|~RPN|operator='                         ex01/RPN.hpp
$> grep -nE 'PmergeMe\(|~PmergeMe|operator='               ex02/PmergeMe.hpp
```

Esperado em cada uma: **construtor padrão, construtor de cópia,
`operator=`, destrutor** — os quatro presentes.

### 1.7 Sem segfault e sem memory leak (durante toda a defesa)

Qualquer crash → nota 0. Qualquer leak → flag de leak.
Nenhum dos três exercícios usa `new` (só containers/pilha), então não há alocação
manual para vazar. Ainda assim, confirme:

```bash
# Linux
$> valgrind --leak-check=full ./ex00/btc /caminho/input.txt
$> valgrind --leak-check=full ./ex01/RPN "1 2 +"
$> valgrind --leak-check=full ./ex02/PmergeMe 5 3 8 1 9

# macOS
$> leaks --atExit -- ./ex02/PmergeMe 5 3 8 1 9
```

Esperado: `0 errors`, `0 leaks` / `All heap blocks were freed`.

### 1.8 Regra dos containers do módulo

*"Um container usado não pode ser reutilizado no resto do módulo."*

| Exercício | Container(es) | Repetiu algum anterior? |
|-----------|---------------|-------------------------|
| ex00 | `std::map` | — |
| ex01 | `std::stack` (sobre `std::list`) | Não |
| ex02 | `std::vector` **e** `std::deque` | Não |

```bash
$> grep -rnE 'std::(vector|list|map|set|deque|stack|queue|multimap|multiset)' ex0*/*.hpp ex0*/*.cpp
```

Confirme visualmente que nenhum container reaparece de um exercício para o outro.

---

## 2. Exercício 00 — Bitcoin Exchange

### 2.1 O que faz (linguagem acessível)

Imagine uma **planilha histórica** com o preço de 1 bitcoin em várias datas
(`data.csv`). Você entrega ao programa um segundo arquivo com linhas do tipo
`data | quantidade`, e ele responde:

> "Nessa data, essa quantidade de bitcoin valia **quantidade × preço do dia**."

Regras:
- Se a data **não existe** na planilha, usa o preço da **data anterior mais
  próxima** (nunca a posterior).
- Valor tem que ser um número entre **0 e 1000**.
- Linhas com erro (data inválida, valor negativo, valor grande demais) geram uma
  mensagem de erro **mas o programa continua** lendo o resto do arquivo.

### 2.2 Como funciona por dentro

- Container: **`std::map<std::string, float>`** — data → preço.
- Por que `map` e não outro? O `map` é uma **árvore ordenada pela chave**. Como a
  data está no formato `AAAA-MM-DD`, a ordem alfabética **é** a ordem
  cronológica. Isso dá de graça:
  - inserção/consulta em **O(log n)**;
  - `lower_bound(data)` acha em O(log n) "a data igual, ou a primeira maior";
    recuando um passo (`--it`) chega-se à **data anterior mais próxima**, que é
    exatamente a regra do subject.
- Um `std::vector` obrigaria a ordenar e fazer busca binária na mão; um
  `unordered_map` (que nem é C++98) não manteria a ordem necessária para o
  "data mais próxima".

### 2.3 Testes — Code review (régua)

| Item da régua | Como checar | Esperado |
|---|---|---|
| Makefile com regras usuais | seção 1.2 nesta pasta | ok |
| Usa ao menos 1 container | `grep -n 'std::map' ex00/*.hpp` | aparece `std::map` |
| Aluno explica a escolha do container | pergunta oral | explica algo como a seção 2.2 |

Se o aluno não souber justificar o container → a avaliação para aqui.

### 2.4 Testes — Tratamento de erros (régua)

Crie um arquivo de entrada com problemas propositais (é o exemplo do subject):

```bash
$> cat > /tmp/input.txt <<'EOF'
date | value
2011-01-03 | 3
2011-01-03 | 2
2011-01-03 | 1
2011-01-03 | 1.2
2011-01-09 | 1
2012-01-11 | -1
2001-42-42
2012-01-11 | 1
2012-01-11 | 2147483648
EOF
```

**Sem argumento:**

```bash
$> ./ex00/btc
Error: could not open file.
```

**Arquivo com erros (não pode parar no meio):**

```bash
$> cd ex00 && ./btc /tmp/input.txt ; cd ..
2011-01-03 => 3 = 0.9
2011-01-03 => 2 = 0.6
2011-01-03 => 1 = 0.3
2011-01-03 => 1.2 = 0.36
2011-01-09 => 1 = 0.32
Error: not a positive number.
Error: bad input => 2001-42-42
2012-01-11 => 1 = 7.1
Error: too large a number.
```

Repare que ele **processou todas as linhas**: a data ruim (`2001-42-42`), o valor
negativo (`-1`) e o valor gigante (`2147483648`) viraram mensagens de erro no
meio, sem interromper o resto.

**Arquivo vazio:**

```bash
$> : > /tmp/vazio.txt
$> ./ex00/btc /tmp/vazio.txt        # não imprime nada, não quebra
```

**Data errada / valor fora da faixa** (pode inventar):

```bash
$> cat > /tmp/borda.txt <<'EOF'
date | value
2011-02-30 | 1
2011-13-40 | 1
abc | 1
2011-01-03 | -5
2011-01-03 | 1000
2011-01-03 | 1000.01
EOF
$> cd ex00 && ./btc /tmp/borda.txt ; cd ..
Error: bad input => 2011-02-30
Error: bad input => 2011-13-40
Error: bad input => abc
Error: not a positive number.
2011-01-03 => 1000 = 300
Error: too large a number.
```

- `2011-02-30` → 30 de fevereiro não existe → `bad input`
- `1000` → limite aceito; `1000.01` → `too large a number`

Se **qualquer** problema acontecer aqui (crash, parar no meio) → a avaliação
para.

### 2.5 Testes — Uso principal com `input.csv` (régua)

Use o `input.csv` que vem no topo da página de avaliação (pode editar os valores
à vontade) e rode:

```bash
$> cd ex00 && ./btc input.csv ; cd ..
```

**Confira algumas datas na mão** contra `data.csv`:

```bash
$> grep '2013-03-06' ex00/data.csv          # 2013-03-06,42
$> cd ex00 && printf 'date | value\n2013-03-06 | 1\n' | ./btc /dev/stdin ; cd ..
2013-03-06 => 1 = 42                          # 1 * 42  = 42  ✔
```

**Data que não existe na planilha → usa a anterior mais próxima:**

```bash
$> grep -n '2022-03-29\|2022-04-01' ex00/data.csv   # existe 2022-03-29,47115.93 ; não existe 2022-04-01
$> cd ex00 && printf 'date | value\n2022-04-01 | 2\n' | ./btc /dev/stdin ; cd ..
2022-04-01 => 2 = 94231.9                     # 2 * 47115.93 (dia 2022-03-29) ✔
```

**Data anterior à primeira da planilha → não há "data mais baixa":**

```bash
$> cd ex00 && printf 'date | value\n2008-01-01 | 1\n' | ./btc /dev/stdin ; cd ..
Error: date not found in database.
```

### 2.6 Observações para explicar na defesa

- A **primeira linha do arquivo de entrada é tratada como cabeçalho**
  (`date | value`) e ignorada — igual ao exemplo do subject.
- O parser aceita variações de espaço: `2011-01-03|3`, `  2011-01-03  |  3  `.
- A validação de data checa formato `AAAA-MM-DD`, faixas de mês/dia **e**
  calendário real (dias por mês, ano bissexto).

---

## 3. Exercício 01 — Reverse Polish Notation (RPN)

### 3.1 O que faz (linguagem acessível)

Notação polonesa reversa é escrever a conta com o **operador depois** dos
números: em vez de `3 + 4`, você escreve `3 4 +`. Não precisa de parênteses.

O programa recebe **uma** expressão dessas como argumento e imprime o resultado.
Ex.: `"8 9 * 9 - 9 - 9 - 4 - 1 +"` → `42`.

Regras:
- Os números **na entrada** têm sempre 1 dígito (`< 10`); os resultados
  intermediários podem ser grandes.
- Operadores aceitos: `+  -  /  *`.
- Erro (expressão malformada, divisão por zero, parênteses…) → imprime `Error` na
  **saída de erro** (stderr).

### 3.2 Como funciona por dentro

Lê os tokens da esquerda para a direita:
- **número** → empilha;
- **operador** → desempilha os **dois** de cima, faz a conta (`a op b`, com `a`
  sendo o penúltimo), empilha o resultado.

No fim, se sobrou **exatamente 1** valor na pilha, ele é o resultado; senão,
`Error`.

- Container: **`std::stack<int, std::list<int> >`**.
- Por que `stack`? A semântica de RPN é literalmente "último a entrar, primeiro a
  sair" (LIFO). `stack` expressa essa intenção diretamente e impede uso indevido
  (não dá para acessar o meio).
- É diferente do container do ex00 (`map`), respeitando a regra do módulo.

### 3.3 Testes — Code review (régua)

| Item da régua | Como checar | Esperado |
|---|---|---|
| Makefile com regras usuais | seção 1.2 em `ex01/` | ok |
| Usa ao menos 1 container | `grep -n 'std::stack' ex01/RPN.hpp` | aparece `std::stack` |
| Explica a escolha do container | pergunta oral | explica LIFO / seção 3.2 |
| Container **diferente** do ex00 | comparar | `stack` ≠ `map` ✔ |

Se repetir o container do ex00, ou não justificar → a avaliação para.

### 3.4 Testes — Uso principal (régua)

```bash
$> ./ex01/RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
42
$> ./ex01/RPN "7 7 * 7 -"
42
$> ./ex01/RPN "1 2 * 2 / 2 * 2 4 - +"
0
$> ./ex01/RPN "(1 + 1)"
Error
```

### 3.5 Testes — Uso avançado (régua, valores exatos do subject)

```bash
$> ./ex01/RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
42
$> ./ex01/RPN "9 8 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -"
42
$> ./ex01/RPN "1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /"
15
```

### 3.6 Testes — Erros (devem ir para stderr, sem crash)

```bash
$> ./ex01/RPN "(1 + 1)"      ; echo "---"   # parênteses não são suportados
Error
---
$> ./ex01/RPN "3 0 /"        ; echo "---"   # divisão por zero
Error
---
$> ./ex01/RPN "42 1 +"      ; echo "---"    # número com mais de 1 dígito na entrada
Error
---
$> ./ex01/RPN "1 2 3"       ; echo "---"    # sobra mais de um valor
Error
---
$> ./ex01/RPN "9 +"         ; echo "---"    # operador sem operandos suficientes
Error
---
$> ./ex01/RPN "a b +"       ; echo "---"    # token inválido
Error
---
$> ./ex01/RPN ""            ; echo "---"    # expressão vazia
Error
---
```

Confirme que `Error` sai em **stderr**:

```bash
$> ./ex01/RPN "3 0 /" 2>/dev/null        # não imprime nada em stdout
$> ./ex01/RPN "3 0 /" 1>/dev/null
Error
```

Qualquer crash aqui → nota 0.

---

## 4. Exercício 02 — PmergeMe

### 4.1 O que faz (linguagem acessível)

Recebe uma sequência de **inteiros positivos** como argumentos e devolve ela
**ordenada**, usando um algoritmo específico (merge-insertion / **Ford-Johnson**).
Além do resultado, ele **mede e mostra o tempo** que levou para ordenar usando
**dois containers diferentes**, para comparar o desempenho.

```
$> ./PmergeMe 3 5 9 7 4
Before: 3 5 9 7 4
After:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : ... us
Time to process a range of 5 elements with std::deque  : ... us
```

### 4.2 Como funciona o Ford-Johnson (linguagem acessível)

É um algoritmo de ordenação que tenta gastar **o mínimo de comparações
possível**. Ideia em 4 passos:

1. **Forma pares** de elementos e compara cada par. Em cada par, o maior é o
   "vencedor" e o menor é o "perdedor".
2. **Ordena só os vencedores**, recursivamente com o mesmo algoritmo. Como cada
   par anda junto, cada perdedor "sabe" quem é o seu vencedor.
3. Monta a **cadeia principal**: o perdedor do menor vencedor já entra de graça
   no começo (ele é menor que todo mundo à frente). Os outros perdedores ainda
   estão de fora ("pending").
4. **Insere os perdedores** um a um, por **busca binária**, mas numa **ordem
   especial** dada pela **sequência de Jacobsthal** (1, 3, 5, 11, 21, 43, …, cada
   termo = anterior + 2×o de antes). Essa ordem faz cada busca binária cair
   sempre num intervalo de tamanho "potência de 2 − 1", que é onde a busca
   binária é mais eficiente. Além disso, cada perdedor só pode entrar **antes do
   seu vencedor** na cadeia, o que limita ainda mais a busca.

### 4.3 Por que dois containers e por que a diferença de tempo

- Containers: **`std::vector`** e **`std::deque`** (ambos inéditos no módulo).
- `vector` = um bloco **contíguo** de memória → ótimo aproveitamento de cache da
  CPU, acesso por índice muito rápido. Custo: inserir no meio empurra o resto.
- `deque` = vários blocos ligados → inserir nas pontas é barato, mas o acesso e a
  inserção no meio têm mais indireção e menos localidade de cache.
- Na prática, para milhares de inteiros, o `vector` costuma sair **um pouco mais
  rápido**. O importante para a régua é o aluno **saber explicar a diferença**.
- `sortVector()` e `sortDeque()` são funções **separadas** (uma implementação por
  container, como o subject aconselha); só a aritmética da sequência de
  Jacobsthal e a recursão dos pares são compartilhadas.

### 4.4 Testes — Code review (régua)

| Item da régua | Como checar | Esperado |
|---|---|---|
| Makefile com regras usuais | seção 1.2 em `ex02/` | ok |
| Usa ao menos **2** containers | `grep -nE 'std::(vector|deque)' ex02/PmergeMe.hpp` | aparecem os dois |
| Explica a escolha dos containers | pergunta oral | seção 4.3 |
| merge-insertion (Ford-Johnson) presente e usado **em cada container** | ler `sortVector` / `sortDeque` / `sortPairsByWinner` / `jacobsthalOrder` em `ex02/PmergeMe.cpp` | pares → vencedores recursivos → inserção por Jacobsthal, chamado para vector e para deque |
| Explicação breve do algoritmo | pergunta oral | seções 4.2/4.3 |
| Containers **não** usados em exercícios anteriores | comparar | `vector`/`deque` ≠ `map`, `stack`, `list` ✔ |

Em caso de dúvida sobre o algoritmo → a avaliação para. Peça ao aluno para
apontar no código onde estão os 4 passos da seção 4.2.

### 4.5 Testes — Uso principal (régua)

**Entre 5 e 10 inteiros positivos à sua escolha:**

```bash
$> ./ex02/PmergeMe 3 5 9 7 4
Before: 3 5 9 7 4
After:  3 4 5 7 9
Time to process a range of 5 elements with std::vector : 6.00 us
Time to process a range of 5 elements with std::deque  : 22.00 us

$> ./ex02/PmergeMe 15 3 8 22 1 9 4 7 42 6
Before: 15 3 8 22 1 [...]
After:  1 3 4 6 7 [...]
Time to process a range of 10 elements with std::vector : ... us
Time to process a range of 10 elements with std::deque  : ... us
```

- `Before:` = sequência original; `After:` = ordenada.
- (Os tempos variam a cada execução; o formato é livre.)
- Se a sequência não sair ordenada → a avaliação para aqui.

**Confirme a ordenação completa** (o print corta em 5 elementos, então valide com
uma sequência que você conheça):

```bash
$> ./ex02/PmergeMe 9 8 7 6 5 4 3 2 1
Before: 9 8 7 6 5 [...]
After:  1 2 3 4 5 [...]
```

### 4.6 Testes — Volume grande (régua)

```bash
# Linux
$> ./ex02/PmergeMe `shuf -i 1-1000 -n 3000 | tr "\n" " "`

# macOS
$> ./ex02/PmergeMe `jot -r 3000 1 1000 | tr '\n' ' '`
```

Esperado: 4 linhas (`Before:`, `After:`, tempo vector, tempo deque), **sem
crash**, resultado ordenado. Exemplo das linhas de tempo:

```
Time to process a range of 3000 elements with std::vector : 3854.00 us
Time to process a range of 3000 elements with std::deque  : 3646.00 us
```

Peça ao aluno para **explicar a diferença de tempo** entre os dois containers
(seção 4.3). Sem explicação → a avaliação para.

Checagem independente de que ordenou mesmo:

```bash
# Linux
$> ./ex02/PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "` > /tmp/out.txt
$> sed -n 's/^After:  //p' /tmp/out.txt   # (só vê os 5 primeiros por causa do "[...]")
```

Para ver a lista inteira ordenada, teste com poucos elementos e confira a olho,
ou rode várias vezes com sequências pequenas conhecidas.

### 4.7 Testes — Erros (stderr, sem crash)

```bash
$> ./ex02/PmergeMe "-1" "2"        ; echo "[exit=$?]"   # número negativo
Error
[exit=1]
$> ./ex02/PmergeMe abc             ; echo "[exit=$?]"   # não é número
Error
[exit=1]
$> ./ex02/PmergeMe 1 2 abc 3       ; echo "[exit=$?]"   # token inválido no meio
Error
[exit=1]
$> ./ex02/PmergeMe                 ; echo "[exit=$?]"   # sem argumentos
Error
[exit=1]
$> ./ex02/PmergeMe 99999999999999  ; echo "[exit=$?]"   # estoura o int
Error
[exit=1]
```

Duplicados são aceitos (o subject deixa isso a critério do aluno):

```bash
$> ./ex02/PmergeMe 5 5 5 1 1 9 2
Before: 5 5 5 1 1 [...]
After:  1 1 2 5 5 [...]
```

---

## 5. Checklist final (flags da régua)

Marque cada item ao terminar:

- [ ] **Repositório** correto, do aluno, clonado em pasta vazia, sem alias malicioso
- [ ] **Compila** nos 3 com `c++ -Wall -Wextra -Werror -std=c++98`, sem warning
- [ ] **Makefile**: `all/clean/fclean/re/$(NAME)` funcionam, **não relinka**
- [ ] Sem `*printf` / `*alloc` / `free` → senão **Forbidden Function**
- [ ] Sem `using namespace` / `friend` → senão **-42**
- [ ] Sem biblioteca externa / recurso pós-C++98
- [ ] Nenhuma função implementada em `.hpp` (exceto template) → senão **0** no ex
- [ ] Include guards em todos os `.hpp`
- [ ] Classes em **Forma Canônica Ortodoxa**
- [ ] **Nenhum segfault / término anormal** em nenhum momento → senão **0**
- [ ] **Nenhum memory leak** (valgrind / leaks) → senão flag de leak
- [ ] **Regra dos containers**: `map` (ex00) → `stack`/`list` (ex01) → `vector`+`deque` (ex02), sem repetição
- [ ] **ex00**: usa container, justifica, trata erros sem parar, "data mais próxima menor" correta
- [ ] **ex01**: usa container (≠ ex00), justifica, resultados 42/42/15 corretos, erros em stderr
- [ ] **ex02**: usa 2 containers (≠ anteriores), justifica, Ford-Johnson presente e por container, 3000 números ok, explica a diferença de tempo
- [ ] **Modificação ao vivo** (se pedida no dia): aluno consegue alterar um trecho pequeno e explicar

---

## 6. Resumo rápido — comando → esperado

| # | Comando | Esperado |
|---|---|---|
| ex00 | `./btc` | `Error: could not open file.` |
| ex00 | `./btc input.txt` (com linhas ruins) | valores calculados + linhas de erro no meio, processa tudo |
| ex00 | data ausente na planilha | usa preço da data anterior mais próxima |
| ex00 | data antes da 1ª da planilha | `Error: date not found in database.` |
| ex01 | `./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"` | `42` |
| ex01 | `./RPN "1 2 * 2 / 2 + 5 * 6 - 1 3 * - 4 5 * * 8 /"` | `15` |
| ex01 | `./RPN "(1 + 1)"` | `Error` (stderr) |
| ex02 | `./PmergeMe 3 5 9 7 4` | `Before:` / `After: 3 4 5 7 9` / 2 linhas de tempo |
| ex02 | `./PmergeMe $(jot -r 3000 1 1000 \| tr '\n' ' ')` | ordena 3000, sem crash, 2 tempos |
| ex02 | `./PmergeMe "-1" "2"` | `Error` (stderr), exit ≠ 0 |

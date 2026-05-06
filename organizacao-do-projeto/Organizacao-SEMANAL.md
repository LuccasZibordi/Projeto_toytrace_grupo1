# Documento para organizar nossas atividades semanais

---

# Semana 1

## Resumo da semana
Nesta semana o foco foi entender completamente a estrutura do projeto toytrace, analisar os arquivos fornecidos pelo professor e compreender o fluxo geral do programa. Também realizamos a configuração inicial do ambiente, execução dos testes básicos e estudo das principais ferramentas utilizadas no projeto, como `ptrace`, `fork`, `waitpid` e a estrutura geral do runtime.

## Bruno

## João

## Luccas

### Dificuldades
Minhas maiores dificuldades foram as sintaxes dos códigos que foram dados e os que nós deveríamos escrever, principalmente as bibliotecas e funções com as quais eu nunca tinha trabalhado antes, além da própria forma como a linguagem C deve ser utilizada nesse tipo de projeto.

## Miguel

---

# Semana 2

## Resumo da semana
O objetivo desta semana foi implementar a criação do processo filho utilizando `fork()`, preparar o tracing com `PTRACE_TRACEME`, pausar o processo com `SIGSTOP` e iniciar o programa alvo usando `execvp()`. Também foi necessário entender o funcionamento do processo pai utilizando `waitpid()` para controlar o filho corretamente.

## Bruno

## João

## Luccas

## Miguel

---

# Semana 3

## Resumo da semana
Nesta etapa o foco foi construir o loop principal de tracing, permitindo detectar chamadas de sistema (syscalls) durante a execução do programa monitorado. Foram estudadas funções relacionadas ao controle do tracing, continuação do processo e detecção dos eventos de syscall.

## Bruno

## João

## Luccas

## Miguel

---

# Semana 4

## Resumo da semana
O objetivo desta semana foi trabalhar diretamente com registradores do processo monitorado utilizando `PTRACE_GETREGS`. Foram analisadas as informações reais das syscalls, incluindo argumentos, valores de retorno e identificação das chamadas realizadas pelo programa.

## Bruno

## João

## Luccas

## Miguel

---

# Semana 5

## Resumo da semana
Nesta semana o projeto começou a receber uma saída mais organizada e legível. O foco foi juntar corretamente os eventos de entrada e saída das syscalls e formatar essas informações de maneira semelhante ao funcionamento do `strace`, além de implementar tratamentos especiais para syscalls importantes.

## Bruno

## João

## Luccas

## Miguel

---

# Semana 6

## Resumo da semana
A última semana foi dedicada à finalização do projeto, correção de bugs, limpeza do código e execução dos testes finais. Também foi realizada a preparação do relatório final em PDF contendo a descrição do projeto, divisão das tarefas, dificuldades encontradas, aprendizados e possíveis melhorias futuras.

## Bruno

## João

## Luccas

## Miguel
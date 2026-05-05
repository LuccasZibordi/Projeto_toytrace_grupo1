# Mapa do codigo - toytrace

Este arquivo resume o entendimento inicial do grupo sobre o `toytrace`, conforme
a Semana 1 da especificacao. Nesta etapa o objetivo e explorar o codigo, entender
o fluxo principal e identificar onde a implementacao funcional vai comecar.

O `toytrace` e uma ferramenta didatica de Sistemas Operacionais inspirada no
`strace`. Ele deve executar um programa alvo, observar syscalls com `ptrace`,
montar eventos, parear entrada e saida e imprimir chamadas de sistema em formato
legivel.

## 1. Comandos testados

- `make`: projeto ja estava compilado. Resultado: `make: Nothing to be done for 'all'.`
- `./toytrace --help`: mostrou o formato `./toytrace trace [opcoes] -- programa [argumentos...]`.
- `./tests/targets/hello_write`: executou o alvo diretamente e imprimiu `ola toytrace`.
- `./toytrace trace -- ./tests/targets/hello_write`: parou no TODO esperado:
  `erro: TODO Semana 2: implementar launch_tracee()`.

Esse ultimo resultado confirma que, na Semana 1, o tracing real ainda nao
acontece. O objetivo e entender o caminho do codigo antes de implementar.

## 2. Fluxo principal

Fluxo por arquivos:

```text
src/main.c -> src/cli.c -> src/trace_runtime.c -> trace_observer()
           -> src/student/pairer.c -> src/student/formatter.c
```

Fluxo por funcoes:

```text
main() -> parse_args() -> trace_program() -> trace_observer()
       -> student_pair_syscall() -> student_format_event()
```

Esse caminho mostra que a execucao sai da CLI, passa pelo runtime de `ptrace` e
chega nas funcoes dos estudantes.

## 3. Onde o programa comeca

O programa comeca em `src/main.c`, na funcao `main(int argc, char **argv)`.

`main()` chama `parse_args()`, prepara `struct trace_state`, guarda a opcao
`--raw-events` e chama `trace_program(opts.target_argv, trace_observer, &state)`.
Essa chamada liga a entrada do usuario ao runtime que controlara o processo
monitorado.

## 4. Onde a CLI e tratada

A CLI fica em `src/cli.c`. A funcao principal e `parse_args()`.

Ela valida o comando `trace`, procura o separador `--`, reconhece `--raw-events`
e guarda em `opts->target_argv` o programa que sera monitorado. Tudo antes de
`--` pertence ao `toytrace`; tudo depois pertence ao alvo.

## 5. Onde o processo alvo e criado

O processo alvo deve ser criado em `src/trace_runtime.c`, na funcao
`launch_tracee(char *const argv[])`.

Ela ainda esta com TODO. Pela especificacao, a Semana 2 deve implementar:

```text
fork()
  filho: PTRACE_TRACEME -> SIGSTOP -> execvp()
  pai: retorna o pid do filho
```

O pai sera o `tracer`, que observa. O filho sera o `tracee`, que e o programa
monitorado.

## 6. Onde o runtime controla o ptrace

O runtime principal fica em `src/trace_runtime.c`, na funcao `trace_program()`.

Fluxo esperado:

1. validar o programa alvo;
2. criar o filho com `launch_tracee()`;
3. esperar `SIGSTOP` inicial com `wait_for_initial_stop()`;
4. configurar `PTRACE_O_TRACESYSGOOD`;
5. continuar com `PTRACE_SYSCALL`;
6. esperar paradas com `waitpid()`;
7. ler registradores com `PTRACE_GETREGS`;
8. preencher `struct syscall_event`;
9. chamar a callback `observer`.

No estado atual, a execucao para em `launch_tracee()`.

## 7. Onde o runtime chama o callback

O callback e chamado dentro de `trace_program()` por meio de `observer(&ev,
userdata)`. O tipo da callback esta em `include/trace_runtime.h` como
`trace_observer_fn`.

No codigo atual, a callback real e `trace_observer()`, em `src/main.c`.

## 8. Papel da callback

`trace_observer()` recebe eventos produzidos pelo runtime. Se `--raw-events`
estiver ativo, chama `student_debug_raw_event()`. No modo normal, chama
`student_pair_syscall()` para combinar entrada e saida e depois
`student_format_event()` para imprimir a syscall completa.

## 9. Estrutura principal de evento

A estrutura central esta em `include/syscall_event.h`


`entering` indica entrada ou saida da syscall. `args[6]` guarda argumentos e
`ret` guarda o retorno, valido principalmente na saida.

## 10. Arquivos que o grupo deve modificar

- `src/trace_runtime.c`: fluxo de `ptrace`, criacao do filho, paradas e eventos.
- `src/student/pairer.c`: pareamento entre entrada e saida de syscalls.
- `src/student/formatter.c`: formatacao das syscalls completas.

TODOs principais: `launch_tracee()`, `wait_for_initial_stop()`,
`configure_trace_options()`, `resume_until_next_syscall()`,
`wait_for_syscall_stop()`, `fill_event_from_regs()`, `student_pair_syscall()` e
`student_format_event()`.

## 11. Arquivos de apoio

- `include/cli.h`: opcoes da CLI.
- `include/trace_runtime.h`: runtime e callback.
- `include/syscall_event.h`: evento de syscall.
- `include/student_api.h`: API dos estudantes.
- `src/syscall_names.c`: nomes de syscalls.
- `src/trace_helpers.c`: leitura de strings do processo filho.
- `tests/targets/`: programas alvo de teste.
- `tests/unit/test_student.c`: testes unitarios.
- `tests/test_integration.py`: testes de integracao.

## 12. Conceito principal

Uma syscall gera duas paradas no `ptrace`: entrada e saida. Na entrada ficam os
argumentos; na saida fica o retorno. Por isso o projeto precisa parear os dois
eventos antes de imprimir algo como `write(1, 0x7ffd0000, 13) = 13`.

## 13. Duvida tecnica atual

A principal duvida tecnica e entender bem a ordem do loop de `ptrace`: quando
chamar `waitpid`, quando usar `PTRACE_SYSCALL`, como reconhecer uma parada de
syscall e como alternar entre entrada e saida.

## 14. Resumo

Na Semana 1, o grupo identificou que o programa comeca em `src/main.c`, a CLI
fica em `src/cli.c`, o runtime fica em `src/trace_runtime.c`, o alvo sera criado
em `launch_tracee()`, o callback usado e `trace_observer()` e os arquivos dos
estudantes ficam em `src/student/`.

O proximo passo, seguindo a especificacao, e iniciar a Semana 2 com
`launch_tracee()` e `wait_for_initial_stop()`.

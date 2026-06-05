# Projeto de Gerenciamento de Memória em um Microkernel simples para RISC-V

O projeto-base utilizado neste trabalho está disponível em: `https://github.com/VielF/BasicMicrokernel`

## Descrição
Sistema que simula um gerenciador de banco de dados utilizando threads e concorrência.

## Disciplina:
Sistemas Operacionais

## Acadêmicas:
- Maria Eduarda Santos
- Yasmin Tarnovski Faccin

## Tecnologias
- Revisar se está correto!
- Linguagem C
- Threads (pthread)
- Mutex (controle de concorrência)
- IPC (FIFO / Named Pipe)

## Requisitos de Execução
Este projeto deve ser executado em ambiente Linux ou no GitHub Codespaces, pois utiliza:
- ***
- Chamadas de sistema específicas do Linux

Obs: Não é garantido funcionamento em Windows sem adaptações.

---

## Como compilar
```bash
cd 
```

## Como executar
Em um terminal:
```bash
./cliente
```

---

## Funcionamento do sistema
1. O c


## Estrutura:
```bash
boot/
│
├── start.S       # Boot inicial e runtime mínimo
└── trap_entry.S  # Entrada de traps
include/
│
├──	fs.h
├──	memory.h
├──	scheduler.h
├──	string.h
├──	taks.h
└──	uart.h
kernel/
│
├── context.S     # Context switch
├── fs.c
├── main.c        # Inicialização do kernel
├── memory.c      # Heap do kernel
├── scheduler.c   # Escalonador
├── string.c
├── syscall.c
├── task.c        # Criação de tasks
├── timer.c       # Timer via SBI
├── trap.c        # Tratamento de interrupções
├── uart.c        # Saída serial
└── user.S
linker.ld         # Layout da memória
Makefile          # Compilação
README.md         # Explicação do projeto
```

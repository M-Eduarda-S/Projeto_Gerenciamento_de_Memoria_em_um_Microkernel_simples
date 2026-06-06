# Projeto de Gerenciamento de Memória em um Microkernel simples para RISC-V

O **projeto-base** utilizado neste trabalho está disponível em: `https://github.com/VielF/BasicMicrokernel`

## Descrição
Este projeto consiste na implementação de um **microkernel simples** para **arquitetura RISC-V**, com foco no **gerenciamento de memória dinâmica** no kernel.

Foram implementadas **funcionalidades** como:
- Alocação dinâmica de memória (`kmalloc`);
- Liberação de memória (`kfree`);
- Reutilização de blocos;
- Divisão de blocos;
- Coalescência de blocos livres;
- Estatísticas do heap.

Além disso, o sistema executa múltiplas tarefas (tasks) com troca de contexto e escalonamento.

## Disciplina:
Sistemas Operacionais

## Acadêmicas:
- Maria Eduarda Santos
- Yasmin Tarnovski Faccin

## Tecnologias
- Linguagem C
- Assembly RISC-V
- QEMU (emulação do hardware)
- GCC RISC-V `riscv64-unknown-elf-gcc`
- GitHub Codespaces / Linux

## Requisitos de Execução
O projeto deve ser executado em ambiente **Linux** ou **GitHub Codespaces**.

**Dependências necessárias:**
- gcc-riscv64-unknown-elf
- binutils-riscv64-unknown-elf
- qemu-system-riscv64
- make
- gdb-multiarch

Obs: Não é garantido funcionamento em Windows sem adaptações.

###  Instalação no GitHub Codespaces
No terminal:
```bash
sudo apt update
sudo apt install -y \
gcc-riscv64-unknown-elf \
binutils-riscv64-unknown-elf \
qemu-system-misc \
make \
gdb-multiarch
```

### Verificação
No terminal:
```bash
riscv64-unknown-elf-gcc --version
qemu-system-riscv64 --version
```

### Como compilar
No termianl:
```bash
make
```

### Como executar
No terminal:
```bash
qemu-system-riscv64 \
-machine virt \
-m 128M \
-nographic \
-bios default \
-kernel kernel.elf
```

---

## Funcionamento do sistema
1. O **kernel** é inicializado (`kernel_main`)
2. O **heap** é configurado (`memory_init`)
3. Duas tarefas são criadas (**task1** e **task2**)
4. O **escalonador** inicia (`scheduler_start`)
5. As tasks alternam execução via **troca de contexto**
6. Cada task realiza **operações de memória** e **imprime** estatísticas

---

## Gerenciamento de Memória
O sistema utiliza uma **lista encadeada de blocos de memória**:

**Cada bloco contém:**
- Tamanho;
- Ponteiro para o próximo bloco;
- Flag de livre/ocupado.

**Funcionalidades implementadas:**
- Múltiplas alocações;
- Liberação de memória;
- Reutilização de blocos livres;
- Divisão de blocos grandes;
- Coalescência de blocos adjacentes;
- Estatísticas do heap.

---

## Troca de Contexto
A troca de contexto foi implementada em Assembly `context.S`, salvando e restaurando registradores das tarefas.

**Problema identificado:**
- Sobrescrita de registradores a0/a1 durante o salvamento

**Solução:**
- Uso de registradores temporários (t5 e t6) para preservar ponteiros de contexto

---
#### Exemplo de Saída ao executar o projeto:
```bash
=== Kernel ===
Task 1 running
Memory used: 4096 bytes
Memory free: 8384440 bytes

Task 2 running
Memory used: 4096 bytes
Memory free: 8384440 bytes
```

---

## Estrutura:
```bash
boot/
│
├── start.S       # Boot inicial e runtime mínimo
└── trap_entry.S  # Entrada de traps
include/
│
├──	fs.h          # Interface do sistema de arquivos (não implementado)
├──	memory.h      # Interface do gerenciador de memória
├──	scheduler.h   # Interface do escalonador
├──	string.h      # Funções auxiliares de manipulação de strings
├──	taks.h        # Estrutura TCB e criação de tasks
└──	uart.h        # Interface de saída serial (UART)
kernel/
│
├── context.S     # Troca de contexto
├── fs.c          # Sistema de arquivos (não implementado)
├── main.c        # Inicialização do kernel
├── memory.c      # Heap do kernel
├── scheduler.c   # Escalonador
├── string.c      # Funções auxiliares de string
├── syscall.c     # Chamada de sistema (não implementado)
├── task.c        # Criação de tasks
├── timer.c       # Timer via SBI (não implementado)
├── trap.c        # Tratamento de interrupções (não implementado)
├── uart.c        # Saída serial
└── user.S        # Código de modo usuário
linker.ld         # Layout da memória
Makefile          # Compilação
README.md         # Explicação do projeto
```

### Observações
- Uso de registradores temporários pode gerar limitações em sistemas maiores;
- Projeto com **foco didático** para compreensão de sistemas operacionais.

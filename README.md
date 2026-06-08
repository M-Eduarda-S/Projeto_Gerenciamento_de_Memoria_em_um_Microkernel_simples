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
3. Um menu interativo é exibido via UART
4. O usuário executa testes de **gerenciamento de memória**
5. Tasks **podem ser** criadas dinamicamente
6. O **escalonador** é iniciado sob demanda (`scheduler_start`)
7. As **tasks** passam a executar em modo cooperativo

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
## Modelo de Escalonamento

O sistema utiliza **escalonamento cooperativo**, onde a troca de contexto ocorre apenas quando a task chama `yield()`.

Não há uso de interrupções de timer, portanto o projeto não implementa multitarefa preemptiva nesta versão. No entanto, a estrutura do sistema já permite evolução futura para esse modelo.

---

## Troca de Contexto
A troca de contexto foi implementada em Assembly `context.S`, salvando e restaurando registradores das tarefas.

**Problema identificado:**
- Sobrescrita de registradores a0/a1 durante o salvamento

**Solução:**
- Uso de registradores temporários (t5 e t6) para preservar ponteiros de contexto

---

## Menu de Testes de Memória
O sistema possui um menu interativo via **UART**, permitindo testar dinamicamente o comportamento do alocador de memória implementado.

### Objetivo
**Demonstrar, na prática:**
- Alocação e liberação de memória
- Reutilização de blocos
- Fragmentação e coalescência
- Uso de memória por tasks
- Estado interno do heap

### Opções do Menu
```bash
=== Menu de demonstracao de memoria ===
1 - Mostrar estatisticas do heap
2 - Teste: alocar p1, p2, p3
3 - Teste: liberar p2 e p1 (coalescencia)
4 - Teste: alocar p4 em memoria liberada
5 - Teste: liberar p3 e p4
6 - Criar tasks com stacks dinamicas
7 - Mostrar mapa do heap
8 - Iniciar o scheduler
0 - Sair do menu e travar kernel
```

### Descrição dos Testes
#### 1 - Estatísticas do Heap
Exibe:
```bash
Memória total
Memória utilizada
Memória livre
```

#### 2 - Alocação inicial (p1, p2, p3)
Realiza três alocações:
```bash
p1 = 1024 bytes
p2 = 2048 bytes
p3 = 512 bytes
```

**Permite observar:**
- Crescimento do uso de memória;
- Organização inicial dos blocos.

#### 3 - Liberação com Coalescência
Libera:
```bash
p2
p1
```
Como os blocos são adjacentes, ocorre **coalescência**, ou seja: os blocos livres são fundidos em um bloco maior.

#### 4 - Reutilização de Memória
Aloca:
```bash
p4 = 1536 bytes
```
**Este teste demonstra:**
- Reutilização de blocos livres;
- Eficiência do allocator (evita crescimento desnecessário do heap).

#### 5 - Liberação Final
Libera:
```bash
p3
p4
```
**Permite verificar:**
- Retorno da memória ao estado livre;
- Coalescência completa do heap.

#### 6 - Criação de Tasks
Cria duas tasks:
```bash
task1
task2
```
**Cada task:**
- Possui stack alocada dinamicamente;
- Imprime estatísticas de memória;
- Usa yield() para troca de contexto.

#### 7 - Dump do Heap
Exibe o estado interno do heap:
```bash
Lista de blocos
Tamanhos
Status (livre/ocupado)
```
Útil para depuração e validação do allocator.

#### 8 - Iniciar Scheduler

Inicia o escalonador cooperativo.

**-> Requisito:**
As tasks devem ser criadas antes (opção 6)

Comportamento das Tasks, após iniciar o scheduler:
```bash
Task 1 rodando
Task 1 estatisticas
Heap total: 8388608 bytes
Heap usado: 4144 bytes
Heap livre: 8384440 bytes

Task 2 rodando
Task 2 estatisticas
Heap total: 8388608 byte
sHeap usado: 4144 bytes
Heap livre: 8384440 bytes

Task 1 rodando
Task 1 estatisticas
Heap total: 8388608 bytes
Heap usado: 4144 bytes
Heap livre: 8384440 bytes

...
```
**As tasks:**
- Executam em loop infinito;
- Alternam via yield();
- Monitoram o estado do heap em tempo real;
- Conclusão dos Testes.


#### 0 - Encerrar Execução
Trava o kernel em loop infinito.

### Exemplo de Fluxo de Teste
2  -> Aloca blocos iniciais
3  -> Libera e testa coalescência
4  -> Testa reutilização
5  -> Libera tudo
6  -> Cria tasks
8  -> Inicia scheduler

**O menu permite validar que o alocador:**
- Reutiliza memória corretamente;
- Realiza coalescência de blocos adjacentes;
- Evita fragmentação excessiva;
- Suporta múltiplas alocações dinâmicas;
- Funciona corretamente com criação de tasks.

O sistema demonstra, de forma prática, conceitos **fundamentais de gerenciamento de memória em kernels**.

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

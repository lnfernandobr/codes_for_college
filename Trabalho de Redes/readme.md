O node é singleThread, o que sifnifica dizer que os seus
processos são executados em apenas uma thread. Por tanto, o node não aproveita o processamento multi-core
por padrão.

Um exemplo pratica que podemos observar essa peculiaridade do node, é que se
executarmos um programa no node da seguinte maneira, `node test.js`, o programa
será executado em um unico processo, digamos também que seu computador tenha 8
cores de processamento, será desperdiçado 7.

Mas isso não é uma limitação, visto que o node oferece serviços para que seja
feito o uso das CPU's com threads, bifurcação (fork), entre outros metodos.

Veremos aqui o `cluster`, 
*Uma única instância do Node.js é executada em um único encadeamento. Para aproveitar os sistemas com vários núcleos, o usuário algumas vezes desejará iniciar um cluster de processos do Node.js para manipular a carga.*


Com o módulo `cluster` do nodejs, o processo pai/master, pode ser bifurcado
em qualquer numero de processos filho/worker  e se comunicar com ele atraves
de comunicação IPC - (Inter-process communication) ``Não Hà memoria compartilhada entre os processos``


Mas algumas notas, vejamos a documentação:

_É importante ter em mente que os processos filhos gerados pelo Node.js são independentes do pai, com exceção do canal de comunicação IPC que é estabelecido entre os dois. Cada processo tem sua própria memória, com suas próprias instâncias do V8. Devido às alocações de recursos adicionais necessárias, a geração de um grande número de processos filhos do Node.js não é recomendada._


Quando um processo de trabalho/worke é criado, um canal IPC é criado entre o trabalhador e o mestre, permitindo que nos cominiquemos entre eles com o método `send`
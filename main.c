#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 10

typedef struct No
{
    int codigo;     // codigo da sala
    int capacidade; // capacidade da sala
    int altura;
    struct No *direita, *esquerda;
} TNo;

typedef struct heap
{
    int *vetor;
    int capacidade; // capacidade do heap
    int codigo;     // codigo da sala
} HeapMax;

typedef struct hash
{
    int codigo;      // codigo da sala
    char status[20]; // status da sala
} THash;

// funções da avl
TNo *criarNo(int codigo, int capacidade);
int maiorValor(int a, int b);
int alturaNo(TNo *no);
int fatorBalanceamento(TNo *no);
TNo *rotacaoDireita(TNo *raiz);
TNo *rotacaoDireitaEsquerda(TNo *raiz);
TNo *inserirNo(TNo *raiz, int codigo, int capacidade);
TNo *balancear(TNo *raiz);
TNo *removerNo(TNo *raiz, int codigo);
void exibirEmOrdem(TNo *raiz);
void lerArquivo(TNo **raiz, char *nomeArquivo);
void escreverArquivo(char *mensagem);
void consultarSala(TNo *raiz, int codigo);
void liberarArvore(TNo *raiz);

// funções da heap
HeapMax *criarHeap(int capacidade);
void inserirNoHeap(HeapMax *heap, int codigo, int prioridade);
void trocarHeap(int *v1, int *v2);
int removerHeap(HeapMax *heap);
void imprimirHeap(HeapMax *heap);
void destruirHeap(HeapMax *heap);
void heapify(HeapMax *heap, int i);
// void cancelarHeap(HeapMax *heap, int id);

// função do hash
void inicializarVet(THash vetHash[TAM]);
int funcaoHash(int chave);
void inserirHash(THash *vetHash, int codigo);
int buscaHash(int tab[], int chave); //
void bloquearSala(THash *vetHash, int codigo);
void desbloquearSala(THash *vetHash, int codigo);
void imprimirHash(THash *vetHash);
void removerHash(THash *vetHash, int codigo);
void buscarHash(THash *vetHash, int codigo);
void limparHash(THash *vetHash);

int main()
{
    int opcao = 0;
    TNo *raiz = NULL;
    char nomeArquivo[20];

    while (opcao != 2)
    {
        printf("\n===== GERENCIADOR DE RESERVA DE SALAS =====\n");
        printf("[1] Ler arquivo\n");
        printf("[2] Encerrar\n");
        printf("-------------------------------------------\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Nome do arquivo: ");
            scanf("%s", nomeArquivo);
            lerArquivo(&raiz, nomeArquivo);
            break;

        case 2:
            printf("Encerrando programa...\n");
            escreverArquivo("=============================================\n");
            liberarArvore(raiz);
            break;
        }
    }
    return 0;
}

/*	cria um novo nó e retorna o endereço do nó criado	*/
TNo *criarNo(int codigo, int capacidade)
{
    TNo *no = (TNo *)malloc(sizeof(TNo));

    if (no)
    {
        no->codigo = codigo;
        no->capacidade = capacidade;
        no->altura = 0; // o nó inserido é um nó folha
        no->direita = NULL;
        no->esquerda = NULL;

        /*  durante os testes iniciais utilizamos um printf pra mostrar o
           funcionamento do programa após a implementação da função pra escrever no
           arquivo preferimos manter a mensagem do printf e passar para a função que
           escreve no arquivo */
        char msg[100];
        sprintf(msg, "\nNovo nó: Sala código %d\n",
                no->codigo); // o sprintf é uma função que permite formatar uma
                             // string -> nesse caso precisamos armazenar o %d, que
                             // corresponde ao código da sala
        escreverArquivo(msg);

        // printf("Novo no -> Codigo %d\n", no->codigo);
    }
    else
    {
        escreverArquivo("\nErro ao alocar nó.\n");
    }

    return no;
}

/*	retorna o maior valor entre 'a' e 'b', que representam as alturas de
 * dois nós da árvore	*/
int maiorValor(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

/* retorna a altura de um nó, retorna -1 se o nó for NULL */
int alturaNo(TNo *no)
{
    if (!no)
    {
        return -1;
    }
    else
    {
        return no->altura;
    }
}

/*	calcula e retorna o fator de balanceamento de um nó
        se o valor for menor que -1 ou maior que 1 então está desbalanceado
 */
int fatorBalanceamento(TNo *no)
{
    if (!no)
    {
        return 0;
    }
    else
    {
        return (alturaNo(no->esquerda) -
                alturaNo(no->direita)); // altura da subarvore da esquerda - altura
                                        // da subarvore da direita
    }
}

/*	realiza rotação à esquerda	*/
TNo *rotacaoEsquerda(TNo *raiz)
{
    TNo *aux, *filho;

    // gif para visualizar a idea -> https://encurtador.com.br/7l0Tz
    aux = raiz->direita;
    filho = aux->esquerda; // guarda o filho (antigo) esquerdo de aux

    aux->esquerda = raiz; // a raiz se torna o filho (novo) esquerdo do aux
    raiz->direita =
        filho; // o filho (antigo) se torna filho direito do filho (novo)

    // a arvore mudou então devemos calcular a altura novamente
    raiz->altura =
        maiorValor(alturaNo(raiz->esquerda), alturaNo(raiz->direita)) + 1;
    aux->altura = maiorValor(alturaNo(aux->esquerda), alturaNo(aux->direita)) + 1;

    char msg[100];
    sprintf(msg, "\nRealizada rotação do nó %d à esquerda\n", raiz->codigo);
    escreverArquivo(msg);

    return aux; // o aux é a nova raiz
}

/*	realiza rotação à direita	*/
TNo *rotacaoDireita(TNo *raiz)
{
    TNo *aux, *filho;

    aux = raiz->esquerda;
    filho = aux->direita;

    aux->direita = raiz;
    raiz->esquerda = filho;

    raiz->altura =
        maiorValor(alturaNo(raiz->esquerda), alturaNo(raiz->direita)) + 1;
    aux->altura = maiorValor(alturaNo(aux->esquerda), alturaNo(aux->direita)) + 1;

    char msg[100];
    sprintf(msg, "\nRealizada rotação do nó %d à direita\n", raiz->codigo);
    escreverArquivo(msg);

    return aux;
}

/* 	realiza uma rotação dupla
        rotação DIREITA-ESQUERDA	*/
TNo *rotacaoDireitaEsquerda(TNo *raiz)
{
    raiz->direita = rotacaoDireita(raiz->direita);
    return rotacaoEsquerda(raiz);
}

/* 	realiza uma rotação dupla
        rotação ESQUERDA-DIREITA	*/
TNo *rotacaoEsquerdaDireita(TNo *raiz)
{
    raiz->esquerda = rotacaoEsquerda(raiz->esquerda);
    return rotacaoDireita(raiz);
}

/*	insere um novo nó na árvore	*/
TNo *inserirNo(TNo *raiz, int codigo, int capacidade)
{
    if (raiz == NULL)
    { // árvore vazia
        return criarNo(codigo, capacidade);
    }
    else
    {
        if (codigo < raiz->codigo)
        {
            raiz->esquerda =
                inserirNo(raiz->esquerda, codigo, capacidade); // inserção à esquerda
        }
        else if (codigo > raiz->codigo)
        {
            raiz->direita =
                inserirNo(raiz->direita, codigo, capacidade); // inserção à direita
        }
        else
        { // o codigo já existe na arvore

            char msg[100];
            sprintf(msg, "\nErro na inserção. Sala código %d já existe\n", codigo);

            escreverArquivo(msg);
        }
    }

    // faz o cálculo da altura
    raiz->altura =
        maiorValor(alturaNo(raiz->esquerda), alturaNo(raiz->direita)) + 1;

    // verifica se é necessário balancear a arvore
    raiz = balancear(raiz);

    return raiz;
}

/*	realiza o balanceamento da àrvore após uma inserção ou remoção
        fb = altura da subarvore esquerda - altura da subarvore direita
        se o valor for menor que -1 ou maior que 1 então está desbalanceado */
TNo *balancear(TNo *raiz)
{
    int fb = fatorBalanceamento(raiz);

    if (fb > 1)
    { // a subarvore esquerda está mais alta que a direita
        if (fatorBalanceamento(raiz->esquerda) >=
            0)
        { // subarvore esquerda da esquerda
            escreverArquivo("\nNecessária rotação à direita\n");
            raiz = rotacaoDireita(raiz);
        }
        else
        { // subarvore direita da esquerda
            escreverArquivo("\nNecessária rotação esquerda-direita\n");
            raiz = rotacaoEsquerdaDireita(raiz);
        }
    }
    else if (fb < -1)
    { // a subarvore direita está mais alta que a esquerda
        if (fatorBalanceamento(raiz->direita) <=
            0)
        { // subarvore direita da direita
            escreverArquivo("\nNecessária rotação à esquerda\n");
            raiz = rotacaoEsquerda(raiz);
        }
        else
        { // subarvore esquerda da direita
            escreverArquivo("\nNecessária rotação direita-esquerda\n");
            raiz = rotacaoDireitaEsquerda(raiz);
        }
    }

    return raiz;
}

/* realiza uma remoção na árvore	*/
TNo *removerNo(TNo *raiz, int codigo)
{
    if (!raiz)
    { // a àrvore está vazia ou o nó não foi encontrado
        printf("Codigo nao encontrado\n");
        return NULL;
    }

    if (codigo < raiz->codigo)
    { // se o código for menor então buscamos na
      // subarvore esquerda
        raiz->esquerda = removerNo(raiz->esquerda, codigo);
    }
    else if (codigo > raiz->codigo)
    { // se o código for maior então buscamos na
      // subarvore direita
        raiz->direita = removerNo(raiz->direita, codigo);
    }
    else if (raiz->codigo == codigo)
    { // o nó foi encontrado

        /* quando o nó correto é encontrado temos 3 opções:
                - nó com 2 filhos
                - nó com 1 filho
                - nó sem filhos (folha) */
        if (raiz->esquerda != NULL && raiz->direita != NULL)
        {
            TNo *aux = raiz->esquerda; // vamos substituir o valor do nó pelo maior
                                       // valor da subarvore esquerda

            // percorre a subarvore esquerda até encontrar o nó mais à direita (maior
            // valor)
            while (aux->direita != NULL)
            {
                aux = aux->direita;
            }
            raiz->codigo = aux->codigo; // substitui o valor do nó atual pelo valor do
                                        // maior a esquerda
            aux->codigo = codigo;
            escreverArquivo("Elemento trocado\n");
            raiz->esquerda = removerNo(raiz->esquerda, codigo);
            return raiz;
        }
        else
        { // nó com um filho
            TNo *aux;
            if (raiz->esquerda != NULL)
            {
                aux = raiz->esquerda;
                escreverArquivo("No com um filho (esquerda) removido\n");
            }
            else
            {
                aux = raiz->direita;
                escreverArquivo("No com um filho (direita) removido\n");
            }
            free(raiz);
            return aux;
        }
    }

    raiz->altura =
        maiorValor(alturaNo(raiz->esquerda), alturaNo(raiz->direita)) + 1;

    raiz = balancear(raiz);

    return raiz;
}

void exibirEmOrdem(TNo *raiz)
{
    if (raiz != NULL)
    {
        exibirEmOrdem(raiz->esquerda);
        char msg[100];
        sprintf(msg, " %d 	|     %d\n", raiz->codigo, raiz->capacidade);
        escreverArquivo(msg);
        exibirEmOrdem(raiz->direita);
    }
}

/*	le o arquivo de entrada	*/
void lerArquivo(TNo **raiz, char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    char operacao[30];
    int codigo, capacidade, prioridade;
    HeapMax *heap = criarHeap(10);
    THash vetHash[TAM];
    inicializarVet(vetHash);

    if (!arquivo)
    {
        printf("Erro o abrir o arquivo de entrada\n");
        return;
    }
    else
    {
        while (fscanf(arquivo, "%s", operacao) != EOF)
        {
            if (strcmp(operacao, "CRIAR_SALA") == 0)
            {
                fscanf(arquivo, "%d %d", &codigo, &capacidade);
                *raiz = inserirNo(*raiz, codigo, capacidade);
                inserirHash(vetHash, codigo);
            }

            else if (strcmp(operacao, "LISTAR_SALAS") == 0)
            {
                escreverArquivo("\n*** AVL ***\n");
                escreverArquivo("Codigo	| Capacidade\n");
                escreverArquivo("--------------------\n");
                exibirEmOrdem(*raiz);
                escreverArquivo("====================\n");

                escreverArquivo("*** HASH ***\n");
                imprimirHash(vetHash);
                escreverArquivo("====================\n");

                escreverArquivo("*** HEAP ***\n");
                imprimirHeap(heap);
            }

            else if (strcmp(operacao, "RESERVAR_SALA") == 0)
            {
                fscanf(arquivo, "%d %d", &codigo, &prioridade);
                inserirNoHeap(heap, codigo, prioridade);
                // função pra reservar sala aqui
            }

            else if (strcmp(operacao, "BLOQUEAR_SALA") == 0)
            {
                fscanf(arquivo, "%d", &codigo);
                bloquearSala(vetHash, codigo);
            }

            else if (strcmp(operacao, "DESBLOQUEAR_SALA") == 0)
            {
                fscanf(arquivo, "%d", &codigo);
                desbloquearSala(vetHash, codigo);
            }

            else if (strcmp(operacao, "CANCELAR_RESERVA") == 0)
            {
                if (fscanf(arquivo, "%d", &codigo) == 1)
                {
                    // removerNo(*raiz, codigo);
                    // removerNoHeap(heap, codigo);
                    removerHash(vetHash, codigo);
                }
            }

            else if (strcmp(operacao, "CONSULTAR_SALA") == 0)
            {
                if (fscanf(arquivo, "%d", &codigo) == 1)
                {
                    buscarHash(vetHash, codigo);
                }
            }

            else if (strcmp(operacao, "ATUALIZAR_PRIORIDADE") == 0)
            {
                if (fscanf(arquivo, "%d %d", &codigo, &prioridade) == 1)
                {
                    //
                }
            }
        }
    }
    fclose(arquivo);
}

/*  escreve no arquivo "saida.txt"
        nesse arquivo teremos os detalhes de todas as operações realizadas (log)
 */
void escreverArquivo(char *mensagem)
{
    FILE *arquivo = fopen("saida.txt", "a");
    if (!arquivo)
    {
        printf("Erro ao abrir o arquivo de saída\n");
        return;
    }
    else
    {
        fprintf(arquivo, "%s", mensagem);
    }
    fclose(arquivo);
}

/*  busca a sala pelo código informado  */
void consultarSala(TNo *raiz, int codigo)
{
    if (raiz != NULL)
    {
        consultarSala(raiz->esquerda, codigo);
        if (raiz->codigo == codigo)
        {
            char msg[100];
            sprintf(msg, "\nSala código %d encontrada\n", raiz->codigo);
            escreverArquivo(msg);
            return;
        }
        consultarSala(raiz->direita, codigo);
    }
    else
    {
        escreverArquivo("\nSala não encontrada.\n");
    }
}

// libera memória da árvore AVL
void liberarArvore(TNo *raiz)
{
    if (raiz != NULL)
    {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

/*============================== FUNÇÕES HEAP ==============================*/

/* separa espaço na memoria e inicializa as variaveis  */
HeapMax *criarHeap(int capacidade)
{
    HeapMax *heap = (HeapMax *)malloc(sizeof(HeapMax));
    heap->vetor = (int *)malloc((capacidade + 1) * sizeof(int));
    heap->vetor[0] = 0;
    heap->capacidade = capacidade;
    heap->codigo = 0;
    return heap;
}

/* verifica se o heap esta lotado, caso estiver faz a realocação e aumenta a
 * capacidade, caso esteja certo insere no heap */
void inserirNoHeap(HeapMax *heap, int codigo, int prioridade)
{
    if (heap->vetor[0] == heap->capacidade)
    { // se o heap estiver lotado
        int *vet = realloc(heap->vetor, (heap->capacidade * 2 + 1) * sizeof(int));
        if (vet != NULL)
        {
            heap->codigo = codigo;
            heap->vetor = vet;
            heap->capacidade *= 2;
        }
        else
        {
            escreverArquivo("nao pode realocar\n");
            return;
        }
    }
    heap->vetor[++heap->vetor[0]] = prioridade;
    int i = heap->vetor[0];
    while (i > 1 && heap->vetor[i] > heap->vetor[i / 2])
    {
        trocarHeap(&heap->vetor[i], &heap->vetor[i / 2]);
        i /= 2;
    }

    char msg[100];
    sprintf(msg, "\nReserva da sala %d com prioridade %d realizada\n", codigo,
            prioridade);
    escreverArquivo(msg);
}

/* troca os nos de lugar dentro do heap */
void trocarHeap(int *v1, int *v2)
{
    int aux = *v1;
    *v1 = *v2;
    *v2 = aux;
}

/* remove o heap prioridade */
int removerHeap(HeapMax *heap)
{
    if (heap->vetor[0] == 0)
    {
        printf("heap vazio\n");
        return -1;
    }
    else
    {
        int raiz = heap->vetor[1];
        heap->vetor[1] = heap->vetor[heap->vetor[0]];
        heap->vetor[0]--;
        heapify(heap, 1);
        return raiz;
    }
}

/* imprime o heap para verificação */
void imprimirHeap(HeapMax *heap)
{
    if (heap->vetor[0] == 0)
    {

        escreverArquivo("Heap vazio\n");
        return;
    }
    else
    {
        escreverArquivo("Heap: ");
        for (int i = 1; i <= heap->vetor[0]; i++)
        {
            char msg[100];
            sprintf(msg, "%d ", heap->vetor[i]);
            escreverArquivo(msg);
        }
        escreverArquivo("\n");
    }
}

/* libera o heap */
void destruirHeap(HeapMax *heap)
{
    if (heap)
    {
        free(heap->vetor);
        free(heap);
    }
}

/* arruma a arvore para caraterizar um heap, nesse caso o maior numero tem
 * prioridade, logo a primeira posição é o maior numero inserido */
void heapify(HeapMax *heap, int i)
{
    int maior = i;
    int esq = 2 * i;
    int dir = 2 * i + 1;

    if (esq <= heap->vetor[0] && heap->vetor[esq] > heap->vetor[maior])
    {
        maior = esq;
    }
    if (dir <= heap->vetor[0] && heap->vetor[dir] > heap->vetor[maior])
    {
        maior = dir;
    }
    if (maior != i)
    {
        trocarHeap(&heap->vetor[i], &heap->vetor[maior]);
        heapify(heap, maior);
    }
}

/*============================== FUNÇÕES HASH ==============================*/

/* inicia a tabela com zeros para verificação de ocupação, para evitar lixo de
 * memoria tambem */
void inicializarVet(THash vetHash[TAM])
{
    for (int i = 0; i < TAM; i++)
    {
        vetHash[i].codigo = 0;              // 0 = posição disponível
        strcpy(vetHash[i].status, "Vazio"); // vazio = POSIÇÃO disponível
    }
}

/* opera o modelo hash escolhido, trabalhar com o resto da divisão do nosso
 * codigo pelo tamanho da tabela */
int funcaoHash(int codigo) { return codigo % TAM; }

// insere a sala no hash - status "disponível"
void inserirHash(THash *vetHash, int codigo)
{
    int id = funcaoHash(codigo); // posicao da sala no vet

    while (vetHash[id].codigo != 0)
    {
        id = funcaoHash(id + 1);
    }

    // quando uma sala é criada é necessário definir o status dela como disponível
    // na tabela hash
    vetHash[id].codigo = codigo;
    strcpy(vetHash[id].status, "Disponível");

    char msg[100];
    sprintf(msg, "\nSala %d disponível\n", codigo);
    escreverArquivo(msg);
}

void bloquearSala(THash *vetHash, int codigo)
{
    int aux = 0; // variavel auxiliar para verificar se a sala foi encontrada

    for (int i = 0; i < TAM; i++)
    {
        if (vetHash[i].codigo == codigo)
        {
            aux = 1;
            strcpy(vetHash[i].status, "Bloqueada");

            char msg[100];
            sprintf(msg, "\nSala %d %s\n", codigo, vetHash[i].status);
            escreverArquivo(msg);
            break;
        }
    }

    if (aux == 0)
    {
        char msg[100];
        sprintf(msg, "Sala %d não encontrada\n", codigo);
        escreverArquivo(msg);
    }
}

void desbloquearSala(THash *vetHash, int codigo)
{
    int aux = 0; // variavel auxiliar para verificar se a sala foi encontrada

    for (int i = 0; i < TAM; i++)
    {
        if (vetHash[i].codigo == codigo)
        {
            aux = 1;
            strcpy(vetHash[i].status, "Disponível");

            char msg[100];
            sprintf(msg, "\nSala %d %s\n", codigo, vetHash[i].status);
            escreverArquivo(msg);
            break;
        }
    }
    if (aux == 0)
    {
        char msg[100];
        sprintf(msg, "Sala %d não encontrada\n", codigo);
        escreverArquivo(msg);
    }
}

void imprimirHash(THash *vetHash)
{
    for (int i = 0; i < TAM; i++)
    {
        char msg[100];
        sprintf(msg, "%d = %d  %s\n", i, vetHash[i].codigo, vetHash[i].status);
        escreverArquivo(msg);
    }
}

void removerHash(THash *vetHash, int codigo)
{
    int aux = 0; // variavel auxiliar para verificar se a sala foi encontrada

    for (int i = 0; i < TAM; i++)
    {
        if (vetHash[i].codigo == codigo)
        {
            aux = 1;
            vetHash[i].codigo = 0;
            strcpy(vetHash[i].status, "Vazio");

            char msg[100];
            sprintf(msg, "\nSala %d removida do hash\n", codigo);
            escreverArquivo(msg);
            break;
        }
    }
    if (aux == 0)
    {
        char msg[100];
        sprintf(msg, "Sala %d não encontrada\n", codigo);
        escreverArquivo(msg);
    }
}

void buscarHash(THash *vetHash, int codigo)
{
    int aux = 0; // variavel auxiliar para verificar se a sala foi encontrada

    for (int i = 0; i < TAM; i++)
    {
        if (vetHash[i].codigo == codigo)
        {
            aux = 1;

            char msg[100];
            sprintf(msg, "\nSala %d %s\n", codigo, vetHash[i].status);
            escreverArquivo(msg);
            break;
        }
    }
    if (aux == 0)
    {
        char msg[100];
        sprintf(msg, "Sala %d não encontrada\n", codigo);
        escreverArquivo(msg);
    }
}

void limparHash(THash *vetHash)
{
    for (int i = 0; i < TAM; i++)
    {
        vetHash[i].codigo = 0;
        strcpy(vetHash[i].status, "");
    }
}
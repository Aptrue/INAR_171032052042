#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OBSTAM 200
#define STRTAM 20

typedef struct AV {
    char nome[STRTAM];
    int valor;
    int id;
    double salario;
    char observacao[OBSTAM];
    struct AV *esq;
    struct AV *dir;
} ARVORE;

/* T21 2a */
ARVORE *buscaARVBin(ARVORE * r, char *nome)
{
    int comp;

    if (r == NULL) {
        return (r);
    }

    comp = strcmp(r->nome, nome);
    if (comp == 0) {
        return (r);             // achou
    }

    if (comp > 0) {
       
        return (buscaARVBin(r->esq, nome));
    }

    
    return (buscaARVBin(r->dir, nome));
}


int impARVVAL(ARVORE * r, int valor)
{
    int pe, pd;
    pe = 0;
    pd = 0;
    if (r == NULL) {
        /* Arvore vazia. profundidade é 0 */
        return 0;
    }

    if (r->valor == valor) {
        printf("%s\n", r->nome);
        /* O visita. Logo eu resolvi usar pre ordem */
    }

/* Chama à esquerda obtendo a profundidade da árvore da esquerda.
 * Depois soma mais 1 para creditar o meu nível 
 * */
    pe = impARVVAL(r->esq, valor) + 1;
    pd = impARVVAL(r->dir, valor) + 1;
    /* Retorna como profundidade desta sub-arvore a maior entre esquerda
     * ou direita */
    return (pe > pd ? pe : pd);
}


int lestring(char s[], int max)
{
    int i = 0;
    char letra;

    for (i = 0; i < (max - 1); i++) {
        letra = fgetc(stdin);
        if (feof(stdin)) {
            s[i] = 0;
            return (i);
        }
        if ((letra == '\n') && (i == 0)) {
            i = i - 1;
            continue;
        }

        if (letra == '\n')
            break;
        s[i] = letra;
    }
    s[i] = 0;
    return (i);
}

int cadastraNO(ARVORE * no)
{
    printf("Digite nome: \n");
    lestring(no->nome, STRTAM);

    printf("Digite valor:\n");
    scanf("%d", &no->valor);

    printf("Digite salario:\n");
    scanf("%lf", &no->salario);

    printf("Digite observacao:\n");
    lestring(no->observacao, OBSTAM);

    return (0);
}

/* A inserção é quase uma pesquisa. Se existir não pode inserir */
int insereNO(ARVORE * r, ARVORE * no)
{
    int comp, ret;

    if (r == NULL) {
        return (-1);            // isso nao deveria acontecer
    }

    comp = strcmp(r->nome, no->nome);
    if (comp == 0) {
        fprintf(stderr, "Ja existe\n");
        return (0);
    }

    if (comp > 0) {             // deve ser inserido na esquerda
        if (r->esq == NULL) {   // não tem esquerda
            r->esq = no;
            return (1);
        }

        ret = insereNO(r->esq, no);
        if (ret > 0) {
            ret++;
        }
        return (ret);
    }
    /* Entao deve ser inserido na direita */
    if (r->dir == NULL) {       // não tem esquerda
        r->dir = no;
        return (1);
    }
    ret = insereNO(r->dir, no);
    if (ret > 0) {
        ret++;
    }
    return (ret);
}


int main()
{
    ARVORE *r;                  // A raiz da árvore
    ARVORE *no;                 // um temp para cadastrar os dados antes de inserir
    int total = 0;
    int prof;
    char resp[10];
    char valorBuscado[STRTAM];

    r = NULL;

    printf("Digite os dados dos funcionários:\n");

    while (1) {
        printf("Voce quer digitar mais um funcionario? (SN)\nPara pesquisar um nome digite P\nPara pesquisar um valor digite V\n");
        lestring(resp, 10);
        if (strcasecmp(resp, "P") == 0) {
            printf("Digite um nome para ser buscado na arvore\n");
            lestring(valorBuscado, STRTAM);
            ARVORE *buscado = buscaARVBin(r, valorBuscado);
            if(buscado != NULL){
                printf("O nome buscado e %s\n", buscado->nome);
                printf("O salario buscado e %f\n", buscado->salario);
                printf("O valor buscado e %d\n", buscado->valor);
                printf("O observacao buscado e %s\n", buscado->observacao);
            } else {
                printf("Valor nao encontrado\n");
            }
        }

        if (strcasecmp(resp, "V") == 0) {
            printf("Digite um valor para ser buscado na arvore\n");
            int valor = 0;
            scanf("%d", &valor);
            impARVVAL(r, valor);
        }
        
        if (strcasecmp(resp, "S") != 0) {
            /* Meio tosco. Só continua se digitar "S" ou "s". Qualquer
             * outra coisa SAI. Se digitar SIM, sai. Cuidado
             * */
            break;
        }

        no = malloc(sizeof(ARVORE));
        if (no == NULL) {
            fprintf(stderr, "ERRO no malloc do nó\n");
            break;
        }
        cadastraNO(no);
        no->dir = NULL;
        no->esq = NULL;

        if (r == NULL) {
            /* Arvore vazia. Então este NO é o raiz */
            r = no;
            r->id = total++;
            printf("DEBUG: \"%s\" inserido como raiz\n", no->nome);
            continue;
        }
        prof = insereNO(r, no);
        if (prof > 0) {
            no->id = total++;
            printf("DEBUG: \"%s\" inserido no nivel %d\n", no->nome, prof);
            continue;
        }
        fprintf(stderr, "NAO FOI POSSIVEL INSERIR \"%s\" na árvore\n", no->nome);
        /* CUIDADO: so faz free para limpar e memória neste caso de erro.
         * Se obteve sucesso o nó entra na árvore e não deve ser dado free
         * */
        free(no);
    }

    printf("Digitados %d funcionarios\n", total);
}
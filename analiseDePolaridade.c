#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
 
#define QTD_SENT 20
#define QTD_PALA 70
#define QTD_LETR 21
 
typedef struct Sentenca {
    char *palavras[QTD_PALA];
    int tam;
} sentenca;
 
typedef struct Paragrafo {
    sentenca sentencas[QTD_SENT];
    int tam;
} paragrafo;
 
typedef struct Polaridade{
    int pos, neg;
} polaridade;
 
void imprimirPolaridde(polaridade pol){
 
    int pos = pol.pos;
    int neg = pol.neg;
 
    char str[9];
    int diff = pos-neg;
 
    if(diff > 3 || (pos > 0 && neg == 0)){
        strcpy(str, "positiva");
    }
    else if(diff < -3 || (neg > 0 && pos == 0)){
        strcpy(str, "negativa");
    }
    else if(neg >= 0 && pos >= 0 && abs(diff) <= 3){
        strcpy(str, "neutra");
    }
 
    printf("%d Palavras positivas, %d Palavras negativas: Polaridade %s\n", pos, neg, str);
}
 
// Remove todas as ocorrências dos caracteres
// especificados em str2 de str1
char *removerCaracteres(char *str1, char *str2){
    int pos = 0;
    char c = str1[pos];
 
    while(c!='\0'){
 
        int encontrou = 0;
 
        if(strchr(str2, c) != NULL){
            encontrou = 1;
            int temp_pos = pos;
 
            while(str1[temp_pos+1] != '\0'){
                str1[temp_pos] = str1[temp_pos+1];
                temp_pos++;
            }
            str1[temp_pos] = '\0';
        }
 
        if (!encontrou) pos ++;
        c = str1[pos];
    }
 
    return str1;
}
 
polaridade determinarPolaridadeSentenca(char *positivas, char *negativas, sentenca sent){
    int pos = 0, neg = 0;
 
    char *pos_save_ptr, *pos_token;
    
    char *pos_ptr = malloc(strlen(positivas) + 1);
    strcpy(pos_ptr, positivas);
 
    pos_token = strtok_r(pos_ptr, " \n", &pos_save_ptr);
    while(1){
 
        for (int i = 0; i < sent.tam; i++){
            //printf("%s <-> %s = %d\n", pos_token, sent.palavras[i], strcasecmp(pos_token, sent.palavras[i]));
            if(strcasecmp(pos_token, sent.palavras[i]) == 0) pos ++;
        }
 
        pos_token = strtok_r(NULL, " \n", &pos_save_ptr);
        if(pos_token == NULL) break;
 
    }
    free(pos_ptr);
 
    char *neg_save_ptr, *neg_token;
    char *neg_ptr = malloc(strlen(negativas) + 1);
    strcpy(neg_ptr, negativas);
 
 
    neg_token = strtok_r(neg_ptr, " \n", &neg_save_ptr);
    while(1){
 
        for (int i = 0; i < sent.tam; i++){
            //printf("%s %s = %d \n", neg_token, sent.palavras[i], strcasecmp(neg_token, sent.palavras[i]));
            if(strcasecmp(neg_token, sent.palavras[i]) == 0) neg ++;          
        }
 
        neg_token = strtok_r(NULL, " \n", &neg_save_ptr);
        if(neg_token == NULL) break;
    }
    free(neg_ptr);
 
    polaridade pol;
    pol.pos = pos;
    pol.neg = neg;
 
    return pol;
}
 
polaridade determinarPolaridadeParagrafo(char *positivas, char *negativas, paragrafo para){
    polaridade parcial, final;
    final.pos = 0;
    final.neg = 0;
 
    for (int i = 0; i < para.tam; i++) {
        parcial = determinarPolaridadeSentenca(positivas, negativas, para.sentencas[i]);    
        final.pos += parcial.pos;
        final.neg += parcial.neg;
    }
 
    return final;
}
 
polaridade determinarPolaridadeTexto(char *positivas, char *negativas, paragrafo *para, int sz_texto){
    polaridade parcial, final;
    final.pos = 0;
    final.neg = 0;
 
    for (int i = 0; i < sz_texto; i++) {
        parcial = determinarPolaridadeParagrafo(positivas, negativas, para[i]);    
        final.pos += parcial.pos;
        final.neg += parcial.neg;
    }
 
    return final;
}
 
 
 
int main(int argc, char const *argv[]){
 
    int sz_texto;
    scanf("%d ", &sz_texto);
 
    paragrafo paras[sz_texto];
 
    for (int i = 0; i < sz_texto; i++){
 
        char full_para[9040];
        fgets(full_para, 9040, stdin);
 
        char *para_ptr, *para_save_ptr, *sent_token;
        para_ptr = full_para;
 
        paragrafo *para = malloc(sizeof(paragrafo));
 
        int j = 0;
        for (;; j++, para_ptr = NULL){
            
            sent_token = strtok_r(para_ptr, ".\n", &para_save_ptr);
            if(sent_token == NULL) break;
 
            sentenca *sent = malloc(sizeof(sentenca));
 
            char *sent_ptr, *sent_save_ptr, *pala_token;
            sent_ptr = sent_token;
 
            int k = 0;
            for (;; k++, sent_ptr = NULL){
 
                pala_token = strtok_r(sent_ptr, " ", &sent_save_ptr);
                if(pala_token == NULL) break;
 
                char* temp = removerCaracteres(pala_token, ",;:?!");
                char* pala = malloc(strlen(temp) + 1);
 
                strcpy(pala, temp);
 
                sent->palavras[k] = pala;
                sent->tam = k+1;
            }
 
            para->sentencas[j] = *sent;
            para->tam = j+1;
 
            free(sent);
        }
 
        paras[i] = *para;
 
        free(para);
    }
 
    char positivas[1000], negativas[1000];
 
    fgets(positivas, 1000, stdin);
    fgets(negativas, 1000, stdin);
 
    int qtnd_questoes;
    scanf("%d ", &qtnd_questoes);
 
 
    for (int i = 0; i < qtnd_questoes; i++){
 
        int tipo_questao;
        scanf("%d ", &tipo_questao);
 
        int ix_paragrafo;
        polaridade pol;
 
        switch (tipo_questao){
        case 1: // Texto inteiro
            pol = determinarPolaridadeTexto(positivas, negativas, paras, sz_texto);
            imprimirPolaridde(pol);
            break;
 
        case 2: // Paragrafo i   
 
            scanf("%d", &ix_paragrafo);
 
            pol = determinarPolaridadeParagrafo(positivas, negativas, paras[ix_paragrafo]);
            imprimirPolaridde(pol);
 
            break;
 
        case 3:; // Sentenca mais longa considerando caracteres
 
            sentenca sent_longa;
            int max_qtd_caracteres = -1;
 
            for (int i = 0; i < sz_texto; i++){
                for (int j = 0; j < paras[i].tam; j++){
                   sentenca sent = paras[i].sentencas[j];
                   int qtd_letras = 0;
 
                    for (int k = 0; k < sent.tam; k++){
                        qtd_letras += strlen(sent.palavras[k]);
                    }
 
                    if (qtd_letras > max_qtd_caracteres){
                        max_qtd_caracteres = qtd_letras;
                        sent_longa = sent;
                    }
                }
            }
 
            pol = determinarPolaridadeSentenca(positivas, negativas, sent_longa);
            imprimirPolaridde(pol);
            
            break;
 
        case 4:; // Sentenca mais curta considerando palavras
            
            sentenca sent_curta;
            int min_qtd_palavras = 99999999;
 
            for (int i = 0; i < sz_texto; i++){
                for (int j = 0; j < paras[i].tam; j++){
                   sentenca sent = paras[i].sentencas[j];
 
                    if (sent.tam < min_qtd_palavras){
                        min_qtd_palavras = sent.tam;
                        sent_curta = sent;
                    }
                }
            }
 
            pol = determinarPolaridadeSentenca(positivas, negativas, sent_curta);
            imprimirPolaridde(pol);
 
            break;
        }
 
    }
 
    for (int i = 0; i < sz_texto; i++){
        for (int j = 0; j < paras[i].tam; j++){
            for (int k = 0; k < paras[i].sentencas[j].tam; k++){
                char* pala = paras[i].sentencas[j].palavras[k];
                //printf("%s ", pala);
                free(pala);
            }
            //printf("\n");
        }
    }
 
 
    return 0;
}
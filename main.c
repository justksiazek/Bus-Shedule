#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bus.h"

int main(int ile, char ** arg)
{
    struct pliki * pPliki; pPliki = NULL;
    int g = przelaczniki(&pPliki, ile, arg);

    struct przystanek * pPrzystanki; pPrzystanki = NULL;
    struct linia * pLinie; pLinie = NULL;

    analizujPlik(&pPrzystanki, &pLinie, pPliki);
    if(g == 1)
        zwrocTablice(pPrzystanki);

    opcje(menu(), &pPrzystanki, &pLinie);
    
    usunLinie(&pLinie);
    usunPrzystanki(&pPrzystanki);
    usunPliki(&pPliki);
    
    return 0;
}
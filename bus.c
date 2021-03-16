#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bus.h"

void dodajPlik(struct pliki ** pGlowa, char nazwa[50])
{
    struct pliki * p = (*pGlowa);
    (*pGlowa) = (struct pliki *)malloc(sizeof(struct pliki));
    strcpy((*pGlowa)->nazwa, nazwa);
    (*pGlowa)->pNext = p;
}

void usunPliki(struct pliki ** pGlowa)
{
    struct pliki * p;
    while(*pGlowa)
    {
        p = (*pGlowa)->pNext;
        free(*pGlowa);
        (*pGlowa) = p;
    }
}

struct przystanek * dodajPrzystanek(struct przystanek ** pGlowa, char nazwa[50])
{
    struct przystanek * p = znajdzPrzystanek(pGlowa, nazwa);
    if(p == NULL)
    {
        p = (*pGlowa);
        (*pGlowa) = (struct przystanek *)malloc(sizeof(struct przystanek));
        strcpy((*pGlowa)->nazwa, nazwa);
        (*pGlowa)->pLinie = NULL;
        (*pGlowa)->pNext = p;
        return (*pGlowa);
    }
    else
        return p;
}

struct przystanek * znajdzPrzystanek(struct przystanek ** pGlowa, char nazwa[50])
{
    struct przystanek * p = (*pGlowa);
    while(p)
    {
        if(strcmp(nazwa, p->nazwa)==0)
            return p;
        p=p->pNext;
    }
    return NULL;
}

void usunPrzystanki(struct przystanek ** pGlowa)
{
    struct przystanek * p;
    while(*pGlowa)
    {
        p = (*pGlowa)->pNext;
        usunListeLini(&(*pGlowa)->pLinie);
        free(*pGlowa);
        (*pGlowa) = p;
    }
}

void dodajListeLini(struct l_lini ** pGlowa, struct linia * linia)
{
    struct l_lini * p = (*pGlowa);
    while(p)
    {
        if(linia == p->pLinia)
            return;
        p=p->pNext;
    }
    p = (*pGlowa);
    (*pGlowa) = (struct l_lini *)malloc(sizeof(struct l_lini));
    (*pGlowa)->pLinia = linia;
    (*pGlowa)->pNext = p;
    return;
}

void usunListeLini(struct l_lini ** pGlowa)
{
    struct l_lini * p;
    while(*pGlowa)
    {
        p = (*pGlowa)->pNext;
        free(*pGlowa);
        (*pGlowa) = p;
    }
}

struct linia * dodajLinie(struct linia ** pGlowa, int numer)
{
    struct linia * p = znajdzLinie(&(*pGlowa), numer);
    if(p == NULL)
    {
        p = (*pGlowa);
        (*pGlowa) = (struct linia *)malloc(sizeof(struct linia));
        (*pGlowa)->numer = numer;
        (*pGlowa)->pNext = p;
        (*pGlowa)->godz = NULL;
        return (*pGlowa);
    }
    else
        return p;
}

struct linia * znajdzLinie(struct linia ** pGlowa, int numer)
{
    struct linia * p = (*pGlowa);
    while(p)
    {
        if(numer == p->numer)
            return p;
        p=p->pNext;
    }
    return NULL;
}

void usunLinie(struct linia ** pGlowa)
{
    struct linia * p;
    while(*pGlowa)
    {
        p = (*pGlowa)->pNext;
        usunGodzine(&(*pGlowa)->godz);
        free(*pGlowa);
        (*pGlowa) = p;
    }
}

void dodajGodzine(struct godzina ** pGlowa, struct przystanek * pPrzystanek, int hour, int min)
{ 
    if(!(*pGlowa))
    {
        (*pGlowa) = (struct godzina *)malloc(sizeof(struct godzina));
        (*pGlowa)->hour = hour;
        (*pGlowa)->min = min;
        (*pGlowa)->przyst = pPrzystanek;
        (*pGlowa)->pNext = NULL;
    }
    else
        dodajGodzine(&((*pGlowa)->pNext), pPrzystanek, hour, min);
        
}

void usunGodzine(struct godzina ** pGlowa)
{
    struct godzina * p;
    while(*pGlowa)
    {
        p = (*pGlowa)->pNext;
        free(*pGlowa);
        (*pGlowa) = p;
    }
}

void zwrocTablice(struct przystanek * pGlowa)
{
    struct przystanek * p = pGlowa;
    while(p)
    {
        // otworzenie pliku
        FILE * plik;
        if(!(plik=fopen(p->nazwa, "w")))
            printf("blad");
        else
        {
            // nazwa przystanku
            fprintf(plik, "%s\n=====\n", p->nazwa);
            struct l_lini * l = p->pLinie;
            while(l)
            {
                // numer linii
                fprintf(plik, "linia %d\n", l->pLinia->numer);
                struct godzina * g = l->pLinia->godz;
                while(g)
                {
                    // godziny odjazdu
                    if(g->przyst == p)
                    {
                        if(g->hour < 10)
                            fprintf(plik, "0%d:", g->hour);
                        else
                            fprintf(plik, "%d:", g->hour);
                        if(g->min < 10)
                            fprintf(plik, "0%d ", g->min);
                        else
                            fprintf(plik, "%d ", g->min);
                    }
                    g = g->pNext;
                }
                l = l->pNext;
                fprintf(plik, "\n");
            }
        }
        fclose(plik);
        p = p->pNext;
    }
}

void analizujPlik(struct przystanek ** pPrzystanki, struct linia ** pLinie, struct pliki * pPliki)
{
    while(pPliki)
    {
        FILE * plik;
        if(!(plik=fopen(pPliki->nazwa, "r")))
            printf("brak pliku '%s'", pPliki->nazwa);
        else
        {
            int n_lini; char trash[50];
            // odczyt numeru linii i dodanie go do listy
            fscanf(plik, "%s", trash);
            fscanf(plik, "%d", &n_lini);
            struct linia * l = dodajLinie(pLinie, n_lini);
            // odczyt rozkladu
            while(1)
            {
                int godz, min; char przst[50];
                fscanf(plik, "%d:", &godz); 
                if(feof(plik))
                    break;
                fscanf(plik, "%d", &min);
                fscanf(plik, "%s", przst);
                // dodanie lub znalezienie przystanku do listy
                struct przystanek * p = dodajPrzystanek(pPrzystanki, przst);
                // dodanie linii do listy asocjacyjnej laczacej przystanki z liniami
                dodajListeLini(&p->pLinie, l);
                // dodanie godziny odjazdu
                dodajGodzine(&l->godz, p, godz, min);
            }
            fclose(plik);
        }
        pPliki = pPliki->pNext;
    }
}

int przelaczniki(struct pliki ** pPliki, int ile, char ** arg)
{
    int i=0, g=0;
    for(i; i<ile; i++)
    {
        // g - program generuje pliki tekstowe (bez arg)
        if(strcmp(arg[i], "-g") == 0)
            g = 1;
        // i - pliki wejsciowe (dowolna liczba arg)
        if(strcmp(arg[i], "-i") == 0)
        {
            // czytanie zalaczonych plikow i dodawanie ich do listy
            printf("zalaczone pliki:\n");
            i++;
            while(i<ile && (arg[i][0] != '-'))
            {
                printf("%s ", arg[i]);
                dodajPlik(pPliki, arg[i]);
                i++;
            }
        }
    }
    printf("\n");
    return g;
}

void wyswietlRozkladPrzystanku(struct przystanek * pGlowa)
{
    printf("\n%s\n=====\n", pGlowa->nazwa);
    struct l_lini * l = pGlowa->pLinie;
    while(l)
    {
        // wyswietl linie odjezdzajace z tego przystanku
        printf("linia %d\n", l->pLinia->numer);
        struct godzina * g = l->pLinia->godz;
        while(g)
        {
            // wyswietl godziny odjazdow z tego przystanku
            if(g->przyst == pGlowa)
            {
                if(g->hour < 10)
                    printf("0%d:", g->hour);
                else
                    printf("%d:", g->hour);
                if(g->min < 10)
                    printf("0%d ", g->min);
                else
                    printf("%d ", g->min);
            }
            g = g->pNext;
        }
        l = l->pNext;
        printf("\n");
    }
}

void wyswietlRozkladLini(struct linia * pGlowa)
{
    printf("linia %d\n", pGlowa->numer);
    struct godzina * g = pGlowa->godz;
    while(g)
    {
        if(g->hour < 10)
            printf("0%d:", g->hour);
        else
            printf("%d:", g->hour);
        if(g->min < 10)
            printf("0%d ", g->min);
        else
            printf("%d ", g->min);
        printf("%s\n", g->przyst->nazwa);
        g = g->pNext;
    }
}

void wyswietlLinie(struct linia ** pGlowa)
{
    struct linia * l = (*pGlowa);
    while(l)
    {
        printf("%d ", l->numer);
        l = l->pNext;
    }
}

void wyswietlPrzystanki(struct przystanek ** pGlowa)
{
    struct przystanek * p = (*pGlowa);
    while(p)
    {
        printf("%s\n", p->nazwa);
        p = p->pNext;
    }
}

void znajdzPolaczenie(struct przystanek * start, struct przystanek * stop)
{   
    struct l_lini * b = start->pLinie;
    while(b)
    {
        // wez linie ze start
        struct l_lini * e = stop->pLinie;
        while(e)
        {
            // sprawdz czy jest taka w stop
            if(b->pLinia == e->pLinia)
            {
                printf("\n%d\n", b->pLinia->numer);
                struct godzina * g = b->pLinia->godz;
                while(g)
                {
                    // wyswietl godziny odjazdow z przystanku startowego
                    if(g->przyst == start)
                    {
                        if(g->hour < 10)
                            printf("0%d:", g->hour);
                        else
                            printf("%d:", g->hour);
                        if(g->min < 10)
                            printf("0%d ", g->min);
                        else
                            printf("%d ", g->min);
                    }
                    g = g->pNext;
                }
                printf("\n");
            }
            e = e->pNext;
        }
        b = b->pNext;
    }
}

char menu()
{
    printf("\nmenu:\n  z -> znajdz polaczenie\n  p -> wyswietl rozklad przystanku\n  l -> wyswietl rozklad linii\n  q -> zakoncz prace ");
    char opcja = getchar();
    while(opcja!='z' && opcja!='p'&& opcja!='l' && opcja!='q')
    {
        printf("nie ma takiej opcji");
        getchar(); opcja = getchar();
    }
    getchar();
    return opcja;
}

void opcje(char opcja, struct przystanek ** pPrzystanki, struct linia ** pLinie)
{
    while(opcja!='q')
    {
        switch(opcja)
        {
            case 'z':
                // znajdz polaczenie
                wyswietlPrzystanki(pPrzystanki);
                char n_start[50], n_stop[50];
                printf("podaj przystanek poczatkowy: "); scanf("%s", n_start);
                printf("podaj przystanek koncowy: "); scanf("%s", n_stop);
                struct przystanek * start = znajdzPrzystanek(pPrzystanki, n_start);
                struct przystanek * stop = znajdzPrzystanek(pPrzystanki, n_stop);
                if(start==NULL || stop == NULL)
                    printf("nie ma takiego przystanku\n");
                else
                    znajdzPolaczenie(start, stop);
                break;
            case 'p':
                // wyswietl rozklad przystanku
                printf("przystanki w bazie:\n");
                wyswietlPrzystanki(pPrzystanki);
                printf("wpisz nazwe przystanku: "); char nazwa[50]; scanf("%s", nazwa);
                struct przystanek * p = znajdzPrzystanek(pPrzystanki, nazwa);
                if(p==NULL)
                    printf("nie ma takiego przystanku\n");
                else
                    wyswietlRozkladPrzystanku(p);                
                break;
            case 'l':
                // wyswietl rozklad linii
                printf("linie w bazie: ");
                wyswietlLinie(pLinie);
                printf("\nwybierz numer linii: "); int numer; scanf("%d", &numer);
                struct linia * l = znajdzLinie(pLinie, numer);
                if(l==NULL)
                    printf("nie ma takiej linii\n");
                else
                    wyswietlRozkladLini(l);
                break;
        }
        getchar();
        opcja = menu();
    }
}

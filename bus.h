#ifndef _BUS_H
#define _BUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct pliki{
    char nazwa[50];
    struct pliki * pNext;
};

struct przystanek;

struct godzina{
    int hour;
    int min;
    struct przystanek * przyst;
    struct godzina * pNext;
};

struct linia{
    int numer;
    struct godzina * godz;
    struct linia * pNext;
};

struct l_lini{
    struct linia * pLinia;
    struct l_lini * pNext;
};

struct przystanek{
    char nazwa[50];
    struct l_lini * pLinie;
    struct przystanek * pNext;
};

// funkcja dodaje nowa pozycje do listy plikow
void dodajPlik(struct pliki ** pGlowa, char nazwa[50]);
// funkcja usuwa liste plikow
void usunPliki(struct pliki ** pGlowa);
// funkcja dodaje nowa pozycje do listy przystankow
struct przystanek * dodajPrzystanek(struct przystanek ** pGlowa, char nazwa[50]);
// funkcja szuka przystanku o danej nazwie i zwraca jego adres (jesli nie ma takiego przystanku zwraca NULL)
struct przystanek * znajdzPrzystanek(struct przystanek ** pGlowa, char nazwa[50]);
// funckja usuwa liste przystankow
void usunPrzystanki(struct przystanek ** pGlowa);
// funckja dodaje nowa pozycje do listy asocjacyjnej, laczacej linie z przystankami
void dodajListeLini(struct l_lini ** pGlowa, struct linia * linia);
// funkcja uuswa liste asocjacyjna
void usunListeLini(struct l_lini ** pGlowa);
// funkcja dodaje nowa pozycje do listy linii
struct linia * dodajLinie(struct linia ** pGlowa, int numer);
// funkcja szuka linii o danym numerze i zwraca jego adres (jesli nie ma takiej lini zwraca NULL)
struct linia * znajdzLinie(struct linia ** pGlowa, int numer);
// funckja usuwa liste linii
void usunLinie(struct linia ** pGlowa);
// funkcja dodaje nowa pozycje do listy godzin
void dodajGodzine(struct godzina ** pGlowa, struct przystanek * pPrzystanek, int hour, int min);
// funkcja usuwa liste godzin
void usunGodzine(struct godzina ** pGlowa);
// funkcja drukuje do plikow tablice przystankowe
void zwrocTablice(struct przystanek * pGlowa);
// funkcja odczytuje pliki i zapisuje informacje do struktur
void analizujPlik(struct przystanek ** pPrzystanki, struct linia ** pLinie, struct pliki * pPliki);
// funkcja analizuje przelaczniki podane przy uruchamianiu programu
int przelaczniki(struct pliki ** pPliki, int ile, char ** arg);
// funkcja wyswietla rozklad linii
void wyswietlRozkladLini(struct linia * pGlowa);
// funkcja wyswietla liste linii
void wyswietlLinie(struct linia ** pGlowa);
// funkcja wyswietla liste przystankow
void wyswietlPrzystanki(struct przystanek ** pGlowa);
// funkcja wywietla w konsoli dana tablice przystankowa
void wyswietlRozkladPrzystanku(struct przystanek * pGlowa);
// funkcja szuka polaczenia miedzy danymi przystankami
void znajdzPolaczenie(struct przystanek * start, struct przystanek * stop);
// funkcja wyswietla menu i zwraca wybrana opcje
char menu();
void opcje(char opcja, struct przystanek ** pPrzystanki, struct linia ** pLinie);

#endif
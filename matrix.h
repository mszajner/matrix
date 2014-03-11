
/*********************************************************************************
 * matrix.h                                                                      *
 *                                                                               *
 * Opis:                                                                         *
 *       Plik   nag³ówkowy  biblioteki   matrix.c.   Zawiera opis  i  deklaracjê *
 *    wszystkich funkcji biblioteki oraz zmiennych z niej wyeksportowanych.      *
 *    Biblioteka zawiera funkcje do obs³ugi i dzia³añ na macierzach. Przy pomocy *
 *    tej  biblioteki   mo¿liwe   jest   tworzenie,   niszczenie,   wczytywanie, *
 *    zapisywanie z/do pliku lub konsoli.                                        *
 *    Dodatkowo funkcja  matrix_error_str zwraca nazwê i opis ostatnio wywo³anej *
 *    funkcji, jej kod  powrotu  (b³êdu)  oraz  opis  b³êdu.  Umo¿liwia to pe³n¹ *
 *    kontrolê nad przebiegiem obliczeñ.                                         *
 *                                                                               *
 * Autor:   Miroslaw Szajner                                                     *
 * E-mail:  noozy@tlen.pl                                                        *
 * License: GPL                                                                  *
 * Country: Poland                                                               *
 * Year:    2004                                                                 *
 *                                                                               *
 *********************************************************************************/

#ifndef __MATRIX_H
#define __MATRIX_H

#include <stdio.h>

/*********************************************************************************
 * EPSILON - dok³adnoœæ obliczeñ                                                 *
 *********************************************************************************/
#define EPSILON  (1.0e-20)

/*********************************************************************************
 * Definicja  typów  u¿ytych  do   deklaracji   struktury  MATRIX.  Wykorzystano *
 * definicje preprocesora,  dziêki czemu mo¿liwa jest ³atwa zmiana u¿ytych typów *
 * w ca³ej bibliotece.                                                           *
 *                                                                               *
 * Definicje domyœlne:                                                           *
 * #define MATRIX               struct tagMATRIX                                 *
 * #define MATRIX_ULONG         unsigned long                                    *
 * #define MATRIX_DOUBLE        double                                           *
 *                                                                               * 
 *********************************************************************************/
#define MATRIX               struct tagMATRIX
#define MATRIX_ULONG         unsigned long
#define MATRIX_DOUBLE        double

/*********************************************************************************
 * Definicja u¿ytecznych komend wykorzystanych w funkcjach biblioteki.           *
 *                                                                               *
 * MATRIX_SIZE(m):                                                               *
 *    Zwraca rozmiar w bajtach tablicy m->matrix.                                *
 *                                                                               *
 * MATRIX_ADDR(m,r,c):                                                           *
 *    Zwraca adres komórki (r,c) macierzy w tablicy m->matrix                    *
 *                                                                               *
 * MATRIX_VALUE(m,x,y):                                                          *
 *    Zwraca wartoœæ komórki (r,c) macierzy  i/lub daje mo¿liwoœæ zapisu do niej *
 * wartoœæi.                                                                     *
 *                                                                               *
 *********************************************************************************/
#define MATRIX_SIZE(m)       (m->rows * m->columns * sizeof(MATRIX_DOUBLE))
#define MATRIX_ADDR(m,r,c)   (m->columns * (r) + (c))
#define MATRIX_VALUE(m,r,c)   m->matrix[MATRIX_ADDR(m,r,c)]

/*********************************************************************************
 * Definicja struktury macierzy - struct tagMATRIX.                              *
 * Sk³adniki:                                                                    *
 *  - MATRIX_ULONG   rows - wysokoœæ macierzy.                                   *
 *  - MATRIX_ULONG   columns - szerokoœæ macierzy.                               *
 *  - MATRIX_DOUBLE *matrix - wskaŸnik  na   dynamiczn¹   tablicê  przechowuj¹c¹ *
 *                            komórki macierzy.                                  *
 *                                                                               *
 *********************************************************************************/
struct tagMATRIX {
	MATRIX_ULONG   rows;
	MATRIX_ULONG   columns;
	MATRIX_DOUBLE *matrix;
};

/*********************************************************************************
 * int matrix_error                                                              *
 *                                                                               *
 * Opis:                                                                         *
 *       Zmienna przechowuje numer ostatniego b³êdu.  Jeœli wyst¹pi³ jakikolwiek *
 *   b³¹d w jakiejkolwiek  funkcji  tej  biblioteki,  to  ten b³¹d zapisany jest *
 *   w tej zmiennej. Kody b³êdów poni¿ej.                                        *
 *                                                                               *
 * Kody b³êdów:                                                                  *
 *    0 - funkcja zakoñczy³a siê powodzeniem (OK)                                *
 *   -1 - brak pamiêci do zaalokowania macierzy (OUT-OF-MEMORY)                  *
 *   -2 - nieoczekiwany koniec pliku                                             *
 *   -3 - b³¹d odczytu pliku                                                     *
 *   -4 - b³¹d zapisu do pliku                                                   *
 *   -5 - wymagana macierz kwadratowa                                            *
 *   -6 - wymiary macierzy nie zgadzaj¹ siê                                      *
 *   -7 - próba dzielenia przez zero                                             *
 *   -9 - nieznany b³¹d                                                          *
 *  -10 - b³¹d podczas wyœwietlania macierzy                                     *
 *                                                                               *
 *********************************************************************************/
extern int matrix_error;

/*********************************************************************************
 * matrix_error_str                                                              *
 *                                                                               *
 * Argumenty:                                                                    *
 *    Brak                                                                       *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja zwraca tekstowy opis ostatniego kodu b³êdu.                     *
 *                                                                               *
 * Zwracana wartoœæ (const char*):                                               *
 *       Funkcja zwraca wskaŸnik na tekst zawieraj¹cy opis kodu b³êdu.           *
 *                                                                               *
 *********************************************************************************/
const char* matrix_error_str(void);

/*********************************************************************************
 * matrix_create                                                                 *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wskaŸnik na strukturê struct tagMATRIX.                           *
 *  - rows - iloœæ wierszy macierzy                                              *
 *  - columns - iloœæ kolumn macierzy                                            *
 *  - value - wartoœæ któr¹ macierz ma zostaæ wype³niona                         *
 *                                                                               *
 * Opis:                                                                         *
 *       Tworzy now¹ macierz. Przydziela pamiêæ dla danych funkcj¹ "malloc" oraz *
 *    kopiuje rozmiar  macierzy  potrzebny  do  dalszych  obliczeñ.  Funkcja nie *
 *    sprawdza  czy  wczeœniej  macierz  zosta³a  ju¿  utworzona,  jeœli by³a to *
 *    wskaŸnik na dane macierzy oraz rozmiar poprzedniej zostanie utracony.      *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Funkcja zwróci 0 (zero)  jeœli macierz zosta³a utworzona.  Jeœli pamiêæ *
 *    nie zosta³a przydzielona na dane, to funkcja zwróci -1.                    *
 *                                                                               *
 *********************************************************************************/
int matrix_create(MATRIX *matrix, MATRIX_ULONG rows, 
				  MATRIX_ULONG columns, MATRIX_DOUBLE value);

/*********************************************************************************
 * matrix_resize                                                                 *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wskaŸnik na strukturê struct tagMATRIX                            *
 *  - rows - iloœæ wierszy macierzy                                              *
 *  - columns - iloœc kolumn macierzy                                            *
 *  - value - domyœlna wartoœc wstawiana do nowych komórek                       *
 *                                                                               *
 * Opis:                                                                         *
 *       Zwiêksza lub zmniejsza  rozmiar  macierzy.  Jeœli  zwiêksza, to do nowo *
 *    dodanych komórek wpisuje podan¹ wartoœæ "value". Funkcja musi byæ wywo³ana *
 *    tylko i wy³¹cznie na  macierzy,  która  zosta³a  wczeœniej  utworzona przy *
 *    pomocy funkcji "matrix_create".                                            *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Podobnie jak  funkcja  "matrix_create"  zwraca  wartoœæ 0 (zero)  jeœli *
 *    wszystko posz³o dobrze. W przeciwnym przypadku -zwraca 1.                  *
 *                                                                               *
 *********************************************************************************/
int matrix_resize(MATRIX *matrix, MATRIX_ULONG rows, 
				  MATRIX_ULONG columns, MATRIX_DOUBLE value);

/*********************************************************************************
 * matrix_destroy                                                                *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wskaŸnik na strukturê struct tagMATRIX                            *
 *                                                                               *
 * Opis:                                                                         *
 *       Niszczenie macierzy.  Funkcja  zwalnia  przydzielon¹ pamieæ dla tablicy *
 *    matrix->matrix oraz zeruje strukturê "matrix".                             *
 *                                                                               *
 * Zwracana wartoœæ (void):                                                      *
 *       Brak.                                                                   *
 *                                                                               *
 *********************************************************************************/
void matrix_destroy(MATRIX *matrix);

/*********************************************************************************
 * matrix_read                                                                   *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wskaŸnik na strukturê macierzy                                    *
 *  - file - wskaŸnik na otwarty plik (do odczytu), mo¿e byæ konsola             *
 *  - binary - flaga formatu odczytu (opis poni¿ej)                              *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja odczytuje z pliku  (w szczególnym przypadku z konsoli) macierz. *
 *    Flaga "binary" informuje czy odczyt jest binarny czy tekstowy.  Jeœli jest *
 *    ustawiona na "0", to tryb odczytu jest tekstowy, jeœli "1" to tryb odczytu *
 *    jest binarny. Nie mo¿na u¿yæ trybu binarnego dla odczytu z konsoli!        *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Funkcja zwraca iloœæ odczytanych komórek + 2 (tryb tekstowy) lub bajtów *
 *    (tryb binarny). Jeœli wyst¹pi³ b³¹d odczytu zwraca wartoœæ -1.             *
 *                                                                               *
 *********************************************************************************/
int matrix_read(MATRIX *matrix, FILE *file, int binary);

/*********************************************************************************
 * matrix_write                                                                  *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wskaŸnik na strukturê macierzy                                    *
 *  - file - wskaŸnik na otwarty plik (do zapisu), mo¿e byæ konsola              *
 *  - binary - flaga formatu zapisu (opis poni¿ej)                               *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja zapisuje do pliku (w szczególnym przypadku do konsoli) macierz. *
 *    Flaga "binary" informuje czy zapis  jest binarny czy tekstowy.  Jeœli jest *
 *    ustawiona na "0",  to tryb zapisu jest tekstowy,  jeœli "1" to tryb zapisu *
 *    jest binarny. Nie mo¿na u¿yæ trybu binarnego dla zapisu do konsoli!        *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Funkcja zwraca iloœæ zapisanych komórek + 2 (tryb tekstowy) lub  bajtów *
 *    (tryb binarny). Jeœli wyst¹pi³ b³¹d zapisuu zwraca wartoœæ -1.             *
 *                                                                               *
 *********************************************************************************/
int matrix_write(MATRIX *matrix, FILE *file, int binary);

/*********************************************************************************
 * matrix_print                                                                  *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wskaŸnik na strukturê macierzy                                    *
 *  - precision - dok³adnoœæ z jak¹ maj¹ zostaæ wyœwietlone komórki              *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja wypisuje  macierzy  na  konsoli  przy  u¿yciu funkcji "printf". *
 *    Mo¿liwa jest zmiana precyzji wyœwietlania komórek. Format wypisywania jest *
 *    taki jak format "%g" z niewielkimi modyfikacjami.                          *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Funkcja zwraca  iloœæ  wypisanych  komórek.  Jeœli  wyst¹pi³ jakiœ b³¹d *
 *    funkcja zwróci -1.                                                         *
 *                                                                               *
 *********************************************************************************/
int matrix_print(MATRIX *matrix, MATRIX_ULONG precision);

/*********************************************************************************
 * matrix_info                                                                   *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wskaŸnik na strukturyê macierzy                                   *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja wyœwietla informacje o  macierzy,  wymiary  macierzy,  zajêtoœæ *
 *    pamiêci...                                                                 *
 *                                                                               *
 *********************************************************************************/
void matrix_info(MATRIX *matrix);

/*********************************************************************************
 * matrix_fill                                                                   *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wskaŸnik na strukturê macierzy                                    *
 *  - value - wartoœæ, któr¹ zostanie wype³niona macierz                         *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja wype³nia macierz zadan¹ wartoœci¹.                              *
 *                                                                               *
 * Zwracana wartoœæ (void):                                                      *
 *       Brak                                                                    *
 *                                                                               *
 *********************************************************************************/
void matrix_fill(MATRIX *matrix, MATRIX_DOUBLE value);

/*********************************************************************************
 * matrix_copy                                                                   *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - dest - wskaŸnik na strukturê macierzy docelowej                            *
 *  - src - wskaŸnik na strukturê macierzy Ÿród³owej                             *
 *  - dest_row - numer wiersza docelowej                                         *
 *  - dest_col - numer kolumny docelowego                                        *
 *  - src_row1 - numer wiersza górnej lewej komórki Ÿród³owej                    *
 *  - src_col1 - numer kolumny górnej lewej komórki Ÿród³owej                    *
 *  - src_row2 - numer wiersza dolnej prawej komórki Ÿród³owej                   *
 *  - src_col2 - numer kolumny dolnej prawej komórki Ÿród³owej                   *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja kopiuje czêœæ  (lub ca³oœæ)  macierzy  "src" do macierzy "dest" *
 *    pod wskazan¹ pozycjê.  Jeœli  macierz docelowa nie jest w stanie pomieœciæ *
 *    komórek  macierzy  Ÿród³owej  zostanie  ona  zwiêkszona.  Natomiast  jeœli *
 *    macierz  "dest"  jest  w  stanie   pomieœciæ   komórki  nie  zostanie  ona *
 *    zmniejszona. Funkcja nadpisuje komórki macierzy "dest".                    *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Funkcja zwraca iloœæ skopiowanych komórek. Jeœli wyst¹pi³ b³¹d podczas  *
 *    kopiowania, to funkcj zwraca -1.                                           *
 *                                                                               *
 *********************************************************************************/
int matrix_copy(MATRIX *dest, MATRIX *src, MATRIX_ULONG dest_row, 
				MATRIX_ULONG dest_col, MATRIX_ULONG src_row1, 
				MATRIX_ULONG src_col1, MATRIX_ULONG src_row2,
				MATRIX_ULONG src_col2);

/*********************************************************************************
 * matrix_unit                                                                   *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - A - wskaŸnik na strukturê macierzy                                         *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja wype³nia macierz A tak, aby sta³a siê macierz¹ jednostkow¹.     *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Jeœli funkcja powiod³a siê zwraca wartoœæ 0, inaczej - 1.               *
 *                                                                               *
 *********************************************************************************/
int matrix_unit(MATRIX *A);

/*********************************************************************************
 * matrix_decomp                                                                 *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - A - wskaŸnik na strukturê rozk³adanej macierzy                             *
 *  - D - macierz dolna wyjœciowa (patrz ni¿ej)                                  *
 *  - G - macierz górna wyjœciowa (patrz ni¿ej)                                  *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja rozk³ada macierz A na  dwie macierze trójk¹tne.  Macierze D i G *
 *    powinny  byæ  puste  (zwolnione)  przed  wywo³aniem  funkcji, gdy¿ funkcja *
 *    odpowiednio jest tworzy i inicjalizuje.                                    *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Jeœli wyst¹pi³ b³¹d podczas rozk³adu funkcja zwraca -1. Jeœli natomiast *
 *    z podanej macierzy nie da siê  utworzyæ  macierzy D i G, to funkcja zwraca *
 *    0 (zero). Jeœli funkcji uda³o siê roz³o¿yæ macierz, to zwraca wartoœæ 1.   *
 *                                                                               *
 *********************************************************************************/
int matrix_decomp(MATRIX *A, MATRIX *D, MATRIX *G);

/*********************************************************************************
 * matrix_det_crout                                                              *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - A - wskaŸnik na macierz, której wyznacznik chcemy policzyæ.                *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza wyznacznik macierzy "A" metod¹ Crouta.                  *
 *                                                                               *
 * Zwracana wartoœæ (MATRIX_DOUBLE):                                             *
 *       Funkcja zwraca wartoœæ wyliczonego wskaŸnika.                           *
 *                                                                               *
 *********************************************************************************/
MATRIX_DOUBLE matrix_det_crout(MATRIX *A);

/*********************************************************************************
 * Domyœlna funkcja wyliczaj¹ca wyznacznik.                                      *
 *                                                                               *
 * #define matrix_det    matrix_det_crout                                        *
 *                                                                               *
 *********************************************************************************/
#define matrix_det  matrix_det_crout

/*********************************************************************************
 * matrix_transp                                                                 *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - A - wskaŸnik na macierz, któr¹ chcemy transponowaæ                         *
 *  - Y - wskaŸnik na macierz wyjœciow¹                                          *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja tworzy macierz transponowan¹ do "A" i zapisuje j¹ w "Y".        *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Funkcja zwraca wartoœæ 0, jeœli uda³o siê, lub -1 jeœli nie.            *
 *                                                                               *
 *********************************************************************************/
int matrix_transp(MATRIX *Y, MATRIX *A);

/*********************************************************************************
 * matrix_minor                                                                  *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - wyjœciowa macierz minorowa, musi byæ czysta przed wywo³., czytaj ni¿ej *
 *  - A - wskaŸnik na macierz, w której chcemy policzyæ minor.                   *
 *  - r - wyciêty wiersz                                                         *
 *  - c - wyciêta kolumna                                                        *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja wycina z macierzy A podan¹ kolumnê (c) oraz wiersz (r), tworz¹c *
 *    minor (r,c)  macierzy A.  Wynik  (nowa macierz)  zapisywana  jest pod (Y). *
 *    Macierz Y jest tworzona  w  funkcji,  zatem  przed  wywo³aniem musi zostaæ *
 *    zwolniona jeœli by³a wczeœniej u¿ywana.                                    *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Funkcja zwraca -1 w przypadku b³êdu, lub 0 jeœli siê powiod³o.          *
 *                                                                               *
 *********************************************************************************/
int matrix_minor(MATRIX *Y, MATRIX *A, MATRIX_ULONG r, MATRIX_ULONG c);

/*********************************************************************************
 * matrix_suppl                                                                  *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - wskaŸnik na macierz wyjœciow¹ - macierz dope³nieñ                      *
 *  - A - wskaŸnik na macierz z której tworzymy macierz dope³nieñ                *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja tworzy macierz dope³nieñ z macierzy "A" i zapisuje j¹ w "Y".    *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Funkcja zwraca wartoœæ 0, jeœli uda³o siê, lub -1 jeœli nie.            *
 *                                                                               *
 *********************************************************************************/
int matrix_suppl(MATRIX *Y, MATRIX *A);

/*********************************************************************************
 * matrix_attach                                                                 *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - wskaŸnik na macierz wyjœciow¹ - macierz do³¹czon¹                      *
 *  - A - wskaŸnik na macierz z której tworzymy macierz do³aczon¹                *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja tworzy macierz dope³nieñ z macierzy "A" i zapisuje j¹ w "Y".    *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Funkcja zwraca wartoœæ 0, jeœli uda³o siê, lub -1 jeœli nie.            *
 *                                                                               *
 *********************************************************************************/
int matrix_attach(MATRIX *Y, MATRIX *A);

/*********************************************************************************
 * matrix_inv                                                                    *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - wskaŸnik na macierz w której zapiszemy odwrócon¹ macierz               *
 *  - A - wskaŸnik na macierz, któr¹ chcemy odwróciæ.                            *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja tworzy macierz odwrotn¹ do "A"  i zapisuje j¹ do "Y".           *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Funkcja zwraca wartoœæ 0, jeœli uda³o siê odwróciæ, lub -1 jeœli nie.   *
 *                                                                               *
 *********************************************************************************/
int matrix_inv(MATRIX *Y, MATRIX *A);

/*********************************************************************************
 * matrix_add                                                                    *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyjœciowa Y = A + B                                            *
 *  - A - macierz A                                                              *
 *  - B - macierz B                                                              *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza  sumê  dwóch  macierzy.  Macierze  A i B musz¹ byæ tych *
 *    samych wymiarów.  Macierz Y  jest  tworzona  w  funkcji, zatem musi zostaæ *
 *    zwolniona przed wywo³aniem, jeœli by³a wczeœniej u¿ywana.                  *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Jeœli uda³o siê dokonaæ operacji, funkcja zwraca 0. Jeœli nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_add(MATRIX *Y, MATRIX *A, MATRIX *B);

/*********************************************************************************
 * matrix_sub                                                                    *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyjœciowa Y = A - B                                            *
 *  - A - macierz A                                                              *
 *  - B - macierz B                                                              *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza ró¿nicê dwóch  macierzy. Macierze  A i B musz¹ byæ tych *
 *    samych wymiarów.  Macierz Y  jest  tworzona  w  funkcji, zatem musi zostaæ *
 *    zwolniona przed wywo³aniem, jeœli by³a wczeœniej u¿ywana.                  *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Jeœli uda³o siê dokonaæ operacji, funkcja zwraca 0. Jeœli nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_sub(MATRIX *Y, MATRIX *A, MATRIX *B);

/*********************************************************************************
 * matrix_mul                                                                    *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyjœciowa Y = A * B                                            *
 *  - A - macierz A (n x r)                                                      *
 *  - B - macierz B (r x m)                                                      *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja  oblicza  iloczyn dwóch  macierzy.  Macierze  A i B  musz¹  byæ *
 *    wymiarów (n x r) oraz  (r x m).  Macierz Y jest  tworzona w funkcji, zatem *
 *    musi zostaæ zwolniona przed wywo³aniem, jeœli by³a wczeœniej u¿ywana.      *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Jeœli uda³o siê dokonaæ operacji, funkcja zwraca 0. Jeœli nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_mul(MATRIX *Y, MATRIX *A, MATRIX *B);

/*********************************************************************************
 * matrix_div                                                                    *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyjœciowa Y = A * B                                            *
 *  - A - macierz A (n x r)                                                      *
 *  - B - macierz B (r x m)                                                      *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja  oblicza  iloraz  dwóch  macierzy.  Macierze  A i B  musz¹  byæ *
 *    wymiarów (n x r) oraz  (r x m).  Macierz Y jest  tworzona w funkcji, zatem *
 *    musi zostaæ zwolniona przed wywo³aniem, jeœli by³a wczeœniej u¿ywana.      *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Jeœli uda³o siê dokonaæ operacji, funkcja zwraca 0. Jeœli nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_div(MATRIX *Y, MATRIX *A, MATRIX *B);

/*********************************************************************************
 * matrix_add_int                                                                *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyjœciowa Y = A + b                                            *
 *  - A - macierz A                                                              *
 *  - b - liczba b                                                               *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja  oblicza  sumê  ka¿dego  elementu  macierzy i  liczby  b. Wynik *
 *    zapisuje w  macierzy  Y.  Macierz  Y  jest  tworzona w funkcji, zatem musi *
 *    zostaæ zwolniona przed wywo³aniem, jeœli by³a wczeœniej u¿ywana.           *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Jeœli uda³o siê dokonaæ operacji, funkcja zwraca 0. Jeœli nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_add_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b);

/*********************************************************************************
 * matrix_sub_int                                                                *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyjœciowa Y = A - b                                            *
 *  - A - macierz A                                                              *
 *  - b - liczba b                                                               *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza ró¿nicego ka¿dego elementu  macierzy i liczby  b. Wynik *
 *    zapisuje w  macierzy  Y.  Macierz  Y  jest  tworzona w funkcji, zatem musi *
 *    zostaæ zwolniona przed wywo³aniem, jeœli by³a wczeœniej u¿ywana.           *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Jeœli uda³o siê dokonaæ operacji, funkcja zwraca 0. Jeœli nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_sub_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b);

/*********************************************************************************
 * matrix_sub_int                                                                *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyjœciowa Y = A * b                                            *
 *  - A - macierz A                                                              *
 *  - b - liczba b                                                               *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza iloczyn  ka¿dego  elementu  macierzy i liczby  b. Wynik *
 *    zapisuje w  macierzy  Y.  Macierz  Y  jest  tworzona w funkcji, zatem musi *
 *    zostaæ zwolniona przed wywo³aniem, jeœli by³a wczeœniej u¿ywana.           *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Jeœli uda³o siê dokonaæ operacji, funkcja zwraca 0. Jeœli nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_mul_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b);

/*********************************************************************************
 * matrix_div_int                                                                *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyjœciowa Y = A / b                                            *
 *  - A - macierz A                                                              *
 *  - b - liczba b                                                               *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza  iloraz  ka¿dego  elementu  macierzy i liczby  b. Wynik *
 *    zapisuje w  macierzy  Y.  Macierz  Y  jest  tworzona w funkcji, zatem musi *
 *    zostaæ zwolniona przed wywo³aniem, jeœli by³a wczeœniej u¿ywana.           *
 *                                                                               *
 * Zwracana wartoœæ (int):                                                       *
 *       Jeœli uda³o siê dokonaæ operacji, funkcja zwraca 0. Jeœli nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_div_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b);

#endif

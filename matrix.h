
/*********************************************************************************
 * matrix.h                                                                      *
 *                                                                               *
 * Opis:                                                                         *
 *       Plik   nag��wkowy  biblioteki   matrix.c.   Zawiera opis  i  deklaracj� *
 *    wszystkich funkcji biblioteki oraz zmiennych z niej wyeksportowanych.      *
 *    Biblioteka zawiera funkcje do obs�ugi i dzia�a� na macierzach. Przy pomocy *
 *    tej  biblioteki   mo�liwe   jest   tworzenie,   niszczenie,   wczytywanie, *
 *    zapisywanie z/do pliku lub konsoli.                                        *
 *    Dodatkowo funkcja  matrix_error_str zwraca nazw� i opis ostatnio wywo�anej *
 *    funkcji, jej kod  powrotu  (b��du)  oraz  opis  b��du.  Umo�liwia to pe�n� *
 *    kontrol� nad przebiegiem oblicze�.                                         *
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
 * EPSILON - dok�adno�� oblicze�                                                 *
 *********************************************************************************/
#define EPSILON  (1.0e-20)

/*********************************************************************************
 * Definicja  typ�w  u�ytych  do   deklaracji   struktury  MATRIX.  Wykorzystano *
 * definicje preprocesora,  dzi�ki czemu mo�liwa jest �atwa zmiana u�ytych typ�w *
 * w ca�ej bibliotece.                                                           *
 *                                                                               *
 * Definicje domy�lne:                                                           *
 * #define MATRIX               struct tagMATRIX                                 *
 * #define MATRIX_ULONG         unsigned long                                    *
 * #define MATRIX_DOUBLE        double                                           *
 *                                                                               * 
 *********************************************************************************/
#define MATRIX               struct tagMATRIX
#define MATRIX_ULONG         unsigned long
#define MATRIX_DOUBLE        double

/*********************************************************************************
 * Definicja u�ytecznych komend wykorzystanych w funkcjach biblioteki.           *
 *                                                                               *
 * MATRIX_SIZE(m):                                                               *
 *    Zwraca rozmiar w bajtach tablicy m->matrix.                                *
 *                                                                               *
 * MATRIX_ADDR(m,r,c):                                                           *
 *    Zwraca adres kom�rki (r,c) macierzy w tablicy m->matrix                    *
 *                                                                               *
 * MATRIX_VALUE(m,x,y):                                                          *
 *    Zwraca warto�� kom�rki (r,c) macierzy  i/lub daje mo�liwo�� zapisu do niej *
 * warto��i.                                                                     *
 *                                                                               *
 *********************************************************************************/
#define MATRIX_SIZE(m)       (m->rows * m->columns * sizeof(MATRIX_DOUBLE))
#define MATRIX_ADDR(m,r,c)   (m->columns * (r) + (c))
#define MATRIX_VALUE(m,r,c)   m->matrix[MATRIX_ADDR(m,r,c)]

/*********************************************************************************
 * Definicja struktury macierzy - struct tagMATRIX.                              *
 * Sk�adniki:                                                                    *
 *  - MATRIX_ULONG   rows - wysoko�� macierzy.                                   *
 *  - MATRIX_ULONG   columns - szeroko�� macierzy.                               *
 *  - MATRIX_DOUBLE *matrix - wska�nik  na   dynamiczn�   tablic�  przechowuj�c� *
 *                            kom�rki macierzy.                                  *
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
 *       Zmienna przechowuje numer ostatniego b��du.  Je�li wyst�pi� jakikolwiek *
 *   b��d w jakiejkolwiek  funkcji  tej  biblioteki,  to  ten b��d zapisany jest *
 *   w tej zmiennej. Kody b��d�w poni�ej.                                        *
 *                                                                               *
 * Kody b��d�w:                                                                  *
 *    0 - funkcja zako�czy�a si� powodzeniem (OK)                                *
 *   -1 - brak pami�ci do zaalokowania macierzy (OUT-OF-MEMORY)                  *
 *   -2 - nieoczekiwany koniec pliku                                             *
 *   -3 - b��d odczytu pliku                                                     *
 *   -4 - b��d zapisu do pliku                                                   *
 *   -5 - wymagana macierz kwadratowa                                            *
 *   -6 - wymiary macierzy nie zgadzaj� si�                                      *
 *   -7 - pr�ba dzielenia przez zero                                             *
 *   -9 - nieznany b��d                                                          *
 *  -10 - b��d podczas wy�wietlania macierzy                                     *
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
 *       Funkcja zwraca tekstowy opis ostatniego kodu b��du.                     *
 *                                                                               *
 * Zwracana warto�� (const char*):                                               *
 *       Funkcja zwraca wska�nik na tekst zawieraj�cy opis kodu b��du.           *
 *                                                                               *
 *********************************************************************************/
const char* matrix_error_str(void);

/*********************************************************************************
 * matrix_create                                                                 *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wska�nik na struktur� struct tagMATRIX.                           *
 *  - rows - ilo�� wierszy macierzy                                              *
 *  - columns - ilo�� kolumn macierzy                                            *
 *  - value - warto�� kt�r� macierz ma zosta� wype�niona                         *
 *                                                                               *
 * Opis:                                                                         *
 *       Tworzy now� macierz. Przydziela pami�� dla danych funkcj� "malloc" oraz *
 *    kopiuje rozmiar  macierzy  potrzebny  do  dalszych  oblicze�.  Funkcja nie *
 *    sprawdza  czy  wcze�niej  macierz  zosta�a  ju�  utworzona,  je�li by�a to *
 *    wska�nik na dane macierzy oraz rozmiar poprzedniej zostanie utracony.      *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Funkcja zwr�ci 0 (zero)  je�li macierz zosta�a utworzona.  Je�li pami�� *
 *    nie zosta�a przydzielona na dane, to funkcja zwr�ci -1.                    *
 *                                                                               *
 *********************************************************************************/
int matrix_create(MATRIX *matrix, MATRIX_ULONG rows, 
				  MATRIX_ULONG columns, MATRIX_DOUBLE value);

/*********************************************************************************
 * matrix_resize                                                                 *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wska�nik na struktur� struct tagMATRIX                            *
 *  - rows - ilo�� wierszy macierzy                                              *
 *  - columns - ilo�c kolumn macierzy                                            *
 *  - value - domy�lna warto�c wstawiana do nowych kom�rek                       *
 *                                                                               *
 * Opis:                                                                         *
 *       Zwi�ksza lub zmniejsza  rozmiar  macierzy.  Je�li  zwi�ksza, to do nowo *
 *    dodanych kom�rek wpisuje podan� warto�� "value". Funkcja musi by� wywo�ana *
 *    tylko i wy��cznie na  macierzy,  kt�ra  zosta�a  wcze�niej  utworzona przy *
 *    pomocy funkcji "matrix_create".                                            *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Podobnie jak  funkcja  "matrix_create"  zwraca  warto�� 0 (zero)  je�li *
 *    wszystko posz�o dobrze. W przeciwnym przypadku -zwraca 1.                  *
 *                                                                               *
 *********************************************************************************/
int matrix_resize(MATRIX *matrix, MATRIX_ULONG rows, 
				  MATRIX_ULONG columns, MATRIX_DOUBLE value);

/*********************************************************************************
 * matrix_destroy                                                                *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wska�nik na struktur� struct tagMATRIX                            *
 *                                                                               *
 * Opis:                                                                         *
 *       Niszczenie macierzy.  Funkcja  zwalnia  przydzielon� pamie� dla tablicy *
 *    matrix->matrix oraz zeruje struktur� "matrix".                             *
 *                                                                               *
 * Zwracana warto�� (void):                                                      *
 *       Brak.                                                                   *
 *                                                                               *
 *********************************************************************************/
void matrix_destroy(MATRIX *matrix);

/*********************************************************************************
 * matrix_read                                                                   *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wska�nik na struktur� macierzy                                    *
 *  - file - wska�nik na otwarty plik (do odczytu), mo�e by� konsola             *
 *  - binary - flaga formatu odczytu (opis poni�ej)                              *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja odczytuje z pliku  (w szczeg�lnym przypadku z konsoli) macierz. *
 *    Flaga "binary" informuje czy odczyt jest binarny czy tekstowy.  Je�li jest *
 *    ustawiona na "0", to tryb odczytu jest tekstowy, je�li "1" to tryb odczytu *
 *    jest binarny. Nie mo�na u�y� trybu binarnego dla odczytu z konsoli!        *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Funkcja zwraca ilo�� odczytanych kom�rek + 2 (tryb tekstowy) lub bajt�w *
 *    (tryb binarny). Je�li wyst�pi� b��d odczytu zwraca warto�� -1.             *
 *                                                                               *
 *********************************************************************************/
int matrix_read(MATRIX *matrix, FILE *file, int binary);

/*********************************************************************************
 * matrix_write                                                                  *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wska�nik na struktur� macierzy                                    *
 *  - file - wska�nik na otwarty plik (do zapisu), mo�e by� konsola              *
 *  - binary - flaga formatu zapisu (opis poni�ej)                               *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja zapisuje do pliku (w szczeg�lnym przypadku do konsoli) macierz. *
 *    Flaga "binary" informuje czy zapis  jest binarny czy tekstowy.  Je�li jest *
 *    ustawiona na "0",  to tryb zapisu jest tekstowy,  je�li "1" to tryb zapisu *
 *    jest binarny. Nie mo�na u�y� trybu binarnego dla zapisu do konsoli!        *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Funkcja zwraca ilo�� zapisanych kom�rek + 2 (tryb tekstowy) lub  bajt�w *
 *    (tryb binarny). Je�li wyst�pi� b��d zapisuu zwraca warto�� -1.             *
 *                                                                               *
 *********************************************************************************/
int matrix_write(MATRIX *matrix, FILE *file, int binary);

/*********************************************************************************
 * matrix_print                                                                  *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wska�nik na struktur� macierzy                                    *
 *  - precision - dok�adno�� z jak� maj� zosta� wy�wietlone kom�rki              *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja wypisuje  macierzy  na  konsoli  przy  u�yciu funkcji "printf". *
 *    Mo�liwa jest zmiana precyzji wy�wietlania kom�rek. Format wypisywania jest *
 *    taki jak format "%g" z niewielkimi modyfikacjami.                          *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Funkcja zwraca  ilo��  wypisanych  kom�rek.  Je�li  wyst�pi� jaki� b��d *
 *    funkcja zwr�ci -1.                                                         *
 *                                                                               *
 *********************************************************************************/
int matrix_print(MATRIX *matrix, MATRIX_ULONG precision);

/*********************************************************************************
 * matrix_info                                                                   *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wska�nik na struktury� macierzy                                   *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja wy�wietla informacje o  macierzy,  wymiary  macierzy,  zaj�to�� *
 *    pami�ci...                                                                 *
 *                                                                               *
 *********************************************************************************/
void matrix_info(MATRIX *matrix);

/*********************************************************************************
 * matrix_fill                                                                   *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - matrix - wska�nik na struktur� macierzy                                    *
 *  - value - warto��, kt�r� zostanie wype�niona macierz                         *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja wype�nia macierz zadan� warto�ci�.                              *
 *                                                                               *
 * Zwracana warto�� (void):                                                      *
 *       Brak                                                                    *
 *                                                                               *
 *********************************************************************************/
void matrix_fill(MATRIX *matrix, MATRIX_DOUBLE value);

/*********************************************************************************
 * matrix_copy                                                                   *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - dest - wska�nik na struktur� macierzy docelowej                            *
 *  - src - wska�nik na struktur� macierzy �r�d�owej                             *
 *  - dest_row - numer wiersza docelowej                                         *
 *  - dest_col - numer kolumny docelowego                                        *
 *  - src_row1 - numer wiersza g�rnej lewej kom�rki �r�d�owej                    *
 *  - src_col1 - numer kolumny g�rnej lewej kom�rki �r�d�owej                    *
 *  - src_row2 - numer wiersza dolnej prawej kom�rki �r�d�owej                   *
 *  - src_col2 - numer kolumny dolnej prawej kom�rki �r�d�owej                   *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja kopiuje cz��  (lub ca�o��)  macierzy  "src" do macierzy "dest" *
 *    pod wskazan� pozycj�.  Je�li  macierz docelowa nie jest w stanie pomie�ci� *
 *    kom�rek  macierzy  �r�d�owej  zostanie  ona  zwi�kszona.  Natomiast  je�li *
 *    macierz  "dest"  jest  w  stanie   pomie�ci�   kom�rki  nie  zostanie  ona *
 *    zmniejszona. Funkcja nadpisuje kom�rki macierzy "dest".                    *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Funkcja zwraca ilo�� skopiowanych kom�rek. Je�li wyst�pi� b��d podczas  *
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
 *  - A - wska�nik na struktur� macierzy                                         *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja wype�nia macierz A tak, aby sta�a si� macierz� jednostkow�.     *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Je�li funkcja powiod�a si� zwraca warto�� 0, inaczej - 1.               *
 *                                                                               *
 *********************************************************************************/
int matrix_unit(MATRIX *A);

/*********************************************************************************
 * matrix_decomp                                                                 *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - A - wska�nik na struktur� rozk�adanej macierzy                             *
 *  - D - macierz dolna wyj�ciowa (patrz ni�ej)                                  *
 *  - G - macierz g�rna wyj�ciowa (patrz ni�ej)                                  *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja rozk�ada macierz A na  dwie macierze tr�jk�tne.  Macierze D i G *
 *    powinny  by�  puste  (zwolnione)  przed  wywo�aniem  funkcji, gdy� funkcja *
 *    odpowiednio jest tworzy i inicjalizuje.                                    *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Je�li wyst�pi� b��d podczas rozk�adu funkcja zwraca -1. Je�li natomiast *
 *    z podanej macierzy nie da si�  utworzy�  macierzy D i G, to funkcja zwraca *
 *    0 (zero). Je�li funkcji uda�o si� roz�o�y� macierz, to zwraca warto�� 1.   *
 *                                                                               *
 *********************************************************************************/
int matrix_decomp(MATRIX *A, MATRIX *D, MATRIX *G);

/*********************************************************************************
 * matrix_det_crout                                                              *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - A - wska�nik na macierz, kt�rej wyznacznik chcemy policzy�.                *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza wyznacznik macierzy "A" metod� Crouta.                  *
 *                                                                               *
 * Zwracana warto�� (MATRIX_DOUBLE):                                             *
 *       Funkcja zwraca warto�� wyliczonego wska�nika.                           *
 *                                                                               *
 *********************************************************************************/
MATRIX_DOUBLE matrix_det_crout(MATRIX *A);

/*********************************************************************************
 * Domy�lna funkcja wyliczaj�ca wyznacznik.                                      *
 *                                                                               *
 * #define matrix_det    matrix_det_crout                                        *
 *                                                                               *
 *********************************************************************************/
#define matrix_det  matrix_det_crout

/*********************************************************************************
 * matrix_transp                                                                 *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - A - wska�nik na macierz, kt�r� chcemy transponowa�                         *
 *  - Y - wska�nik na macierz wyj�ciow�                                          *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja tworzy macierz transponowan� do "A" i zapisuje j� w "Y".        *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Funkcja zwraca warto�� 0, je�li uda�o si�, lub -1 je�li nie.            *
 *                                                                               *
 *********************************************************************************/
int matrix_transp(MATRIX *Y, MATRIX *A);

/*********************************************************************************
 * matrix_minor                                                                  *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - wyj�ciowa macierz minorowa, musi by� czysta przed wywo�., czytaj ni�ej *
 *  - A - wska�nik na macierz, w kt�rej chcemy policzy� minor.                   *
 *  - r - wyci�ty wiersz                                                         *
 *  - c - wyci�ta kolumna                                                        *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja wycina z macierzy A podan� kolumn� (c) oraz wiersz (r), tworz�c *
 *    minor (r,c)  macierzy A.  Wynik  (nowa macierz)  zapisywana  jest pod (Y). *
 *    Macierz Y jest tworzona  w  funkcji,  zatem  przed  wywo�aniem musi zosta� *
 *    zwolniona je�li by�a wcze�niej u�ywana.                                    *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Funkcja zwraca -1 w przypadku b��du, lub 0 je�li si� powiod�o.          *
 *                                                                               *
 *********************************************************************************/
int matrix_minor(MATRIX *Y, MATRIX *A, MATRIX_ULONG r, MATRIX_ULONG c);

/*********************************************************************************
 * matrix_suppl                                                                  *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - wska�nik na macierz wyj�ciow� - macierz dope�nie�                      *
 *  - A - wska�nik na macierz z kt�rej tworzymy macierz dope�nie�                *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja tworzy macierz dope�nie� z macierzy "A" i zapisuje j� w "Y".    *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Funkcja zwraca warto�� 0, je�li uda�o si�, lub -1 je�li nie.            *
 *                                                                               *
 *********************************************************************************/
int matrix_suppl(MATRIX *Y, MATRIX *A);

/*********************************************************************************
 * matrix_attach                                                                 *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - wska�nik na macierz wyj�ciow� - macierz do��czon�                      *
 *  - A - wska�nik na macierz z kt�rej tworzymy macierz do�aczon�                *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja tworzy macierz dope�nie� z macierzy "A" i zapisuje j� w "Y".    *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Funkcja zwraca warto�� 0, je�li uda�o si�, lub -1 je�li nie.            *
 *                                                                               *
 *********************************************************************************/
int matrix_attach(MATRIX *Y, MATRIX *A);

/*********************************************************************************
 * matrix_inv                                                                    *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - wska�nik na macierz w kt�rej zapiszemy odwr�con� macierz               *
 *  - A - wska�nik na macierz, kt�r� chcemy odwr�ci�.                            *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja tworzy macierz odwrotn� do "A"  i zapisuje j� do "Y".           *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Funkcja zwraca warto�� 0, je�li uda�o si� odwr�ci�, lub -1 je�li nie.   *
 *                                                                               *
 *********************************************************************************/
int matrix_inv(MATRIX *Y, MATRIX *A);

/*********************************************************************************
 * matrix_add                                                                    *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyj�ciowa Y = A + B                                            *
 *  - A - macierz A                                                              *
 *  - B - macierz B                                                              *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza  sum�  dw�ch  macierzy.  Macierze  A i B musz� by� tych *
 *    samych wymiar�w.  Macierz Y  jest  tworzona  w  funkcji, zatem musi zosta� *
 *    zwolniona przed wywo�aniem, je�li by�a wcze�niej u�ywana.                  *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Je�li uda�o si� dokona� operacji, funkcja zwraca 0. Je�li nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_add(MATRIX *Y, MATRIX *A, MATRIX *B);

/*********************************************************************************
 * matrix_sub                                                                    *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyj�ciowa Y = A - B                                            *
 *  - A - macierz A                                                              *
 *  - B - macierz B                                                              *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza r�nic� dw�ch  macierzy. Macierze  A i B musz� by� tych *
 *    samych wymiar�w.  Macierz Y  jest  tworzona  w  funkcji, zatem musi zosta� *
 *    zwolniona przed wywo�aniem, je�li by�a wcze�niej u�ywana.                  *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Je�li uda�o si� dokona� operacji, funkcja zwraca 0. Je�li nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_sub(MATRIX *Y, MATRIX *A, MATRIX *B);

/*********************************************************************************
 * matrix_mul                                                                    *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyj�ciowa Y = A * B                                            *
 *  - A - macierz A (n x r)                                                      *
 *  - B - macierz B (r x m)                                                      *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja  oblicza  iloczyn dw�ch  macierzy.  Macierze  A i B  musz�  by� *
 *    wymiar�w (n x r) oraz  (r x m).  Macierz Y jest  tworzona w funkcji, zatem *
 *    musi zosta� zwolniona przed wywo�aniem, je�li by�a wcze�niej u�ywana.      *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Je�li uda�o si� dokona� operacji, funkcja zwraca 0. Je�li nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_mul(MATRIX *Y, MATRIX *A, MATRIX *B);

/*********************************************************************************
 * matrix_div                                                                    *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyj�ciowa Y = A * B                                            *
 *  - A - macierz A (n x r)                                                      *
 *  - B - macierz B (r x m)                                                      *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja  oblicza  iloraz  dw�ch  macierzy.  Macierze  A i B  musz�  by� *
 *    wymiar�w (n x r) oraz  (r x m).  Macierz Y jest  tworzona w funkcji, zatem *
 *    musi zosta� zwolniona przed wywo�aniem, je�li by�a wcze�niej u�ywana.      *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Je�li uda�o si� dokona� operacji, funkcja zwraca 0. Je�li nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_div(MATRIX *Y, MATRIX *A, MATRIX *B);

/*********************************************************************************
 * matrix_add_int                                                                *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyj�ciowa Y = A + b                                            *
 *  - A - macierz A                                                              *
 *  - b - liczba b                                                               *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja  oblicza  sum�  ka�dego  elementu  macierzy i  liczby  b. Wynik *
 *    zapisuje w  macierzy  Y.  Macierz  Y  jest  tworzona w funkcji, zatem musi *
 *    zosta� zwolniona przed wywo�aniem, je�li by�a wcze�niej u�ywana.           *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Je�li uda�o si� dokona� operacji, funkcja zwraca 0. Je�li nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_add_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b);

/*********************************************************************************
 * matrix_sub_int                                                                *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyj�ciowa Y = A - b                                            *
 *  - A - macierz A                                                              *
 *  - b - liczba b                                                               *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza r�nicego ka�dego elementu  macierzy i liczby  b. Wynik *
 *    zapisuje w  macierzy  Y.  Macierz  Y  jest  tworzona w funkcji, zatem musi *
 *    zosta� zwolniona przed wywo�aniem, je�li by�a wcze�niej u�ywana.           *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Je�li uda�o si� dokona� operacji, funkcja zwraca 0. Je�li nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_sub_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b);

/*********************************************************************************
 * matrix_sub_int                                                                *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyj�ciowa Y = A * b                                            *
 *  - A - macierz A                                                              *
 *  - b - liczba b                                                               *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza iloczyn  ka�dego  elementu  macierzy i liczby  b. Wynik *
 *    zapisuje w  macierzy  Y.  Macierz  Y  jest  tworzona w funkcji, zatem musi *
 *    zosta� zwolniona przed wywo�aniem, je�li by�a wcze�niej u�ywana.           *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Je�li uda�o si� dokona� operacji, funkcja zwraca 0. Je�li nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_mul_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b);

/*********************************************************************************
 * matrix_div_int                                                                *
 *                                                                               *
 * Argumenty:                                                                    *
 *  - Y - macierz wyj�ciowa Y = A / b                                            *
 *  - A - macierz A                                                              *
 *  - b - liczba b                                                               *
 *                                                                               *
 * Opis:                                                                         *
 *       Funkcja oblicza  iloraz  ka�dego  elementu  macierzy i liczby  b. Wynik *
 *    zapisuje w  macierzy  Y.  Macierz  Y  jest  tworzona w funkcji, zatem musi *
 *    zosta� zwolniona przed wywo�aniem, je�li by�a wcze�niej u�ywana.           *
 *                                                                               *
 * Zwracana warto�� (int):                                                       *
 *       Je�li uda�o si� dokona� operacji, funkcja zwraca 0. Je�li nie, -1.      *
 *                                                                               *
 *********************************************************************************/
int matrix_div_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b);

#endif

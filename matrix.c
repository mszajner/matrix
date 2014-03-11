
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include "matrix.h"

/* zmienne globalne */
int matrix_error = 0;
static int matrix_function = -1;

const char* matrix_error_str(void)
{
	/* iloœæ funkcji */
	#define MAX_FUNCTION  25

	/* iloœæ b³êdow */
	#define MAX_ERROR     14

	/* tablica z nazwami funkcji */
	struct tagFUNCTION {
		int code;
		const char *name;
		const char *description;
	};

	/* tablica z opisem b³êdów */
	struct tagERROR {
		int code;
		const char *text;
	};

	const struct tagFUNCTION functions[MAX_FUNCTION] = {
		      0, "matrix_create", "Tworzenie nowej macierzy",
		      1, "matrix_resize", "Zmiana rozmiaru macierzy",
		      2, "matrix_destroy", "Niszczenie macierzy",
		      3, "matrix_read", "Odczyt macierzy z pliku",
		      4, "matrix_write", "Zapis macierzy do pliku",
			  5, "matrix_print", "Wyswietlanie macierzy na konsoli",
			  6, "matrix_info", "Informacje o macierzy",
			  7, "matrix_fill", "Wypelnianie macierzy",
			  8, "matrix_copy", "Kopiowanie macierzy",
			  9, "matrix_unit", "Wypelnia macierz tak, aby stala sie macierza jednostkowa",
			  10, "matrix_decomp", "Rozklad macierzy na dwie macierze trojkatne A=D*G",
			  11, "matrix_det_crout", "Obliczanie wyznacznika metoda Crouta",
			  12, "matrix_transp", "Transponowanie macierzy",
			  13, "matrix_minor", "Tworzenie minora z macierzy",
			  14, "matrix_suppl", "Tworzenie macierzy dopelnien",
			  15, "matrix_attach", "Tworzy macierz dolaczona",
			  16, "matrix_inv", "Wyznaczanie macierzy odwrotnej",
			  17, "matrix_add", "Sumowanie macierzy",
			  18, "matrix_sub", "Odejmowanie macierzy",
			  19, "matrix_mul", "Mnozenie macierzy",
			  20, "matrix_div", "Dzielenie macierzy",
			  21, "matrix_add_int", "Dodawanie liczby do macierzy",
			  22, "matrix_sub_int", "Odejmowanie liczby od macierzy",
			  23, "matrix_mul_int", "Mnozenie macierzy przez liczbe",
			  24, "matrix_div_int", "Dzielenie macierzy przez liczbe",
	};

	const struct tagERROR errors[MAX_ERROR] = {
		      0, "Operacja zakonczona sukcesem!",
		     -1, "Brak pamieci do zaalokowania danych",
			 -2, "Nieoczekiwany koniec pliku",
			 -3, "Blad odczytu pliku",
			 -4, "Blad zapisu do pliku",
			 -5, "Macierz jest prostokatna, wymagana macierz kwadratowa!",
			 -6, "Wymiary macierzy nie zgadzaja sie",
			 -7, "Proba dzielenia przez zero",
			 -8, "Nieznany blad",
			 -9, "Obliczenia sa poza zakresem",
		    -10, "Blad podczas wyswietlania macierzy",
			-11, "Podany adres komorki docelowej wychodzi poza macierz",
			-12, "Podany adres komorek zrodlowych wychodzi poza macierz",
			-13, "Podany adres wychodzi poza macierz",
	};

	int i;
	static char buffer[512] = "";
	const struct tagERROR *error = NULL;
	const struct tagFUNCTION *function = NULL;

	/* znajdŸ nazwê funkcji */
	for (i = 0; i < MAX_FUNCTION; i++)
	{
		if (functions[i].code == matrix_function)
		{
			function = &functions[i];
			break;
		}
	}

	/* znajdŸ opis b³êdu */
	for (i = 0; i < MAX_ERROR; i++)
	{
		if (errors[i].code == matrix_error)
		{
			error = &errors[i];
			break;
		}
	}

	/* znaleziono nazwê funkcji i opis b³êdu */
	if (function && error)
	{
		sprintf(buffer, "\nFunkcja:     %s\nOpis:        %s\nKod bledu:   %d\nOpis bledu:  %s\n",
			function->name, function->description, matrix_error, error->text);
	}
	/* znaleziono tylko nazwê funkcji */
	else if (function && !error)
	{
		sprintf(buffer, "\nFunkcja:     %s\nOpis:        %s\nKod bledu:   %d\n",
			function->name, function->description, matrix_error);
	}
	/* znaleziono tylko opis b³êdu */
	else if (!function && error)
	{
		sprintf(buffer, "\nKod bledu:   %d\nOpis bledu:  %s\n",
			matrix_error, error->text);
	}
	/* nie znaleziono ani nazwy funkcji ani opisu b³êdu */
	else if (!function && !error)
	{
		sprintf(buffer, "\nNieznany kod bledu: %d\n", matrix_error);
	}

	/* zwróæ wskaŸnik na bufor */
	return (const char*)buffer;
}

int matrix_create(MATRIX *matrix, MATRIX_ULONG rows, MATRIX_ULONG columns, MATRIX_DOUBLE value)
{
	/* spróbuj przydzieliæ pamiêæ */
	matrix->matrix = (MATRIX_DOUBLE*)malloc(rows * columns * sizeof(MATRIX_DOUBLE));

	/* sprawdz czy przydzielono */
	if (matrix->matrix == NULL)
		/* zwroc -1 - blad */
		return matrix_function = 0, matrix_error = -1;

	/* zapisz iloœæ wierszy */
	matrix->rows = rows;

	/* zapisz iloœæ kolumn */
	matrix->columns = columns;

	/* wypelnij macierz podan¹ wartoœci¹ */
	matrix_fill(matrix, value);

	/* zwroc 0 - OK */
	return matrix_function = 0, matrix_error = 0;
}

int matrix_resize(MATRIX *matrix, MATRIX_ULONG rows, 
				  MATRIX_ULONG columns, MATRIX_DOUBLE value)
{
	MATRIX_ULONG r, c;
	MATRIX backup = { 0, 0, NULL };

	/* spróbuj utworzyæ macierz o nowych wymiarach */
	if (matrix_create(&backup, rows, columns, value) < 0)
		/* zwroc -1 - blad */
		return matrix_function = 1, -1;

	/* skopiuj/wype³nij now¹ macierz danymi */
	for (r = 0; r < rows; r++)
	{
		for (c = 0; c < columns; c++)
		{
			if ((r < matrix->rows) && (c < matrix->columns))
				MATRIX_VALUE((&backup), r, c) = MATRIX_VALUE(matrix, r, c);
		}
	}

	/* zniszcz star¹ macierz */
	matrix_destroy(matrix);

	/* skopiuj strukturê nowej macierzy do starej */
	memcpy(matrix, &backup, sizeof(MATRIX));

	/* zwroæ 0 - OK */
	return matrix_function = 1, matrix_error = 0;
}

void matrix_destroy(MATRIX *matrix)
{
	/* zwolnij pamiêæ */
	free((void*)matrix->matrix);

	/* wyzeruj strukturê */
	memset(matrix, 0, sizeof(MATRIX));

	/* zapamiêtaj numer ostatniej funkcji */
	matrix_function = 2;

	/* wyzeruj kod b³êdu */
	matrix_error = 0;
}

int matrix_read(MATRIX *matrix, FILE *file, int binary)
{
	MATRIX_ULONG count = 0, rows, columns;

	/* czy odczyt binarny? */
	if (binary)
	{
		/* spróbuj odczytaæ iloœæ wierszy*/
		count += (MATRIX_ULONG)fread(&rows, sizeof(MATRIX_ULONG), 1, file) * sizeof(MATRIX_ULONG);

		/* spróbuj odczytaæ iloœæ kolumn*/
		count += (MATRIX_ULONG)fread(&columns, sizeof(MATRIX_ULONG), 1, file) * sizeof(MATRIX_ULONG);

		/* sprawdz czy wyst¹pi³ koniec pliku */
		if (feof(file))
			/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
			return matrix_function = 3, matrix_error = -2, -1;

		/* sprawdz czy wyst¹pi³ b³¹d odczytu */
		if (ferror(file))
			/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
			return matrix_function = 3, matrix_error = -3, -1;

		/* spróbuj zmieniæ rozmiar macierzy */
		if (matrix_resize(matrix, rows, columns, (MATRIX_DOUBLE)0.0) < 0)
			/* przywróæ nr funkcji, zwróc -1 - b³¹d */
			return matrix_function = 3, -1;

		/* spróbuj odczytaæ macierz */
		count += (MATRIX_ULONG)fread(matrix->matrix, sizeof(MATRIX_DOUBLE),
			matrix->rows * matrix->columns, file) * sizeof(MATRIX_DOUBLE);

		/* sprawdz czy wyst¹pi³ koniec pliku */
		if (feof(file))
		{
			/* zniszcz macierz */
			matrix_destroy(matrix);

			/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
			return matrix_function = 3, matrix_error = -2, -1;
		}

		/* sprawdz czy wyst¹pi³ b³¹d odczytu */
		if (ferror(file))
		{
			/* zniszcz macierz */
			matrix_destroy(matrix);

			/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
			return matrix_function = 3, matrix_error = -3, -1;
		}

		/* wyzeruj nr b³êdu, zwróæ iloœæ odczytanych bajtów */
		return matrix_function = 3, matrix_error = 0, (int)count;
	}
	else
	{
		MATRIX_ULONG i;

		/* spróbuj odczytaæ iloœæ wierszy i kolumn */
		count += (MATRIX_ULONG)fscanf(file, "%u%u", &rows, &columns);

		/* sprawdz czy wyst¹pi³ koniec pliku */
		if (feof(file))
			/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
			return matrix_function = 3, matrix_error = -2, -1;

		/* sprawdz czy wyst¹pi³ b³¹d odczytu */
		if (ferror(file))
			/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
			return matrix_function = 3, matrix_error = -3, -1;

		/* spróbuj zmieniæ rozmiar macierzy */
		if (matrix_resize(matrix, rows, columns, (MATRIX_DOUBLE)0.0) < 0)
			/* przywróæ nr funkcji, zwróc -1 - b³¹d*/
			return matrix_function = 3, -1;

		/* petla odczytujaca macierz */
		for (i = 0; i < matrix->rows * matrix->columns; i++)
		{
			/* sprawdz czy wyst¹pi³ koniec pliku */
			if (feof(file))
			{
				/* zniszcz macierz */
				matrix_destroy(matrix);

				/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
				return matrix_function = 3, matrix_error = -2, -1;
			}

			/* spróbuj odczytaæ komórkê */
			count += (MATRIX_ULONG)fscanf(file, "%Lg", &matrix->matrix[i]);

			/* sprawdz czy wyst¹pi³ b³¹d odczytu */
			if (ferror(file))
			{
				/* zniszcz macierz */
				matrix_destroy(matrix);

				/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
				return matrix_function = 3, matrix_error = -3, -1;
			}
		}

		/* wyzeruj kod b³êdu, zwróæ iloœæ odczytanych elementów */
		return matrix_function = 3, matrix_error = 0, (int)count;
	}
}

int matrix_write(MATRIX *matrix, FILE *file, int binary)
{
	MATRIX_ULONG count = 0;

	/* zapamiêtaj numer ostatniej funkcji */
	matrix_function = 4;

	/* sprawdŸ czy zapis binarny */
	if (binary)
	{
		/* spróbuj zapisaæ iloœæ wierszy */
		count += (MATRIX_ULONG)fwrite(&matrix->rows, sizeof(MATRIX_ULONG), 1, file) * sizeof(MATRIX_ULONG);

		/* sróbuj zapisaæ iloœæ kolumn */
		count += (MATRIX_ULONG)fwrite(&matrix->columns, sizeof(MATRIX_ULONG), 1, file) * sizeof(MATRIX_ULONG);

		/* sprawdz czy wyst¹pi³ b³¹d zapisu */
		if (ferror(file))
			/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
			return matrix_error = -4, -1;

		/* spróbuj zapisaæ macierz */
		count += (MATRIX_ULONG)fwrite(matrix->matrix, sizeof(MATRIX_DOUBLE),
			matrix->rows * matrix->columns, file) * sizeof(MATRIX_DOUBLE);

		/* sprawdz czy wyst¹pi³ b³¹d zapisu */
		if (ferror(file))
			/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
			return matrix_error = -4, -1;

		/* wyzeruj nr b³êdu, zwróæ iloœæ zapisanych bajtów */
		return matrix_error = 0, (int)count;
	}
	else
	{
		MATRIX_ULONG i;

		/* spróbuj zapisaæ iloœæ wierszy i kolumn */
		count += (MATRIX_ULONG)fprintf(file, "%u %u", matrix->rows, matrix->columns);

		/* sprawdz czy wyst¹pi³ b³¹d zapisu */
		if (ferror(file))
			/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
			return matrix_error = -4, -1;

		/* petla zapisuj¹ca komórki */
		for (i = 0; i < matrix->rows * matrix->columns; i++)
		{
			/* spróbuj zapisaæ komórkê */
			count += (MATRIX_ULONG)fprintf(file, "%c%g", (i % matrix->columns == 0) ?
				'\n' : ' ', matrix->matrix[i]);

			/* sprawdz czy wyst¹pi³ b³¹d zapisu */
			if (ferror(file))
				/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d */
				return matrix_error = -4, -1;
		}

		/* wyzeruj nr b³êdu, zwróæ iloœc zapisanych komórek + 2 (rows, columns) */
		return matrix_error = 0, (int)count;
	}
}

int matrix_print(MATRIX *matrix, MATRIX_ULONG precision)
{
	MATRIX_ULONG i;

	/* zapamiêtaj numer ostatniej funkcji */
	matrix_function = 5;

	/* wyœwietl komórki macierzy */
	for (i = 0; i < matrix->rows * matrix->columns; i++)
	{
		/* spróbuj wyswietliæ pojedyncz¹ komórkê */
		if (printf("%c%*.*e", (i % matrix->columns == 0) ? '\n' : ' ', 
				precision+8, precision, matrix->matrix[i]) < 0)
			/* zapamiêtaj kod b³êdu, zwróæ -1 - b³¹d*/
			return matrix_error = -10, -1;
	}

	/* wyzeruj kod b³êdu, zwróæ iloœæ wyœwietlonych komórek */
	return matrix_error = 0, (int)i;
}

void matrix_info(MATRIX *matrix)
{
	/* zapamiêtaj numer ostatniej funkcji */
	matrix_function = 6;

	/* wyzeruj kod b³êdu */
	matrix_function = 0;

	printf("\nInformacje o macierzy:");
	printf("\n\tAdres macierzy:             %p", matrix);
	printf("\n\tAdres komorek macierzy:     %p", matrix->matrix);
	printf("\n\tWymiary (wierszy x kolumn): %ux%u", matrix->rows, matrix->columns);
	printf("\n\tZajetosc pamieci:           %lu bajtow", MATRIX_SIZE(matrix));
	printf("\n");
}

void matrix_fill(MATRIX *matrix, MATRIX_DOUBLE value)
{
	MATRIX_ULONG i;

	/* zapamiêtaj numer ostatniej funkcji */
	matrix_function = 7;

	/* wype³nia macierz */
	for (i = 0; i < matrix->rows * matrix->columns; i++)
		matrix->matrix[i] = value;

	/* wyzeruj kod b³êdu */
	matrix_error = 0;
}

int matrix_copy(MATRIX *dest, MATRIX *src, 
				MATRIX_ULONG dest_row, MATRIX_ULONG dest_col,
				MATRIX_ULONG src_row1, MATRIX_ULONG src_col1, 
				MATRIX_ULONG src_row2, MATRIX_ULONG src_col2)
{
	MATRIX_ULONG nrows = 0, ncolumns = 0;

	/* sprawdz czy podano adresy w dobrej kolejnosci */
	if ((src_row1 > src_row2) || (src_col1 > src_col2))
		/* zwroc -1 - blad */
		return matrix_function = 8, matrix_error = -11, -1;

	/* sprawdz zakresy celu */
	if ((dest_row > dest->rows) || (dest_col > dest->columns))
		/* zwroc -1 - blad */
		return matrix_function = 8, matrix_error = -11, -1;

	/* sprawdz zakresy zrodla */
	if ((src_row2 > src->rows) || (src_col2 > src->columns))
		/* zwric -1 - blad */
		return matrix_function = 8, matrix_error = -12, -1;

	/* sprawdz czy trzeba zwiekszyc macierz */
	if (dest_row + (src_row2 - src_row1) > dest->rows)
		nrows = dest_row + (src_row2 - src_row1);

	/* sprawdz czy trzeba poszerzyc macierz */
	if (dest_col + (src_col2 - src_col1) > dest->columns)
		ncolumns = dest_col + (src_col2 - src_col1);

	/* czy trzeba zwiekszac ? */
	if ((nrows > 0) || (ncolumns > 0))
	{
		/* zmien rozmiar macierzy */
		if (matrix_resize(dest, nrows, ncolumns, (MATRIX_DOUBLE)0.0) < 0)
			/* jesli sie nie udalo zakoncz funkcje, -1 - b³¹d */
			return matrix_function = 8, -1;
	}

	/* petla kopiujaca macierz */
	for (nrows = 0; nrows < src_row2-src_row1; nrows++)
		for (ncolumns = 0; ncolumns < src_col2-src_col1; ncolumns++)
		{
			MATRIX_VALUE(dest, nrows + dest_row, ncolumns + dest_col) =
				MATRIX_VALUE(src, nrows + src_row1, ncolumns + src_col1);
		}

	/* zwroc ilosc skopiowanych komorek */
	return matrix_function = 8, matrix_error = 0,
		(int)((src_row2-src_row1) * (src_col2-src_col1));
}

int matrix_unit(MATRIX *A)
{
	MATRIX_ULONG i;

	/* sprawdz czy macierz jest kwadratowa */
	if (A->rows != A->columns)
		/* zwroc -1 */
		return matrix_function = 9, matrix_error = -5, -1;

	/* wypelnij macierz zerami */
	matrix_fill(A, (MATRIX_DOUBLE)0.0);

	/* wypelnij przekatna jedynkami */
	for (i = 0; i < A->rows; i++)
		MATRIX_VALUE(A, i, i) = (MATRIX_DOUBLE)1.0;

	/* zwroc 0 - OK */
	return matrix_function = 9, matrix_error = 0;
}

int matrix_decomp(MATRIX *A, MATRIX *D, MATRIX *G)
{
	MATRIX_ULONG r, c, k;

	/* sprawdz czy macierz jest kwadratowa*/
	if (A->rows != A->columns)
		/* zwroc -1, oraz zapisz kod bledu */
		return matrix_function = 10, matrix_error = -5, -1;

	/* zmien rozmiar macierzy D */
	if (matrix_resize(D, A->rows, A->columns, (MATRIX_DOUBLE)0.0) < 0)
		/* zakoncz z bledem */
		return matrix_function = 10, -1;

	/* zmien rozmiar macierzy G */
	if (matrix_resize(G, A->rows, A->columns, (MATRIX_DOUBLE)0.0) < 0)
	{
		/* zachowaj kod bledu */
		k = (MATRIX_ULONG)matrix_error;

		/* usun macierz D */
		matrix_destroy(D);

		/* zakoncz z bledem*/
		return matrix_function = 10, matrix_error = (int)k, -1;
	}

	/* macierz G - jednostkowa */
	if (matrix_unit(G) < 0)
		/* zakoncz z bledem */
		return matrix_function = 10, -1;

	/* wyzeruj znacznik bledu */
	matrix_error = 0;

	/* rozklad macierzy A na macierze D i G */
	for (c = 0; c < A->columns; c++)
	{
		/* wypelnianie macierzy D */
		for (r = c; r < A->rows; r++)
		{
			MATRIX_VALUE(D, r, c) = MATRIX_VALUE(A, r, c);
			for (k = 0; k < c; k++)
			{
				MATRIX_VALUE(D, r, c) -= 
					MATRIX_VALUE(D, r, k)*MATRIX_VALUE(G, k, c);
			}
		}

		/* sprawdz poprawnosc obliczen */
		if (fabs(MATRIX_VALUE(D,c,c)) < EPSILON)
			/* ustaw znacznik bledu */
            matrix_error = -9;

		/* wypelnianie macierzy G */
		for (r = c+1; r < A->rows; r++)
		{
			MATRIX_VALUE(G, c, r) = MATRIX_VALUE(A, c, r);
			for (k = 0; k < c; k++)
			{
				MATRIX_VALUE(G, c, r) -= 
					MATRIX_VALUE(D, c, k)*MATRIX_VALUE(G, k, r);
			}
			MATRIX_VALUE(G, c, r) /= MATRIX_VALUE(D, c, c);
		}
	}

	/* zwroc 0 - OK */
	return matrix_function = 10, matrix_error < 0 ? -1 : 0;
}

MATRIX_DOUBLE matrix_det_crout(MATRIX *A)
{
	MATRIX_ULONG i;
	MATRIX_DOUBLE B = 1.0;
	MATRIX D = { 0, 0, NULL };
	MATRIX G = { 0, 0, NULL };

	/* rozloz macierz A na dwie macierze trojkatne D i G */
	if (matrix_decomp(A, &D, &G) >= 0)
	{
		/* oblicz wartosc wyznacznika */
		for (i = 0; i < A->columns; i++)
            B *= MATRIX_VALUE((&D), i, i);

		/* usun macierz D */
		matrix_destroy(&D);

		/* usun macierz G */
		matrix_destroy(&G);

		/* zachowaj nr bledu - brak bledu */
		matrix_error = 0;
	}

	/* zwroc wartosc wyznacznika */
	return matrix_function = 11, B;
}

int matrix_transp(MATRIX *Y, MATRIX *A)
{
	MATRIX_ULONG r, c;

	/* ustaw rozmiar macierzy Y taki jak macierzy A */
	if (matrix_resize(Y, A->columns, A->rows, (MATRIX_DOUBLE)0.0) < 0)
		/* zakoncz z bledem */
		return matrix_function = 12, -1;

	/* transponowanie macierzy */
	for (r = 0; r < A->rows; r++)
		for (c = 0; c < A->columns; c++)
			MATRIX_VALUE(Y, c, r) = MATRIX_VALUE(A, r, c);

	/* zwroc 0 - OK */
	return matrix_function = 12, matrix_error = 0;
}

int matrix_minor(MATRIX *Y, MATRIX *A, MATRIX_ULONG r, MATRIX_ULONG c)
{
	MATRIX_ULONG i, j, k, l;

	/* sprawdz zakresy */
	if ((r >= A->rows) || (c >= A->columns))
		/* ustaw kod bledu */
		return matrix_function = 13, matrix_error = -13, -1;

	/* ustal rozmiar macierzy wyjœciowej */
	if (matrix_resize(Y, A->rows-1, A->columns-1, (MATRIX_DOUBLE)0.0) < 0)
		/* zwróæ b³¹d */
        return matrix_function = 13, -1;

	/* wytnij wiersz r i kolumnê c */
	for (i = 0, k = 0; i < A->rows; i++)
	{
		if (i != r)
		{
			for (j = 0, l = 0; j < A->columns; j++)
			{
				if (j != c)
				{
					MATRIX_VALUE(Y, k, l) = MATRIX_VALUE(A, i, j);
					l++;
				}
			}
			k++;
		}
	}

	/* zwroc 0 - OK */
	return matrix_function = 13, matrix_error = 0;
}

int matrix_suppl(MATRIX *Y, MATRIX *A)
{
	MATRIX_ULONG r, c;
	MATRIX M = { 0, 0, NULL };

	/* sprawdz czy macierz kwadratowa */
	if (A->rows != A->columns)
		/* zwróæ -1 - b³¹d */
		return matrix_function = 14, matrix_error = -5, -1;

	/* ustal rozmiar macierzy wyjœciowej */
	if (matrix_resize(Y, A->rows, A->columns, (MATRIX_DOUBLE)0.0) < 0)
		/* zwróæ -1 - b³¹d */
		return matrix_function = 14, -1;

	/* tworzenie macierzy dope³nieñ */
	for (r = 0; r < A->rows; r++)
		for (c = 0; c < A->columns; c++)
		{
			/* stwórz minor dla elementu r, c */
			if (matrix_minor(&M, A, r, c) < 0)
			{
				/* zachowaj b³¹d z funkcji matrix_minor */
				r = (MATRIX_ULONG)matrix_error;

				/* zniszcz minor */
				matrix_destroy(&M);

				/* zniszcz macierz wyjsciow¹ */
				matrix_destroy(Y);

				/* zwróæ -1 - b³¹d */
				return matrix_function = 14, matrix_error = (int)r, -1;
			}

			/* zachowaj wyznacznik z minora w macierzy wyjœciowej */
			MATRIX_VALUE(Y,r,c) = matrix_det(&M) * ((r+c)%2 ? -1.0 : 1.0);
		}

	/* zniszcz tymczasow¹ macierz minorów */
	matrix_destroy(&M);

	/* zwróæ 0 - OK */
	return matrix_function = 14, matrix_error = 0;
}

int matrix_attach(MATRIX *Y, MATRIX *A)
{
	MATRIX T = { 0, 0, NULL };

	/* sprawdz czy macierz kwadratowa */
	if (A->rows != A->columns)
		/* zwróæ -1 - b³¹d */
		return matrix_function = 15, matrix_error = -5, -1;

	/* stwórz macierz dope³nieñ */
	if (matrix_suppl(&T, A) < 0)
		/* zwróæ -1 - b³¹d */
		return matrix_function = 15, -1;

	if (matrix_transp(Y, &T) < 0)
	{
		/* zachowaj kod bledu z funkcji matrix_transp */
		int temp = matrix_error;

		/* zniszcz macierz tymczasow¹ */
		matrix_destroy(&T);

		/* zwróæ -1 - b³¹d */
		return matrix_function = 15, matrix_error = temp, -1;
	}

	/* zniszcz macierz tymczasow¹ */
	matrix_destroy(&T);

	/* zwróæ 0 - OK */
	return matrix_function = 15, matrix_error = 0;
}

int matrix_inv(MATRIX *Y, MATRIX *A)
{
	MATRIX_DOUBLE w;
	MATRIX D = { 0, 0, NULL };

	/* oblicz wartosc wyznacznika */
	w = matrix_det(A);

	/* sprawdŸ czy dobrze obliczono wartoœæ wyznacznika */
	if (matrix_error < 0)
		/* zwróæ -1 - b³¹d */
		return matrix_function = 16, -1;

	/* oblicz macierz do³¹czon¹ */
	if (matrix_attach(&D, A) < 0)
		/* zwróæ -1 - b³¹d */
		return matrix_function = 16, -1;

	/* pomnó¿ macierz do³¹czon¹ przez odwrotnoœæ wyznacznika */
	if (matrix_div_int(Y, &D, w) < 0)
	{
		/* zapamiêtaj kod b³êdu z funkcji matrix_div_int */
		int temp = matrix_error;

		/* zniszcz macierz do³¹czon¹ */
		matrix_destroy(&D);

		/* zwróæ -1 - b³¹d */
		return matrix_function = 16, matrix_error = temp, -1;
	}

	/* zniszcz macierz do³¹czon¹ */
	matrix_destroy(&D);

	/* zwróæ 0 - OK */
	return matrix_function = 16, matrix_error = 0;
}

int matrix_add(MATRIX *Y, MATRIX *A, MATRIX *B)
{
	MATRIX_ULONG i;

	/* sprawdz wymiary macierzy */
	if ((A->rows != B->rows) || (A->columns != B->columns))
		/* zwroc -1 - blad */
		return matrix_function = 17, matrix_error = -6, -1;

	/* ustal rozmiar macierzy wyjsciowej */
	if (matrix_resize(Y, A->rows, A->columns, (MATRIX_DOUBLE)0.0) < 0)
		/* zwroc -1 - blad */
		return matrix_function = 17, -1;

	/* dodaj macierze */
	for (i = 0; i < A->rows * A->columns; i++)
		Y->matrix[i] = A->matrix[i] + B->matrix[i];

	/* zwroc 0 - OK */
	return matrix_function = 17, matrix_error = 0;
}

int matrix_sub(MATRIX *Y, MATRIX *A, MATRIX *B)
{
	MATRIX_ULONG i;

	/* sprawdz wymiary macierzy */
	if ((A->rows != B->rows) || (A->columns != B->columns))
		/* zwroc -1 - blad */
		return matrix_function = 18, matrix_error = -6, -1;

	/* ustal rozmiar macierzy wyjsciowej */
	if (matrix_resize(Y, A->rows, A->columns, (MATRIX_DOUBLE)0.0) < 0)
		/* zwroc -1 - blad */
		return matrix_function = 18, -1;

	/* dodaj macierze */
	for (i = 0; i < A->rows * A->columns; i++)
		Y->matrix[i] = A->matrix[i] - B->matrix[i];

	/* zwroc 0 - OK */
	return matrix_function = 18, matrix_error = 0;
}

int matrix_mul(MATRIX *Y, MATRIX *A, MATRIX *B)
{
	MATRIX_ULONG i, j, k;

	/* sprawdz wymiary macierzy */
	if (A->columns != B->rows)
		/* zwroc -1 - blad */
		return matrix_function = 19, matrix_error = -6, -1;

	/* ustal rozmiar macierzy wyjsciowej */
	if (matrix_resize(Y, A->rows, B->columns, (MATRIX_DOUBLE)0.0) < 0)
		/* zwroc -1 - blad */
		return matrix_function = 19, -1;

	/* wykonaj mno¿enie */
	for (i = 0; i < A->rows; i++)
		for (j = 0; j < B->columns; j++)
		{
			MATRIX_VALUE(Y, i, j) = (MATRIX_DOUBLE)0.0;
			for (k = 0; k < A->columns; k++)
				MATRIX_VALUE(Y, i, j) += MATRIX_VALUE(A, i, k) * MATRIX_VALUE(B, k, j);
		}

	/* zwróæ 0 - OK */
	return matrix_function = 19, matrix_error = 0;
}

int matrix_div(MATRIX *Y, MATRIX *A, MATRIX *B)
{
	MATRIX INV = { 0, 0, NULL };

	/* odwróæ macierz B */
	if (matrix_inv(&INV, B) < 0)
		/* zwróæ -1 - b³¹d */
		return matrix_function = 20, -1;

	/* pomnó¿ macierze A i B^(-1)*/
	if (matrix_mul(Y, A, &INV) < 0)
	{
		/* zapamiêtaj kod b³êdu z funcji matrix_mul */
		int temp = matrix_error;

		/* zniszcz macierz odwrotn¹ */
		matrix_destroy(&INV);

		/* zwróæ -1 - b³¹d */
		return matrix_function = 20, matrix_error = temp, -1;
	}

	/* zniszcz macierz odwrotn¹ */
	matrix_destroy(&INV);

	/* zwróæ 0 - OK */
	return matrix_function = 20, matrix_error = 0, -1;
}

int matrix_add_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b)
{
	MATRIX_ULONG i;

	/* ustal rozmiar macierzy wyjœciowej */
	if (matrix_resize(Y, A->rows, A->columns, (MATRIX_DOUBLE)0.0) < 0)
		/* zwróæ -1 - b³¹d */
		return matrix_function = 21, -1;

	/* wykonaj dodawanie */
	for (i = 0; i < A->rows * A->columns; i++)
		Y->matrix[i] = A->matrix[i] + b;

	/* zwróæ 0 - OK */
	return matrix_function = 21, matrix_error = 0;
}

int matrix_sub_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b)
{
	/* wykonaj odejmowanie */
	return matrix_add_int(Y, A, -b), matrix_function = 22, matrix_error;
}

int matrix_mul_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b)
{
	MATRIX_ULONG i;

	/* ustal rozmiar macierzy wyjœciowej */
	if (matrix_resize(Y, A->rows, A->columns, (MATRIX_DOUBLE)0.0) < 0)
		/* zwróæ -1 - b³¹d */
		return matrix_function = 23, -1;

	/* podziel wszystkie elementy przez b */
	for (i = 0; i < A->rows * A->columns; i++)
		Y->matrix[i] = A->matrix[i] * b;

	/* zwróæ 0 - OK */
	return matrix_function = 23, matrix_error = 0;
}

int matrix_div_int(MATRIX *Y, MATRIX *A, MATRIX_DOUBLE b)
{
	/* sprawdŸ czy to nie bêdzie dzielenie przez 0 */
	if (b == 0.0)
		/* zwróæ -1 - b³¹d */
		return matrix_function = 24, matrix_error = -7, -1;

	/* wykonaj dzielenie */
	return matrix_mul_int(Y, A, 1 / b), matrix_function = 24, matrix_error;
}

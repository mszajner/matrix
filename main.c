#include <string.h>
#include "matrix.h"
#include "parser.h"

void main_about(FILE *file)
{
	fputs("\nMatrix Version 1.0", file);
	fputs("\n\nProjekt zaliczeniowy z \"Programowanie komputerow\"\n", file);
	fputs("\nAutor:  Miroslaw Szajner", file);
	fputs("\nRok:    2FD 2004/2005", file);
	fputs("\nE-mail: mszajner@rexoft.pl\n", file);
	fputs("\nWpisz \"help\" aby uzyskac pomoc...\n\n", file);
}

int main(int argc, char *argv[])
{
	/* bufor i strumien wejsciowy */
	char buffer[255];
	FILE *file = NULL;

	/* wyswietl informacje o programie */
	main_about(stdout);

	/* sprawdz parametry wywolania */
	if (argc > 1)
	{
		/* sprobuj otworzyc plik */
		file = fopen(argv[1], "rt");

		/* sprawdz czy sie nie udalo */
		if (file == NULL)
		{
			/* wyswietl komunikat */
			printf("\nNie odnaleziono \"%s\", zostanie uzyta konsola\n", argv[1]);

			/* ustaw standardowy strumien wejsciowy */
			file = stdin;
		}
	}
	else
	{
		/* ustaw standardowy strumien wejsciowy */
		file = stdin;
	}

	/* zainicjalizuj parser */
	if (parser_init() < 0)
	{
		/* wyswietl komunikat o bledzie */
		fputs("\nWystapil blad podczas inicjalizacji parsera.", stdout);
	}
	else
	{
		int done = 0;

		/* sprawdz czy wczytujemy z konsoli */
		if (file == stdin)
		{
			/* wyswietl znak zachety */
			fputs("> ", stdout);
		}

		/* petla odczytujaca dane */
		while (!done && fgets(buffer, 255, file))
		{
			/* wykonaj instrukcje */
			if ((done = parser_run(buffer)) == -1)
			{
				/* sprawdz czy blad pochodzi z parser.c */
				if (parser_error < 0)
				{
					/* przejdz do nowej linii i wyswietl info skad blad */
					fputs("\nparser.c:\n", stdout);

					/* wyswietl blad wykonania instrukcji */
					fputs(parser_error_str(), stdout);

					/* przejdz do nowej linii */
					fputs("\n\n", stdout);

					/* wyzeruj kod bledu */
					parser_error = 0;
				}

				/* nie koncz petli */
				done = 0;
			}

			/* sprawdz czy blad wystapil w bibliotece matrix.c */
			if (matrix_error < 0)
			{
				/* wyswietl info skad blad */
				fputs("\nmatrix.c:", stdout);

				/* wyswietl komunikat bledu */
				fputs(matrix_error_str(), stdout);

				/* przejdz do nowej linii */
				fputs("\n", stdout);

				/* wyzeruj kod bledu */
				matrix_error = 0;
			}

			/* sprawdz czy wczytujemy z konsoli */
			if (file == stdin)
			{
				/* wyswietl znak zachety */
				fputs("> ", stdout);
			}
		}

		/* zwolnij parser */
		parser_free();
	}

	/* sprawdz czy odczytywano z pliku */
	if (file != stdin)
	{
		/* zamknij plik */
		fclose(file);

		/* wyswietl komunikat */
		printf("\nNacisnij ENTER aby zakonczyc...");

		/* poczekaj na enter */
		fgetc(stdin);
	}

	/* zwroc zero - OK */
	return 0;
}

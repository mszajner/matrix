
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "matrix.h"
#include "parser.h"

#define MAX_MATRIX     ('Z'-'A')
#define MAX_VARIABLE   ('z'-'a')
#define PARAMETER      struct tagPARAMETER
#define FUNCTION       struct tagFUNCTION

struct tagPARAMETER {
	char name[32];
	enum {
		tNone     = 0x0000,
		tMatrix   = 0x0001,
		tVariable = 0x0002,
		tValue    = 0x0004,
		tName     = 0x0008,
	} type;
	union {
		MATRIX         *matrix;
		MATRIX_DOUBLE  *variable;
		MATRIX_DOUBLE   value;
		void           *pointer;
	};
};

struct tagFUNCTION {
	const char *name;
	int (*function)(PARAMETER *params, int count);
};

int mnew(PARAMETER *params, int count);
int mdelete(PARAMETER *params, int count);
int mread(PARAMETER *params, int count);
int mwrite(PARAMETER *params, int count);
int mprint(PARAMETER *params, int count);
int minfo(PARAMETER *params, int count);
int mfill(PARAMETER *params, int count);
int mcopy(PARAMETER *params, int count);
int munit(PARAMETER *params, int count);
int mdecomp(PARAMETER *params, int count);
int mdet(PARAMETER *params, int count);
int mtransp(PARAMETER *params, int count);
int mminor(PARAMETER *params, int count);
int msup(PARAMETER *params, int count);
int mattach(PARAMETER *params, int count);
int minv(PARAMETER *params, int count);
int madd(PARAMETER *params, int count);
int msub(PARAMETER *params, int count);
int mmul(PARAMETER *params, int count);
int mdiv(PARAMETER *params, int count);
int mhelp(PARAMETER *params, int count);

static MATRIX        matrixes[MAX_MATRIX];
static MATRIX_DOUBLE variables[MAX_VARIABLE];

int parser_error = 0;

MATRIX* parser_matrix(char name)
{
	/* sprawdz czy nazwa jest poprawna */
	if (isupper(name))
		/* zwroc wskaznik na macierz */
		return &matrixes[name - 'A'];

	/* zwroc pusty wskaznik */
	return NULL;
}

MATRIX_DOUBLE* parser_variable(char name)
{
	/* sprawdz czy nazwa jest poprawna */
	if (islower(name))
		/* zwroc wartosc */
		return &variables[name - 'a'];

	/* zwroc NULL - nie znaleziono */
	return NULL;
}

int parser_value(const PARAMETER *param, MATRIX_DOUBLE *value)
{
	switch (param->type)
	{
	case tValue:
		value[0] = param->value;
		break;
	case tVariable:
		value[0] = param->variable[0];
		break;
	default:
		return parser_error = -8, -1;
	}
	return parser_error = 0;
}

const FUNCTION* parser_find(const char *function)
{
	#define MAX_FUNCTION 22

	static FUNCTION functions[MAX_FUNCTION] = {
		"new",        &mnew,
		"delete",     &mdelete,
		"read",       &mread,
		"write",      &mwrite,
		"print",      &mprint,
		"info",       &minfo,
		"fill",       &mfill,
		"copy",       &mcopy,
		"unit",       &munit,
		"decompose",  &mdecomp,
		"det",        &mdet,
		"transpose",  &mtransp,
		"minor",      &mminor,
		"sup",        &msup,
		"attach",     &mattach,
		"inv",        &minv,
		"add",        &madd,
		"sub",        &msub,
		"mul",        &mmul,
		"div",        &mdiv,
		"exit",       NULL,
		"help",       &mhelp,
	};

	int i;

	/* znajdz funkcje */
	for (i = 0; i < MAX_FUNCTION; i++)
		/* porownaj nazwe funkcji i z podana nazwa */
		if (strcmp(function, functions[i].name) == 0)
			/* zwroc wskaznik do znalezionej funkcji */
			return &functions[i];

	/* zwroc pusty wskaznik */
	return NULL;
}

int parser_init(void)
{
	/* wyczysc macierze */
	memset(matrixes, 0, MAX_MATRIX * sizeof(MATRIX));

	/* wyzeruj zmienne */
	memset(variables, 0, MAX_VARIABLE * sizeof(MATRIX_DOUBLE));

	/* zwroc 0 - OK */
	return parser_error = 0;
}

const char* parser_error_str(void)
{
	#define MAX_ERROR 16

	/* tablica z opisem b³êdów */
	struct tagERROR {
		int code;
		const char *text;
	};
	
	const struct tagERROR errors[MAX_ERROR] = {
	      0, "Operacja zakonczona sukcesem!",
		 -1, "Podana funkcja nie jest dostepna",
		 -2, "Wystapil blad podczas konwersji tekstu na liczbe",
		 -3, "Za dluga nazwa zmiennej lub macierzy",
		 -4, "Nazwa zmiennej lub macierzy niedozwolona",
		 -5, "Nieznany parametr",
		 -6, "Za duzo parametrow",
		 -7, "Za malo parametrow",
		 -8, "Oczekiwano wartoœci lub zmiennej",
		 -9, "Oczekiwano zmiennej lub macierzy",
		-10, "Oczekiwano macierzy",
		-11, "Oczekiwano zmiennej",
		-12, "Proba dzielenia przez zero",
		-13, "Oczekiwano sta³ej lub zmiennej",
		-14, "Podany plik nie istnieje",
		-15, "Blad podczas tworzenia pliku",
	};

	int i;

	/* znajdŸ opis b³êdu */
	for (i = 0; i < MAX_ERROR; i++)
		if (errors[i].code == parser_error)
			return errors[i].text;

	return "Nieznany blad";
}

int parser_run(char *buffer)
{
	#define MAX_PARAMETER  8

	int i;
	char *token = NULL;
	const char *seps = " \n\t";
	const FUNCTION *function = NULL;
	PARAMETER params[MAX_PARAMETER];

	/* pobierz nazwe funkcji */
	token = strtok(buffer, seps);

	/* sprawdz czy podano nazwe funkcji */
	if (token == NULL)
		/* nie podano - pusta linia, zwroc 0 - OK */
		return 0;

	/* znajdz funkcje */
	if ((function = parser_find(token)) == NULL)
	{
		/* brak funkcji */
		parser_error = -1;

		/* zakoncz funkcje */
		return -1;
	}

	/* sprawdz czy to funkcja wyjscia */
	if (strcmp(token, "exit") == 0)
		/* zakoncz funkcje */
		return 1;

	/* pobierz nastepny token */
	token = strtok(NULL, seps);

	/* wyczysc tablice parametrow */
	memset(params, 0, MAX_PARAMETER * sizeof(PARAMETER));

	/* parsing loop */
	for (i = 0; (token != NULL) && (i < 8); i++, token = strtok(NULL, seps))
	{
		/* ustaw nazwe parametru na spacje */
		params[i].name[0] = 0;

		/* sprawdcz czy to jest liczba */
		if (isdigit(token[0]) || (token[0] == '-') || (token[0] == '+'))
		{
			/* parametrem jest wartosc */
			params[i].type = tValue;

			/* odczytaj wartosc z bufora i zapisz do parametru */
			if (sscanf(token, "%Lg", &params[i].value) == 0)
			{
				/* zwroc -1 - blad */
				return parser_error = -2, -1;
			}
		}
		else
		{
			/* sprawdz czy nazwa ma tylko jedna litere */
			if (strlen(token) == 1)
			{
				/* sprawdz czy jest to mala litera */
				if (islower(token[0]))
				{
					/* parametrem jest zmienna */
					params[i].type = tVariable;

					/* zapamietaj nazwe zmiennej */
					strncpy(params[i].name, token, 31);

					/* pobierz adres zmiennej */
					params[i].variable = parser_variable(params[i].name[0]);
				}
				else if (isupper(token[0]))
				{
					/* parametrem jest macierz */
					params[i].type = tMatrix;

					/* zapamietaj nazwe macierzy */
					strncpy(params[i].name, token, 31);

					/* pobierz adres zmiennej */
					params[i].matrix = parser_matrix(params[i].name[0]);
				}
				else
				{
					/* zwroc -1 - blad */
					return parser_error = -5, -1;
				}
			}
			else
			{
				/* parametrem jest string */
				params[i].type = tName;

				/* zapamietaj string */
				strncpy(params[i].name, token, 31);
			}
		}
	}

	/* uruchamianie */
	if (function->function(params, i) < 0)
	{
		/* zwroc -1 - blad */
		return -1;
	}

	/* zwroc 0 - OK */
	return parser_error = 0;
}

void parser_free(void)
{
	int i;

	/* zniszcz wszystkie macierzy */
	for (i = 0; i < MAX_MATRIX; i++)
		/* zniszcz macierz i */
		matrix_destroy(&matrixes[i]);

	/* wyzeruj kod bledu */
	parser_error = 0;
}

int mnew(PARAMETER *params, int count)
{
	MATRIX_DOUBLE rows, columns, value = 0.0;

	if (count < 1)
		return parser_error = -7, -1;

	switch (params[0].type)
	{
	case tMatrix:
        if (count < 3)
			return parser_error = -7, -1;

		parser_value(&params[1], &rows);

		parser_value(&params[2], &columns);

		if (parser_error < 0)
			return -1;

		if (count > 3)
		{
			if (count > 4)
				return parser_error = -6, -1;

			parser_value(&params[3], &value);

			if (parser_error < 0)
				return -1;
		}
        
		if (matrix_create(params[0].matrix, (MATRIX_ULONG)rows, 
			(MATRIX_ULONG)columns, value))
			return -1;
		break;

	case tVariable:
        if (count > 1)
		{
			if (count > 2)
				return parser_error = -6, -1;

			parser_value(&params[1], &value);

			if (parser_error < 0)
				return -1;
		}

		params[0].variable[0] = value;
		break;

	default:
		return parser_error = -9, -1;
	}
	
	return parser_error = 0;
}

int mdelete(PARAMETER *params, int count)
{
	if (count > 1)
		return parser_error = -6, -1;
	else if (count < 1)
		return parser_error = -7, -1;

	switch (params[0].type)
	{
	case tMatrix:
		matrix_destroy(params[0].matrix);

		if (matrix_error < 0)
			return -1;
		break;

	case tVariable:
		params[0].variable[0] = 0.0;
		break;

	default:
		return parser_error = -9, -1;
	}

	return parser_error = 0;
}

int mread(PARAMETER *params, int count)
{
	FILE *file = stdin;

	if (count > 0)
	{
		switch (params[0].type)
		{
		case tMatrix:
			if (count > 1)
			{
				if (count > 2)
					return parser_error -6, -1;

				if (params[1].type == tName)
				{
					file = fopen(params[1].name, "rt");
					if (file == NULL)
						return parser_error = -14, -1;
				}
			}

			if (file == stdin)
				printf("\nPodaj macierz %s: ", params[0].name);

			if (matrix_read(params[0].matrix, file, 0) < 0)
			{
				fclose(file);
				return -1;
			}

			if (file != stdin)
				fclose(file);
			else
				fflush(file);

			fputc('\n', stdout);
			break;

		case tVariable:
			if (count > 1)
			{
				if (count > 2)
					return parser_error -6, -1;

				if (params[1].type == tName)
				{
					file = fopen(params[1].name, "rt");
					if (file == NULL)
						return parser_error = -14, -1;
				}
			}

			if (file == stdin)
				printf("\nPodaj wartosc zmiennej %s: ", params[0].name);
	
			fscanf(file, "%Lg", params[0].variable);

			if (file != stdin)
				fclose(file);
			else 
			{
				fputc('\n', stdout);
				fflush(file);
			}

			break;

		default:
			return parser_error = -9, -1;
		}
	} else
		return parser_error = -7, -1;

	return 0;
}

int mwrite(PARAMETER *params, int count)
{
	FILE *file = stdout;

	if (count > 0)
	{
		switch (params[0].type)
		{
		case tMatrix:
			if (count > 1)
			{
				if (count > 2)
					return parser_error -6, -1;

				if (params[1].type == tName)
				{
					file = fopen(params[1].name, "wt");
					if (file == NULL)
						return parser_error = -12, -1;
				}
			}

			if (file == stdout)
				printf("\nMacierz %s:\n", params[0].name);

			if (matrix_write(params[0].matrix, file, 0) < 0)
			{
				fclose(file);
				return -1;
			}

			if (file != stdout)
				fclose(file);
			else
				fputs("\n\n", file);
			break;

		case tVariable:
			if (count > 1)
			{
				if (count > 2)
					return parser_error -6, -1;

				if (params[1].type == tName)
				{
					file = fopen(params[1].name, "wt");
					if (file == NULL)
						return parser_error = -15, -1;
				}
			}

			if (file == stdout)
				printf("\nWartosc zmiennej %s: ", params[0].name);
	
			fprintf(file, "%g", params[0].variable[0]);

			if (file != stdout)
				fclose(file);
			else
			{
				fputc('\n', stdout);
				fflush(file);
			}
			break;

		default:
			return parser_error = -9, -1;
		}
	} else
		return parser_error = -7, -1;

	return 0;
}

int mprint(PARAMETER *params, int count)
{
	MATRIX_DOUBLE precision = 4.0;

	if (count > 1)
	{
		if (count > 2)
			return parser_error = -6, -1;

		parser_value(&params[1], &precision);

		if (parser_error < 0)
			return -1;
	} else if (count < 1)
		return parser_error = -7, -1;

	switch (params[0].type)
	{
	case tMatrix:
		printf("\n%s=", params[0].name);

		matrix_print(params[0].matrix, (MATRIX_ULONG)precision);

		printf("\n\n");

		if (matrix_error < 0)
			return -1;
		break;

	case tVariable:
		printf("\n%s=\n\t", params[0].name);
		printf("% .*g\n\n", (MATRIX_ULONG)precision, params[0].variable[0]);
		break;

	case tValue:
		printf("\n\t% .*g\n\n", (MATRIX_ULONG)precision, params[0].value);
		break;

	default:
		return parser_error = -9, -1;
	}

	return parser_error = 0;
}

int minfo(PARAMETER *params, int count)
{
	if (count < 2)
	{
		switch (params[0].type)
		{
		case tMatrix:
			matrix_info(params[0].matrix);
			fputs("\n", stdout);
			break;

		case tVariable:
			printf("\nAdres zmiennej:   %p", params[0].variable);
			printf("\nZajetosc pamieci: %u bajtow\n\n", sizeof(params[0].variable[0]));
			break;

		case tValue:
			printf("\nZajetosc pamieci: %u bajtow\n\n", sizeof(params[0].value));
			break;

		default:
			return parser_error = -5, -1;
		}
	}
	else if (count < 1)
		return parser_error = - 7, -1;
	else
		return parser_error = - 6, -1;

	return parser_error = 0;
}

int mfill(PARAMETER *params, int count)
{
	if (count < 1)
		return parser_error = -7, -1;
	else
	{
		if (params[0].type == tMatrix)
		{
			MATRIX_DOUBLE value = 0.0;
			if (count > 1)
			{
				if (count > 2)
					return parser_error = -6, -1;

				parser_value(&params[1], &value);

				if (parser_error < 0)
					return -1;
			}

			if (params[0].type == tMatrix)
				matrix_fill(params[0].matrix, value);
		}
		else
			return parser_error = -10, -1;
	}

	return parser_error = 0;
}

int mcopy(PARAMETER *params, int count)
{
	if (count == 8)
	{
		if ((params[0].type == tMatrix) && (params[1].type == tMatrix))
		{
			if ((params[2].type != tMatrix) && (params[3].type != tMatrix) &&
				(params[4].type != tMatrix) && (params[5].type != tMatrix) &&
				(params[6].type != tMatrix) && (params[7].type != tMatrix))
			{
				MATRIX_DOUBLE destR, destC, srcR1, srcC1, srcR2, srcC2;

				parser_value(&params[2], &destR);
				parser_value(&params[3], &destC);
				parser_value(&params[4], &srcR1);
				parser_value(&params[5], &srcC1);
				parser_value(&params[6], &srcR2);
				parser_value(&params[7], &srcC2);

				if (parser_error < 0)
					return -1;

				if (matrix_copy(params[0].matrix, params[1].matrix, (MATRIX_ULONG)destR,
						(MATRIX_ULONG)destC, (MATRIX_ULONG)srcR1, (MATRIX_ULONG)srcC1,
						(MATRIX_ULONG)srcR2, (MATRIX_ULONG)srcC2) < 0)
					return -1;
			} else
				return parser_error = -13, -1;
		} else
			return parser_error = -10, -1;
	} else if (count == 2)
	{
		if ((params[0].type == tMatrix) && (params[1].type == tMatrix))
		{
			if (matrix_copy(params[0].matrix, params[1].matrix, 0, 0, 0, 0, 
					params[1].matrix->rows, params[1].matrix->columns) < 0)
				return -1;
		}
		else if ((params[0].type == tVariable) && (params[1].type == tVariable))
		{
			params[0].variable[0] = params[1].variable[0];
		} else
			return parser_error = -9, -1;
	} else
		return parser_error = -7, -1;

	return parser_error = 0;
}

int munit(PARAMETER *params, int count)
{
	if (count == 1)
	{
		switch (params[0].type)
		{
		case tMatrix:
			if (matrix_unit(params[0].matrix) < 0)
				return -1;
			break;

		case tVariable:
			params[0].variable[0] = 1.0;
			break;

		default:
			return parser_error = -9, -1;
		}
	} else if (count > 1)
		return parser_error = -6, -1;
	else
		return parser_error = -7, -1;

	return parser_error = 0;
}

int mdecomp(PARAMETER *params, int count)
{
	if (count == 3)
	{
		if ((params[0].type == tMatrix) && (params[1].type
				== tMatrix) && (params[2].type == tMatrix))
		{
			if (matrix_decomp(params[0].matrix,
					params[1].matrix, params[2].matrix) < 0)
				return -1;
		}
		else
			return parser_error = -10, -1;
	}
	else if (count < 3)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int mdet(PARAMETER *params, int count)
{
	if (count == 2)
	{
		if (params[0].type == tVariable)
		{
			if (params[1].type == tMatrix)
			{
				params[0].variable[0] = matrix_det(params[1].matrix);
				if (matrix_error < 0)
					return -1;
			} 
			else
				return parser_error = -10, -1;
		} 
		else
			return parser_error = -11, -1;
	} else if (count < 2)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int mtransp(PARAMETER *params, int count)
{
	if (count == 2)
	{
		if ((params[0].type == tMatrix) && (params[1].type = tMatrix))
		{
			if (matrix_transp(params[0].matrix, params[1].matrix) < 0)
				return -1;
		}
		else
			return parser_error = -10, -1;
	} else if (count < 2)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int mminor(PARAMETER *params, int count)
{
	if (count == 4)
	{
		if ((params[0].type == tMatrix) && (params[1].type == tMatrix))
		{
			if ((params[2].type != tMatrix) && (params[3].type != tMatrix))
			{
				MATRIX_DOUBLE r, c;

				parser_value(&params[2], &r);

				parser_value(&params[3], &c);

				if (matrix_minor(params[0].matrix, params[1].matrix, 
						(MATRIX_ULONG)r, (MATRIX_ULONG)c) < 0)
					return -1;
			} else
				return parser_error = -13, -1;
		} else
			return parser_error = -10, -1;
	} else if (count < 4)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int msup(PARAMETER *params, int count)
{
	if (count == 2)
	{
		if ((params[0].type == tMatrix) && (params[1].type = tMatrix))
		{
			if (matrix_suppl(params[0].matrix, params[1].matrix) < 0)
				return -1;
		}
		else
			return parser_error = -10, -1;
	} else if (count < 2)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int mattach(PARAMETER *params, int count)
{
	if (count == 2)
	{
		if ((params[0].type == tMatrix) && (params[1].type = tMatrix))
		{
			if (matrix_attach(params[0].matrix, params[1].matrix) < 0)
				return -1;
		}
		else
			return parser_error = -10, -1;
	} else if (count < 2)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int minv(PARAMETER *params, int count)
{
	if (count == 2)
	{
		if ((params[0].type == tMatrix) && (params[1].type = tMatrix))
		{
			if (matrix_inv(params[0].matrix, params[1].matrix) < 0)
				return -1;
		}
		else
			return parser_error = -10, -1;
	} else if (count < 2)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int madd(PARAMETER *params, int count)
{
	if (count == 3)
	{
		switch (params[0].type)
		{
		case tMatrix:
			if ((params[1].type == tMatrix) || (params[2].type == tMatrix))
			{
				if ((params[1].type != tMatrix) || (params[2].type != tMatrix))
				{
					MATRIX *m;
					MATRIX_DOUBLE v;
		
					if (params[1].type == tMatrix)
					{
						m = params[1].matrix;

						parser_value(&params[2], &v);

						if (parser_error < 0)
							return -1;
					}
					else
					{
						m = params[2].matrix;

						parser_value(&params[1], &v);

						if (parser_error < 0)
							return -1;
					}

					if (matrix_add_int(params[0].matrix, m, v) < 0)
						return -1;
				} else
				{
					if (matrix_add(params[0].matrix, params[1].matrix, params[2].matrix) < 0)
						return -1;
				}
			} else
				return parser_error = -10, -1;
			break;

		case tVariable:
			if ((params[1].type != tMatrix) && (params[2].type != tMatrix))
			{
				MATRIX_DOUBLE part1, part2;

				parser_value(&params[1], &part1);
				parser_value(&params[2], &part2);

				if (parser_error < 0)
					return -1;

				params[0].variable[0] = part1 + part2;
			} else
				return parser_error = -11, -1;
			break;

		default:
			return parser_error = -9, -1;
		}
	} else if (count < 3)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int msub(PARAMETER *params, int count)
{
	if (count == 3)
	{
		switch (params[0].type)
		{
		case tMatrix:
			if ((params[1].type == tMatrix) || (params[2].type == tMatrix))
			{
				if ((params[1].type != tMatrix) || (params[2].type != tMatrix))
				{
					MATRIX *m;
					MATRIX_DOUBLE v;
		
					if (params[1].type == tMatrix)
					{
						m = params[1].matrix;

						parser_value(&params[2], &v);

						if (parser_error < 0)
							return -1;
					}
					else
					{
						m = params[2].matrix;

						parser_value(&params[1], &v);

						if (parser_error < 0)
							return -1;
					}

					if (matrix_sub_int(params[0].matrix, m, v) < 0)
						return -1;
				} else
				{
					if (matrix_sub(params[0].matrix, params[1].matrix, params[2].matrix) < 0)
						return -1;
				}
			} else
				return parser_error = -10, -1;
			break;

		case tVariable:
			if ((params[1].type != tMatrix) && (params[2].type != tMatrix))
			{
				MATRIX_DOUBLE part1, part2;

				parser_value(&params[1], &part1);
				parser_value(&params[2], &part2);

				if (parser_error < 0)
					return -1;

				params[0].variable[0] = part1 - part2;
			} else
				return parser_error = -11, -1;
			break;

		default:
			return parser_error = -9, -1;
		}
	} else if (count < 3)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int mmul(PARAMETER *params, int count)
{
	if (count == 3)
	{
		switch (params[0].type)
		{
		case tMatrix:
			if ((params[1].type == tMatrix) || (params[2].type == tMatrix))
			{
				if ((params[1].type != tMatrix) || (params[2].type != tMatrix))
				{
					MATRIX *m;
					MATRIX_DOUBLE v;
		
					if (params[1].type == tMatrix)
					{
						m = params[1].matrix;

						parser_value(&params[2], &v);

						if (parser_error < 0)
							return -1;
					}
					else
					{
						m = params[2].matrix;

						parser_value(&params[1], &v);

						if (parser_error < 0)
							return -1;
					}

					if (matrix_mul_int(params[0].matrix, m, v) < 0)
						return -1;
				} else
				{
					if (matrix_mul(params[0].matrix, params[1].matrix, params[2].matrix) < 0)
						return -1;
				}
			} else
				return parser_error = -10, -1;
			break;

		case tVariable:
			if ((params[1].type != tMatrix) && (params[2].type != tMatrix))
			{
				MATRIX_DOUBLE part1, part2;

				parser_value(&params[1], &part1);
				parser_value(&params[2], &part2);

				if (parser_error < 0)
					return -1;

				params[0].variable[0] = part1 * part2;
			} else
				return parser_error = -11, -1;
			break;

		default:
			return parser_error = -9, -1;
		}
	} else if (count < 3)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int mdiv(PARAMETER *params, int count)
{
	if (count == 3)
	{
		switch (params[0].type)
		{
		case tMatrix:
			if ((params[1].type == tMatrix) || (params[2].type == tMatrix))
			{
				if ((params[1].type != tMatrix) || (params[2].type != tMatrix))
				{
					MATRIX *m;
					MATRIX_DOUBLE v;
		
					if (params[1].type == tMatrix)
					{
						m = params[1].matrix;

						parser_value(&params[2], &v);

						if (parser_error < 0)
							return -1;
					}
					else
					{
						m = params[2].matrix;

						parser_value(&params[1], &v);

						if (parser_error < 0)
							return -1;
					}

					if (matrix_div_int(params[0].matrix, m, v) < 0)
						return -1;
				} else
				{
					if (matrix_div(params[0].matrix, params[1].matrix, params[2].matrix) < 0)
						return -1;
				}
			} else
				return parser_error = -10, -1;
			break;

		case tVariable:
			if ((params[1].type != tMatrix) && (params[2].type != tMatrix))
			{
				MATRIX_DOUBLE part1, part2;

				parser_value(&params[1], &part1);
				parser_value(&params[2], &part2);

				if (parser_error < 0)
					return -1;

				if (part2 == 0)
					return parser_error = -12, -1;

				params[0].variable[0] = part1 / part2;
			} else
				return parser_error = -11, -1;
			break;

		default:
			return parser_error = -9, -1;
		}
	} else if (count < 3)
		return parser_error = -7, -1;
	else
		return parser_error = -6, -1;

	return parser_error = 0;
}

int mhelp(PARAMETER *params, int count)
{
	int c;
	FILE *file;
	char name[64] = { "help\\help" };

	switch (params[0].type)
	{
	case tName:
		strncpy(name, "help\\", 63);
		strncat(name, params[0].name, 58);

	default:
		file = fopen(name, "rt");
		if (file != NULL)
		{
			while ((c = fgetc(file)) != EOF)
				fputc(c, stdout);
			fclose(file);
		} else
		{
			printf("\nBrak pliku pomocy %s!\n", params[0].name);
		}
		fputc('\n', stdout);
		break;
	}

	return 0;
}

#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>

struct colourchart
{
	char colour[3];
	int sig;
	float tol;
	int temp;
};

int main()
{
	char buffer[32];
	float tol = 20.0;
	int i, j, res = 0, mlp = 0, temp = 0, map[] = {1, 2, 2, 3};
	struct colourchart code[] = {
		{"BK", 0, 0, 250},
		{"BN", 1, 1.0, 100},
		{"RD", 2, 2.0, 50},
		{"OG", 3, 0.05, 15},
		{"YE", 4, 0.02, 25},
		{"GN", 5, 0.5, 20},
		{"BU", 6, 0.25, 10},
		{"VT", 7, 0.1, 5},
		{"GY", 8, 0.01, 1},
		{"WH", 9},
		{"GD", -1, 5.0},
		{"SR", -2, 10.0},
		{"PK", -3},
	};

	//ENABLE_VIRTUAL_TERMINAL_PROCESSING
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= 0x0004;
	SetConsoleMode(hOut, dwMode);

	printf("Colour Coding:-\n");
	printf("\n\x1b[38;5;0m\x1b[48;5;15mBK = Black\x1b[0m\x1b[38;5;94m\tBN = Brown\x1b[38;5;160m\tRD = Red\x1b[38;5;202m\tOG = Orange\x1b[38;5;226m\tYE = Yellow\x1b[38;5;40m\tGN = Green\n");
	printf("\x1b[38;5;39mBU = Blue\x1b[38;5;56m\tVT= Violate\x1b[38;5;244m\tGY = Gray\x1b[38;5;15m\tWH = White\x1b[38;5;214m\tGD = Gold\x1b[38;5;250m\tSR = Silver\x1b[38;5;201m\tPK = Pink\x1b[0m\n");

	printf("\nEnter the Color Coding on Resistor[Separate by space(Case Insensitive)]: ");
	scanf("%[^\n]s", buffer);

	const int size = (strlen(buffer) + 1) / 3;
	//Checking less or more character
	if (size < 3 || size > 6)
	{
		printf("\nInvalid Band Number. The Commercial Resistor has 3-6 Bands\n");
		return 0;
	}
	char colours[size][3];

	//Modifying the buffer array
	for (i = 0; i < strlen(buffer); i++)
	{
		//Uppercasing
		if (buffer[i] > 96)
			buffer[i] -= 32;
		//Cheacking invalid character
		if (!((buffer[i] > 64 && buffer[i] < 91) || buffer[i] == ' ' || buffer[i] == '\0'))
		{
			printf("\nInvalid Character. You entered other then alphabetical character\n");
			return 0;
		}
		//Shifting buffer array to left if multiple space detected
		if ((buffer[i] == buffer[i + 1]) || buffer[0] == ' ')
		{
			while (buffer[i] != '\0')
			{
				buffer[i] = buffer[i + 1];
				i++;
			}
			i = 0;
		}
		//Copy the buffer into the colours array as a 1d array
		colours[i / 3][i % 3] = buffer[i];
		//Modifying colours array for access as a 2d array
		colours[i / 3][2] = '\0';
	}

	//Calculating the resistance value
	for (i = 0, j = 0; i < size; i++, j = 0)
	{
		while (1)
		{
			if (j > 12)
			{
				printf("\nInvaid Colour Code. You entered other Code than given above\n");
				return 0;
			}
			if (strcmp(colours[i], code[j].colour) == 0)
			{
				if (i < size - map[size - 3])
					res = res * 10 + code[j].sig;
				else if (i < size - map[size - 3] + 1)
					mlp = code[j].sig;
				else if (i < size - map[size - 3] + 2)
					tol = code[j].tol;
				else
					temp = code[j].temp;
				break;
			}
			j++;
		}
	}

	//Resistance Value as Output 
	_setmode(_fileno(stdout), _O_U16TEXT);
	wprintf(L"\nThe Resistance Value R = %d X 10^%d \x2126 \x00B1%.2f %d ppm/K\n", res, mlp, tol, temp);
	_setmode(_fileno(stdout), _O_TEXT);

	getch();
	return 0;
}
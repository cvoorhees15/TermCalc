#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

int update_history(int last_result)
{
	// allocate space for last_result string conversion
	char *result_space = malloc(sizeof(int)); 

	if (result_space == NULL)
	{
		printf("\nError allocating memory\n");
		return 1;
	}
	
	// convert calculation result to string
	int conversion_val = snprintf(result_space, sizeof(result_space), "%d", last_result);

	if (conversion_val < 0 || conversion_val > sizeof(result_space))
	{
		printf("\nError converting last result to string\n");
		return 1;
	}	

	// open history file
	FILE *fptr = fopen("history.txt", "w");
	
	if (fptr == NULL)
	{
		printf("\nError opening history file\n");
		return 1;
	}
	
	// overwrite history file with the latest result
	if (fprintf(fptr, "%s", result_space) < 0)
	{
		printf("\nError writing to history file\n");
		return 1;
	}

	// close history file
	if (fclose(fptr) != 0)
	{
		printf("\nError closing history file\n");
		return 1;
	}	

	free(result_space);

	// success
	return 0;
}

int read_history(int *history)
{
	// open history file
	FILE *fptr = fopen("history.txt", "r");	

	if (fptr == NULL)
	{
		printf("\nError opening history file\n");
		return 1;
	}

	// allocate space for value stored in history file
	char* history_val = malloc(sizeof(int));

	if (history_val == NULL)
	{
		printf("\nError allocating memory\n");
	}

	// store value from history file in a string
	fgets(history_val, sizeof(int), fptr);
	
	// close history file
	if (fclose(fptr) != 0)
	{
		printf("\nError closing history file\n");
	
		return 1;
	}

	*history = atoi(history_val);
	
	free(history_val);

	// success
	return (0);
}

int safe_calculate(int first_num, char operation, int second_num, int *result)
{
	switch (operation)
	{
		case '+':
			*result = first_num + second_num;
			// addition bounds check
			if ((first_num > 0 && second_num > 0 && *result < 0) || (first_num < 0 && second_num < 0 && *result > 0))
			{
				printf("\nError: result out of bounds\n");
				return 1;
			}
			break;
		case '-':
			*result = first_num - second_num;
			// subtraction bounds check
			if ((second_num > 0 && first_num < INT_MIN + second_num) || (second_num < 0 && first_num > INT_MAX - second_num))
			{
				printf("\nError: result out of bounds\n");
				return 1;
			}
			break;
		case 'x':
			*result = first_num * second_num;
			// multiplication bounds check
			if (first_num > INT_MAX / second_num || second_num > INT_MAX / first_num)
			{
				printf("\nError: result out of bounds\n");
				return 1;
			}
			break;
		case '/':
			// check for divion by zero before the operation
			if (second_num == 0)
			{
				printf("\nError: cannot divide by zero\n");
				return 1;	
			}
			*result = first_num / second_num;
			break;
		default:
			printf("\nError: invalid operation\n");
			return 1;
	}

	// success
	return 0;
}

int main(int argc, char *argv[])
{
	
	int total = 0; 
	
	if (argc == 2)
	{
		if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0)
		{
			printf("\nTermCalc Help Page\n---------------------------------------------------------------\n\nTermCalc supports any basic arithmetic operation of two integers\n\n---------------------\nAddition: +\nSubtraction: -\nMultiplication: x\nDivision: /\n---------------------\n\nFormat: calc {number} {operation} {number}\n\nEx: calc 2 + 2\n\n\nUse the result of your previous calculation with the \"last\" argument\n\nEx: calc last x 10\n---------------------------------------------------------------\n\n");

			// success
			return 0;
		}
		else
		{
			printf("\nUse standard format: \"calc {number} {operation} {number}\"\n\n");
			return 1;	
		}
	}
	else if (argc == 4)
	{
		if (strcmp(argv[1], "last") == 0)
		{
			// grab last calc result from history file
			int history_val = 0;
			read_history(&history_val);

			// run calculation using the last result (from history file)
			if (safe_calculate(history_val, argv[2][0], atoi(argv[3]), &total) != 0)
				return 1;
		}
		else if (strcmp(argv[3], "last") == 0)
		{
			// grab last calc result from history file
			int history_val = 0;
			read_history(&history_val);

			// run calculation using the last result (from history file)
			if (safe_calculate(atoi(argv[1]), argv[2][0], history_val, &total) != 0)
				return 1;
		}

		else
		{
			// run calculation using normal arguments
			if (safe_calculate(atoi(argv[1]), argv[2][0], atoi(argv[3]), &total) != 0)
				return 1;
		}

		update_history(total);

		printf("\nTotal is: %d", total);

		printf("\nResult is stored at this address: %p", &total);

		printf("\nResult is %zu bytes in size", sizeof(total));

		// success
		return 0;
	}
	else
	{
		printf("\nUse standard format: \"calc {number} {operation} {number}\"\n\n");
		return 1;	
	}
}

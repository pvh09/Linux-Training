#include "file_handling.h"

#include <stdio.h>
#include <stdlib.h>

double validate_num_input(bool flag_check_data)
{
    char input_string[20] = {0};
    double input = 0;
    char *p_end = NULL;
    long long int_part = 0;
    while (1)
    {
        __fpurge(stdin);
        scanf("%19s", input_string);

        if (flag_check_data == 1)
        {
            // Allow only integers
            int_part = strtoll(input_string, &p_end, 10);
            if (p_end != input_string && *p_end == '\0' && int_part >= 0)
            {
                input = int_part;
                return input;
            }
            printf("\nWARNING: Only integer input is allowed. Please enter again: ");
        }
        else if (flag_check_data == 0)
        {
            // Allow integers and doubles
            double numeric_part = strtod(input_string, &p_end);
            if (p_end != input_string && *p_end == '\0')
            {
                input = numeric_part;
                return input;
            }
            printf("\nWARNING: Only numeric input is allowed. Please enter again: ");
        }
        else
        {
            // Invalid flag_check_data value
            printf("\nERROR: Invalid flag_check_data value. Please use 0 or 1.\n");
            return input;
        }
    }
    return input;
}

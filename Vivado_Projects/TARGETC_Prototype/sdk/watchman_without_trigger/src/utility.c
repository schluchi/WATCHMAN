/*
 * utility.c
 *
 *  Created on: 24 oct. 2018
 *      Author: Anthony
 */
#include "utility.h"

/****************************************************************************/
/**
* @brief	Function to print a decimal value in hexadecimal
*
* @param	n: decimal value to print
*
* @return	None
*
* @note		-
*
****************************************************************************/
void decToHexa(int n)
{
    // char array to store hexadecimal number
    char hexaDeciNum[100];

    // counter for hexadecimal number array
    int i = 0;
    while(n!=0)
    {
        // temporary variable to store remainder
        int temp  = 0;

        // storing remainder in temp variable.
        temp = n % 16;

        // check if temp < 10
        if(temp < 10)
        {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else
        {
            hexaDeciNum[i] = temp + 55;
            i++;
        }

        n = n/16;
    }

    // printing hexadecimal number array in reverse order
    for(int j=i-1; j>=0; j--)
        xil_printf("%c",hexaDeciNum[j]);


}

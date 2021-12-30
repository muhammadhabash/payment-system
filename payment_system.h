/**************************************************************/
/*                 Author    : Muhammad Ali Habash            */
/*                 Version   : 1.0                            */
/*                 Date      : 12/27/2021                     */
/*                 Last Edit : N/A                            */
/**************************************************************/

#ifndef __PAYMENT_SYSTEM_H__
#define __PAYMENT_SYSTEM_H__

/***************************************************/
/*                Files Inclusions                 */
/***************************************************/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "STD_types.h"

/***************************************************/
/*                End Of Section                   */
/***************************************************/


/***************************************************/
/*                Constants                        */
/***************************************************/

const float gf_maxAmount = 5000.0;

/***************************************************/
/*                End Of Section                   */
/***************************************************/

/***************************************************/
/*                Enums Definitions                */
/***************************************************/

typedef enum
{
	DECLINED,
	APPROVED
}EN_transStat_t;

/***************************************************/
/*                End Of Section                   */
/***************************************************/


/***************************************************/
/*                Structs Definitions              */
/***************************************************/

typedef struct ST_cardData_t
{
	uint8_t cardHolderName[25];
	uint8_t primaryAccountNumber[20];
	uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef struct ST_terminalData_t
{
	float transmitAmount;
	float maxTransAmount;
	uint8_t transactionDate[11];
}ST_terminalData_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t transData;
	EN_transStat_t transStat;
}ST_transaction_t;

typedef struct ST_accountBalance_t
{
	float balance;
	uint8_t primaryAccountNumber[20];
}ST_accountBalance_t;

/***************************************************/
/*                End Of Section                   */
/***************************************************/


/***************************************************/
/*                Functions Prototypes             */
/***************************************************/


uint8_t is_literals(uint8_t* au8_line);
uint8_t is_digit(uint8_t* au8_line);
uint8_t expiry_date_valid(uint8_t* au8_line);
uint8_t transaction_date_valid(uint8_t* au8_line);
uint8_t panFound(uint8_t* au8_line);
uint8_t notExpired (uint8_t* expiryDate, uint8_t* transactionDate);
uint8_t transactionAmountValid (float amount, float maxAmount, float balance);
uint8_t transaction (void);
int8_t  proceed(void);
void    readInput (uint8_t* au8_line);


/***************************************************/
/*                End Of Section                   */
/***************************************************/
#endif
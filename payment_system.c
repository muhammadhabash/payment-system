/**************************************************************/
/*                 Author    : Muhammad Ali Habash            */
/*                 Version   : 1.0                            */
/*                 Date      : 12/27/2021                     */
/*                 Last Edit : N/A                            */
/**************************************************************/


/***************************************************/
/*                Files Inclusions                 */
/***************************************************/

#include "payment_system.h"

/***************************************************/
/*                End Of Section                   */
/***************************************************/


/***************************************************/
/*                Macros                           */
/***************************************************/
 
#define TRUE         1
#define FALSE        0
#define FOUND        1
#define NOT_FOUND    0
#define VALID        1
#define NOT_VALID    0
#define NOT_EXPIRED  1
#define EXPIRED      0
#define SUCCESS      1
#define NOT_SUCCESS  0

/***************************************************/
/*                End Of Section                   */
/***************************************************/


/***************************************************/
/*                Global Variables                 */
/***************************************************/

float   gf_cardBalance;
uint8_t gu8_clientIndex;
float   gf_amount;
int8_t  gu8_transStat;
uint8_t gu8_endProgram;

/***************************************************/
/*                End Of Section                   */
/***************************************************/


/***************************************************/
/*                Global array of structs          */
/***************************************************/

ST_accountBalance_t availableAccounts[10] = {
	                                          {100.00 , "123456789 " },
											  {6000.00, "234567891 " },
											  {3250.25, "567891234 " },
											  {1500.12, "456789123 " },
											  {500.00 , "258649173 " },
											  {2100.00, "654823719 " },
											  {0.00   , "971362485 " },
											  {1.00   , "793148625 " },
											  {10.12  , "123123456 " },
											  {0.55   , "456789321 " }
                                            };

/***************************************************/
/*                End Of Section                   */
/***************************************************/

uint32_t extract_numbers (uint8_t* u8_Line)
{
	uint8_t u8_Index, u8_Number, u8_Weight;
	uint32_t u32_Sum = 0;
	u8_Weight        = strlen(u8_Line) - 2;
	for(u8_Index = 0 ; u8_Index < strlen(u8_Line) - 1 ; u8_Index++)
	{
	    u8_Number = ( (u8_Line[u8_Index] - 48 ) );
	    u32_Sum += ( u8_Number * pow(10, u8_Weight) ); 
		u8_Weight --;
    }
	return u32_Sum;
}

uint8_t is_literals(uint8_t* au8_line)
{
	uint8_t u8_Index, u8_Index2;
	for ( u8_Index = 0 ; u8_Index < strlen(au8_line) - 1 ; u8_Index++ )
	{
		if ( au8_line[u8_Index] == ' ')
		{
			u8_Index++;
		    if ( !(au8_line[u8_Index] >= 'A' && au8_line[u8_Index] <= 'z'))
		    {
			    return FALSE;
		    }
		}
		else if (!(au8_line[u8_Index] >= 'A' && au8_line[u8_Index] <= 'z'))
		{
			return FALSE;
		}
	}
	return TRUE;
}

uint8_t is_digit(uint8_t* au8_line)
{
	uint8_t u8_Index, u8_Index2;
	for ( u8_Index = 0 ; u8_Index < strlen(au8_line) - 1 ; u8_Index++ )
	{
		if (!(au8_line[u8_Index] >= '0' && au8_line[u8_Index] <= '9'))
		{
			return FALSE;
		}
	}
	return TRUE;
}

uint8_t expiry_date_valid(uint8_t* au8_line)
{
	if ( ( au8_line[0] == '0') && (au8_line[1] >= '1' && au8_line[1] <='9') )
	{
	    if ( ( au8_line[2] == '/') && ( au8_line[3] >= '0' && au8_line[3] <= '9' ) && ( au8_line[4] >= '1' && au8_line[4] <= '9' ) )
	    {
		    return TRUE;
	    }
		else 
			return FALSE;
	}
	else if ( ( au8_line[0] == '1' ) && (au8_line[1] >= '0' && au8_line[1] <='2') )
	{
		if ( ( au8_line[2] == '/') && ( au8_line[3] >= '0' && au8_line[3] <= '9' ) && ( au8_line[4] >= '1' && au8_line[4] <= '9' ) )
		{	
		    return TRUE;
		}
		else
			return FALSE;
	}
	else 
		return FALSE;
}

uint8_t transaction_date_valid(uint8_t* au8_line)
{
	if ( (au8_line[0] >= '0' && au8_line[0] <= '2') && (au8_line[1] >= '1' && au8_line[1] <= '9') )
	{
	    if ( au8_line[2] == '/' )
		{
			if ( ( au8_line[3] == '0' ) && ( au8_line[4] >= '1' && au8_line[4] <= '9' ) )
			{
				if ( au8_line[5] == '/')
				{
					if ( ( au8_line[6] >= '2' && au8_line[6] <= '9' ) && ( au8_line[7] >= '0' && au8_line[7] <= '9' ) && ( au8_line[8] >= '0' && au8_line[8] <= '9' ) && ( au8_line[9] >= '0' && au8_line[9] <= '9' ) )
					{
						return TRUE;
					}
					else 
						return FALSE;
				}
				else 
					return FALSE;
			}
			else if ( ( au8_line[3] == '1' ) && ( au8_line[4] >= '0' && au8_line[4] <= '2' ) )
			{
				if ( au8_line[5] == '/')
				{
				    if ( ( au8_line[6] >= '0' && au8_line[6] <= '9' ) && ( au8_line[7] >= '0' && au8_line[7] <= '9' ) && ( au8_line[8] >= '0' && au8_line[8] <= '9' ) && ( au8_line[9] >= '0' && au8_line[9] <= '9' ) )
					{
					    return TRUE;
					}
					else 
					{
						return FALSE;
					}
			    }
                else				
			        return FALSE;
		    }
		    else 
			    return FALSE;	
	    }
	}
    else if ( au8_line[0] == '3' && (au8_line[1] >= '0' && au8_line[1] <= '1' ) )
	{
		 if ( au8_line[2] == '/' )
		 {
		     if ( ( au8_line[3] == '0' ) && ( au8_line[4] >= '1' && au8_line[4] <= '9' ) )
			 {
				  if ( au8_line[5] == '/')
				  {
				      if ( ( au8_line[6] >= '2' && au8_line[6] <= '9' ) && ( au8_line[7] >= '0' && au8_line[7] <= '9' ) && ( au8_line[8] >= '0' && au8_line[8] <= '9' ) && ( au8_line[9] >= '0' && au8_line[9] <= '9' ) )
					  {
					      return TRUE;
					  }
					  else 
						  return FALSE;
				  }
				  else 
					  return FALSE;
			 }
			 else if ( ( au8_line[3] == '1' ) && ( au8_line[4] >= '0' && au8_line[4] <= '2' ) )
			 {
				 if ( au8_line[5] == '/')
				  {
				      if ( ( au8_line[6] >= '2' && au8_line[6] <= '9' ) && ( au8_line[7] >= '0' && au8_line[7] <= '9' ) && ( au8_line[8] >= '0' && au8_line[8] <= '9' ) && ( au8_line[9] >= '0' && au8_line[9] <= '9' ) )
					  {
					      return TRUE;
					  }
					  else 
						  return FALSE;
				  }
				  else 
					  return FALSE; 
			 }
			 else
				 return FALSE;
		 }
		 else 
			 return FALSE;
	}
	else
        return FALSE;				
}

uint8_t panFound(uint8_t* au8_line)
{
	uint8_t u8_accountIndex, u8_letterIndex, u8_match;
	uint32_t no_1, no_2;
	no_1 = extract_numbers( au8_line );
	for ( u8_accountIndex = 0 ; u8_accountIndex < 10 ; u8_accountIndex ++ )
	{
        no_2 = extract_numbers ( availableAccounts[u8_accountIndex].primaryAccountNumber );
		if ( no_1 == no_2  )
		{
			u8_match = 1;
		}
		else 
		{
            u8_match = 0;			
		}
		if ( u8_match ) 
		{
			gf_cardBalance = availableAccounts[u8_accountIndex].balance;
			gu8_clientIndex = u8_accountIndex;
			printf("\nBalance = %f\n", gf_cardBalance);
			return FOUND;
		}
	}
	return NOT_FOUND;
}

uint8_t transactionAmountValid (float amount, float maxAmount, float balance)
{
	if (amount <= maxAmount)
	{
		if (amount <= balance)
		{
			return VALID;
		}
		else 
			return NOT_VALID;
	}
	else
		return NOT_VALID; 
}

uint8_t notExpired (uint8_t* expiryDate, uint8_t* transactionDate)
{
	if ( expiryDate[3] < transactionDate[8] )
	{
		return EXPIRED;
	}
	else if ( expiryDate[3] == transactionDate[8] )
	{
		if (expiryDate[4] < transactionDate[9])
		{
			return EXPIRED;
		}
		else if ( expiryDate[0] < transactionDate[3] )
		{
			return EXPIRED;
		}
        else
		{
			if ( expiryDate[1] < transactionDate[4] )
			{
				return EXPIRED;
			}
			else
                return NOT_EXPIRED;				
		}			
	}
	else 
		return NOT_EXPIRED;
}

void readInput (uint8_t* au8_line)
{
	fflush(stdin);
	fgets(au8_line, 100 , stdin);
}

uint8_t transaction (void)
{
	uint8_t u8_noChoice = TRUE;
	ST_transaction_t newTransaction;
	
	printf("Please Enter Card Data:");
	while (u8_noChoice)
	{
		uint8_t au8_line[25];
	    printf("\nPlease Enter the Card Holder Name:\n");
		readInput(au8_line);
		if (is_literals(au8_line))
		{
			u8_noChoice = FALSE;
			strcpy(newTransaction.cardHolderData.cardHolderName, au8_line);
		}
		else 
		{
			printf("\nWrong Input!!\n");
		}
	}
	
	u8_noChoice = TRUE;
	while(u8_noChoice)
	{
		uint8_t au8_line[25], return_t;
		printf("\nPlease Enter the Primary Account Number:\n");
		readInput(au8_line);
		if (is_digit(au8_line))
		{
			if(panFound(au8_line))
			{ 
			    u8_noChoice = FALSE;
			    strcpy(newTransaction.cardHolderData.primaryAccountNumber , au8_line);
			}
			else 
			{
				newTransaction.transStat = DECLINED;
				return NOT_SUCCESS;
			}
				
		}
		else 
		{
			printf("\nWrong Input!!\n");
		}
	}
	
	u8_noChoice = TRUE;
	while(u8_noChoice)
	{
		uint8_t au8_line[6];
		printf("\nPlease Enter card Expiry Date:\n");
		readInput(au8_line);
		if (expiry_date_valid(au8_line))
		{
			u8_noChoice = FALSE;
			strcpy(newTransaction.cardHolderData.cardExpirationDate, au8_line);
		}
		else 
		{
			printf("\nWrong Input!!\n");
		}
	}
	
	u8_noChoice = TRUE;
	printf("\nPlease Enter Terminal Data:\n");
	
	while (u8_noChoice)
	{
		u8_noChoice = FALSE;
		printf("\nPlese Enter The Transaction Amount:\n");
		scanf("%f", &gf_amount);
	}

	if (transactionAmountValid(gf_amount, gf_maxAmount, gf_cardBalance))
	{
	    u8_noChoice = TRUE;
	    while (u8_noChoice)
	    {
		    uint8_t au8_line[15];
			printf("\nPlease Enter Transaction Date:\n");
		    readInput(au8_line);
			if (transaction_date_valid(au8_line))
		    {
			    u8_noChoice = FALSE;
			    strcpy(newTransaction.transData.transactionDate, au8_line);
		    }
			else 
			{
				printf("\nWrong Input!!\n");
			}
	    }
	}
	else 
	{
		newTransaction.transStat = DECLINED;
		return NOT_SUCCESS;
	}
	
	if (notExpired(newTransaction.cardHolderData.cardExpirationDate, newTransaction.transData.transactionDate))
	{
		newTransaction.transStat = APPROVED;
		return SUCCESS;
	}
	else 
	{
		newTransaction.transStat = DECLINED;
		return NOT_SUCCESS;
	}	
}

int8_t proceed(void)
{
	uint8_t u8_line[3];
	uint8_t u8_noChoice = TRUE;
	while (u8_noChoice)
	{
	    printf("\nDo you want to continue (y/n)?:\n");
	    readInput(u8_line);
	    if ( strcmp(u8_line,"y") == 1 )
	    {
			u8_noChoice = FALSE;
		    gu8_transStat = transaction();
			return 0;
	    }
	    else if ( strcmp(u8_line,"n") == 1 )
	    {
			u8_noChoice = FALSE;
		    printf("\nHave a nice day!");
			gu8_endProgram = TRUE;
	    }
	    else 
	    {
		    printf("\nWrong Input!!\n");
	    }
	}
}

uint8_t main()
{
	uint8_t u8_line[2];
	gu8_transStat = transaction();
	while ( TRUE )
	{
	    if (gu8_transStat == SUCCESS)
	    {
		    printf("\nThe transaction is APPROVED\n");
		    availableAccounts[gu8_clientIndex].balance -= gf_amount;
		    proceed();
	    }
	    else 
	    {
		    printf("\nThe transaction is DECLINED\n");
		    proceed();
			if (gu8_endProgram)
			{
				break;
			}
	    }
	}
}
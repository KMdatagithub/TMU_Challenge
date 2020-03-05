#include "BCM_TESTER.h"
#include #include"../SL/BCM.h"

void Test_BCM_DeInit(void)
{
    printf("\n\nTesting BCM_DeInit Function\n");
    printf("_____________\n\n");

    /************************************************************************************
    * Target Function: BCM_DeInit
    * Test Case: DeInit Without Init
    * Expected OUTPUT: BCM_ERROR + NOT_INIT
    ************************************************************************************/

    ERROR_STATUS ret_error_code, expected_error_code;

    expected_error_code = BCM_ERROR + NOT_INIT;

    BCM_cfg_s BCM1;

    ret_error_code = BCM_DeInit(&BCM1);

    if(ret_error_code == expected_error_code)
        printf("Test Case1: Test Passed\n");
    else
        printf("Test Case1: Test Failed\n");

    /************************************************************************************
    * Target Function: BCM_DeInit
    * Test Case: Null Pointer Input Structure
    * Expected OUTPUT: BCM_ERROR + NULL_PTR
    ************************************************************************************/

    expected_error_code = BCM_ERROR + NULL_PTR;

    ret_error_code = BCM_DeInit(NULL);

    if(ret_error_code == expected_error_code)
        printf("Test Case2: Test Passed\n");
    else
        printf("Test Case2: Test Failed\n");
}

void Notify(enum_BcmStatus a){}
void Test_Setup_RxBuffer(void)
{

    printf("\n\nTesting BCM_Setup_RxBuffer Function\n");
    printf("_____________\n\n");

    /************************************************************************************
    * Target Function: BCM_Setup_RxBuffer
    * Test Case: Null Pointer Input Structure
    * Expected OUTPUT: BCM_ERROR + NULL_PTR
    ************************************************************************************/

    ERROR_STATUS ret_error_code, expected_error_code;
    uint16_t a_Buffer_Len;
    uint8_t* buf;
    BCM_cfg_s BCM1;

    expected_error_code = BCM_ERROR + NULL_PTR;

    ret_error_code = BCM_Setup_RxBuffer(NULL, a_Buffer_Len, buf, Notify);

    if(ret_error_code == expected_error_code)
        printf("Test Case1: Test Passed\n");
    else
        printf("Test Case1: Test Failed\n");

    /************************************************************************************
    * Target Function: BCM_Setup_RxBuffer
    * Test Case: Null Pointer Input Structure
    * Expected OUTPUT: BCM_ERROR + NULL_PTR
    ************************************************************************************/

    BCM1.BCM_CH_ID = 1;
    BCM1.Mode = BCM_Tx_Mode;
    BCM1.Protocol = SPI_Protocol;

    ret_error_code = BCM_Setup_RxBuffer(&BCM1, a_Buffer_Len, NULL, Notify);

    if(ret_error_code == expected_error_code)
        printf("Test Case2: Test Passed\n");
    else
        printf("Test Case2: Test Failed\n");

    /************************************************************************************
    * Target Function: BCM_Setup_RxBuffer
    * Test Case: Null Pointer To Notify Function
    * Expected OUTPUT: BCM_ERROR + NULL_PTR
    ************************************************************************************/

    BCM1.BCM_CH_ID = 1;
    BCM1.Mode = BCM_Rx_Mode;
    BCM1.Protocol = SPI_Protocol;
    BCM_Init(&BCM1);

    ret_error_code = BCM_Setup_RxBuffer(&BCM1, a_Buffer_Len, buf, NULL);

    if(ret_error_code == expected_error_code)
        printf("Test Case3: Test Passed\n");
    else
        printf("Test Case3: Test Failed\n");

    /************************************************************************************
    * Target Function: BCM_Setup_RxBuffer
    * Test Case: Invalid Input Parameters
    * Expected OUTPUT: BCM_ERROR + INVALID_IN
    ************************************************************************************/

    BCM1.BCM_CH_ID = 1;
    BCM1.Mode = 5;   /* InValid Mode Number */
    BCM1.Protocol = SPI_Protocol;

    expected_error_code = BCM_ERROR + INVALID_IN;

    ret_error_code = BCM_Setup_RxBuffer(&BCM1, a_Buffer_Len, buf, Notify);

    if(ret_error_code == expected_error_code)
        printf("Test Case4: Test Passed\n");
    else
        printf("Test Case4: Test Failed\n");

    /************************************************************************************
    * Target Function: BCM_Setup_RxBuffer
    * Test Case: Trying To Setup_RxBuffer While BCM In Transmitter Mode
    * Expected OUTPUT: BCM_ERROR + FUCN_IN_DIF_CFG
    ************************************************************************************/

    BCM_DeInit(&BCM1);
    BCM1.BCM_CH_ID = 1;
    BCM1.Mode = BCM_Tx_Mode;
    BCM1.Protocol = SPI_Protocol;
    BCM_Init(&BCM1);

    expected_error_code = BCM_ERROR + FUCN_IN_DIF_CFG;

    ret_error_code = BCM_Setup_RxBuffer(&BCM1, a_Buffer_Len, buf, Notify);

    if(ret_error_code == expected_error_code)
        printf("Test Case5: Test Passed\n");
    else
        printf("Test Case5: Test Failed\n");

}





void test_function(enum_BcmStatus x){}


void Test_BCM_init(void)
{

    sint16_t state_error = E_OK ;

    printf("Testing BCM Init Function\n");
    printf("_____________\n");

    /************************************************************************************
    * Target Function: BCM Init Function
    * Test Case: test null pointer.
    * Expected OUTPUT: TNULL_PTR + BCM_ERROR
    ************************************************************************************/

    BCM_cfg_s * a_ptr_BCM1;
    a_ptr_BCM1 = NULL ;

    state_error = BCM_Init(a_ptr_BCM1) ;

    printf("Test case 1 : ");

    if(state_error == NULL_PTR + BCM_ERROR)
        printf("passed \n");
    else
        printf("failed \n");



        BCM_cfg_s a_BCM1;

    /************************************************************************************
    * Target Function: BCM Init Function
    * Test Case: valid parameters .
    * Expected OUTPUT: E_OK + BCM_ERROR
    ************************************************************************************/


    a_BCM1.BCM_CH_ID = 1 ;
    a_BCM1.Mode = BCM_Rx_Mode ;
    a_BCM1.Protocol = SPI_Protocol ;

    state_error = BCM_Init(&a_BCM1) ;

    printf("Test case 2 : ");

    if(state_error == BCM_ERROR + E_OK)
        printf("passed \n");
    else
        printf("failed \n");



    /************************************************************************************
    * Target Function: BCM Init Function
    * Test Case: re-init .
    * Expected OUTPUT: BCM_ERROR + ALREADY_INIT
    ************************************************************************************/


    a_BCM1.BCM_CH_ID = 3 ;
    a_BCM1.Mode = BCM_Rx_Mode ;
    a_BCM1.Protocol = SPI_Protocol ;

    state_error = BCM_Init(&a_BCM1) ;

    printf("Test case 3 : ");

    if(state_error == BCM_ERROR + ALREADY_INIT)
        printf("passed \n");
    else
        printf("failed \n");

    BCM_DeInit(&a_BCM1);
    /************************************************************************************
    * Target Function: BCM Init Function
    * Test Case: INVALID parameters(invalid mode) .
    * Expected OUTPUT: TNULL_PTR + BCM_ERROR
    ************************************************************************************/

    a_BCM1.BCM_CH_ID = 4 ;
    a_BCM1.Mode = INVALID_PARAMETER ;
    a_BCM1.Protocol = SPI_Protocol;

    state_error = BCM_Init(&a_BCM1) ;

    printf("Test case 4 : ");

    if(state_error == INVALID_IN + BCM_ERROR)
        printf("passed \n");
    else
        printf("failed \n");


    /************************************************************************************
    * Target Function: BCM Init Function
    * Test Case: INVALID parameters(invalid protocol) .
    * Expected OUTPUT: TNULL_PTR + BCM_ERROR
    ************************************************************************************/


    a_BCM1.BCM_CH_ID = 5 ;
    a_BCM1.Mode = BCM_Rx_Mode ;
    a_BCM1.Protocol = INVALID_PARAMETER ;

    state_error = BCM_Init(&a_BCM1) ;

    printf("Test case 5 : ");

    if(state_error == INVALID_IN + BCM_ERROR)
        printf("passed \n");
    else
        printf("failed \n");


}



void Test_BCM_send(void){

 sint16_t state_error = E_OK ;

    printf("Testing BCM_send Function\n");
    printf("_____________\n");


    BCM_cfg_s BCM1 ;
    BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = SPI_Protocol;
	BCM_Init(&BCM1);

	uint8_t * Buffer = NULL ;
	uint8_t buffer_value = 8 ;
    uint8_t index ;
    Notify_FunPtr ptr_func = NULL ;

    /************************************************************************************
    * Target Function: BCM_send Function
    * Test Case: test null pointer (Buffer).
    * Expected OUTPUT: NULL_PTR + BCM_ERROR
    ************************************************************************************/
    index = 5 ;
    ptr_func = test_function ;
    Buffer = &buffer_value ;

    state_error = BCM_Send(NULL ,index ,&BCM1 , ptr_func) ;

    printf("Test case 1 : ");

    if(state_error == NULL_PTR + BCM_ERROR)
        printf("passed \n");
    else
        printf("failed \n");


    /************************************************************************************
    * Target Function: BCM_send Function
    * Test Case: test null pointer (structure).
    * Expected OUTPUT: NULL_PTR + BCM_ERROR
    ************************************************************************************/


    state_error = BCM_Send(Buffer ,index ,NULL , ptr_func) ;

    printf("Test case 2 : ");

    if(state_error == NULL_PTR + BCM_ERROR)
        printf("passed \n");
    else
        printf("failed \n");



    /************************************************************************************
    * Target Function: BCM_send Function
    * Test Case: test null pointer (pointer to function).
    * Expected OUTPUT: NULL_PTR + BCM_ERROR
    ************************************************************************************/


    state_error = BCM_Send(Buffer ,index ,&BCM1 , NULL) ;

    printf("Test case 3 : ");

    if(state_error == NULL_PTR + BCM_ERROR)
        printf("passed \n");
    else
        printf("failed \n");




    /************************************************************************************
    * Target Function: BCM_send Function
    * Test Case: send invalid parameters (mode).
    * Expected OUTPUT: INVALID_IN + BCM_ERROR
    ************************************************************************************/
    BCM1.BCM_CH_ID = 1;
	BCM1.Mode = INVALID_PARAMETER;
	BCM1.Protocol = SPI_Protocol;

    state_error = BCM_Send(Buffer ,index ,&BCM1 , ptr_func) ;

    printf("Test case 4 : ");

    if(state_error == INVALID_IN + BCM_ERROR)
        printf("passed \n");
    else
        printf("failed \n");

    /************************************************************************************
    * Target Function: BCM_send Function
    * Test Case: send invalid parameters (protocol).
    * Expected OUTPUT: INVALID_IN + BCM_ERROR
    ************************************************************************************/
    BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = INVALID_PARAMETER;

    state_error = BCM_Send(Buffer ,index ,&BCM1 , ptr_func) ;

    printf("Test case 5 : ");

    if(state_error == INVALID_IN + BCM_ERROR)
        printf("passed \n");
    else
        printf("failed \n");


    BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = SPI_Protocol;

    BCM_DeInit(&BCM1);


    /************************************************************************************
    * Target Function: BCM_send Function
    * Test Case: use bcm_send without init.
    * Expected OUTPUT: INVALID_IN + BCM_ERROR
    ************************************************************************************/
    BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Tx_Mode;
	BCM1.Protocol = SPI_Protocol;

    state_error = BCM_Send(Buffer ,index ,&BCM1 , ptr_func) ;

    printf("Test case 6 : ");

    if(state_error == NOT_INIT + BCM_ERROR)
        printf("passed \n");
    else
        printf("failed \n");



    /************************************************************************************
    * Target Function: BCM_send Function
    * Test Case: call send from different mode.
    * Expected OUTPUT: FUNC_IN_DIF_CFG + BCM_ERROR
    ************************************************************************************/
    BCM1.BCM_CH_ID = 1;
	BCM1.Mode = BCM_Rx_Mode;
	BCM1.Protocol = SPI_Protocol;

	BCM_Init(&BCM1) ;
    state_error = BCM_Send(Buffer ,index ,&BCM1 , ptr_func) ;

    printf("Test case 7 : ");

    if(state_error == FUNC_IN_DIF_CFG + BCM_ERROR)
        printf("passed \n");
    else
        printf("failed \n");






}




#include "BCM_TESTER.h"
#include "D:\OTHERS\Sprints BootCamp\Challenges\Challenge16- TMU\Deliverables\TMU_Challenge\BCM_unit_test\SL\BCM.h"

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

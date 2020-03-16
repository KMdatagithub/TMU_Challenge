/*
 * SEND.c
 *
 *
 * Created: 2020-03-01 9:49:13 PM
 * Author : Ahmed Salama
 */

#include <stdlib.h>




/*Testing LCD_Init API
*@Parm : none
*@Output : ERROR Type
*/
void Test_LCD_Init(void)
{
    /*Test 1 Description : Happy scenario everything done as expected
    *Expected Output : E_OK
    *Output : E_OK
    *Test State: Passed
    */
    printf("%d",LCD_init());


    /*Test 2 Description: Multiple Init
    *Expected Output: Error Output
    Output : ERROR_MULTI_INIT
    Test State :  Passed
    */
    LCD_init();
    printf("%d",LCD_init());
}



/*Testing LCD_Data API
*@Parm : data to be sent
*@output: Error Value
*/
void Test_LCD_DATA(void)
{
    /*Test 1 Description : Happy scenario everything done as expected
    *Expected Output : E_OK
    *Output : E_OK
    *Test State: Passed
    */
    printf("%d",LCD_DATA(5));
}



/*Testing LCD_sendcommand API
*@Parm : command to be send
*@output: Error Value
*/
void Test_LCD_sendCommand(void)
{
    /*Test 1 Description : Happy scenario everything done as expected
    *Expected Output : E_OK
    *Output : E_OK
    *Test State: Passed
    */
    printf("%d",LCD_sendCommand(5));

}


/*Testing LCD_displayChar API
*@Parm : char to be send
*@output: Error Value
*/
void Test_LCD_displayChar(void)
{
    /*Test 1 Description : Happy scenario everything done as expected
    *Expected Output : E_OK
    *Output : E_OK
    *Test State: Passed
    */
    LCD_init();
   printf("%d",LCD_displayChar(5));


   /*Test 2 Description : Sending Without Init
    *Expected Output : ERROR
    *Output : ERROR_NOT_INIT
    *Test State: Passed
    */

   printf("%d",LCD_displayChar(5));
}


/*Testing LCD_gotoRowColumn API
*@Parm : row,col
*@output: Error Value
*/
void Test_LCD_gotoRowColumn(void)
{
    /*Test 1 Description : Happy scenario everything done as expected
    *Expected Output : E_OK
    *Output : E_OK
    *Test State: Passed
    */
    LCD_init();
    printf("%d",LCD_gotoRowColumn(1,2));


    /*Test 2 Description : With no init
    *Expected Output : Error not init
    *Output : ERROR_NOT_INITIALIZED
    *Test State: Passed
    */
    printf("%d",LCD_gotoRowColumn(1,2));


    /*Test 3 Description : inserting wrong row or col num
    *Expected Output : Error
    *Output : E_OK
    *Test State: Failed
    *Solution: Handled in code
    */
    LCD_init();
    printf("%d",LCD_gotoRowColumn(100,100));
}


/*Testing LCD_displayString API
*@Parm : row,col
*@output: Error Value
*/
void Test_LCD_displayString(void)
{
    /*Test 1 Description : Happy scenario everything done as expected
    *Expected Output : E_OK
    *Output : E_OK
    *Test State: Passed
    */
    LCD_init();
    printf("%d",LCD_displayString("AHMED"));


    /*Test 2 Description : Send Without init
    *Expected Output : Error
    *Output : Error
    *Test State: Passed
    */
    printf("%d",LCD_displayString("AHMED"));


    /*Test 3 Description : Sending null
    *Expected Output : Error
    *Output : Error
    *Test State: Passed
    */
    LCD_init();
    printf("%d",LCD_displayString(0));
}


/*Testing LCD_displayStringRowColumn API
*@Parm : row,col
*@output: Error Value
*/
void Test_LCD_displayStringRowColumn(void)
{
    /*Test 1 Description : Happy scenario everything done as expected
    *Expected Output : E_OK
    *Output : E_OK
    *Test State: Passed
    */
  //  LCD_init();
  //  printf("%d",LCD_displayStringRowColumn(1,2,"AHMED"));


    /*Test 2 Description : With no init
    *Expected Output : Error not init
    *Output : ERROR_NOT_INITIALIZED
    *Test State: Passed
    */
   // printf("%d",LCD_displayStringRowColumn(1,2,"AHMED"));


    /*Test 3 Description : inserting wrong row or col num
    *Expected Output : Error
    *Output : E_OK
    *Test State: Failed
    *Solution: Handled in code
    */
 //   LCD_init();
  //  printf("%d",LCD_gotoRowColumn(100,100,"AHMED"));


    /*Test 4 Description: sending null
    *Expected Output : Error
    *Output : E_OK
    *Test State : Passed
    *Solution : Handled
    */
    LCD_init();
    printf("%d",LCD_displayStringRowColumn(1,1,NULL));
}

int main(void)
{
    Test_LCD_displayStringRowColumn();
}


/*
Q5: The addresses get smaller.
Q6: No because if there are two variables with the same address,
the compiler will not know which variable to update
Q7: I expect it to be stored in the stack and after running the program,
my expectations were correct.

       +------------------+
       |  TEXT            | x10000?
       |                  |
       |  instructions in | 0x40138d = proc1
       |  machine code    | 0x401473  = proc2
       |                  |
       +------------------+ 0x401665 = etext
       |  DATA            | 0x40404c = g1   global initialized
       |  - initialized   | 0x404050 = g3
       |    variables     | 0x404054 = ls1  static initialized
       |                  | 0x404058 = ls3
       |                  | 
       | ---------------  | 0x40405c = edata
       |                  | 0x404080 = g0   global uninitialized
       |                  | 0x4040a0 = g2   big array 
       |                  | 0x405810 = g4
       |  - uninitialized | 0x405818 = ls0  static uninitialized
       |    variables     | 0x40581c = ls2
       |                  |
       |                  | 
       | ---------------  | 0x405820 = end 
       |  - heap for      | 		 pointed to by heap1
       |  dynamic         | 		 pointed to by heap2
       |  allocation      |
       |                  |
       |                  |
       +------------------+ 
                |
                |
                V

                .
                .
                .

                ^
                |
                |
       +------------------+
       |  STACK           | 0x7ffc61d03d18 = lc4, lc6   | Activation record for 
       |  - function      | 0x7ffc61d03d1c = lc3, lc5   | proc1 or proc2
       |    activation    	
       |    records       |
       |  - local         | 0x7ffc61d03d34 = lc2       | Activation record for
       |    automatic     | 0x7ffc61d03d38 = lc1       | main
       |    variables     | 0x7ffc61d03d3c = lc0       |
       +------------------+

*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

int g0; /* global variable, uninitialized */
int g1 = 14; /* global variable, initialized */
int g2[1500]; /* global variable, uninitialized */
int g3 = 16; /* global variable, initialized */
int g4; /* global variable, uninitialized */

void proc1();
void proc2();
int fact(int n);

int main() {
        extern int etext[], edata[], end[];
        int lc0; /* local variable, stored on stack */
        int lc1 = 27; /* local variable, stored on stack; mix init and uninit */
        int lc2; /* local variable, stored on stack */
        static int ls0; /* local static variable, uninitialized data */
        static int ls1 = 19; /* local static variable, initialized data */
        int *pheap1;
        int *pheap2;
        pheap1 = (int *) malloc(sizeof(int));
        pheap2 = (int *) malloc(sizeof(int));
        printf("\n\n---------- main -------------------\n\n");
        printf("%14p (%15lu): Last address\n", 0xffffffffffff, 999999999999999); 
        printf("%14p (%15lu): Address etext\n",etext, etext);
        printf("%14p (%15lu): Address edata\n", edata, edata);
        printf("%14p (%15lu): Address end\n",end, end);
        printf("%14p (%15lu): Address of code for proc1\n",&proc1, &proc1);
        printf("%14p (%15lu): Address of code for proc2\n", &proc2, &proc2);
        printf("%14p (%15lu): Address of uninitialized global variable g0\n",&g0, &g0);
        printf("%14p (%15lu): Address of initialized global variable g1\n",&g1, &g1);
        printf("%14p (%15lu): Address of uninitialized global array g2\n",&g2[0], &g2[0]);
        printf("%14p (%15lu): Address of initialized global variable g3\n",&g3, &g3);
        printf("%14p (%15lu): Address of uninitialized global variable g4\n",&g4, &g4);
        printf("%14p (%15lu): Address heap1 in heap space\n", pheap1, (unsigned long) pheap1>
        printf("%14p (%15lu): Address heap2 in heap space\n", pheap2, (unsigned long) pheap2>
        printf("%14p (%15lu): Address of local variable lc0\n", &lc0, &lc0);
        printf("%14p (%15lu): Address of local variable lc1\n", &lc1, &lc1);
        printf("%14p (%15lu): Address of local variable lc2\n", &lc2, &lc2);
        printf("%14p (%15lu): Address of local uninitialized static var ls0\n", &ls0, &ls0);
        printf("%14p (%15lu): Address of local initialized static var ls1\n", &ls1, &ls1);

        proc1();
        proc2();

        int n = 0;
        cout << endl;
        cout << "enter a positive integer n: ";
        cin >> n;
        if(n < 0)
          return 0;

        cout << "The factorial of " << n << " is " << fact(n) << endl;

        // printf("%14p (%15lu): Address of fact function \n", &fact, &fact);
        // printf("%14p (%15lu): Address of n \n", &n, &n);

        return 0;
}

int fact(int n)
{
        printf("%14p (%15lu): Address of fact function \n", &fact, &fact);
        printf("%14p (%15lu): Address of n \n", &n, &n);

        if(n == 0)
          return 1;
        else
          return n * fact(n-1);
}

void proc1()
{
        int lc3;
        int lc4 = 37;
        printf("\n\n----------- proc1 ------------------\n\n");
        printf("%14p (%15lu): Address of code for proc1\n", &proc1, &proc1);
        printf("%14p (%15lu): Address of global variable g0\n",&g0, &g0);
        printf("%14p (%15lu): Address of global variable g1\n", &g1, &g1);
        printf("%14p (%15lu): Address of global variable g2\n", &g2[0], &g2[0]);
        printf("%14p (%15lu): Address of global variable g3\n",&g3, &g3);
        printf("%14p (%15lu): Address of global variable g4\n", &g4, &g4);
        printf("%14p (%15lu): Address of local variable lc3\n", &lc3, &lc3);
        printf("%14p (%15lu): Address of local variable lc4\n",&lc4, &lc4);
}
void proc2()
{
        int lc5;
        int lc6 = 51;
        static int ls2;
        static int ls3 = 47;
        printf("\n\n------------ proc2 -----------------\n\n");
        printf("%14p (%15lu): Address of code for proc2\n",&proc2, &proc2);
        printf("%14p (%15lu): Address of global variable g0\n",&g0, &g0);
        printf("%14p (%15lu): Address of global variable g1\n", &g1, &g1);
        printf("%14p (%15lu): Address of global variable g2\n", &g2[0], &g2[0]);
        printf("%14p (%15lu): Address of global variable g3\n",&g3, &g3);
        printf("%14p (%15lu): Address of global variable g4\n", &g4, &g4);
        printf("%14p (%15lu): Address of local variable lc5\n", &lc5, &lc5);
        printf("%14p (%15lu): Address of local variable lc6\n",&lc6, &lc6);
        printf("%14p (%15lu): Address of local uninitialized static var ls2\n",&ls2, &ls2);
        printf("%14p (%15lu): Address of local initialized static var ls3\n",&ls3, &ls3);
}

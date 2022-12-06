%{
#include <stdio.h>
#include <string.h>
#include <math.h>

int res = 0;
int s = 1;

void yyerror(const char *str)
{
        fprintf(stderr,"ошибка: %s\n",str);
}
 
int yywrap()
{
        return 1;
} 
main()
{
        yyparse();
} 

int check(int num) {
        if(s == 1) {
                if ((long) 0x7FFFFFFF - (long) (res * 16) < num) {
                        return 1;
                }
        } else {
                if ((long) -0x7FFFFFFF + (long)(res * 16) > -num) {
                        return -1;
                }
        }
        return 0;
}

%}

%token START LETTER DIGIT END ERROR SIGN

%%

b :
        |
        b beg

beg: sign begin symbols ending

sign:
        |
        SIGN
        {
            if (strcmp($$,"-") == 0) {
                s = -1;
            }
            if (strcmp($$,"+") == 0) {
                s = 1;
            }
        }

symbols: 
        |
        symbols symbol
        ;

symbol:
       sym_digit | sym_letter
       ;

begin:
        START
        {
            res = 0;
        }
        ;
    
sym_letter:
        LETTER
        {
            int num = (int)strtol($$, NULL, 16);
            int ch = check(num);
            if (ch == 0) {
                res = res * 16 + num;
            } else {
                if (ch == 1) {
                        yyerror("number is too big");
                } else {
                        yyerror("number is too small");
                } 
                return -1;
            }
        }
        ;

sym_digit:
        DIGIT
        {
           int num = atoi($$);
           int ch = check(num);
           if (ch == 0) {
                res = res * 16 + num;
           } else {
                if (ch == 1) {
                        yyerror("number is too big");
                } else {
                        yyerror("number is too small");
                } 
                return -1;
            }
        }
        ;

ending:
        END
        {
            printf("\t%d\n", res * s);
            s = 1;
        }
        ;

%%
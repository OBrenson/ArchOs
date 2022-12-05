%{
#include <stdio.h>
#include <string.h>
#include <math.h>

int counter;
int res = 0;

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

%}

%token START LETTER DIGIT END ERROR

%%

b :
        |
        b beg

beg: begin symbols ending

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
            counter = 0;
            res = 0;
        }
        ;
    
sym_letter:
        LETTER
        {
            int num = (int)strtol($$, NULL, 16);
            res = res * 16 + num;
            counter++;
        }
        ;

sym_digit:
        DIGIT
        {
           int num = atoi($$);
           res = res * 16 + num;
           counter++;
        }
        ;

ending:
        END
        {
            printf("\t%d\n", res);
        }
        ;

%%
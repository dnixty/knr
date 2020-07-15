#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAXOP   100
#define MAXVAL  100
#define BUFSIZE 100
#define NUMBER  '0'
#define NAME 'n'

int getop(char []);
void push(double);
double top(void);
void dup(void);
void printst(void);
int getch(void);
void ungetch(int);
void mathfnc(char []);


int sp = 0;
double val[MAXVAL];
char buf[BUFSIZE];
int bufp = 0;

void push(double f)
{
    if(sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}

double pop(void)
{
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

double top(void)
{
    double v = pop();
    sp++;
    return v;
}

void dup(void)
{
    push(val[sp]);
}

void swap(void)
{
    double t, s;
    t = pop();
    s = pop();
    push(t);
    push(s);
}

void clear(void)
{
    sp = 0;
}

int getch(void)
{
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}


int getop(char s[])
{
    int i, c;

    while ((s[0] = c = getch()) == ' ' || c == '\t')
        ;
    s[1] = '\0';
    i = 0;
    if (islower(c)) {
        while (islower(s[++i] = c = getch()))
            ;
        s[i] = '\0';
        if (c != EOF)
            ungetch(c);
        if (strlen(s) > 1)
            return NAME;
        else
            return c;
    }
    if (!isdigit(c) && c != '.' && c != '-')
        return c;
    if (c == '-') {
        if (isdigit(c = getch())) {
            s[++i] = c;
        } else {
            if (c != EOF)
                ungetch(c);
            return '-';
        }
    }
    if (isdigit(c))
        while (isdigit(s[++i] = c = getch()))
            ;
    if (c == '.')
        while (isdigit(s[++i] = c = getch()))
            ;
    s[i] = '\0';
    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

void mathfnc(char s[]) {
    double op2;

    if (strcmp(s, "sin") == 0)
        push(sin(pop()));
    else if (strcmp(s, "cos") == 0)
        push(cos(pop()));
    else if (strcmp(s, "exp") == 0)
        push(exp(pop()));
    else if (strcmp(s, "pow") == 0) {
        op2 = pop();
        push(pow(pop(), op2));
    } else
        printf("error: %s is not supported\n", s);
}


int main()
{
    int type;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
        case NUMBER:
            push(atof(s));
            break;
        case NAME:
            mathfnc(s);
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if (op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case '%':
            op2 = pop();
            if (op2 != 0)
                push(fmod(pop(), op2));
            else
                printf("error: zero divisor\n");
            break;
        case '?':
            printf("\t%.8g\n", top());
            break;
        case 'c':
            clear();
            break;
        case 'd':
            dup();
            break;
        case 's':
            swap();
            break;
        case '\n':
            printf("\t%.8g\n", pop());
            break;
        default:
            printf("error: unknown command %s\n", s);
            break;
        }
    }
    return 0;
}

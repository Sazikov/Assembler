#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <sys\stat.h>
#include <typeinfo>
#include <iostream>

#include "comp_functions.h"


struct text_cmd input(struct text_cmd* text)
{
    assert(text != NULL);

    FILE* fpin = fopen("Code.txt", "r");

    assert(fpin != NULL);


    size_t text_size = text_len(fpin);

    (*text).buf = (char*)calloc(text_size + 1, sizeof(char));


    assert((*text).buf != NULL);


    (*text).num_symbols = fread((*text).buf, 1, text_size, fpin);

    (*text).num_str = number_str((*text).buf, (*text).num_symbols, '\n');

    (*text).strings = (struct line*)calloc((*text).num_str + 1, sizeof(struct line));
    text = pointer(text, text_size, '\n');


    fclose(fpin);

    return *text;
}

size_t number_str(char buf[], size_t n, char ch)
{
    assert(buf != NULL);

    size_t num = 0;
    for (size_t i = 0; i < n; ++i)
    {
        if (buf[i] == ch)
            ++num;
    }
    return num;
}


struct text_cmd* pointer(struct text_cmd* text, size_t text_size, char end_ch)
{
    assert(text != NULL);

    (*text).strings[0].ptr = &(*text).buf[0];

    size_t nline = 1;
    for (size_t i = 0; i < text_size - 1; ++i)
    {
        if ((*text).buf[i] == end_ch)
        {
            (*text).strings[nline].ptr = &(*text).buf[i + 1];
            (*text).strings[nline - 1].len = (*text).strings[nline].ptr - (*text).strings[nline - 1].ptr;
            (*text).buf[i] = '\0';
            ++nline;
        }
    }
    (*text).strings[nline - 1].len = (*text).strings[nline].ptr - (*text).strings[nline - 1].ptr;

    return text;
}

size_t text_len(FILE* fp)
{
    assert(fp != NULL);

    struct stat buff = {};
    fstat(fileno(fp), &buff);

    return buff.st_size;
}

void AssembleArg(int* code, int ip, char* line, char* cmd, int* num_labels, struct label* labels)
{
    int temp = 0;
    int fl = 1;

    *(line + strlen(cmd)) = ' ';
    char* arg = strtok(line + strlen(cmd), " ");

    if (*arg == '-')
    {
        fl = -1;
        arg++;
    }

    if ((int) *arg <= 57 && (int) *arg >= 48)
    {
        for (; *arg != '\0'; arg++)
        {
            temp = temp * 10 + *arg - 48;
        }
    }
    else 
    {
        for (int i = 0; i < *num_labels; i++)
        {
            if (stricmp(arg, labels[i].name) == 0)
            {
                temp = labels[i].ip;
            }
        }
    }
    
    code[ip] = temp * fl;

}

#define DEF_CMD(name, num, arg, ...) \
		if (stricmp(cmd, #name) == 0)\
		{\
			code[ip] = num;\
			ip++;\
			if(arg){ \
				AssembleArg(code, ip, text.strings[i].ptr, cmd, num_labels, labels); ip++;}\
		}\
        else

int* AssemblerMain(int* len, struct label* labels, int* num_labels)
{
    struct text_cmd text = {0, NULL, 0, NULL };
    text = input(&text);

    const size_t len_code = text.num_str * 2;
    int* code = (int *)calloc (len_code, sizeof (int));
    if (code == NULL)
    {
        printf("No memory for massive\n");
        return NULL;
    }
    
    int ip = 0;
    for (size_t i = 0; i <= text.num_str; ++i)
    {
        char* cmd = strtok(text.strings[i].ptr, " ");
        if (CheckLabel(cmd) != 0)
        {
            labels->ip = ip;
            labels->name = cmd;
            (*num_labels)++;
        }
        else
        {
            #include "..\cmd.h"
            /*else*/ printf("Syntax error\n");
        }
    }
    *len = ip;
    return code;
}

#undef  DEF_CMD

int CheckLabel (char* cmd)
{
    for (int i = 0; cmd[i] != '\0'; i++)
        {
            
            if (cmd[i] == ':')
            {
                return i;
            }
        }
    return 0;
}
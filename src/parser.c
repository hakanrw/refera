/*
 * This file is part of Refera, the interpreted scripting language.
 *
 * Refera is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Refera is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Refera.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2024  Ahmet Hakan Candar
 * Copyright (C) 2024  Berk Tahir Kılıç
 * Copyright (C) 2024  Barış Getiren
 * Copyright (C) 2024  Egemen Aybir
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "parser.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

struct ParseTable PT[1];

void parser_display_table() {
    printf("\nParse Table:\n");
    printf("| Operation | Destination | Source1 | Source2 | If | Condition | Operand1 (Symbol, idx1, idx2) | Operand2 (Symbol, idx1, idx2) |\n");
    printf("| %d         | %s            | %s        | %s        | %d  | %d         | %s, %d, %d                        | %s, %d, %d                        |\n",
           PT[0].operation,
           PT[0].destination,
           PT[0].source1,
           PT[0].source2,
           PT[0].ifexists,
           PT[0].cond,
           PT[0].operand1.symbol,
           PT[0].operand1.idx1,
           PT[0].operand1.idx2,
           PT[0].operand2.symbol,
           PT[0].operand2.idx1,
           PT[0].operand2.idx2
    );
}

void parser_reset_table()
{
    PT[0].operation = 0;
    PT[0].ifexists = 0;
    PT[0].destination[0]= '\0';
    PT[0].source1[0] = '\0';
    PT[0].source2[0] = '\0';
    PT[0].cond = 0;
    PT[0].operand1.symbol[0] = '\0';
    PT[0].operand1.idx1 = 0;
    PT[0].operand1.idx2 = 0;
    PT[0].operand2.symbol[0] = '\0';
    PT[0].operand2.idx1 = 0;
    PT[0].operand2.idx2 = 0;
}

void tokenize(const char *statement, char tokens[50][10], int *token_count) {
    char temp[20];
    int temp_index = 0, token_index = 0;
    for (int i = 0; statement[i] != '\0'; i++)
    {
        char c = statement[i];
        if ((c == '<' && statement[i+1] == '>')
        || (c == '<' && statement[i+1] == '=' )
        || (c == '>' && statement[i+1] == '=')
        || (c == '=' && statement[i+1] == '='))
        {
            if (temp_index > 0) {
                temp[temp_index] = '\0';
                strcpy(tokens[token_index++], temp);
                temp_index = 0;
            }
            temp[temp_index] = c;
            temp[temp_index + 1] = statement[i+1];
            temp[temp_index + 2] = '\0';
            strcpy(tokens[token_index++], temp);
            i++;
        }
        else if (c == 'i' && statement[i+1] == 'f')
        {
            temp[temp_index] = c;
            temp[temp_index + 1] = statement[i+1];
            temp[temp_index + 2] = '\0';
            strcpy(tokens[token_index++], temp);
            i++;
        }
        else if (strchr("()/*+-[],<>=", c))
        {
            if (temp_index > 0) {
                temp[temp_index] = '\0';
                strcpy(tokens[token_index++], temp);
                temp_index = 0;
            }
            temp[0] = c;
            temp[1] = '\0';
            strcpy(tokens[token_index++], temp);
        }
        else if (c == ' ')
        {
            if (temp_index > 0)
            {
                temp[temp_index] = '\0';
                strcpy(tokens[token_index++], temp);
                temp_index = 0;
            }
        }
        else
        {
            temp[temp_index++] = c;
            temp[temp_index + 1] = '\0';
        }
    }
    if (temp_index > 0)
    {
        temp[temp_index] = '\0';
        strcpy(tokens[token_index++], temp);
    }
    *token_count = token_index;
}
int check_operations(const char* token) {
    if (strcmp(token, "+") == 0) {
        return 1;
    }
    else if (strcmp(token, "-") == 0) {
        return 2;
    }
    else if (strcmp(token, "*") == 0) {
        return 3;
    }
    else if (strcmp(token, "/") == 0) {
        return 4;
    }
    else if (strcmp(token, "<>") == 0) {
        return 11;
    }

    else {
        return 0;
    }
}
int check_cond(const char* token) {
    if (strcmp(token, "==") == 0)
    {
        return 1;
    }
    else if (strcmp(token, "!=") == 0)
    {
        return 2;
    }
    else if (strcmp(token, ">") == 0)
    {
        return 3;
    }
    else if (strcmp(token, ">=") == 0)
    {
        return 4;
    }
    else if (strcmp(token, "<") == 0)
    {
        return 5;
    }
    else if (strcmp(token, "<=") == 0)
    {
        return 6;
    }
    else
    {
        return 0;
    }
}

void parser_parse_statement(const char* statement)
{
    parser_reset_table();
    char tokens[50][10];
    int token_count;
    tokenize(statement, tokens, &token_count);

//    for (int i = 0; i < token_count; i++)
//    {
//        printf("%s ", tokens[i]);
//    }

    int ifexists = 0;
    int i = 0;

    while(i < token_count)
    {
        if (strcmp(tokens[i], "if") == 0)
        {
            PT[0].ifexists = 1;
            ifexists = 1;
            i++;
        }
        else if (strcmp(tokens[i], "(") == 0)
        {
            i++;
            while(strcmp(tokens[i], ")") != 0)
            {
                //printf("ifexist: %d\n",PT[0].ifexists);
                if (strcmp(tokens[i - 2], "if") != 0 && PT[0].ifexists == 0) {
                    printf("copying %s to destination\n", tokens[i-2]);
                    strcpy(PT[0].destination, tokens[i-2]);
                    printf("destination: %s\n", PT[0].destination);
                    if (isdigit(tokens[i][0])) {
                        printf("copying %s to source1\n", tokens[i]);
                        strcpy(PT[0].source1, tokens[i]);
                        printf("source1: %s\n", PT[0].source1);
                    }
                    i++;
                    if (strcmp(tokens[i], ",") == 0) {
                        i++;
                        strcpy(PT[0].source2, tokens[i]);
                        i++;
                    }
                }
                else if (strcmp(tokens[i+1], "[") == 0)
                {
                    if (PT[0].operand1.symbol[0] != '\0')
                    {
                        strcpy(PT[0].operand2.symbol, tokens[i]);
                        PT[0].operand2.idx1 = atoi(tokens[i+2]);
                        PT[0].operand2.idx2 = atoi(tokens[i+4]);
                        i += 6;
                    }
                    else
                    {
                        strcpy(PT[0].operand1.symbol, tokens[i]);
                        PT[0].operand1.idx1 = atoi(tokens[i+2]);
                        PT[0].operand1.idx2 = atoi(tokens[i+4]);
                        i += 6;
                    }
                }
                else if (check_cond(tokens[i]) > 0)
                {
                    PT[0].cond = check_cond(tokens[i]);
                    i++;
                }
                else
                {
                    if (PT[0].operand1.symbol[0] != '\0')
                    {
                        strcpy(PT[0].operand2.symbol, tokens[i]);
                        i++;
                    }
                    else
                    {
                        strcpy(PT[0].operand1.symbol, tokens[i]);
                        i ++;
                    }
                }
            }
            i++;
        }
        else if (strcmp(tokens[i], "[") == 0)
        {
            if (PT[0].destination[0] == '\0')
            {
                strcpy(PT[0].destination, tokens[i-1]);
                strcpy(PT[0].operand1.symbol, tokens[i-1]);
                PT[0].operand1.idx1 = atoi(tokens[i+1]);
                if (strcmp(tokens[i+2], ",") == 0) {
                    PT[0].operand1.idx2 = atoi(tokens[i+3]);
                    i += 5;
                } else {
                    PT[0].operand1.idx2 = 0;
                    i += 3;
                }
            }
            else if (PT[0].source1[0] == '\0')
            {
                strcpy(PT[0].source1, tokens[i-1]);
                strcpy(PT[0].operand2.symbol, tokens[i-1]);
                PT[0].operand2.idx1 = atoi(tokens[i+1]);
                if (strcmp(tokens[i+2], ",") == 0) {
                    PT[0].operand2.idx2 = atoi(tokens[i+3]);
                    i += 5;
                } else {
                    PT[0].operand2.idx2 = 0;
                    i += 3;
                }
            }
        }

        else if (check_operations(tokens[i]) > 0)
        {
            PT[0].operation = check_operations(tokens[i]);
            i++;
        }
        else if (strcmp(tokens[i], "=") == 0)
        {
            if (strcmp(tokens[i+1],"[") == 0) {
                if (PT[0].operation == 0) {
                    PT[0].operation = 5;

                }
                else {
                    break;
                }
                while (strcmp(tokens[i], "]") != 0)
                {
                    i++;
                }
                break;
            }
            else if ((strcmp(tokens[i+1],"[") != 0)) {
                strcpy(PT[0].destination, tokens[i - 1]);
                i++;
            }
            if (isdigit(tokens[i][0]))
            {
                strcpy(PT[0].source1, tokens[i]);
                PT[0].operation = 6;
                i++;
            }
            else if (strcmp(tokens[i] ,"sum") == 0)
            {
                PT[0].operation = 8;
                i+=2;
                strcpy(PT[0].source1, tokens[i]);
                i++;
            }
            else if (strcmp(tokens[i] ,"aver") == 0)
            {
                PT[0].operation = 9;
                i+=2;
                strcpy(PT[0].source1, tokens[i]);
                i++;
            }
            else if (strcmp(tokens[i] ,"diag") == 0)
            {
                PT[0].operation = 10;
                i+=2;
                strcpy(PT[0].source1, tokens[i]);
                i++;
            }
            else if (tokens[i][0]) {
                strcpy(PT[0].source1, tokens[i]);
                PT[0].operation = 7;
                i++;
                if (check_operations(tokens[i]) > 0 ) {
                    PT[0].operation = check_operations(tokens[i]);
                    i++;
                    strcpy(PT[0].source2, tokens[i]);
                }
            }


        }
        else if (strcmp(tokens[i],"print") == 0) {
            PT[0].operation = 12;
            i++;
            if (!isdigit(tokens[i][0])) {
                strcpy(PT[0].source1,tokens[i]);
                i++;
            }


        }

        else
        {
            i++;
        }



    }

    if (!ifexists)
    {
        PT[0].ifexists = 0;
    }
}



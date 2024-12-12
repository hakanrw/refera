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
#include "parser.h"

struct ParseTable PT[1];
void displayParseTable() {
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

void parser_parse_statement(const char* statement){
    char *statement_ptr = statement;
    while (*statement_ptr != '\0') {
        if (strncmp(statement,"if",2) == 0) {
            PT[0].ifexists = 1;
        }
        statement_ptr ++;
    }
    displayParseTable();
}


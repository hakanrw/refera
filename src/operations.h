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

#ifndef OPERATIONS_H
#define OPERATIONS_H

typedef unsigned char refera_operation_t; // Operation type 0..255

/* Builtin statements */

/* Addition */
#define R_ADD	1
/* Subtraction */
#define R_SUB	2
/* Multiplication */
#define R_MUL	3
/* Division */
#define R_DIV	4
/* Array initialization */
#define R_INI	5
/* Assignment */
#define R_SET	6
/* Copy */
#define R_CPY	7
/* Exchange array elements */
#define R_EXC	11

/* Builtin functions */

/* sum() */
#define R_SUM	8
/* aver() */
#define R_AVG	9
/* diag() */
#define R_DIA	10
/* print */
#define R_PRI	12

#define R_EQU 1

#define R_NEQ 2

#define R_GRT 3

#define R_GTE 4

#define R_LSR 5

#define R_LRE 6

#endif /* OPERATIONS_H */

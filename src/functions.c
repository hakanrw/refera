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
#include <stddef.h>
#include <stdlib.h>

#include "refera.h"

refera_symbol_t refera_sum(refera_symbol_t symbol)
{
	refera_symbol_t retval;
	// TODO
	return retval;
}

refera_symbol_t refera_aver(refera_symbol_t symbol)
{
	refera_symbol_t retval;
	// TODO
	return retval;
}

refera_symbol_t refera_diag(refera_symbol_t symbol)
{
	refera_symbol_t retval;
	// TODO
	return retval;
}

void refera_print(refera_symbol_t symbol)
{
	if(symbol.dim == 0)
	{
		int value = *symbol.base;
		printf("%d\n", value);
	}
	else if(symbol.dim == 1)
	{
		for (int i = 0; i < symbol.size1; i++)
		{
			printf("%d ", *(symbol.base + i));
		}
		printf("\n");
	}
	else if(symbol.dim == 2)
	{
		int a = 0;
		for (int i = 0; i < symbol.size1 * symbol.size2; i++)
		{
			a++;
			printf("%d ", *(symbol.base + i));
			if(a == symbol.size2)
			{
				a = 0;
				if(i != (symbol.size1 * symbol.size2) - 1)
					printf(", ");
			}
		}
		printf("\n");
	}
}

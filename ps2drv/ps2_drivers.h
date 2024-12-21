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

#ifndef PS2_DRIVERS_H
#define PS2_DRIVERS_H

extern unsigned char usbd[];
extern unsigned int size_usbd;

extern unsigned char ps2kbd[];
extern unsigned int size_ps2kbd;

extern unsigned char iomanx[];
extern unsigned int size_iomanx;

extern unsigned char filexio[];
extern unsigned int size_filexio;

extern unsigned char mcman[];
extern unsigned int size_mcman;

extern unsigned char mcserv[];
extern unsigned int size_mcserv;

#endif /* PS2_DRIVERS_H */

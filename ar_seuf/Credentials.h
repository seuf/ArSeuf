/**
 * Copyright 2012, Thierry Sallé
 *
 * This file is part of ArSeuf.
 *  
 *  ArSeuf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ArSeuf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ArSeuf.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef __CREDENTIALS_H__
#define __CREDENTIALS_H__

// Wifi parameters
//Adhoc mode
const boolean use_adhoc = false;
const char *ssid_adhoc = "ArduinoWiFly";

//Normal mode (WPA2)
char passphrase[] = "passphrase";
char ssid[] = "apero";

const boolean is_wpa = true;

#endif


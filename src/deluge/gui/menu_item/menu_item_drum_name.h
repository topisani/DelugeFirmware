/*
 * Copyright © 2019-2023 Synthstrom Audible Limited
 *
 * This file is part of The Synthstrom Audible Deluge Firmware.
 *
 * The Synthstrom Audible Deluge Firmware is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MENUITEMDRUMNAME_H_
#define MENUITEMDRUMNAME_H_

#include "gui/menu_item/menu_item.h"

class MenuItemDrumName final : public MenuItem {
public:
	MenuItemDrumName(char const* newName = NULL) : MenuItem(newName) {}
	void beginSession(MenuItem* navigatedBackwardFrom);
	bool isRelevant(Sound* sound, int whichThing);
};

#endif /* MENUITEMDRUMNAME_H_ */

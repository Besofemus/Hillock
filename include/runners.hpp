/*
 * SPDX-License-Identifier: GPL-3.0-or-later

 * Copyright (C) 2019 Roman Erdyakov

 * This file is part of Memedar (flashcard system)
 * Memedar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef HILLOCK_MODEL_RUNNERS_HPP
#define HILLOCK_MODEL_RUNNERS_HPP


#include <list>

#include "filepath.hpp"


namespace model {
	class runners;
}


class model::runners
{
public:
	virtual model::filepath& add(QString&& filepath);
	
	void remove(const model::filepath& filepath);
	void run();
	virtual ~runners() = default;
private:
	std::list<model::filepath> m_runners;
};


#endif // HILLOCK_MODEL_RUNNERS_HPP

/*
 *  Hydrotopia
 *  Copyright (C) 2022 Johan Norberg <lonezor@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>
 */

#pragma once

#include <string>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

enum class subsystem
{
    unknown,

    /** Core subsystem: Components that are always running or ready to act */
    core,

    /** Main usecase: Equiment for main use case */
    main,

    /** Extra: Supporting equipment (good to have but not critical) */
    extra,
};

//---------------------------------------------------------------------------------------------------------------------

/** Subsystem as string */
std::string subsystem_str(subsystem subsystem);

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl

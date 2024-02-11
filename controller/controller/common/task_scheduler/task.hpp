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

/** @file task.h
 *  @brief Internal representation of task for the scheduler
 */

#include <chrono>
#include <cstdbool>
#include <cstdint>
#include <functional>
#include <unordered_map>

#include <common/task_scheduler/data_types.hpp>

namespace hydroctrl {
namespace common {

//-------------------------------------------------------------------------------------------------------------------

/** Task
 *
 * This structure is used for book keeping among all
 * tasks, representing the task itself from a scheduler's
 * perspective.
 */
struct task
{
    /** Task context
     *
     * This structure is presented in each callback.
     */
    task_context ctx;

    /** @brief std::bind() configured callback
     *
     * The std::function object cannot be assigned
     * (no match for 'operator=') so it is wrapped
     * inside a std::shared_ptr container.
     *
     * The std::bind() object requires one argument
     * from the scheduler, while the rest is added
     * as is from the application level.
     *
     * With the std::shared_ptr<> the callback
     * invocation would be:
     *
     *\code
     * (*tsk->cb)(tsk->ctx);
     *\endcode
     *
     * The huge benefit with the usage of std::bind()
     * in the task scheduler is that application
     * specific data types for the 'user_data' parameter
     * are completely detached from the scheduler. It
     * becomes generic.
     */
    std::shared_ptr<std::function<void(task_context)>> cb;

    /** Timer id */
    uint64_t timer_id{0};
};

//-------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
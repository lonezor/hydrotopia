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

/** @file data_types.h
 *  @brief Task scheduler data types
 */

#include <chrono>
#include <cstdbool>
#include <cstdint>
#include <functional>

namespace hydroctrl {
namespace common {

//-------------------------------------------------------------------------------------------------------------------

/** Task Id */
using task_id = uint64_t;

//-------------------------------------------------------------------------------------------------------------------

/** Task scheduler mode */
enum class task_scheduler_mode
{
    /** @brief Scheduler not running
     *
     * This is the initial state.
     */
    not_running,

    /** @brief Foreground mode
     *
     * This mode enables an event loop where run() blocks
     * forever while callbacks are invoked. This pattern
     * works well for certain types of applications.
     */
    foreground,

    /** @brief Background mode
     *
     * This mode enables background processing. The application
     * runs its own thread while the task scheduler has its own
     * independent thread. Callbacks are invoked within the
     * context of the scheduler thread. Therefore a mutex is
     * required for safe concurrency.
     */
    background,
};

//-------------------------------------------------------------------------------------------------------------------

/** Cancel info (both request and indication) */
enum class cancel_info
{
    /** No cancellation requested */
    cancellation_not_requested,

    /** Cancel immediately without
     * triggering last callback. */
    immediate_without_callback,

    /** Cancel immediately and
     * trigger last callback. */
    immediate_with_callback,

    /** Let the task reach its expected
     * point in time and trigger the
     * last callback. */
    graceful_with_callback,
};

//-------------------------------------------------------------------------------------------------------------------

/** Task context
 *
 * This structure provides contextual information
 * for the application of a task callback. It clearly
 * indicates if it is the last callback or not.
 */
struct task_context
{
    /** Task id */
    task_id id{0};

    /** Monotonic reference timestamp */
    std::chrono::time_point<std::chrono::steady_clock> timestamp;

    /** Task interval */
    std::chrono::milliseconds interval{std::chrono::milliseconds(0)};

    /** Current number of callback iterations so far
     * for the task */
    uint64_t current_iterations{0};

    /** Total number of expected iterations (zero represents no limit) */
    uint64_t total_iterations{0};

    /** Cancel status */
    cancel_info cancel_status{cancel_info::cancellation_not_requested};

    /** Indicates the end of a task.
     *
     * Last callback is set to true when no more callbacks
     * will occur. This is an important trigger point for
     * ending many use cases (e.g. a repeated ping routine).
     */
    bool last_callback{false};

    /** Graceful cancellation requested */
    bool graceful_cancellation{false};
};

//-------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl

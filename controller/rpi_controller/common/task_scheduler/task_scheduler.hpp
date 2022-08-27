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

/** @file windows_task_scheduler.h
 *  @brief Windows task scheduler, implementing the task scheduler interface
 */

#include <common/io_monitor.hpp>
#include <common/task_scheduler/task.hpp>
#include <common/task_scheduler/task_scheduler_interface.hpp>

namespace hydroctrl {
namespace common {

//-------------------------------------------------------------------------------------------------------------------

/** Task scheduler, implementing task_scheduler_interface */
class task_scheduler : public task_scheduler_interface
{
  public:
    task_scheduler();

    /** @brief Register single task
     *
     * See task_scheduler_interface::register_single_task() description for more
     * information.
     *
     * @param interval   Interval. Millisecond duration compared to current
     * timestamp
     * @param cb         Function wrapper containing std::bind() object for the
     * callback. It is assumed that the application adds an user_data parameter
     *                   after the task_context to be able to handle the
     * callback.
     */
    task_id register_single_task(std::chrono::milliseconds interval,
                                 std::function<void(task_context)> cb) final;

    /** @brief Register periodic task with unlimited number of repetitions
     *
     * See task_scheduler_interface::register_periodic_task() description for
     * more information.
     *
     * @param interval   Interval. Millisecond periodicity
     * @param cb         Function wrapper containing std::bind() object for the
     * callback. It is assumed that the application adds an user_data parameter
     *                   after the task_context to be able to handle the
     * callback.
     */
    task_id register_periodic_task(std::chrono::milliseconds interval,
                                   std::function<void(task_context)> cb) final;

    /** @brief Register periodic task with fixed number of repetitions
     *
     * See task_scheduler_interface::register_periodic_task() description for
     * more information.
     *
     * @param interval        Interval. Millisecond periodicity
     * @param nr_iterations   Number of iterations
     * @param cb         Function wrapper containing std::bind() object for the
     * callback. It is assumed that the application adds an user_data parameter
     *                   after the task_context to be able to handle the
     * callback.
     */
    task_id register_periodic_task(std::chrono::milliseconds interval,
                                   size_t nr_iterations,
                                   std::function<void(task_context)> cb) final;

    /** @brief  Cancel ongoing task
     *
     * See task_scheduler_interface::cancel_task() description for more
     * information.
     *
     * @param tid                Task identifier
     * @param cancel_behaviour   Cancel behaviour
     */
    void cancel_task(task_id tid, cancel_info cancel_behaviour) final;

    /** @brief Run scheduler
     *
     * See task_scheduler_interface::run() description for more information.
     */
    void run(task_scheduler_mode mode) final;

    /** @brief Teardown running scheduler
     *
     * This stops running tasks. If run() is currently being
     * invoked it is returning.
     */
    void teardown() final;

  private:
    /** Helper function for foreground scheduling loop */
    void run_foreground_scheduler(task_scheduler_mode mode);

    /** Helpder function for creating task */
    std::shared_ptr<task> create_task(std::chrono::milliseconds interval,
                                      std::function<void(task_context)> cb);

    /** Helper function for creating timer */
    void create_timer(std::shared_ptr<task> tsk);

    /** Helper function for preparing task context and
     * invoking user callback.
     */
    void invoke_callback(std::shared_ptr<task> tsk);

    /** Helper function for destroying timer */
    void destroy_timer(std::shared_ptr<task> tsk);

    /** @brief Next task identifier
     *
     * This id increments forever to ensure unique numbers.
     * It will never overflow in practice, but if it did
     * the application would have processed the low numbers
     * already.
     */
    task_id next_task_id_{0};

    /** Task scheduler mode */
    task_scheduler_mode scheduler_mode_{task_scheduler_mode::not_running};

    /** Task map */
    std::unordered_map<task_id, std::shared_ptr<task>> task_map_;

    /** Lookup task from timer id */
    std::shared_ptr<task> lookup_task_from_timer(uint64_t timer_id);

    /** Timer id to generic task id */
    std::unordered_map<uint64_t, task_id> timer_map_;

    std::shared_ptr<common::io_monitor> io_monitor_;

    /** Exit flag */
    bool exit_{false};
};

//-------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
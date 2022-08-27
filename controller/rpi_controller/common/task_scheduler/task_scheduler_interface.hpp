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

/** @file task_scheduler.hpp
 *  @brief Task scheduler
 *
 * 1) Overview:
 *
 * The task scheduler enables arranging tasks along a timeline. In many
 *situations there is a need to describe a single invocation or periodic
 *invocations in the future.
 *
 * The framework doesn't know anything about the tasks themselves, it only
 * handles task identifiers and the timeline itself. The application instructs
 * the scheduler to invoke a callback when the time has come. The callback
 * includes both a task identifier for context as well as user provided data.
 *
 * The following callback is expected:
 * void task_scheduler_callback(task_context ctx, user_data);
 *
 * The data type for 'user_data' is controlled by the user. It can be an
 *integer, an object or a template<typename T>. This makes it flexible without
 *affecting the scheduler.
 *
 * 2) Details:
 *
 * The framework relies on the use of std::bind() together with the
 * std::function<> wrapper.
 *
 * std::bind() operates on the input that is given when it was
 * created. It uses 'by value' parameter passing. Optionally,
 * std::ref() may be used when setting up std::bind().
 *
 * The bound object cannot be passed around the program freely
 * so the std::function<> wrapper is used. The wrapper's argument
 * list represents how the bound object itself will be invoked,
 * which is different from the user specified callback function.
 *
 * The user provided function will be invoked via the std::bind()
 * object that uses the std::placeholders mechanism for the task
 * identifier. This value is provided by the scheduler and the
 * user_data argument is automatically included.
 *
 * 3) Usage examples:
 *
 * Example #1: User data as an integer (by value)
 *\code
 * void task_cb(task_context ctx, int user_data) {
 *     // ...
 * }
 *
 * void schedule_task(task_scheduler& scheduler)
 * {
 *     int user_data = 123; // value 123 is copied into std::bind()
 *     auto bf = std::bind(&task_cb, std::placeholders::_1, user_data);
 *     auto tid =
 *scheduler.register_single_task(std::chrono::milliseconds(1000), bf);
 * }
 *\endcode
 *
 * Example #2: User data as an integer (by reference)
 *\code
 * void task_cb(task_context ctx, int user_data) {
 *     // ...
 * }
 *
 * void schedule_task(task_scheduler& scheduler)
 * {
 *     static int user_data = 456; // value 456 is not copied but referenced by
 *std::bind() auto bf = std::bind(&task_cb, std::placeholders::_1,
 *std::ref(user_data)); auto tid =
 *scheduler.register_single_task(std::chrono::milliseconds(1000), bf);
 * }
 *\endcode
 *
 * Example #3: User data based on templates (task_cb<int>)
 *\code
 * template<typename T>
 * void task_cb(task_context ctx, T user_data) {
 *     // ...
 * }
 *
 * void schedule_task(task_scheduler& scheduler)
 * {
 *     int user_data = 789;
 *     auto bf = std::bind(&task_cb<int>, std::placeholders::_1, user_data);
 *     auto tid =
 *scheduler.register_single_task(std::chrono::milliseconds(1000),(bf);
 * }
 *\endcode
 *
 * Example #4: User data with shared object
 *\code
 * void task_cb(task_context ctx, std::shared_ptr<UserType> user_data) {
 *     // ...
 * }
 *
 * void schedule_task(task_scheduler& scheduler)
 * {
 *     auto user_data = std::make_shared<UserType>();
 *     auto bf = std::bind(&task_cb, std::placeholders::_1, user_data);
 *     auto tid =
 *scheduler.register_single_task(std::chrono::milliseconds(1000), bf);
 * }
 *\endcode
 *
 * Example #5: Use case with own class instance
 *\code
 * void task_cb(task_context ctx, test_app* user_data) {
 *     // ...
 * }
 *
 * void test_app::schedule_task(task_scheduler& scheduler)
 * {
 *     auto bf = std::bind(&task_cb, std::placeholders::_1, this);
 *     auto tid =
 *scheduler.register_single_task(std::chrono::milliseconds(1000), bf);
 * }
 *\endcode
 *
 * As shown in the examples, as long as the the application complies with the
 *assumption of std::placeholders::_1 as first argument. Thus the framework
 *provides a lookup context with the help of the task identifier.
 */

//-------------------------------------------------------------------------------------------------------------------

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <unordered_map>

#include <common/task_scheduler/data_types.hpp>

namespace hydroctrl {
namespace common {

//-------------------------------------------------------------------------------------------------------------------

/** Task scheduler interface */
class task_scheduler_interface
{
  public:
    /** Virtual destructor */
    virtual ~task_scheduler_interface() = default;

    /** @brief Register single task
     *
     * This method instructs the scheduler to invoke the callback
     * a single time after given amount of time.
     *
     * @param interval   Interval. Millisecond duration compared to current
     * timestamp
     * @param cb         Function wrapper containing std::bind() object for the
     * callback. It is assumed that the application adds an user_data parameter
     *                   after the task_context to be able to handle the
     * callback.
     */
    virtual task_id
    register_single_task(std::chrono::milliseconds interval,
                         std::function<void(task_context)> cb) = 0;

    /** @brief Register periodic task with unlimited number of repetitions
     *
     * This method instructs the scheduler to invoke the callback
     * periodically with given delay. The schedule must be ended
     * invoking cancel_task().
     *
     * @param interval   Interval. Millisecond periodicity
     * @param cb         Function wrapper containing std::bind() object for the
     * callback. It is assumed that the application adds an user_data parameter
     *                   after the task_context to be able to handle the
     * callback.
     */
    virtual task_id
    register_periodic_task(std::chrono::milliseconds interval,
                           std::function<void(task_context)> cb) = 0;

    /** @brief Register periodic task with fixed number of repetitions
     *
     * This method instructs the scheduler to invoke the callback
     * periodically with given delay. The schedule must be ended
     * invoking cancel_task().
     *
     * @param interval        Interval. Millisecond periodicity
     * @param nr_iterations   Number of iterations
     * @param cb              Function wrapper containing std::bind() object for
     * the callback. It is assumed that the application adds an user_data
     * parameter after the task_context to be able to handle the callback.
     */
    virtual task_id
    register_periodic_task(std::chrono::milliseconds interval,
                           size_t nr_iterations,
                           std::function<void(task_context)> cb) = 0;

    /** @brief  Cancel ongoing task
     *
     * If the given tid is currently scheduled it is cancelled. Invalid
     * identifiers are ignored.
     *
     * @param tid                Task identifier
     * @param cancel_behaviour   Cancel behaviour
     */
    virtual void cancel_task(task_id tid, cancel_info cancel_behaviour) = 0;

    /** @brief Run scheduler
     *
     * When foreground mode is requested, the method
     * blocks forever.
     *
     * When background mode is requested, a background
     * thread is spawned and the method returns directly.
     *
     * The scheduler will not perform any callback until
     * run() has been called.
     */
    virtual void run(task_scheduler_mode mode) = 0;

    /** @brief Teardown running scheduler
     *
     * This stops running tasks. If run() is currently being
     * invoked it is returning.
     */
    virtual void teardown() = 0;
};

//-------------------------------------------------------------------------------------------------------------------

/** @brief Create task scheduler
 *
 * This returns an allocated task scheduler.
 */
std::shared_ptr<task_scheduler_interface> create_task_scheduler();

//-------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl

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

#include <iostream>

#include <common/task_scheduler/task_scheduler.hpp>

namespace hydroctrl {
namespace common {

//-------------------------------------------------------------------------------------------------------------------

std::shared_ptr<task_scheduler_interface> create_task_scheduler()
{
    return std::make_shared<task_scheduler>();
}

//-------------------------------------------------------------------------------------------------------------------

task_scheduler::task_scheduler()
{
    io_monitor_ = std::make_shared<common::io_monitor>(common::io_monitor());
}

//-------------------------------------------------------------------------------------------------------------------

task_id
task_scheduler::register_single_task(std::chrono::milliseconds interval,
                                     std::function<void(task_context)> cb)
{
    auto tsk = create_task(interval, cb);

    constexpr uint64_t iterations_single_task = 1;
    tsk->ctx.total_iterations = iterations_single_task;
    create_timer(tsk);

    auto id = tsk->ctx.id;
    task_map_[id] = tsk;
    return id;
}

//-------------------------------------------------------------------------------------------------------------------

task_id
task_scheduler::register_periodic_task(std::chrono::milliseconds interval,
                                       std::function<void(task_context)> cb)
{
    auto tsk = create_task(interval, cb);

    constexpr uint64_t iterations_forever = 0;
    tsk->ctx.total_iterations = iterations_forever;
    create_timer(tsk);

    auto id = tsk->ctx.id;
    task_map_[id] = tsk;
    return id;
}

//-------------------------------------------------------------------------------------------------------------------

task_id
task_scheduler::register_periodic_task(std::chrono::milliseconds interval,
                                       size_t nr_iterations,
                                       std::function<void(task_context)> cb)
{
    auto tsk = create_task(interval, cb);

    tsk->ctx.total_iterations = nr_iterations;
    create_timer(tsk);

    auto id = tsk->ctx.id;
    task_map_[id] = tsk;
    return id;
}

//-------------------------------------------------------------------------------------------------------------------

void task_scheduler::cancel_task(task_id tid, cancel_info cancel_behaviour)
{
    if (task_map_.find(tid) == task_map_.end()) {
        return;
    }

    auto tsk = task_map_[tid];

    switch (cancel_behaviour) {
    case cancel_info::cancellation_not_requested:
        return;
    case cancel_info::immediate_without_callback: {
        destroy_timer(tsk);
        break;
    }
    case cancel_info::immediate_with_callback: {
        destroy_timer(tsk);

        // Invoke callback
        tsk->ctx.current_iterations++;
        tsk->ctx.last_callback = true;
        invoke_callback(tsk);
        break;
    }
    case cancel_info::graceful_with_callback: {
        tsk->ctx.graceful_cancellation = true;
        break;
    }
    }

    task_map_.erase(tid);
}

//-------------------------------------------------------------------------------------------------------------------

void task_scheduler::run(task_scheduler_mode mode)
{
    switch (mode) {
    case task_scheduler_mode::foreground: {
        run_foreground_scheduler(mode);
        break;
    }
    case task_scheduler_mode::background: {
        break;
    }
    }
}

//-------------------------------------------------------------------------------------------------------------------

std::shared_ptr<task>
task_scheduler::create_task(std::chrono::milliseconds interval,
                            std::function<void(task_context)> cb)
{
    auto tsk = std::make_shared<task>();

    // Rely on default values and set needed ones
    tsk->ctx.interval = interval;
    tsk->ctx.id = next_task_id_++;

    // cb cannot be assigned so put it in a std::shared_ptr container
    tsk->cb = std::make_shared<std::function<void(task_context)>>(cb);

    return tsk;
}

//-------------------------------------------------------------------------------------------------------------------

void task_scheduler::invoke_callback(std::shared_ptr<task> tsk)
{
    tsk->ctx.timestamp = std::chrono::steady_clock::now();

    // Invoke std::bind() object while providing
    // std::placeholders::_1 with task context
    (*tsk->cb)(tsk->ctx);
}

//-------------------------------------------------------------------------------------------------------------------

void task_scheduler::run_foreground_scheduler(task_scheduler_mode mode)
{
    while (true) {
        auto events = io_monitor_->wait_for_events();

        // No action required
        if (events.empty()) {
            continue;
        }

        // The server must be able to handle bad client input
        try {
            for (auto &&event : events) {
                common::event_type event_type = event->get_type();

                // Timer event
                if (event_type == common::event_type::timer) {

                    auto timer_event =
                        std::dynamic_pointer_cast<common::timer_event>(event);
                    auto timer_id = timer_event->get_id();

                    auto tsk = lookup_task_from_timer(timer_id);
                    if (tsk != nullptr) {
                        tsk->ctx.current_iterations++;
                        if ((tsk->ctx.total_iterations != 0 &&
                             tsk->ctx.current_iterations >=
                                 tsk->ctx.total_iterations) ||
                            tsk->ctx.graceful_cancellation) {
                            tsk->ctx.last_callback = true;
                            destroy_timer(tsk);
                        }

                        invoke_callback(tsk);

                        if (tsk->ctx.last_callback) {
                            task_map_.erase(tsk->ctx.id);
                        } else {
                            create_timer(tsk);
                        }
                    }
                }
            }
        } catch (const std::system_error &ex) {
            continue;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------

void task_scheduler::create_timer(std::shared_ptr<task> tsk)
{
    auto duration = std::chrono::microseconds(tsk->ctx.interval.count() * 1000);
    tsk->timer_id = io_monitor_->register_one_shot_timer(duration);

    timer_map_[tsk->timer_id] = tsk->ctx.id;
}

//-------------------------------------------------------------------------------------------------------------------

std::shared_ptr<task> task_scheduler::lookup_task_from_timer(uint64_t timer_id)
{
    if (timer_map_.find(timer_id) != timer_map_.end()) {
        auto task_id = timer_map_[timer_id];
        if (task_map_.find(task_id) != task_map_.end()) {
            return task_map_[task_id];
        }
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------------------------

void task_scheduler::destroy_timer(std::shared_ptr<task> tsk)
{
    if (tsk->timer_id == 0) {
        return;
    }

    timer_map_.erase(tsk->timer_id);
    tsk->timer_id = 0;
}

//-------------------------------------------------------------------------------------------------------------------

void task_scheduler::teardown() { exit_ = true; }

//-------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
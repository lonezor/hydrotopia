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

#include <common/error_code.hpp>

//-------------------------------------------------------------------------------------------------------------------

namespace hydroctrl {
namespace common {

/** @brief Packet engine category
 *
 * This class is used when throwing packet engine related exceptions
 *
 * It defines its own domain specific error codes and provides
 * string representation as well as the default equivalent portable error code.
 *
 * Note:
 * The class is indirectly used when calling the following method:
 * const request_handling_category_impl& request_handling_category();
 */
class request_handling_category_impl : public std::error_category
{
  public:
    /** @brief Get name of category
     */
    const char *name() const noexcept override;

    /** @brief Get string representation of error code
     */
    std::string message(int ev) const override;

    /** @brief Get default error condition that is equivalent to the given error
     * code
     */
    std::error_condition
    default_error_condition(int ev) const noexcept override;
};

/** @brief Return static instance of the social media test category
 */
const request_handling_category_impl &request_handling_category();

//-------------------------------------------------------------------------------------------------------------------

const char *request_handling_category_impl::name() const noexcept
{
    return "request_handling";
}

//-------------------------------------------------------------------------------------------------------------------

std::string request_handling_category_impl::message(int ev) const
{
    auto error = static_cast<hydroctrl_error>(ev);

    std::string msg = "ERROR_CODE_REQUEST_HANDLING_UNKNOWN";

    switch (error) {
    case hydroctrl_error::target_already_registered:
        msg = "ERROR_CODE_REQUEST_HANDLING_TARGET_ALREADY_REGISTERED";
        break;
    }

    return msg;
}

//-------------------------------------------------------------------------------------------------------------------

std::error_condition
request_handling_category_impl::default_error_condition(int ev) const noexcept
{
    (void)ev;

    std::error_condition econd = std::errc::operation_not_supported;

    // https://en.cppreference.com/w/cpp/error/errc

    // Best effort match against generic code
    econd = std::errc::operation_not_supported;

    return econd;
}

//-------------------------------------------------------------------------------------------------------------------

const request_handling_category_impl &request_handling_category()
{
    static request_handling_category_impl instance;
    return instance;
}

//-------------------------------------------------------------------------------------------------------------------

std::error_code make_error_code(hydroctrl_error e)
{
    return {static_cast<int>(e), request_handling_category()};
}

//-------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl

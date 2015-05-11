/*
 * Copyright (c) 2015-2015, Roland Bock
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SQLPP_INTERVAL_H
#define SQLPP_INTERVAL_H

#include <sqlpp11/basic_expression_operators.h>
#include <sqlpp11/type_traits.h>
#include <sqlpp11/exception.h>
#include <sqlpp11/value_type.h>
#include <sqlpp11/assignment.h>
#include <sqlpp11/result_field.h>

namespace sqlpp
{
	// interval value type
	struct interval
	{
		using _traits = make_traits<interval, tag::is_value_type>;
		using _tag = tag::is_interval;
		//using _cpp_value_type = chrono::time_point;

		template<typename T>
			using _is_valid_operand = is_interval_t<T>;
	};

	// interval expression operators
	template<typename Base>
		struct expression_operators<Base, interval>: public basic_expression_operators<Base, interval>
	{
		template<typename T>
			using _is_interval_operand = is_valid_operand<interval, T>;

		template<typename T>
			plus_t<Base, value_type_t<T>, wrap_operand_t<T>> operator +(T t) const
			{
				using rhs = wrap_operand_t<T>;
				static_assert(_is_interval_operand<rhs>::value, "invalid rhs operand");

				return { *static_cast<const Base*>(this), {t} };
			}

		template<typename T>
			minus_t<Base, value_type_t<T>, wrap_operand_t<T>> operator -(T t) const
			{
				using rhs = wrap_operand_t<T>;
				static_assert(_is_interval_operand<rhs>::value, "invalid rhs operand");

				return { *static_cast<const Base*>(this), {t} };
			}
	};

	// interval column operators
	template<typename Base>
		struct column_operators<Base, interval>
		{
			template<typename T>
				using _is_interval_operand = is_valid_operand_template<is_integral_t, T>;

			template<typename T>
				auto operator +=(T t) const -> assignment_t<Base, plus_t<Base, value_type_t<T>, wrap_operand_t<T>>>
				{
					using rhs = wrap_operand_t<T>;
					static_assert(_is_interval_operand<rhs>::value, "invalid rhs assignment operand");

					return { *static_cast<const Base*>(this), {{*static_cast<const Base*>(this), rhs{t}}}};
				}

			template<typename T>
				auto operator -=(T t) const -> assignment_t<Base, minus_t<Base, value_type_t<T>, wrap_operand_t<T>>>
				{
					using rhs = wrap_operand_t<T>;
					static_assert(_is_interval_operand<rhs>::value, "invalid rhs assignment operand");

					return { *static_cast<const Base*>(this), {{*static_cast<const Base*>(this), rhs{t}}}};
				}

		};

}
#endif

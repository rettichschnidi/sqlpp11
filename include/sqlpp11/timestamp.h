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

#ifndef SQLPP_TIMESTAMP_H
#define SQLPP_TIMESTAMP_H

#include <sqlpp11/basic_expression_operators.h>
#include <sqlpp11/type_traits.h>
#include <sqlpp11/exception.h>
#include <sqlpp11/value_type.h>
#include <sqlpp11/assignment.h>
#include <sqlpp11/result_field.h>

namespace sqlpp
{
	struct interval;

	// timestamp value type
	struct timestamp
	{
		using _traits = make_traits<timestamp, tag::is_value_type>;
		using _tag = tag::is_timestamp;
		//using _cpp_value_type = chrono::time_point;

		template<typename T>
			using _is_valid_operand = is_timestamp_t<T>;
	};

	// timestamp expression operators
	template<typename Base>
		struct expression_operators<Base, timestamp>: public basic_expression_operators<Base, timestamp>
	{
		template<typename T>
			using _is_timestamp_operand = is_valid_operand<timestamp, T>;

		template<typename T>
			using _is_interval_operand = is_valid_operand_template<is_interval_t, T>;

		template<typename T>
			plus_t<Base, value_type_t<timestamp>, wrap_operand_t<T>> operator +(T t) const
			{
				using rhs = wrap_operand_t<T>;
				static_assert(_is_interval_operand<rhs>::value, "invalid rhs operand");

				return { *static_cast<const Base*>(this), {t} };
			}

		template<typename T>
			using minus_value_type_t = typename std::conditional<is_timestamp_t<T>::value, interval, timestamp>::type;

		template<typename T>
			minus_t<Base, minus_value_type_t<value_type_t<T>>, wrap_operand_t<T>> operator -(T t) const
			{
				using rhs = wrap_operand_t<T>;
				static_assert(_is_timestamp_operand<rhs>::value or _is_interval_operand<rhs>::value, "invalid rhs operand");

				return { *static_cast<const Base*>(this), {t} };
			}
	};

	// timestamp column operators
	template<typename Base>
		struct column_operators<Base, timestamp>
		{
			template<typename T>
				using _is_interval_operand = is_valid_operand_template<is_interval_t, T>;

			template<typename T>
				auto operator +=(T t) const -> assignment_t<Base, plus_t<Base, timestamp, wrap_operand_t<T>>>
				{
					using rhs = wrap_operand_t<T>;
					static_assert(_is_interval_operand<rhs>::value, "invalid rhs assignment operand");

					return { *static_cast<const Base*>(this), {{*static_cast<const Base*>(this), rhs{t}}}};
				}

			template<typename T>
				auto operator -=(T t) const -> assignment_t<Base, minus_t<Base, timestamp, wrap_operand_t<T>>>
				{
					using rhs = wrap_operand_t<T>;
					static_assert(_is_interval_operand<rhs>::value, "invalid rhs assignment operand");

					return { *static_cast<const Base*>(this), {{*static_cast<const Base*>(this), rhs{t}}}};
				}

		};

}
#endif

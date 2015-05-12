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

#ifndef SQLPP_DATE_H
#define SQLPP_DATE_H

#include <sqlpp11/basic_expression_operators.h>
#include <sqlpp11/type_traits.h>
#include <sqlpp11/exception.h>
#include <sqlpp11/value_type.h>
#include <sqlpp11/assignment.h>
#include <sqlpp11/result_field.h>

namespace sqlpp
{
	struct interval;
	struct timestamp;

	// date value type
	struct date
	{
		using _traits = make_traits<date, tag::is_value_type>;
		using _tag = tag::is_date;
		//using _cpp_value_type = chrono::time_point;

		template<typename T>
			using _is_valid_operand = is_date_or_timestamp_t<T>;
	};

	template<typename T>
		struct date_plus_minus_value_type
		{
			using type = integral;
		};

	template<>
		struct date_plus_minus_value_type<timestamp>
		{
			using type = interval;
		};

	template<>
		struct date_plus_minus_value_type<integral>
		{
			using type = date;
		};

	template<>
		struct date_plus_minus_value_type<interval>
		{
			using type = timestamp;
		};

	// date expression operators
	template<typename Base>
		struct expression_operators<Base, date>: public basic_expression_operators<Base, date>
	{
		template<typename T>
			using _is_date_operand = is_valid_operand<date, T>;

		template<typename T>
			using _is_integral_operand = is_valid_operand_template<is_integral_t, T>;

		template<typename T>
			using _is_integral_operand = is_valid_operand_template<is_integral_t, T>;

		template<typename T>
			plus_t<Base, value_type_t<date>, wrap_operand_t<T>> operator +(T t) const
			{
				using rhs = wrap_operand_t<T>;
				static_assert(_is_integral_operand<rhs>::value, "invalid rhs operand");

				return { *static_cast<const Base*>(this), {t} };
			}

		template<typename T>
			using minus_value_type_t = typename std::conditional<is_date_t<T>::value, integral, date>::type;

		template<typename T>
			minus_t<Base, minus_value_type_t<value_type_t<T>>, wrap_operand_t<T>> operator -(T t) const
			{
				using rhs = wrap_operand_t<T>;
				static_assert(_is_date_operand<rhs>::value or _is_integral_operand<rhs>::value, "invalid rhs operand");

				return { *static_cast<const Base*>(this), {t} };
			}
	};

	// date column operators
	template<typename Base>
		struct column_operators<Base, date>
		{
			template<typename T>
				using _is_integral_operand = is_valid_operand_template<is_integral_t, T>;

			template<typename T>
				auto operator +=(T t) const -> assignment_t<Base, plus_t<Base, date, wrap_operand_t<T>>>
				{
					using rhs = wrap_operand_t<T>;
					static_assert(_is_integral_operand<rhs>::value, "invalid rhs assignment operand");

					return { *static_cast<const Base*>(this), {{*static_cast<const Base*>(this), rhs{t}}}};
				}

			template<typename T>
				auto operator -=(T t) const -> assignment_t<Base, minus_t<Base, date, wrap_operand_t<T>>>
				{
					using rhs = wrap_operand_t<T>;
					static_assert(_is_integral_operand<rhs>::value, "invalid rhs assignment operand");

					return { *static_cast<const Base*>(this), {{*static_cast<const Base*>(this), rhs{t}}}};
				}

		};

}
#endif

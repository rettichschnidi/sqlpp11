/*
 * Copyright (c) 2013-2014, Roland Bock
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

#ifndef SQLPP_COLUMN_H
#define SQLPP_COLUMN_H

#include <sqlpp11/alias.h>
#include <sqlpp11/column_fwd.h>
#include <sqlpp11/default_value.h>
#include <sqlpp11/null.h>
#include <sqlpp11/sort_order.h>
#include <sqlpp11/type_traits.h>
#include <sqlpp11/assignment.h>
#include <sqlpp11/expression.h>
#include <sqlpp11/serializer.h>
#include <sqlpp11/wrong.h>
#include <sqlpp11/detail/type_set.h>

namespace sqlpp
{
	struct column_base_t {};

	template<typename Table, typename Column, typename ColumnSpec>
		struct crtp_column_t: public ColumnSpec::_value_type::template expression_operators<Column>,
		public ColumnSpec::_value_type::template column_operators<Column>,
		public column_base_t
	{ 
		using _traits = make_traits<typename ColumnSpec::_value_type, tag::column, tag::expression, tag::named_expression>;
		struct _recursive_traits
		{
			using _parameters = std::tuple<>;
			using _provided_tables = detail::type_set<>;
			using _required_tables = detail::type_set<Table>;
			using _extra_tables = detail::type_set<>;
		};

		using _spec_t = ColumnSpec;
		using _table = Table;
		using _column_type = typename _spec_t::_column_type;
		using _name_t = typename _spec_t::_name_t;

		template<typename T>
			using _is_valid_operand = typename _spec_t::_value_type::template _is_valid_operand<T>;

		template<typename AliasProvider>
			using _column_of_table_alias = crtp_column_t<AliasProvider, Column, ColumnSpec>;

		crtp_column_t() = default;
		crtp_column_t(const crtp_column_t&) = default;
		crtp_column_t(crtp_column_t&&) = default;
		crtp_column_t& operator=(const crtp_column_t&) = default;
		crtp_column_t& operator=(crtp_column_t&&) = default;
		~crtp_column_t() = default;

		static constexpr const char* _get_name()
		{
			return _name_t::_get_name();
		}

		Column& column() { return static_cast<Column&>(*this); }
		const Column& column() const { return static_cast<const Column&>(*this); }

		template<typename alias_provider>
			expression_alias_t<Column, alias_provider> as(const alias_provider&) const
			{
				return { column() };
			}

		template<typename T>
			auto operator =(T t) const -> assignment_t<Column, wrap_operand_t<T>>
			{
				using rhs = wrap_operand_t<T>;
				static_assert(_is_valid_operand<rhs>::value, "invalid rhs operand assignment operand");

				return { column(), rhs{t} };
			}

		auto operator =(sqlpp::null_t) const
			->assignment_t<Column, sqlpp::null_t>
			{
				static_assert(can_be_null_t<crtp_column_t>::value, "column cannot be null");
				return { column(), {} };
			}

		auto operator =(sqlpp::default_value_t) const
			->assignment_t<Column, sqlpp::default_value_t>
			{
				return { column(), {} };
			}
	};

	template<typename Table, typename ColumnSpec>
		struct column_t: public crtp_column_t<Table, column_t<Table, ColumnSpec>, ColumnSpec> {};

	template<typename Context, typename X>
		struct serializer_t<Context, X, typename std::enable_if<std::is_base_of<column_base_t, X>::value, void>::type>
		{
			using T = X;

			static Context& _(const T& t, Context& context)
			{
				context << T::_table::_name_t::_get_name() << '.' << T::_name_t::_get_name();
				return context;
			}
		};

}

#endif

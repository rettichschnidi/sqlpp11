#ifndef TEST_SAMPLE_H
#define TEST_SAMPLE_H

#include <sqlpp11/table.h>
#include <sqlpp11/column_types.h>

namespace test
{
  struct TabFoo;
  namespace TabFoo_
  {
    struct Delta_
    {
      struct _name_t
      {
        static constexpr const char* _get_name() { return "delta"; }
        template<typename T>
        struct _member_t
          {
            T delta;
            T& operator()() { return delta; }
            const T& operator()() const { return delta; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
    };
    struct Delta: public sqlpp::column_t<TabFoo, Delta_>{};
    struct Epsilon_
    {
      struct _name_t
      {
        static constexpr const char* _get_name() { return "epsilon"; }
        template<typename T>
        struct _member_t
          {
            T epsilon;
            T& operator()() { return epsilon; }
            const T& operator()() const { return epsilon; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::bigint, sqlpp::tag::can_be_null>;
    };
    struct Epsilon: public sqlpp::column_t<TabFoo, Epsilon_>{};
    struct Omega_
    {
      struct _name_t
      {
        static constexpr const char* _get_name() { return "omega"; }
        template<typename T>
        struct _member_t
          {
            T omega;
            T& operator()() { return omega; }
            const T& operator()() const { return omega; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::floating_point, sqlpp::tag::can_be_null>;
    };
    struct Omega: public sqlpp::column_t<TabFoo, Omega_>{};
  }

  struct TabFoo: sqlpp::table_t<TabFoo,
               TabFoo_::Delta,
               TabFoo_::Epsilon,
               TabFoo_::Omega>
  {
    struct _name_t
    {
      static constexpr const char* _get_name() { return "tab_foo"; }
      template<typename T>
      struct _member_t
      {
        T tabFoo;
        T& operator()() { return tabFoo; }
        const T& operator()() const { return tabFoo; }
      };
    };
  };
  struct TabBar;
  namespace TabBar_
  {
    struct Alpha_
    {
      struct _name_t
      {
        static constexpr const char* _get_name() { return "alpha"; }
        template<typename T>
        struct _member_t
          {
            T alpha;
            T& operator()() { return alpha; }
            const T& operator()() const { return alpha; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::bigint, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update, sqlpp::tag::can_be_null>;
    };
    struct Alpha: public sqlpp::column_t<TabBar, Alpha_>{};
    struct Beta_
    {
      struct _name_t
      {
        static constexpr const char* _get_name() { return "beta"; }
        template<typename T>
        struct _member_t
          {
            T beta;
            T& operator()() { return beta; }
            const T& operator()() const { return beta; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
    };
    struct Beta: public sqlpp::column_t<TabBar, Beta_>{};
    struct Gamma_
    {
      struct _name_t
      {
        static constexpr const char* _get_name() { return "gamma"; }
        template<typename T>
        struct _member_t
          {
            T gamma;
            T& operator()() { return gamma; }
            const T& operator()() const { return gamma; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::boolean, sqlpp::tag::require_insert>;
    };
    struct Gamma: public sqlpp::column_t<TabBar, Gamma_>{};
    struct Delta_
    {
      struct _name_t
      {
        static constexpr const char* _get_name() { return "delta"; }
        template<typename T>
        struct _member_t
          {
            T delta;
            T& operator()() { return delta; }
            const T& operator()() const { return delta; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::can_be_null>;
    };
    struct Delta: public sqlpp::column_t<TabBar, Delta_>{};
  }

  struct TabBar: sqlpp::table_t<TabBar,
               TabBar_::Alpha,
               TabBar_::Beta,
               TabBar_::Gamma,
               TabBar_::Delta>
  {
    struct _name_t
    {
      static constexpr const char* _get_name() { return "tab_bar"; }
      template<typename T>
      struct _member_t
      {
        T tabBar;
        T& operator()() { return tabBar; }
        const T& operator()() const { return tabBar; }
      };
    };
  };
}
#endif

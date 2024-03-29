// Copyright (C) 2014 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this library; see the file COPYING3.  If not see
// <http://www.gnu.org/licenses/>.

// { dg-options "-std=gnu++14" }

#include <experimental/functional>
#include <testsuite_hooks.h>

int func(int, char) { return 0; }

struct F
{
  bool operator()() { return false; }
  bool operator()() const { return true; }
  bool operator()(int) { return false; }
  bool operator()(int) volatile { return true; }
};

void
test01()
{
  using std::experimental::not_fn;

  auto f1 = not_fn(func);
  VERIFY( f1(1, '2') == true );

  auto f2 = not_fn( [] { return true; } );
  VERIFY( f2() == false );

  auto f3 = not_fn( F{} );
  VERIFY( f3() == true );
  VERIFY( f3(1) == true );
  const auto f4 = f3;
  VERIFY( f4() == false );
  volatile auto f5 = f3;
  VERIFY( f5(1) == false );
}

int
main()
{
  test01();
}

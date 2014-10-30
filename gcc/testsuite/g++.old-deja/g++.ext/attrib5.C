// { dg-do run { xfail i?86-pc-cygwin i?86-pc-msys } }
// Test that attributes weak and alias coexist.
// { dg-require-weak "" }
// { dg-require-alias "" }

extern "C" {
  void _f () { }
  void f () __attribute__((weak, alias ("_f")));
}

int main ()
{
  f ();
}

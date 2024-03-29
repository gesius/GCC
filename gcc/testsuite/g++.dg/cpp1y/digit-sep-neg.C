// { dg-do compile { target c++14 } }

int
main()
{
  int i = 0;
  i = 1048''576; // { dg-error "adjacent digit separators" }
  i = 0X'100000; // { dg-error "digit separator after base indicator" }
  i = 0x'100000; // { dg-error "digit separator after base indicator" }
  i = 0004''000'000; // { dg-error "adjacent digit separators" }
  i = 0B1'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0'0; // OK
  i = 0b'0001'0000'0000'0000'0000'0000; // { dg-error "digit separator after base indicator" }
  i = 0b0001'0000'0000'0000'0000'0000'; // { dg-error "digit separator outside digit sequence" }
  unsigned u = 0b0001'0000'0000'0000'0000'0000'U; // { dg-error "digit separator outside digit sequence" }

  double d = 0.0;
  d = 1'.602'176'565e-19; // { dg-error "digit separator adjacent to decimal point" }
  d = 1.'602'176'565e-19; // { dg-error "digit separator adjacent to decimal point" }
  d = 1.602''176'565e-19; // { dg-error "adjacent digit separators" }
  d = 1.602'176'565'e-19; // { dg-error "digit separator adjacent to exponent" }
  d = 1.602'176'565e'-19; // { dg-error "digit separator adjacent to exponent" }
  d = 1.602'176'565e-'19; // { dg-error "digit separator adjacent to exponent" }
  d = 1.602'176'565e-1'9; // OK
  d = 1.602'176'565e-19'; // { dg-error "digit separator outside digit sequence" }
  float f = 1.602'176'565e-19'F; // { dg-error "digit separator outside digit sequence" }
}

!    Implementation of the IEEE_ARITHMETIC standard intrinsic module
!    Copyright (C) 2013 Free Software Foundation, Inc.
!    Contributed by Francois-Xavier Coudert <fxcoudert@gcc.gnu.org>
! 
! This file is part of the GNU Fortran runtime library (libgfortran).
! 
! Libgfortran is free software; you can redistribute it and/or
! modify it under the terms of the GNU General Public
! License as published by the Free Software Foundation; either
! version 3 of the License, or (at your option) any later version.
! 
! Libgfortran is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU General Public License for more details.
! 
! Under Section 7 of GPL version 3, you are granted additional
! permissions described in the GCC Runtime Library Exception, version
! 3.1, as published by the Free Software Foundation.
! 
! You should have received a copy of the GNU General Public License and
! a copy of the GCC Runtime Library Exception along with this program;
! see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
! <http://www.gnu.org/licenses/>.  */

#include "config.h"
#include "kinds.inc"
#include "c99_protos.inc"
#include "fpu-target.inc"

module IEEE_ARITHMETIC

  use IEEE_EXCEPTIONS
  implicit none
  private

  ! Every public symbol from IEEE_EXCEPTIONS must be made public here
  public :: IEEE_FLAG_TYPE, IEEE_INVALID, IEEE_OVERFLOW, &
    IEEE_DIVIDE_BY_ZERO, IEEE_UNDERFLOW, IEEE_INEXACT, IEEE_USUAL, &
    IEEE_ALL, IEEE_STATUS_TYPE, IEEE_GET_FLAG, IEEE_GET_HALTING_MODE, &
    IEEE_GET_STATUS, IEEE_SET_FLAG, IEEE_SET_HALTING_MODE, &
    IEEE_SET_STATUS, IEEE_SUPPORT_FLAG, IEEE_SUPPORT_HALTING

  ! Derived types and named constants

  type, public :: IEEE_CLASS_TYPE
    private
    integer :: hidden
  end type

  type(IEEE_CLASS_TYPE), parameter, public :: &
    IEEE_OTHER_VALUE       = IEEE_CLASS_TYPE(0), &
    IEEE_SIGNALING_NAN     = IEEE_CLASS_TYPE(1), &
    IEEE_QUIET_NAN         = IEEE_CLASS_TYPE(2), &
    IEEE_NEGATIVE_INF      = IEEE_CLASS_TYPE(3), &
    IEEE_NEGATIVE_NORMAL   = IEEE_CLASS_TYPE(4), &
    IEEE_NEGATIVE_DENORMAL = IEEE_CLASS_TYPE(5), &
    IEEE_NEGATIVE_ZERO     = IEEE_CLASS_TYPE(6), &
    IEEE_POSITIVE_ZERO     = IEEE_CLASS_TYPE(7), &
    IEEE_POSITIVE_DENORMAL = IEEE_CLASS_TYPE(8), &
    IEEE_POSITIVE_NORMAL   = IEEE_CLASS_TYPE(9), &
    IEEE_POSITIVE_INF      = IEEE_CLASS_TYPE(10)

  type, public :: IEEE_ROUND_TYPE
    private
    integer :: hidden
  end type

  type(IEEE_ROUND_TYPE), parameter, public :: &
    IEEE_NEAREST           = IEEE_ROUND_TYPE(GFC_FPE_TONEAREST), &
    IEEE_TO_ZERO           = IEEE_ROUND_TYPE(GFC_FPE_TOWARDZERO), &
    IEEE_UP                = IEEE_ROUND_TYPE(GFC_FPE_UPWARD), &
    IEEE_DOWN              = IEEE_ROUND_TYPE(GFC_FPE_DOWNWARD), &
    IEEE_OTHER             = IEEE_ROUND_TYPE(0)


  ! Equality operators on the derived types
  interface operator (==)
    module procedure IEEE_CLASS_TYPE_EQ, IEEE_ROUND_TYPE_EQ
  end interface
  public :: operator(==)

  interface operator (/=)
    module procedure IEEE_CLASS_TYPE_NE, IEEE_ROUND_TYPE_NE
  end interface
  public :: operator (/=)


  ! IEEE_IS_FINITE

  interface
    elemental logical function _gfortran_ieee_is_finite_4(X)
      real(kind=4), intent(in) :: X
    end function
    elemental logical function _gfortran_ieee_is_finite_8(X)
      real(kind=8), intent(in) :: X
    end function
  end interface

  interface IEEE_IS_FINITE
    procedure _gfortran_ieee_is_finite_4, _gfortran_ieee_is_finite_8
  end interface
  public :: IEEE_IS_FINITE

  ! IEEE_IS_NAN

  interface
    elemental logical function _gfortran_ieee_is_nan_4(X)
      real(kind=4), intent(in) :: X
    end function
    elemental logical function _gfortran_ieee_is_nan_8(X)
      real(kind=8), intent(in) :: X
    end function
  end interface

  interface IEEE_IS_NAN
    procedure _gfortran_ieee_is_nan_4, _gfortran_ieee_is_nan_8
  end interface
  public :: IEEE_IS_NAN

  ! IEEE_IS_NEGATIVE

  interface
    elemental logical function _gfortran_ieee_is_negative_4(X)
      real(kind=4), intent(in) :: X
    end function
    elemental logical function _gfortran_ieee_is_negative_8(X)
      real(kind=8), intent(in) :: X
    end function
  end interface

  interface IEEE_IS_NEGATIVE
    procedure _gfortran_ieee_is_negative_4, _gfortran_ieee_is_negative_8
  end interface
  public :: IEEE_IS_NEGATIVE

  ! IEEE_IS_NORMAL

  interface
    elemental logical function _gfortran_ieee_is_normal_4(X)
      real(kind=4), intent(in) :: X
    end function
    elemental logical function _gfortran_ieee_is_normal_8(X)
      real(kind=8), intent(in) :: X
    end function
  end interface

  interface IEEE_IS_NORMAL
    procedure _gfortran_ieee_is_normal_4, _gfortran_ieee_is_normal_8
  end interface
  public :: IEEE_IS_NORMAL

  ! IEEE_COPY_SIGN

  interface
    elemental real(kind=4) function _gfortran_ieee_copy_sign_4_4 (X,Y)
      real(kind=4), intent(in) :: X
      real(kind=4), intent(in) :: Y
    end function
    elemental real(kind=4) function _gfortran_ieee_copy_sign_4_8 (X,Y)
      real(kind=4), intent(in) :: X
      real(kind=8), intent(in) :: Y
    end function
    elemental real(kind=8) function _gfortran_ieee_copy_sign_8_4 (X,Y)
      real(kind=8), intent(in) :: X
      real(kind=4), intent(in) :: Y
    end function
    elemental real(kind=8) function _gfortran_ieee_copy_sign_8_8 (X,Y)
      real(kind=8), intent(in) :: X
      real(kind=8), intent(in) :: Y
    end function
  end interface

  interface IEEE_COPY_SIGN
    procedure _gfortran_ieee_copy_sign_4_4, _gfortran_ieee_copy_sign_4_8, &
              _gfortran_ieee_copy_sign_8_4, _gfortran_ieee_copy_sign_8_8
  end interface
  public :: IEEE_COPY_SIGN

  ! IEEE_UNORDERED

  interface
    elemental logical function _gfortran_ieee_unordered_4_4 (X,Y)
      real(kind=4), intent(in) :: X
      real(kind=4), intent(in) :: Y
    end function
    elemental logical function _gfortran_ieee_unordered_4_8 (X,Y)
      real(kind=4), intent(in) :: X
      real(kind=8), intent(in) :: Y
    end function
    elemental logical function _gfortran_ieee_unordered_8_4 (X,Y)
      real(kind=8), intent(in) :: X
      real(kind=4), intent(in) :: Y
    end function
    elemental logical function _gfortran_ieee_unordered_8_8 (X,Y)
      real(kind=8), intent(in) :: X
      real(kind=8), intent(in) :: Y
    end function
  end interface

  interface IEEE_UNORDERED
    procedure _gfortran_ieee_unordered_4_4, _gfortran_ieee_unordered_4_8, &
              _gfortran_ieee_unordered_8_4, _gfortran_ieee_unordered_8_8
  end interface
  public :: IEEE_UNORDERED

  ! IEEE_LOGB

  interface
    elemental real(kind=4) function _gfortran_ieee_logb_4 (X)
      real(kind=4), intent(in) :: X
    end function
    elemental real(kind=8) function _gfortran_ieee_logb_8 (X)
      real(kind=8), intent(in) :: X
    end function
  end interface

  interface IEEE_LOGB
    procedure _gfortran_ieee_logb_4, _gfortran_ieee_logb_8
  end interface
  public :: IEEE_LOGB

  ! IEEE_NEXT_AFTER

  interface
    elemental real(kind=4) function _gfortran_ieee_next_after_4_4 (X, Y)
      real(kind=4), intent(in) :: X
      real(kind=4), intent(in) :: Y
    end function
    elemental real(kind=4) function _gfortran_ieee_next_after_4_8 (X, Y)
      real(kind=4), intent(in) :: X
      real(kind=8), intent(in) :: Y
    end function
    elemental real(kind=8) function _gfortran_ieee_next_after_8_4 (X, Y)
      real(kind=8), intent(in) :: X
      real(kind=4), intent(in) :: Y
    end function
    elemental real(kind=8) function _gfortran_ieee_next_after_8_8 (X, Y)
      real(kind=8), intent(in) :: X
      real(kind=8), intent(in) :: Y
    end function
  end interface

  interface IEEE_NEXT_AFTER
    procedure _gfortran_ieee_next_after_4_4, _gfortran_ieee_next_after_4_8, &
              _gfortran_ieee_next_after_8_4, _gfortran_ieee_next_after_8_8
  end interface
  public :: IEEE_NEXT_AFTER

  ! IEEE_REM

  interface
    elemental real(kind=4) function _gfortran_ieee_rem_4_4 (X, Y)
      real(kind=4), intent(in) :: X
      real(kind=4), intent(in) :: Y
    end function
    elemental real(kind=8) function _gfortran_ieee_rem_4_8 (X, Y)
      real(kind=4), intent(in) :: X
      real(kind=8), intent(in) :: Y
    end function
    elemental real(kind=8) function _gfortran_ieee_rem_8_4 (X, Y)
      real(kind=8), intent(in) :: X
      real(kind=4), intent(in) :: Y
    end function
    elemental real(kind=8) function _gfortran_ieee_rem_8_8 (X, Y)
      real(kind=8), intent(in) :: X
      real(kind=8), intent(in) :: Y
    end function
  end interface

  interface IEEE_REM
    procedure _gfortran_ieee_rem_4_4, _gfortran_ieee_rem_4_8, &
              _gfortran_ieee_rem_8_4, _gfortran_ieee_rem_8_8
  end interface
  public :: IEEE_REM

  ! IEEE_RINT

  interface
    elemental real(kind=4) function _gfortran_ieee_rint_4 (X)
      real(kind=4), intent(in) :: X
    end function
    elemental real(kind=8) function _gfortran_ieee_rint_8 (X)
      real(kind=8), intent(in) :: X
    end function
  end interface

  interface IEEE_RINT
    procedure _gfortran_ieee_rint_4, _gfortran_ieee_rint_8
  end interface
  public :: IEEE_RINT

  ! IEEE_SCALB

  interface
    elemental real(kind=4) function _gfortran_ieee_scalb_4 (X, I)
      real(kind=4), intent(in) :: X
      integer, intent(in) :: I
    end function
    elemental real(kind=8) function _gfortran_ieee_scalb_8 (X, I)
      real(kind=8), intent(in) :: X
      integer, intent(in) :: I
    end function
  end interface

  interface IEEE_SCALB
    procedure _gfortran_ieee_scalb_4, _gfortran_ieee_scalb_8
  end interface
  public :: IEEE_SCALB

  ! IEEE_VALUE

  interface IEEE_VALUE
    module procedure IEEE_VALUE_4, IEEE_VALUE_8
  end interface
  public :: IEEE_VALUE

  ! IEEE_CLASS

  interface IEEE_CLASS
    module procedure IEEE_CLASS_4, IEEE_CLASS_8
  end interface
  public :: IEEE_CLASS

  ! Public declarations for contained procedures
  public :: IEEE_GET_ROUNDING_MODE, IEEE_SET_ROUNDING_MODE
  public :: IEEE_GET_UNDERFLOW_MODE, IEEE_SET_UNDERFLOW_MODE
  public :: IEEE_SELECTED_REAL_KIND

  ! IEEE_SUPPORT_ROUNDING

  interface IEEE_SUPPORT_ROUNDING
    module procedure IEEE_SUPPORT_ROUNDING_4, IEEE_SUPPORT_ROUNDING_8, &
#ifdef HAVE_GFC_REAL_10
                     IEEE_SUPPORT_ROUNDING_10, &
#endif
#ifdef HAVE_GFC_REAL_16
                     IEEE_SUPPORT_ROUNDING_16, &
#endif
                     IEEE_SUPPORT_ROUNDING_NOARG
  end interface
  public :: IEEE_SUPPORT_ROUNDING
  
  ! Interface to the FPU-specific function
  interface
    pure integer function support_rounding_helper(flag) &
        bind(c, name="_gfortrani_support_fpu_rounding_mode")
      integer, intent(in), value :: flag
    end function
  end interface

  ! IEEE_SUPPORT_UNDERFLOW_CONTROL

  interface IEEE_SUPPORT_UNDERFLOW_CONTROL
    module procedure IEEE_SUPPORT_UNDERFLOW_CONTROL_4, &
                     IEEE_SUPPORT_UNDERFLOW_CONTROL_8, &
#ifdef HAVE_GFC_REAL_10
                     IEEE_SUPPORT_UNDERFLOW_CONTROL_10, &
#endif
#ifdef HAVE_GFC_REAL_16
                     IEEE_SUPPORT_UNDERFLOW_CONTROL_16, &
#endif
                     IEEE_SUPPORT_UNDERFLOW_CONTROL_NOARG
  end interface
  public :: IEEE_SUPPORT_UNDERFLOW_CONTROL
  
  ! Interface to the FPU-specific function
  interface
    pure integer function support_underflow_control_helper(kind) &
        bind(c, name="_gfortrani_support_fpu_underflow_control")
      integer, intent(in), value :: kind
    end function
  end interface

! IEEE_SUPPORT_* generic functions

#if defined(HAVE_GFC_REAL_10) && defined(HAVE_GFC_REAL_16)
# define MACRO1(NAME) NAME/**/_4, NAME/**/_8, NAME/**/_10, NAME/**/_16, NAME/**/_NOARG
#elif defined(HAVE_GFC_REAL_10)
# define MACRO1(NAME) NAME/**/_4, NAME/**/_8, NAME/**/_10, NAME/**/_NOARG
#elif defined(HAVE_GFC_REAL_16)
# define MACRO1(NAME) NAME/**/_4, NAME/**/_8, NAME/**/_16, NAME/**/_NOARG
#else
# define MACRO1(NAME) NAME/**/_4, NAME/**/_8, NAME/**/_NOARG
#endif

#define SUPPORTGENERIC(NAME) \
  interface NAME ; module procedure MACRO1(NAME) ; end interface ; \
  public :: NAME

SUPPORTGENERIC(IEEE_SUPPORT_DATATYPE)
SUPPORTGENERIC(IEEE_SUPPORT_DENORMAL)
SUPPORTGENERIC(IEEE_SUPPORT_DIVIDE)
SUPPORTGENERIC(IEEE_SUPPORT_INF)
SUPPORTGENERIC(IEEE_SUPPORT_IO)
SUPPORTGENERIC(IEEE_SUPPORT_NAN)
SUPPORTGENERIC(IEEE_SUPPORT_SQRT)
SUPPORTGENERIC(IEEE_SUPPORT_STANDARD)

contains

  ! Equality operators for IEEE_CLASS_TYPE and IEEE_ROUNDING_MODE
  elemental logical function IEEE_CLASS_TYPE_EQ (X, Y) result(res)
    implicit none
    type(IEEE_CLASS_TYPE), intent(in) :: X, Y
    res = (X%hidden == Y%hidden)
  end function

  elemental logical function IEEE_CLASS_TYPE_NE (X, Y) result(res)
    implicit none
    type(IEEE_CLASS_TYPE), intent(in) :: X, Y
    res = (X%hidden /= Y%hidden)
  end function

  elemental logical function IEEE_ROUND_TYPE_EQ (X, Y) result(res)
    implicit none
    type(IEEE_ROUND_TYPE), intent(in) :: X, Y
    res = (X%hidden == Y%hidden)
  end function

  elemental logical function IEEE_ROUND_TYPE_NE (X, Y) result(res)
    implicit none
    type(IEEE_ROUND_TYPE), intent(in) :: X, Y
    res = (X%hidden /= Y%hidden)
  end function

  ! IEEE_SELECTED_REAL_KIND
  integer function IEEE_SELECTED_REAL_KIND (P, R, RADIX) result(res)
    implicit none
    integer, intent(in), optional :: P, R, RADIX
    integer :: p2, r2

    p2 = 0 ; r2 = 0
    if (present(p)) p2 = p
    if (present(r)) r2 = r

    ! The only IEEE types we support right now are binary
    if (present(radix)) then
      if (radix /= 2) then
        res = -5
        return
      endif
    endif

    ! Does IEEE float fit?
    if (precision(0.) >= p2 .and. range(0.) >= r2) then
      res = kind(0.)
      return
    endif

    ! Does IEEE double fit?
    if (precision(0.d0) >= p2 .and. range(0.d0) >= r2) then
      res = kind(0.d0)
      return
    endif

    if (precision(0.d0) < p2 .and. range(0.d0) < r2) then
      res = -3
      return
    endif

    if (precision(0.d0) < p2) then
      res = -1
      return
    endif

   res = -2
  end function


  ! IEEE_CLASS

  elemental function IEEE_CLASS_4 (X) result(res)
    implicit none
    real(kind=4), intent(in) :: X
    type(IEEE_CLASS_TYPE) :: res

    interface
      pure integer function _gfortrani_ieee_class_helper_4(val)
        real(kind=4), intent(in) :: val
      end function
    end interface

    res = IEEE_CLASS_TYPE(_gfortrani_ieee_class_helper_4(X))
  end function

  elemental function IEEE_CLASS_8 (X) result(res)
    implicit none
    real(kind=8), intent(in) :: X
    type(IEEE_CLASS_TYPE) :: res

    interface
      pure integer function _gfortrani_ieee_class_helper_8(val)
        real(kind=8), intent(in) :: val
      end function
    end interface

    res = IEEE_CLASS_TYPE(_gfortrani_ieee_class_helper_8(X))
  end function

  ! IEEE_VALUE

  elemental real(kind=4) function IEEE_VALUE_4(X, C) result(res)
    implicit none
    real(kind=4), intent(in) :: X
    type(IEEE_CLASS_TYPE), intent(in) :: C

    select case (C%hidden)
      case (1)     ! IEEE_SIGNALING_NAN
        res = -1
        res = sqrt(res)
      case (2)     ! IEEE_QUIET_NAN
        res = -1
        res = sqrt(res)
      case (3)     ! IEEE_NEGATIVE_INF
        res = huge(res)
        res = (-res) * res
      case (4)     ! IEEE_NEGATIVE_NORMAL
        res = -42
      case (5)     ! IEEE_NEGATIVE_DENORMAL
        res = -tiny(res)
        res = res / 2
      case (6)     ! IEEE_NEGATIVE_ZERO
        res = 0
        res = -res
      case (7)     ! IEEE_POSITIVE_ZERO
        res = 0
      case (8)     ! IEEE_POSITIVE_DENORMAL
        res = tiny(res)
        res = res / 2
      case (9)     ! IEEE_POSITIVE_NORMAL
        res = 42
      case (10)    ! IEEE_POSITIVE_INF
        res = huge(res)
        res = res * res
      case default ! IEEE_OTHER_VALUE, should not happen
        res = 0
     end select
  end function

  elemental real(kind=8) function IEEE_VALUE_8(X, C) result(res)
    implicit none
    real(kind=8), intent(in) :: X
    type(IEEE_CLASS_TYPE), intent(in) :: C

    select case (C%hidden)
      case (1)     ! IEEE_SIGNALING_NAN
        res = -1
        res = sqrt(res)
      case (2)     ! IEEE_QUIET_NAN
        res = -1
        res = sqrt(res)
      case (3)     ! IEEE_NEGATIVE_INF
        res = huge(res)
        res = (-res) * res
      case (4)     ! IEEE_NEGATIVE_NORMAL
        res = -42
      case (5)     ! IEEE_NEGATIVE_DENORMAL
        res = -tiny(res)
        res = res / 2
      case (6)     ! IEEE_NEGATIVE_ZERO
        res = 0
        res = -res
      case (7)     ! IEEE_POSITIVE_ZERO
        res = 0
      case (8)     ! IEEE_POSITIVE_DENORMAL
        res = tiny(res)
        res = res / 2
      case (9)     ! IEEE_POSITIVE_NORMAL
        res = 42
      case (10)    ! IEEE_POSITIVE_INF
        res = huge(res)
        res = res * res
      case default ! IEEE_OTHER_VALUE, should not happen
        res = 0
     end select
  end function


  ! IEEE_GET_ROUNDING_MODE

  subroutine IEEE_GET_ROUNDING_MODE (ROUND_VALUE)
    implicit none
    type(IEEE_ROUND_TYPE), intent(out) :: ROUND_VALUE

    interface
      integer function helper() &
        bind(c, name="_gfortrani_get_fpu_rounding_mode")
      end function
    end interface

    ROUND_VALUE = IEEE_ROUND_TYPE(helper())
  end subroutine


  ! IEEE_SET_ROUNDING_MODE

  subroutine IEEE_SET_ROUNDING_MODE (ROUND_VALUE)
    implicit none
    type(IEEE_ROUND_TYPE), intent(in) :: ROUND_VALUE

    interface
      subroutine helper(val) &
          bind(c, name="_gfortrani_set_fpu_rounding_mode")
        integer, value :: val
      end subroutine
    end interface
    
    call helper(ROUND_VALUE%hidden)
  end subroutine


  ! IEEE_GET_UNDERFLOW_MODE

  subroutine IEEE_GET_UNDERFLOW_MODE (GRADUAL)
    implicit none
    logical, intent(out) :: GRADUAL

    interface
      integer function helper() &
        bind(c, name="_gfortrani_get_fpu_underflow_mode")
      end function
    end interface

    GRADUAL = (helper() /= 0)
  end subroutine


  ! IEEE_SET_UNDERFLOW_MODE

  subroutine IEEE_SET_UNDERFLOW_MODE (GRADUAL)
    implicit none
    logical, intent(in) :: GRADUAL

    interface
      subroutine helper(val) &
          bind(c, name="_gfortrani_set_fpu_underflow_mode")
        integer, value :: val
      end subroutine
    end interface

    call helper(merge(1, 0, GRADUAL))
  end subroutine

! IEEE_SUPPORT_ROUNDING

  pure logical function IEEE_SUPPORT_ROUNDING_4 (ROUND_VALUE, X) result(res)
    implicit none
    real(kind=4), intent(in) :: X
    type(IEEE_ROUND_TYPE), intent(in) :: ROUND_VALUE
    res = (support_rounding_helper(ROUND_VALUE%hidden) /= 0)
  end function

  pure logical function IEEE_SUPPORT_ROUNDING_8 (ROUND_VALUE, X) result(res)
    implicit none
    real(kind=8), intent(in) :: X
    type(IEEE_ROUND_TYPE), intent(in) :: ROUND_VALUE
    res = (support_rounding_helper(ROUND_VALUE%hidden) /= 0)
  end function

#ifdef HAVE_GFC_REAL_10
  pure logical function IEEE_SUPPORT_ROUNDING_10 (ROUND_VALUE, X) result(res)
    implicit none
    real(kind=10), intent(in) :: X
    type(IEEE_ROUND_TYPE), intent(in) :: ROUND_VALUE
    res = .false.
  end function
#endif

#ifdef HAVE_GFC_REAL_16
  pure logical function IEEE_SUPPORT_ROUNDING_16 (ROUND_VALUE, X) result(res)
    implicit none
    real(kind=16), intent(in) :: X
    type(IEEE_ROUND_TYPE), intent(in) :: ROUND_VALUE
    res = .false.
  end function
#endif

  pure logical function IEEE_SUPPORT_ROUNDING_NOARG (ROUND_VALUE) result(res)
    implicit none
    type(IEEE_ROUND_TYPE), intent(in) :: ROUND_VALUE
#if defined(HAVE_GFC_REAL_10) || defined(HAVE_GFC_REAL_16)
    res = .false.
#else
    res = (support_rounding_helper(ROUND_VALUE%hidden) /= 0)
#endif
  end function

! IEEE_SUPPORT_UNDERFLOW_CONTROL

  pure logical function IEEE_SUPPORT_UNDERFLOW_CONTROL_4 (X) result(res)
    implicit none
    real(kind=4), intent(in) :: X
    res = (support_underflow_control_helper(4) /= 0)
  end function

  pure logical function IEEE_SUPPORT_UNDERFLOW_CONTROL_8 (X) result(res)
    implicit none
    real(kind=8), intent(in) :: X
    res = (support_underflow_control_helper(8) /= 0)
  end function

#ifdef HAVE_GFC_REAL_10
  pure logical function IEEE_SUPPORT_UNDERFLOW_CONTROL_10 (X) result(res)
    implicit none
    real(kind=10), intent(in) :: X
    res = .false.
  end function
#endif

#ifdef HAVE_GFC_REAL_16
  pure logical function IEEE_SUPPORT_UNDERFLOW_CONTROL_16 (X) result(res)
    implicit none
    real(kind=16), intent(in) :: X
    res = .false.
  end function
#endif

  pure logical function IEEE_SUPPORT_UNDERFLOW_CONTROL_NOARG () result(res)
    implicit none
#if defined(HAVE_GFC_REAL_10) || defined(HAVE_GFC_REAL_16)
    res = .false.
#else
    res = (support_underflow_control_helper(4) /= 0 &
           .and. support_underflow_control_helper(8) /= 0)
#endif
  end function

! IEEE_SUPPORT_* functions

#define SUPPORTMACRO(NAME, INTKIND, VALUE) \
  pure logical function NAME/**/_/**/INTKIND (X) result(res) ; \
    implicit none                                            ; \
    real(INTKIND), intent(in) :: X(..)                       ; \
    res = VALUE                                              ; \
  end function

#define SUPPORTMACRO_NOARG(NAME, VALUE) \
  pure logical function NAME/**/_NOARG () result(res) ; \
    implicit none                                     ; \
    res = VALUE                                       ; \
  end function

! IEEE_SUPPORT_DATATYPE

SUPPORTMACRO(IEEE_SUPPORT_DATATYPE,4,.true.)
SUPPORTMACRO(IEEE_SUPPORT_DATATYPE,8,.true.)
#ifdef HAVE_GFC_REAL_10
SUPPORTMACRO(IEEE_SUPPORT_DATATYPE,10,.false.)
#endif
#ifdef HAVE_GFC_REAL_16
SUPPORTMACRO(IEEE_SUPPORT_DATATYPE,16,.false.)
#endif
#if defined(HAVE_GFC_REAL_10) || defined(HAVE_GFC_REAL_16)
SUPPORTMACRO_NOARG(IEEE_SUPPORT_DATATYPE,.false.)
#else
SUPPORTMACRO_NOARG(IEEE_SUPPORT_DATATYPE,.true.)
#endif

! IEEE_SUPPORT_DENORMAL

SUPPORTMACRO(IEEE_SUPPORT_DENORMAL,4,.true.)
SUPPORTMACRO(IEEE_SUPPORT_DENORMAL,8,.true.)
#ifdef HAVE_GFC_REAL_10
SUPPORTMACRO(IEEE_SUPPORT_DENORMAL,10,.false.)
#endif
#ifdef HAVE_GFC_REAL_16
SUPPORTMACRO(IEEE_SUPPORT_DENORMAL,16,.false.)
#endif
#if defined(HAVE_GFC_REAL_10) || defined(HAVE_GFC_REAL_16)
SUPPORTMACRO_NOARG(IEEE_SUPPORT_DENORMAL,.false.)
#else
SUPPORTMACRO_NOARG(IEEE_SUPPORT_DENORMAL,.true.)
#endif

! IEEE_SUPPORT_DIVIDE

SUPPORTMACRO(IEEE_SUPPORT_DIVIDE,4,.true.)
SUPPORTMACRO(IEEE_SUPPORT_DIVIDE,8,.true.)
#ifdef HAVE_GFC_REAL_10
SUPPORTMACRO(IEEE_SUPPORT_DIVIDE,10,.false.)
#endif
#ifdef HAVE_GFC_REAL_16
SUPPORTMACRO(IEEE_SUPPORT_DIVIDE,16,.false.)
#endif
#if defined(HAVE_GFC_REAL_10) || defined(HAVE_GFC_REAL_16)
SUPPORTMACRO_NOARG(IEEE_SUPPORT_DIVIDE,.false.)
#else
SUPPORTMACRO_NOARG(IEEE_SUPPORT_DIVIDE,.true.)
#endif

! IEEE_SUPPORT_INF

SUPPORTMACRO(IEEE_SUPPORT_INF,4,.true.)
SUPPORTMACRO(IEEE_SUPPORT_INF,8,.true.)
#ifdef HAVE_GFC_REAL_10
SUPPORTMACRO(IEEE_SUPPORT_INF,10,.false.)
#endif
#ifdef HAVE_GFC_REAL_16
SUPPORTMACRO(IEEE_SUPPORT_INF,16,.false.)
#endif
#if defined(HAVE_GFC_REAL_10) || defined(HAVE_GFC_REAL_16)
SUPPORTMACRO_NOARG(IEEE_SUPPORT_INF,.false.)
#else
SUPPORTMACRO_NOARG(IEEE_SUPPORT_INF,.true.)
#endif

! IEEE_SUPPORT_IO

SUPPORTMACRO(IEEE_SUPPORT_IO,4,.true.)
SUPPORTMACRO(IEEE_SUPPORT_IO,8,.true.)
#ifdef HAVE_GFC_REAL_10
SUPPORTMACRO(IEEE_SUPPORT_IO,10,.false.)
#endif
#ifdef HAVE_GFC_REAL_16
SUPPORTMACRO(IEEE_SUPPORT_IO,16,.false.)
#endif
#if defined(HAVE_GFC_REAL_10) || defined(HAVE_GFC_REAL_16)
SUPPORTMACRO_NOARG(IEEE_SUPPORT_IO,.false.)
#else
SUPPORTMACRO_NOARG(IEEE_SUPPORT_IO,.true.)
#endif

! IEEE_SUPPORT_NAN

SUPPORTMACRO(IEEE_SUPPORT_NAN,4,.true.)
SUPPORTMACRO(IEEE_SUPPORT_NAN,8,.true.)
#ifdef HAVE_GFC_REAL_10
SUPPORTMACRO(IEEE_SUPPORT_NAN,10,.false.)
#endif
#ifdef HAVE_GFC_REAL_16
SUPPORTMACRO(IEEE_SUPPORT_NAN,16,.false.)
#endif
#if defined(HAVE_GFC_REAL_10) || defined(HAVE_GFC_REAL_16)
SUPPORTMACRO_NOARG(IEEE_SUPPORT_NAN,.false.)
#else
SUPPORTMACRO_NOARG(IEEE_SUPPORT_NAN,.true.)
#endif

! IEEE_SUPPORT_SQRT

SUPPORTMACRO(IEEE_SUPPORT_SQRT,4,.true.)
SUPPORTMACRO(IEEE_SUPPORT_SQRT,8,.true.)
#ifdef HAVE_GFC_REAL_10
SUPPORTMACRO(IEEE_SUPPORT_SQRT,10,.false.)
#endif
#ifdef HAVE_GFC_REAL_16
SUPPORTMACRO(IEEE_SUPPORT_SQRT,16,.false.)
#endif
#if defined(HAVE_GFC_REAL_10) || defined(HAVE_GFC_REAL_16)
SUPPORTMACRO_NOARG(IEEE_SUPPORT_SQRT,.false.)
#else
SUPPORTMACRO_NOARG(IEEE_SUPPORT_SQRT,.true.)
#endif

! IEEE_SUPPORT_STANDARD

SUPPORTMACRO(IEEE_SUPPORT_STANDARD,4,.true.)
SUPPORTMACRO(IEEE_SUPPORT_STANDARD,8,.true.)
#ifdef HAVE_GFC_REAL_10
SUPPORTMACRO(IEEE_SUPPORT_STANDARD,10,.false.)
#endif
#ifdef HAVE_GFC_REAL_16
SUPPORTMACRO(IEEE_SUPPORT_STANDARD,16,.false.)
#endif
#if defined(HAVE_GFC_REAL_10) || defined(HAVE_GFC_REAL_16)
SUPPORTMACRO_NOARG(IEEE_SUPPORT_STANDARD,.false.)
#else
SUPPORTMACRO_NOARG(IEEE_SUPPORT_STANDARD,.true.)
#endif

end module IEEE_ARITHMETIC

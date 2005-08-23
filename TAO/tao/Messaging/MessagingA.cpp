// -*- C++ -*-
//
// $Id$

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

#include "tao/AnyTypeCode/Null_RefCount_Policy.h"
#include "tao/AnyTypeCode/TypeCode_Constants.h"
#include "tao/AnyTypeCode/Alias_TypeCode_Static.h"
#include "tao/AnyTypeCode/Objref_TypeCode_Static.h"
#include "tao/AnyTypeCode/Value_TypeCode_Static.h"
#include "tao/AnyTypeCode/TypeCode_Value_Field.h"
#include "tao/AnyTypeCode/Recursive_Type_TypeCode.h"
#include "Messaging.h"
#include "tao/CDR.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/AnyTypeCode/Any_Impl_T.h"
#include "tao/AnyTypeCode/OctetSeqA.h"

// TAO_IDL - Generated from
// be\be_visitor_typecode/value_typecode.cpp:61



// TAO_IDL - Generated from
// be\be_visitor_typecode/typecode_defn.cpp:937

static TAO::TypeCode::Value_Field<char const *, CORBA::TypeCode_ptr const *> const _tao_fields_Messaging_ExceptionHolder[] =
  {
    { "is_system_exception", &CORBA::_tc_boolean, CORBA::PUBLIC_MEMBER },
    { "byte_order", &CORBA::_tc_boolean, CORBA::PUBLIC_MEMBER },
    { "marshaled_exception", &CORBA::_tc_OctetSeq, CORBA::PUBLIC_MEMBER }

  };
static TAO::TypeCode::Value<char const *,
                     CORBA::TypeCode_ptr const *,
                     TAO::TypeCode::Value_Field<char const *, CORBA::TypeCode_ptr const *> const *,
                     TAO::Null_RefCount_Policy>
  _tao_tc_Messaging_ExceptionHolder (
    CORBA::tk_value,
    "IDL:omg.org/Messaging/ExceptionHolder:1.0",
    "ExceptionHolder",
    CORBA::VM_NONE,
    &CORBA::_tc_null,
    _tao_fields_Messaging_ExceptionHolder,
    3);

namespace Messaging
{
  ::CORBA::TypeCode_ptr const _tc_ExceptionHolder =
    &_tao_tc_Messaging_ExceptionHolder;
}



// TAO_IDL - Generated from
// be\be_visitor_typecode/objref_typecode.cpp:76

static TAO::TypeCode::Objref<char const *,
                             TAO::Null_RefCount_Policy>
  _tao_tc_Messaging_ReplyHandler (
    CORBA::tk_objref,
    "IDL:omg.org/Messaging/ReplyHandler:1.0",
    "ReplyHandler");

namespace Messaging
{
  ::CORBA::TypeCode_ptr const _tc_ReplyHandler =
    &_tao_tc_Messaging_ReplyHandler;
}



// TAO_IDL - Generated from
// be\be_visitor_valuetype/any_op_cs.cpp:57

namespace TAO
{
  template<>
  CORBA::Boolean
  Any_Impl_T<Messaging::ExceptionHolder>::to_value (
      CORBA::ValueBase *&_tao_elem
    ) const
  {
    CORBA::add_ref (this->value_);
    _tao_elem = this->value_;
    return 1;
  }
}

// Copying insertion.
void
operator<<= (
    CORBA::Any &_tao_any,
    Messaging::ExceptionHolder *_tao_elem
  )
{
  CORBA::add_ref (_tao_elem);
  _tao_any <<= &_tao_elem;
}

// Non-copying insertion.
void
operator<<= (
    CORBA::Any &_tao_any,
    Messaging::ExceptionHolder **_tao_elem
  )
{
  TAO::Any_Impl_T<Messaging::ExceptionHolder>::insert (
      _tao_any,
      Messaging::ExceptionHolder::_tao_any_destructor,
      Messaging::_tc_ExceptionHolder,
      *_tao_elem
    );
}

CORBA::Boolean
operator>>= (
    const CORBA::Any &_tao_any,
    Messaging::ExceptionHolder *&_tao_elem
  )
{
  return
    TAO::Any_Impl_T<Messaging::ExceptionHolder>::extract (
        _tao_any,
        Messaging::ExceptionHolder::_tao_any_destructor,
        Messaging::_tc_ExceptionHolder,
        _tao_elem
      );
}



// TAO_IDL - Generated from
// be\be_visitor_interface/any_op_cs.cpp:50

namespace TAO
{
  template<>
  CORBA::Boolean
  Any_Impl_T<Messaging::ReplyHandler>::to_object (
      CORBA::Object_ptr &_tao_elem
    ) const
  {
    _tao_elem = CORBA::Object::_duplicate (this->value_);
    return true;
  }
}

// Copying insertion.
void
operator<<= (
    CORBA::Any &_tao_any,
    Messaging::ReplyHandler_ptr _tao_elem
  )
{
  Messaging::ReplyHandler_ptr _tao_objptr =
    Messaging::ReplyHandler::_duplicate (_tao_elem);
  _tao_any <<= &_tao_objptr;
}

// Non-copying insertion.
void
operator<<= (
    CORBA::Any &_tao_any,
    Messaging::ReplyHandler_ptr *_tao_elem
  )
{
  TAO::Any_Impl_T<Messaging::ReplyHandler>::insert (
      _tao_any,
      Messaging::ReplyHandler::_tao_any_destructor,
      Messaging::_tc_ReplyHandler,
      *_tao_elem
    );
}

CORBA::Boolean
operator>>= (
    const CORBA::Any &_tao_any,
    Messaging::ReplyHandler_ptr &_tao_elem
  )
{
  return
    TAO::Any_Impl_T<Messaging::ReplyHandler>::extract (
        _tao_any,
        Messaging::ReplyHandler::_tao_any_destructor,
        Messaging::_tc_ReplyHandler,
        _tao_elem
      );
}

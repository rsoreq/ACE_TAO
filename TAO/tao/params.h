/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    params.h
//
// = AUTHOR
//    Chris Cleeland
//
// ============================================================================

#ifndef TAO_PARAMS_H
#define TAO_PARAMS_H

#include "tao/corbafwd.h"
#include "tao/IOR_LookupTable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward decls.

class TAO_Active_Object_Map_Impl;
class TAO_Reverse_Active_Object_Map_Impl;
class TAO_IOR_LookupTable;

// This is a quick hack to avoid having to unravel the intricacies of
// the all the hairy order interdepencies that currently exist in TAO.
// #if ! defined (__ACE_INLINE__)
// #define TAO_LOCAL_INLINE
// #else
// #define TAO_LOCAL_INLINE ACE_INLINE
// #endif /* ! __ACE_INLINE__ */

typedef ACE_Unbounded_Set<ACE_CString> TAO_EndpointSet;
typedef ACE_Unbounded_Set_Iterator<ACE_CString> TAO_EndpointSetIterator;

// @@ Using an ACE_Unbounded_Queue to contain the preconnects may not
//    be the best container to use.  However, it will only be used
//    during client side initialization.  Also, a template
//    instantiation of ACE_Unbounded_Queue<ACE_CString> already exists
//    in ACE so we do not have to worry about increasing TAO's
//    footprint by using this container.
typedef ACE_Unbounded_Queue<ACE_CString> TAO_PreconnectSet;
typedef ACE_Unbounded_Queue_Iterator<ACE_CString> TAO_PreconnectSetIterator;

// -------------------------------------------------------------------
// The following utility classes are used to insert endpoints and
// preconnects into their corresponding containers, but at the same
// time allow the same parsing code to be used for endpoints and
// preconnects.

// Base class that provides interface for insertion of
class TAO_Base_Endpoint_Insertion_Strategy
{
public:
  virtual int insert (const ACE_CString &endpoint) = 0;
};

// Utility class to insert endpoints into endpoint container.
class TAO_Endpoint_Insertion_Strategy
  : public TAO_Base_Endpoint_Insertion_Strategy
{
public:
  TAO_Endpoint_Insertion_Strategy (TAO_EndpointSet &endpoint_set);

  virtual int insert (const ACE_CString &endpoint);

private:
  TAO_EndpointSet &endpoints_;
};

// Utility class to insert preconnects into preconnect container.
class TAO_Preconnect_Insertion_Strategy
  : public TAO_Base_Endpoint_Insertion_Strategy
{
public:
  TAO_Preconnect_Insertion_Strategy (TAO_PreconnectSet &preconnect_set);

  virtual int insert (const ACE_CString &preconnect);

private:
  TAO_PreconnectSet &preconnects_;
};
// -------------------------------------------------------------------

class TAO_Export TAO_ORB_Parameters
  // = TITLE
  //    Parameters that are specific to the ORB.  These parameters can
  //    be for the client, the server, or for both.
  //
  // = NOTE
  // Some of the functions have inline definitions in the class header
  // below.  Please do not move them back to the .i file.
  // cygnus-2.7.2-960126, distributed with Tornado 1.0.1, can't deal
  // with returing a const reference here.  It raises an internal
  // compiler error at ace/Svc_Handler.cpp:46, of all places.
{
public:
  TAO_ORB_Parameters (void);
  // Constructor.

  ~TAO_ORB_Parameters (void);
  // Destructor.

  int preconnects (ACE_CString &preconnects);
  TAO_PreconnectSet &preconnects (void);
  void add_preconnect (ACE_CString &preconnect);
  // Specifies the endpoints to pre-establish connections on.

  int endpoints (ACE_CString &endpoints);
  TAO_EndpointSet &endpoints (void);
  void add_endpoint (ACE_CString &endpoint);
  // Specifies the endpoints on which this server is willing to
  // listen for requests.

  CORBA::UShort name_service_port (void) const;
  void name_service_port (CORBA::UShort port);
  // Set/Get the port of our name service.

  CORBA::UShort trading_service_port (void) const;
  void trading_service_port (CORBA::UShort port);
  // Set/Get the port of our trading service.

  CORBA::UShort implrepo_service_port (void) const;
  void implrepo_service_port (CORBA::UShort port);
  // Set/Get the port of the Implementation Repository service.

  int sock_rcvbuf_size (void) const;
  void sock_rcvbuf_size (int);
  // Set/Get the size to be used for a socket's receive buffer.

  int sock_sndbuf_size (void) const;
  void sock_sndbuf_size (int);
  // Set/Get the size to be used for a socket's send buffer.

  int nodelay (void) const;
  void nodelay (int);
  // Set/Get the status of whether to use TCP_NODELAY or not.

  int cdr_memcpy_tradeoff (void) const;
  void cdr_memcpy_tradeoff (int);
  // Octet sequences are marshalled without doing any copies, we
  // simply append a block to the CDR message block chain. When the
  // octet sequence is small enough and there is room in the current
  // message block it is more efficient just to copy the buffer.

  int use_lite_protocol (void) const;
  void use_lite_protocol (int);
  // The ORB will use a modified version of GIOP that minimizes the
  // header size. By default we use the standard GIOP protocol.

  int use_dotted_decimal_addresses (void) const;
  void use_dotted_decimal_addresses (int);
  // The ORB will use the dotted decimal notation for addresses. By
  // default we use the full ascii names.

  TAO_IOR_LookupTable * ior_lookup_table (void);
  // The table used by the ORB for looking up the ObjectID:IOR mappings
  // specified on the commandline through the -ORBInitRef and
  // -ORBDefaultInitRef parameters.

  int add_to_ior_table (ACE_CString init_ref);
  // Add the init_ref (objectID->IOR) to the Lookup Table

  char *default_init_ref (void) const;
  void default_init_ref (const ACE_CString &default_init_ref);
  // Set/Get the Init Reference of an arbitrary ObjectID.

  int std_profile_components (void) const;
  void std_profile_components (int x);
  // Disable the OMG standard profile components, useful for
  // homogenous environments.

private:
  // Each "endpoint" is of the form:
  //
  //   protocol://V.v@addr1,...,W.w@addrN/
  //
  // or:
  //
  //   protocol://addr1,addr2,...,addrN/
  //
  // where "V.v" and "W.w" are optional versions.
  //
  // Multiple sets of endpoints may be seperated by a semi-colon `;'.
  // For example:
  //
  //   iiop://space:2001,odyssey:2010;uiop://foo,bar
  //
  // All preconnect or endpoint strings should be of the above form(s).

  int parse_endpoints (ACE_CString &endpoints,
                       TAO_Base_Endpoint_Insertion_Strategy &endpoints_list);

  TAO_PreconnectSet preconnects_list_;
  // List of endpoints used to pre-establish connections.

  TAO_Preconnect_Insertion_Strategy preconnect_insertion_strategy_;
  // Strategy used to insert endpoints into the endpoint container.

  TAO_EndpointSet endpoints_list_;
  // List of endpoints this server is willing to accept requests
  // on.

  TAO_Endpoint_Insertion_Strategy endpoint_insertion_strategy_;
  // Strategy used to insert endpoints into the endpoint container.

  CORBA::UShort name_service_port_;
  // The port number of our configured Naming Service.

  CORBA::UShort trading_service_port_;
  // The port number of our configured Trading Service.

  CORBA::UShort implrepo_service_port_;
  // The port number of our configured Implementation Repository.

  ACE_CString init_ref_;
  // Initial Reference supplied as <ObjectID>:<IOR>

  TAO_IOR_LookupTable ior_lookup_table_;
  // Table that has the mapping <ObjectID>:<IOR>
  // The IOR could be in any of the following formats :
  // IOR: ...  / iiop: ...  / iioploc: ... / iiopname: ...

  ACE_CString default_init_ref_;
  // List of comma separated prefixes from ORBDefaultInitRef.

  int sock_rcvbuf_size_;
  // Size to be used for a socket's receive buffer.

  int sock_sndbuf_size_;
  // Size to be used for a socket's send buffer.

  int nodelay_;
  // 1 if we're using TCP_NODELAY and 0 otherwise.

  int cdr_memcpy_tradeoff_;
  // Control the strategy for copying vs. appeding octet sequences in
  // CDR streams.

  int use_lite_protocol_;
  // For selecting a liteweight version of the GIOP protocol.

  int use_dotted_decimal_addresses_;
  // For selecting a address notation

  int std_profile_components_;
  // If true then the standard OMG components are not generated.
};

#if defined (__ACE_INLINE__)
# include "tao/params.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PARAMS_H */

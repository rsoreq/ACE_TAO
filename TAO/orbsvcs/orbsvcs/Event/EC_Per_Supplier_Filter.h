/* -*- C++ -*- */
/**
 *  @file   EC_Per_Supplier_Filter.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_PER_SUPPLIER_FILTER_H
#define TAO_EC_PER_SUPPLIER_FILTER_H
#include "ace/pre.h"

#include "EC_Supplier_Filter.h"
#include "EC_Supplier_Filter_Builder.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class PROXY> class TAO_ESF_Proxy_Collection;
class TAO_EC_Event_Channel_Base;

/**
 * @class TAO_EC_Per_Supplier_Filter
 *
 * @brief Filter the events on each supplier.
 *
 * This is a filtering strategy for the suppliers. In this
 * particular case we keep a collection of the consumers that
 * could potentially be interested in any event generated by a
 * particular supplier.
 * This minimizes the amount of consumers touched by the EC when
 * dispatching an event.
 */
class TAO_RTEvent_Export TAO_EC_Per_Supplier_Filter : public TAO_EC_Supplier_Filter
{
public:
  /// Constructor
  TAO_EC_Per_Supplier_Filter (TAO_EC_Event_Channel_Base* ec);

  /// Destructor
  virtual ~TAO_EC_Per_Supplier_Filter (void);

  // = The TAO_EC_Supplier_Filter methods.
  virtual void bind (TAO_EC_ProxyPushConsumer* consumer);
  virtual void unbind (TAO_EC_ProxyPushConsumer* consumer);
  virtual void connected (TAO_EC_ProxyPushSupplier* supplier
                          ACE_ENV_ARG_DECL);
  virtual void reconnected (TAO_EC_ProxyPushSupplier* supplier
                            ACE_ENV_ARG_DECL);
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier
                             ACE_ENV_ARG_DECL);
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_ProxyPushConsumer *consumer
                     ACE_ENV_ARG_DECL_NOT_USED);
  virtual void push_scheduled_event (RtecEventComm::EventSet &event,
                                     const TAO_EC_QOS_Info &event_info
                                     ACE_ENV_ARG_DECL);
  virtual CORBA::ULong _decr_refcnt (void);
  virtual CORBA::ULong _incr_refcnt (void);

private:
  /// The event channel, used to locate the set of consumers.
  TAO_EC_Event_Channel_Base *event_channel_;

  /// The proxy for the supplier we are bound to.
  TAO_EC_ProxyPushConsumer* consumer_;

  /// Keep the collection of proxies for the consumers that may be
  /// interested in our events.
  TAO_ESF_Proxy_Collection<TAO_EC_ProxyPushSupplier>* collection_;

  /// Reference counting
  CORBA::ULong refcnt_;

  /// Locking
  TAO_SYNCH_MUTEX lock_;
};

// ****************************************************************

/**
 * @class TAO_EC_Per_Supplier_Filter_Builder
 *
 * @brief Create Per_Supplier_Filter objects
 *
 */
class TAO_RTEvent_Export TAO_EC_Per_Supplier_Filter_Builder : public TAO_EC_Supplier_Filter_Builder
{
public:
  /// constructor....
  TAO_EC_Per_Supplier_Filter_Builder (TAO_EC_Event_Channel_Base* ec);

  // = The TAO_EC_Supplier_Filter_Builder methods...
  virtual TAO_EC_Supplier_Filter*
      create (RtecEventChannelAdmin::SupplierQOS& qos);
  virtual void
      destroy (TAO_EC_Supplier_Filter *filter);

private:
  /// The event channel
  TAO_EC_Event_Channel_Base* event_channel_;
};

#if defined (__ACE_INLINE__)
#include "EC_Per_Supplier_Filter.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_PER_SUPPLIER_FILTER_H */

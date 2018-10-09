/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Leonard Tracy <lentracy@gmail.com>
 *         To Thanh Hai <tthhai@gmail.com>
 */

#ifndef LORA_NET_DEVICE_H
#define LORA_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/pointer.h"
#include "ns3/traced-callback.h"
#include "lora-address.h"
#include <list>

#include <cstring>
#include "ns3/node.h"
#include "ns3/address.h"
#include "ns3/net-device.h"
#include "ns3/callback.h"
#include "ns3/packet.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/ptr.h"
#include "ns3/mac48-address.h"

#include "lora-phy.h"

namespace ns3 {

class LoraChannel;
class LoraPhy;
class LoraMac;
class LoraTransducer;

/**
 * Net device for LORA models.
 */
class LoraNetDevice : public NetDevice
{
public:
  /** List of LoraPhy objects. */
  typedef std::list<Ptr<LoraPhy> > LoraPhyList;
  /** List of LoraTransducer objects. */
  typedef std::list<Ptr<LoraTransducer> > LoraTransducerList;

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);

  /** Default constructor */
  LoraNetDevice ();
  /** Dummy destructor, DoDispose. */
  virtual ~LoraNetDevice ();

  /**
   * Set the MAC layer for this device.
   * 
   * \param mac The MAC layer.
   */
  void SetMac (Ptr<LoraMac> mac);

  /**
   * Set the Phy layer for this device.
   *   
   * \param phy The PHY layer.
   */
  void SetPhy (Ptr<LoraPhy> phy);

  /**
   * Attach a channel.
   *
   * \param channel The channel.
   */
  void SetChannel (Ptr<LoraChannel> channel);

  /**
   * Get the MAC used by this device.
   *
   * \return The MAC.
   */
  Ptr<LoraMac> GetMac (void) const;

  /**
   * Get the Phy used by this device.
   *
   * \return The Phy.
   */
  Ptr<LoraPhy> GetPhy (void) const;

  /**
   * Get the transducer associated with this device.
   *
   * \return The transducer.
   */
  Ptr<LoraTransducer> GetTransducer (void) const;
  /**
   * Set the transdcuer used by this device.
   *
   * \param trans The transducer.
   */
  void SetTransducer (Ptr<LoraTransducer> trans);

  /** Clear all pointer references. */
  void Clear (void);

  /**
   * Set the Phy SLEEP mode.
   *
   * \param sleep SLEEP on or off.
   */
  void SetSleepMode (bool sleep);


  void SetChannelMode (uint32_t mode);
  uint32_t GetChannelMode (void);

  void SetTransmitStartTime (uint32_t transmitStartTime);
  double GetTransmitStartTime (void);

  bool SendCsma (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  bool SendFromCsma (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);

  virtual Address GetGWAddress (void) const;
  virtual void SetGWAddress (Address gwAddress);


  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
  virtual Ptr<Channel> GetChannel (void) const;
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual Address GetMulticast (Ipv6Address addr) const;
  virtual bool IsBridge (void) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual Ptr<Node> GetNode (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual bool NeedsArp (void) const;
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom (void) const;
  virtual void AddLinkChangeCallback (Callback<void> callback);
  virtual void SetAddress (Address address);

  /**
   * TracedCallback signature for MAC send/receive events.
   *
   * \param [in] packet The Packet.
   * \param [in] address The source address.
   */
  typedef void (* RxTxTracedCallback)
    (const Ptr<const Packet> packet, const LoraAddress address);
  
private:
  /**
   * Forward the packet to a higher level, set with SetReceiveCallback.
   *
   * \param pkt The packet.
   * \param src The source address.
   */
  virtual void ForwardUp (Ptr<Packet> pkt, const LoraAddress &src);
  
  /** \return The channel attached to this device. */
  Ptr<LoraChannel> DoGetChannel (void) const;

  Ptr<LoraTransducer> m_trans;      //!< The Transducer attached to this device.
  Ptr<Node> m_node;                //!< The node hosting this device.
  Ptr<LoraChannel> m_channel;       //!< The channel attached to this device.
  Ptr<LoraMac> m_mac;               //!< The MAC layer attached to this device.
  Ptr<LoraPhy> m_phy;               //!< The PHY layer attached to this device.

  LoraTxMode m_txMode;

  uint32_t m_ifIndex;              //!< The interface index of this device.
  uint16_t m_mtu;                  //!< The device MTU value, in bytes.
  bool m_linkup;                   //!< The link state, true if up.
  TracedCallback<> m_linkChanges;  //!< Callback to invoke when the link state changes to UP.
  ReceiveCallback m_forwardUp;     //!< The receive callback.


  uint32_t v_mode;
  double v_transmitStartTime;

  /** Trace source triggered when forwarding up received payload from the MAC layer. */
  TracedCallback<Ptr<const Packet>, LoraAddress> m_rxLogger;
  /** Trace source triggered when sending to the MAC layer */
  TracedCallback<Ptr<const Packet>, LoraAddress> m_txLogger;

  /** Flag when we've been cleared. */
  bool m_cleared;


/**
   * Start Sending a Packet Down the Wire.
   *
   * The TransmitStart method is the method that is used internally in
   * the CsmaNetDevice to begin the process of sending a packet
   * out on the channel.  A corresponding method is called on the
   * channel to let it know that the physical device this class
   * represents has actually started sending signals, this causes the
   * channel to enter the BUSY state.  An event is scheduled for the time at
   * which the bits have been completely transmitted. 
   *
   * If the channel is found to be BUSY, this method reschedules itself for
   * execution at a later time (within the backoff period).
   *
   */
  void TransmitStart ();


  /**
   * Stop Sending a Packet Down the Wire and Begin the Interframe Gap.
   *
   * The TransmitCompleteEvent method is used internally to finish the process
   * of sending a packet out on the channel.  During execution of this method
   * the TransmitEnd method is called on the channel to let it know that the
   * physical device this class represents has finished sending simulated
   * signals.  The channel uses this event to begin its speed of light delay
   * timer after which it notifies the Net Device(s) at the other end of the 
   * link that new bits have arrived (it delivers the Packet).  During this 
   * method, the net device also schedules the TransmitReadyEvent at which
   * time the transmitter becomes ready to send the next packet.
   *
   * \see CsmaChannel::TransmitEnd ()
   * \see TransmitReadyEvent ()
   */
  void TransmitCompleteEvent (void);

  /**
   * Cause the Transmitter to Become Ready to Send Another Packet.
   *
   * The TransmitReadyEvent method is used internally to re-enable the 
   * transmit machine of the net device.  It is scheduled after a suitable
   * interframe gap after the completion of the previous transmission.
   * The queue is checked at this time, and if there is a packet waiting on
   * the queue, the transmission process is begun.
   *
   * If a packet is in the queue, it is extracted for the queue as the
   * next packet to be transmitted by the net device.
   *
   * \see TransmitStart ()
   */
  void TransmitReadyEvent (void);

  /**
   * Aborts the transmission of the current packet
   *
   * If the net device has tried to transmit a packet for more times
   * than the maximum allowed number of retries (channel always busy)
   * then the packet is dropped.
   */
  void TransmitAbort (void);


  /**
   * Enumeration of the states of the transmit machine of the net device.
   */
  enum TxMachineState
  {
    READY,   /**< The transmitter is ready to begin transmission of a packet */
    BUSY,    /**< The transmitter is busy transmitting a packet */
    GAP,      /**< The transmitter is in the interframe gap time */
//    BACKOFF      /**< The transmitter is waiting for the channel to be free */
  };

  /**
   * The state of the Net Device transmit state machine.
   * \see TxMachineState
   */
  TxMachineState m_txMachineState;
  
  /**
   * The data rate that the Net Device uses to simulate packet transmission
   * timing.
   * \see class DataRate
   */
  DataRate m_bps;

  /**
   * The interframe gap that the Net Device uses insert time between packet
   * transmission
   * \see class Time
   */
  Time m_tInterframeGap;

  /**
   * Holds the backoff parameters and is used to calculate the next
   * backoff time to use when the channel is busy and the net device
   * is ready to transmit
   */
//  Backoff m_backoff;

  /**
   * Next packet that will be transmitted (if transmitter is not
   * currently transmitting) or packet that is currently being
   * transmitted.
   */
  Ptr<Packet> m_currentPkt;

  /**
   * Save destination and protocolNumber for TransmitStart
   */

  Address m_dest;  
  Address  m_gwAddress;
  uint16_t m_protocolNumber;

protected:
  virtual void DoDispose ();

};  // class LoraNetDevice

} // namespace ns3

#endif /* LORA_NET_DEVICE_H */

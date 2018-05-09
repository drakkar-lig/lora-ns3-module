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

#ifndef LORA_TRANSDUCER_H
#define LORA_TRANSDUCER_H

#include "ns3/object.h"
#include "ns3/packet.h"
#include "lora-tx-mode.h"
#include "ns3/lora-prop-model.h"

#include <list>

namespace ns3 {

class LoraPhy;
class LoraChannel;


/**
 *
 * Class consisting of packet arrival information (Time, RxPower, mode, PDP).
 */
class LoraPacketArrival
{
public:

  /** Default constructor. */
  LoraPacketArrival () 
  {
  }

  /**
   * Constructor.
   *
   * \param packet Packet arriving.
   * \param rxPowerDb RX signal power in dB of arriving packet.
   * \param txMode TX mode of arriving packet.
   * \param pdp Power delay profile of arriving packet.
   * \param arrTime Arrival time of packet.
   */
  LoraPacketArrival (Ptr<Packet> packet, double rxPowerDb, LoraTxMode txMode, LoraPdp pdp, Time arrTime)
    : m_packet (packet),
      m_rxPowerDb (rxPowerDb),
      m_txMode (txMode),
      m_pdp (pdp),
      m_arrTime (arrTime)
  {
  }

  /** Destructor */
  ~LoraPacketArrival()
  {
    m_packet = 0;
  }

  /**
   * Get the arriving packet.
   *
   * \return Pointer to packet.
   */
  inline Ptr<Packet> GetPacket (void) const
  {
    return m_packet;
  }
  /**
   * Get the received signal strength.
   *
   * \return Received signal strength in dB re 1uPa
   */
  inline double GetRxPowerDb (void) const
  {
    return m_rxPowerDb;
  }
  /**
   * Get the transmission mode of the packet.
   *
   * \return LoraTxMode.
   */
  inline const LoraTxMode &GetTxMode (void) const
  {
    return m_txMode;
  }
  /**
   * Get the packet arrival time.
   *
   * \return Arrival time.
   */
  inline Time GetArrivalTime (void) const
  {
    return m_arrTime;
  }
  /**
   * Get the propagation delay profile.
   *
   * \return PDP of arriving signal.
   */
  inline LoraPdp GetPdp (void) const
  {
    return m_pdp;
  }
private:
  Ptr<Packet> m_packet;  //!< The arrived packet.
  double m_rxPowerDb;    //!< The received power, in dB.
  LoraTxMode m_txMode;    //!< The transmission mode.
  LoraPdp m_pdp;          //!< The propagation delay profile.
  Time m_arrTime;        //!< The arrival time.

};  // class LoraPacketArrival

/**
 *
 * Virtual base for Transducer objects.
 *
 */
class LoraTransducer : public Object
{
public:
  /**
   * Register this type.
   * \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  /** Transducer state. */
  enum State {
    TX,  //!< Transmitting.
    RX   //!< Receiving.
  };

  /** List of arriving packets overlapping in time. */
  typedef std::list<LoraPacketArrival> ArrivalList;
  /** List of LoraPhy objects. */
  typedef std::list<Ptr<LoraPhy> > LoraPhyList;

  /**
   * Get the transducer state.
   *
   * \return State (TX or RX) of this transducer.
   */
  virtual State GetState (void) const = 0;

  /**
   * Is the state receiving (or available for reception)?
   *
   * \return True if this transducer is available for receiving
   *   an incoming packet.
   */
  virtual bool IsRx (void) const = 0;
  /**
   * Is the state transmitting?
   * 
   * \return True if there is a packet being transmitted from this transducer.
   */
  virtual bool IsTx (void) const = 0;
  /**
   * Get the list of overlapped (in time) packets at this transducer.
   *
   * \return List of all packets currently crossing this node in the water.
   */
  virtual const ArrivalList &GetArrivalList (void) const = 0;
  /**
   * Notify this object that a new packet has arrived at this nodes location
   *
   * \param packet Packet arriving.
   * \param rxPowerDb Signal power in dB of arriving packet.
   * \param txMode Mode arriving packet is using.
   * \param pdp PDP of arriving signal.
   */
  virtual void Receive (Ptr<Packet> packet, double rxPowerDb, LoraTxMode txMode, LoraPdp pdp) = 0;
  /**
   * Transmit a packet from this transducer.
   *
   * \param src Source PHY.
   * \param packet Packet to transmit.
   * \param txPowerDb Outgoing Tx power of packet.
   * \param txMode Mode to transmit packet with.
   */
  virtual void Transmit (Ptr<LoraPhy> src, Ptr<Packet> packet, double txPowerDb, LoraTxMode txMode) = 0;
  /**
   * Attach this transducer to a channel.
   * 
   * \param chan The channel
   */
  virtual void SetChannel (Ptr<LoraChannel> chan) = 0;
  /**
   * Get the attached channel.
   *
   * \return The channel.
   */
  virtual Ptr<LoraChannel> GetChannel (void) const = 0;
  /**
   * Attach a physical network layer above this transducer.
   *
   * More than one physical layer may be attached.
   *
   * \param phy The physical layer.
   */
  virtual void AddPhy (Ptr<LoraPhy> phy) = 0;
  /**
   * Get the list of physical layer above this transducer.
   *
   * \return List of attached physical layers.
   */
  virtual const LoraPhyList &GetPhyList (void) const = 0;
  /**
   * Clears all pointer references.
   */
  virtual void Clear (void) = 0;

};  // class LoraTransducer

} // namespace ns3

#endif /* LORA_TRANSDUCER_H */

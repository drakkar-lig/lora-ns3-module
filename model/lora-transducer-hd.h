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

#ifndef LORA_TRANSDUCER_HD_H
#define LORA_TRANSDUCER_HD_H

#include "lora-transducer.h"
#include "ns3/simulator.h"
namespace ns3 {

/**
 *
 * Half duplex implementation of transducer object
 *
 * This class will only allow attached Phy's to receive packets
 * if not in TX mode.
 */
class LoraTransducerHd : public LoraTransducer
{
public:
  /** Constructor */
  LoraTransducerHd ();
  /** Dummy destructor, see DoDispose */
  virtual ~LoraTransducerHd ();

  /**
   * Register this type.
   * \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  // inherited methods
  virtual State GetState (void) const;
  virtual bool IsRx (void) const;
  virtual bool IsTx (void) const;
  virtual const ArrivalList &GetArrivalList (void) const;
  virtual void Receive (Ptr<Packet> packet, double rxPowerDb, LoraTxMode txMode, LoraPdp pdp);
  virtual void Transmit (Ptr<LoraPhy> src, Ptr<Packet> packet, double txPowerDb, LoraTxMode txMode);
  virtual void SetChannel (Ptr<LoraChannel> chan);
  virtual Ptr<LoraChannel> GetChannel (void) const;
  virtual void AddPhy (Ptr<LoraPhy>);
  virtual const LoraPhyList &GetPhyList (void) const;
  virtual void Clear (void);

private:
  State m_state;              //!< Transducer state.
  ArrivalList m_arrivalList;  //!< List of arriving packets which overlap in time.
  LoraPhyList m_phyList;       //!< List of physical layers attached above this tranducer.
  Ptr<LoraChannel> m_channel;  //!< The attached channel.
  EventId m_endTxEvent;       //!< Event scheduled for end of transmission.
  Time m_endTxTime;           //!< Time at which transmission will be completed.
  bool m_cleared;             //!< Flab when we've been cleared.

  /**
   * Remove an entry from the arrival list.
   *
   * \param arrival The packet arrival to remove.
   */
  void RemoveArrival (LoraPacketArrival arrival);
  /** Handle end of transmission event. */
  void EndTx (void);
protected:
  virtual void DoDispose ();

};  // class LoraTransducerHd

} // namespace ns3

#endif /* LORA_TRANSDUCER_HD_H */

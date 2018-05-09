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

#ifndef MAC_LORA_H
#define MAC_LORA_H

#include "lora-mac.h"
#include "lora-address.h"

namespace ns3
{


class LoraPhy;
class LoraTxMode;

/**
 * 
 * Packets received on multiple channels and multiple spreading factors.
 */
class MacLoraAca : public LoraMac
{
public:
  /** Default constructor */
  MacLoraAca ();
  /** Dummy destructor, see DoDispose. */
  virtual ~MacLoraAca ();
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);


  // Inherited methods
  Address GetAddress (void);
  virtual void SetAddress (LoraAddress addr);
  virtual bool Enqueue (Ptr<Packet> pkt, const Address &dest, uint16_t protocolNumber);
  virtual void SetForwardUpCb (Callback<void, Ptr<Packet>, const LoraAddress& > cb);
  virtual void AttachPhy (Ptr<LoraPhy> phy);
  virtual Address GetBroadcast (void) const;
  virtual void Clear (void);
  int64_t AssignStreams (int64_t stream);

private:
  /** The MAC address. */
  LoraAddress m_address;
  /** PHY layer attached to this MAC. */
  Ptr<LoraPhy> m_phy;
  /** Forwarding up callback. */
  Callback<void, Ptr<Packet>, const LoraAddress& > m_forUpCb;
  /** Flag when we've been cleared. */
  bool m_cleared;

  /**
   * Receive packet from lower layer (passed to PHY as callback).
   *
   * \param pkt Packet being received.
   * \param sinr SINR of received packet.
   * \param txMode Mode of received packet.
   */
  void RxPacketGood (Ptr<Packet> pkt, double sinr, LoraTxMode txMode);

  /**
   * Packet received at lower layer in error.
   *
   * \param pkt Packet received in error.
   * \param sinr SINR of received packet.
   */
  void RxPacketError (Ptr<Packet> pkt, double sinr);
protected:
  virtual void DoDispose ();

};  // class MacLoraAca

} // namespace ns3

#endif /* MAC_LORA_H */

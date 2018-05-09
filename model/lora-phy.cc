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

#include "lora-phy.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LoraPhyCalcSinr);

TypeId LoraPhyCalcSinr::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPhyCalcSinr")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
  ;
  return tid;
}

void
LoraPhyCalcSinr::Clear ()
{
}

void
LoraPhyCalcSinr::DoDispose ()
{
  Clear ();
  Object::DoDispose ();
}

NS_OBJECT_ENSURE_REGISTERED (LoraPhyPer);

TypeId LoraPhyPer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPhyPer")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
  ;
  return tid;
}

void
LoraPhyPer::Clear ()
{
}

void
LoraPhyPer::DoDispose ()
{
  Clear ();
  Object::DoDispose ();
}

NS_OBJECT_ENSURE_REGISTERED (LoraPhy);

TypeId LoraPhy::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPhy")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
    .AddTraceSource ("PhyTxBegin",
                     "Trace source indicating a packet has "
                     "begun transmitting over the channel medium.",
                     MakeTraceSourceAccessor (&LoraPhy::m_phyTxBeginTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyTxEnd",
                     "Trace source indicating a packet has "
                     "been completely transmitted over the channel.",
                   MakeTraceSourceAccessor (&LoraPhy::m_phyTxEndTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyTxDrop",
                     "Trace source indicating a packet has "
                     "been dropped by the device during transmission.",
                     MakeTraceSourceAccessor (&LoraPhy::m_phyTxDropTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyRxBegin",
                     "Trace source indicating a packet has "
                     "begun being received from the channel medium by the device.",
                     MakeTraceSourceAccessor (&LoraPhy::m_phyRxBeginTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyRxEnd",
                     "Trace source indicating a packet has "
                     "been completely received from the channel medium by the device.",
                     MakeTraceSourceAccessor (&LoraPhy::m_phyRxEndTrace),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("PhyRxDrop",
                     "Trace source indicating a packet has "
                     "been dropped by the device during reception.",
                     MakeTraceSourceAccessor (&LoraPhy::m_phyRxDropTrace),
                     "ns3::Packet::TracedCallback")
  ;
  return tid;
}


void
LoraPhy::NotifyTxBegin (Ptr<const Packet> packet)
{
  m_phyTxBeginTrace (packet);
}

void
LoraPhy::NotifyTxEnd (Ptr<const Packet> packet)
{
  m_phyTxEndTrace (packet);
}

void
LoraPhy::NotifyTxDrop (Ptr<const Packet> packet)
{
  m_phyTxDropTrace (packet);
}

void
LoraPhy::NotifyRxBegin (Ptr<const Packet> packet)
{
  m_phyRxBeginTrace (packet);
}

void
LoraPhy::NotifyRxEnd (Ptr<const Packet> packet)
{
  m_phyRxEndTrace (packet);
}

void
LoraPhy::NotifyRxDrop (Ptr<const Packet> packet)
{
  m_phyRxDropTrace (packet);
}

} // namespace ns3

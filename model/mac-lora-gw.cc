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

#include "mac-lora-gw.h"
#include "lora-tx-mode.h"
#include "lora-address.h"
#include "ns3/log.h"
#include "lora-phy.h"
#include "lora-header-common.h"

#include <iostream>

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("MacLoraAca");
  
NS_OBJECT_ENSURE_REGISTERED (MacLoraAca);

MacLoraAca::MacLoraAca ()
  : LoraMac (),
    m_cleared (false)
{
}



MacLoraAca::~MacLoraAca ()
{
}

void
MacLoraAca::Clear ()
{
  if (m_cleared)
    {
      return;
    }
  m_cleared = true;
  if (m_phy)
    {
      m_phy->Clear ();
      m_phy = 0;
    }
}

void
MacLoraAca::DoDispose ()
{
  Clear ();
  LoraMac::DoDispose ();
}

TypeId
MacLoraAca::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::MacLoraAca")
    .SetParent<Object> ()
    .SetGroupName ("LoraAca")
    .AddConstructor<MacLoraAca> ()
  ;
  return tid;
}

Address
MacLoraAca::GetAddress (void)
{
  return m_address;
}

void
MacLoraAca::SetAddress (LoraAddress addr)
{
  m_address=addr;
}
bool
MacLoraAca::Enqueue (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber)
{
  NS_LOG_DEBUG ("" << Simulator::Now ().GetSeconds () << " MAC " << LoraAddress::ConvertFrom (GetAddress ()) << " Queueing packet for " << LoraAddress::ConvertFrom (dest));

  if (!m_phy->IsStateTx ())
    {
      LoraAddress src = LoraAddress::ConvertFrom (GetAddress ());
      LoraAddress udest = LoraAddress::ConvertFrom (dest);

      LoraHeaderCommon header;
      header.SetSrc (src);
      header.SetDest (udest);
      header.SetType (0);
      header.SetPayload(10);
      header.SetPreamble(12);

      packet->AddHeader (header);
      m_phy->SendPacket (packet, protocolNumber);
      return true;
    }
  else
    return false;
}

void
MacLoraAca::SetForwardUpCb (Callback<void, Ptr<Packet>, const LoraAddress& > cb)
{
  m_forUpCb = cb;
}
void
MacLoraAca::AttachPhy (Ptr<LoraPhy> phy)
{
  m_phy = phy;
  m_phy->SetReceiveOkCallback (MakeCallback (&MacLoraAca::RxPacketGood, this));
  m_phy->SetReceiveErrorCallback (MakeCallback (&MacLoraAca::RxPacketError, this));

}
void
MacLoraAca::RxPacketGood (Ptr<Packet> pkt, double sinr, LoraTxMode txMode)
{
  LoraHeaderCommon header;
  pkt->RemoveHeader (header);
  NS_LOG_DEBUG ("Receiving packet from " << header.GetSrc () << " For " << header.GetDest ());

  if (header.GetDest () == GetAddress () || header.GetDest () == LoraAddress::GetBroadcast ())
    {
      m_forUpCb (pkt, header.GetSrc ());
    }

}

void
MacLoraAca::RxPacketError (Ptr<Packet> pkt, double sinr)
{
  NS_LOG_DEBUG ("" << Simulator::Now () << " MAC " << LoraAddress::ConvertFrom (GetAddress ()) << " Received packet in error with sinr " << sinr);
}

Address
MacLoraAca::GetBroadcast (void) const
{
  LoraAddress broadcast (255);
  return broadcast;
}

int64_t
MacLoraAca::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  return 0;
}

}

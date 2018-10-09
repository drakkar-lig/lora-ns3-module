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

#include "ns3/trace-source-accessor.h"
#include "ns3/traced-callback.h"
#include "ns3/pointer.h"
#include "ns3/node.h"
#include "ns3/assert.h"
#include "lora-net-device.h"
#include "lora-phy.h"
#include "lora-mac.h"
#include "lora-channel.h"
#include "lora-transducer.h"
#include "ns3/log.h"

#include "ns3/simulator.h"
#include "ns3/ethernet-header.h"
#include "ns3/ethernet-trailer.h"
#include "ns3/llc-snap-header.h"
#include "ns3/error-model.h"
#include "ns3/enum.h"
#include "ns3/boolean.h"
#include "ns3/uinteger.h"
#include "ns3/lora-tx-mode.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LoraNetDevice");

NS_OBJECT_ENSURE_REGISTERED (LoraNetDevice);

LoraNetDevice::LoraNetDevice ()
  : NetDevice (),
    m_mtu (64000),
    m_cleared (false)
{
}

LoraNetDevice::~LoraNetDevice ()
{
}

void
LoraNetDevice::Clear ()
{
  if (m_cleared)
    {
      return;
    }
  m_cleared = true;
  m_node = 0;
  if (m_channel)
    {
      m_channel->Clear ();
      m_channel = 0;
    }
  if (m_mac)
    {
      m_mac->Clear ();
      m_mac = 0;
    }
  if (m_phy)
    {
      m_phy->Clear ();
      m_phy = 0;
    }
  if (m_trans)
    {
      m_trans->Clear ();
      m_trans = 0;
    }
}

void
LoraNetDevice::DoDispose ()
{
  Clear ();
  NetDevice::DoDispose ();
}

TypeId
LoraNetDevice::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::LoraNetDevice")
    .SetParent<NetDevice> ()
    .SetGroupName ("Lora")
    .AddAttribute ("Channel", "The channel attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&LoraNetDevice::DoGetChannel, &LoraNetDevice::SetChannel),
                   MakePointerChecker<LoraChannel> ())
    .AddAttribute ("Phy", "The PHY layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&LoraNetDevice::GetPhy, &LoraNetDevice::SetPhy),
                   MakePointerChecker<LoraPhy> ())
    .AddAttribute ("Mac", "The MAC layer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&LoraNetDevice::GetMac, &LoraNetDevice::SetMac),
                   MakePointerChecker<LoraMac> ())
    .AddAttribute ("Transducer", "The Transducer attached to this device.",
                   PointerValue (),
                   MakePointerAccessor (&LoraNetDevice::GetTransducer,
                                        &LoraNetDevice::SetTransducer),
                   MakePointerChecker<LoraTransducer> ())
    .AddTraceSource ("Rx", "Received payload from the MAC layer.",
                     MakeTraceSourceAccessor (&LoraNetDevice::m_rxLogger),
                     "ns3::LoraNetDevice::RxTxTracedCallback")
    .AddTraceSource ("Tx", "Send payload to the MAC layer.",
                     MakeTraceSourceAccessor (&LoraNetDevice::m_txLogger),
                     "ns3::LoraNetDevice::RxTxTracedCallback")
  ;
  return tid;
}

void
LoraNetDevice::SetMac (Ptr<LoraMac> mac)
{
  if (mac != 0)
    {
      m_mac = mac;
      NS_LOG_DEBUG ("Set MAC");

      if (m_phy != 0)
        {
          m_phy->SetMac (mac);
          m_mac->AttachPhy (m_phy);
          NS_LOG_DEBUG ("Attached MAC to PHY");
        }
      m_mac->SetForwardUpCb (MakeCallback (&LoraNetDevice::ForwardUp, this));
    }

}

void
LoraNetDevice::SetPhy (Ptr<LoraPhy> phy)
{
  if (phy != 0)
    {
      m_phy = phy;
      m_phy->SetDevice (Ptr<LoraNetDevice> (this));
      NS_LOG_DEBUG ("Set PHY");
      if (m_mac != 0)
        {
          m_mac->AttachPhy (phy);
          m_phy->SetMac (m_mac);
          NS_LOG_DEBUG ("Attached PHY to MAC");
        }
      if (m_trans != 0)
        {
          m_phy->SetTransducer (m_trans);
          NS_LOG_DEBUG ("Added PHY to trans");
        }

    }
}

void
LoraNetDevice::SetChannel (Ptr<LoraChannel> channel)
{
  if (channel != 0)
    {
      m_channel = channel;
      NS_LOG_DEBUG ("Set CHANNEL");
      if (m_trans != 0)
        {

          m_channel->AddDevice (this, m_trans);
          NS_LOG_DEBUG ("Added self to channel device list");
          m_trans->SetChannel (m_channel);
          NS_LOG_DEBUG ("Set Transducer channel");
        }
      if (m_phy != 0 )
        {
          m_phy->SetChannel (channel);
        }
    }
}

Ptr<LoraChannel>
LoraNetDevice::DoGetChannel (void) const
{
  return m_channel;

}
Ptr<LoraMac>
LoraNetDevice::GetMac () const
{
  return m_mac;
}

Ptr<LoraPhy>
LoraNetDevice::GetPhy () const
{
  return m_phy;
}

void
LoraNetDevice::SetIfIndex (uint32_t index)
{
  m_ifIndex = index;
}

uint32_t
LoraNetDevice::GetIfIndex () const
{
  return m_ifIndex;
}

Ptr<Channel>
LoraNetDevice::GetChannel () const
{
  return m_channel;
}

Address
LoraNetDevice::GetAddress () const
{
  return m_mac->GetAddress ();
}

bool
LoraNetDevice::SetMtu (uint16_t mtu)
{
  NS_LOG_WARN ("LoraNetDevice:  MTU is not implemented");
  m_mtu = mtu;
  return true;
}

uint16_t
LoraNetDevice::GetMtu () const
{
  return m_mtu;
}

void
LoraNetDevice::SetChannelMode (uint32_t mode) {
  v_mode = mode;
}

uint32_t
LoraNetDevice::GetChannelMode (void) {
   return v_mode;       
}

void
LoraNetDevice::SetTransmitStartTime (uint32_t transmitStartTime) {
  v_transmitStartTime = transmitStartTime;
}

double 
LoraNetDevice::GetTransmitStartTime (void) {
  return v_transmitStartTime;
}

bool
LoraNetDevice::IsLinkUp () const
{
  return  (m_linkup && (m_phy != 0));
}

bool
LoraNetDevice::IsBroadcast () const
{
  return true;
}

Address
LoraNetDevice::GetBroadcast () const
{
  return m_mac->GetBroadcast ();
}

bool
LoraNetDevice::IsMulticast () const
{
  return false;
}

Address
LoraNetDevice::GetMulticast (Ipv4Address multicastGroup) const
{
  NS_FATAL_ERROR ("LoraNetDevice does not support multicast");
  return m_mac->GetBroadcast ();
}

Address
LoraNetDevice::GetMulticast (Ipv6Address addr) const
{
  NS_FATAL_ERROR ("LoraNetDevice does not support multicast");
  return m_mac->GetBroadcast ();
}

bool
LoraNetDevice::IsBridge (void) const
{
  return false;
}
bool
LoraNetDevice::IsPointToPoint () const
{
  return false;
}

bool
LoraNetDevice::Send (Ptr<Packet> packet, const Address &dest, uint16_t protocolNumber)
{
  return m_mac->Enqueue (packet, dest, protocolNumber);
}

bool
LoraNetDevice::SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber)
{
  NS_ASSERT_MSG (0, "Not yet implemented");
  return false;
}



Ptr<Node>
LoraNetDevice::GetNode () const
{
  return m_node;
}

void
LoraNetDevice::SetNode (Ptr<Node> node)
{
  m_node = node;
}

bool
LoraNetDevice::NeedsArp () const
{
  return false;
}

void
LoraNetDevice::SetReceiveCallback (NetDevice::ReceiveCallback cb)
{
  m_forwardUp = cb;
}

void
LoraNetDevice::ForwardUp (Ptr<Packet> pkt, const LoraAddress &src)
{
  NS_LOG_DEBUG ("Forwarding packet up to application");
  m_rxLogger (pkt, src);
  m_forwardUp (this, pkt, 0, src);

}

Ptr<LoraTransducer>
LoraNetDevice::GetTransducer (void) const
{
  return m_trans;
}
void
LoraNetDevice::SetTransducer (Ptr<LoraTransducer> trans)
{

  if (trans != 0)
    {
      m_trans = trans;
      NS_LOG_DEBUG ("Set Transducer");
      if (m_phy != 0)
        {
          m_phy->SetTransducer (m_trans);
          NS_LOG_DEBUG ("Attached Phy to transducer");
        }

      if (m_channel != 0)
        {
          m_channel->AddDevice (this, m_trans);
          m_trans->SetChannel (m_channel);
          NS_LOG_DEBUG ("Added self to channel device list");
        }
    }

}

void
LoraNetDevice::AddLinkChangeCallback (Callback<void> callback)
{
  m_linkChanges.ConnectWithoutContext (callback);
}


void
LoraNetDevice::SetPromiscReceiveCallback (PromiscReceiveCallback cb)
{
  NS_ASSERT_MSG (0, "Not yet implemented");
}

bool
LoraNetDevice::SupportsSendFrom (void) const
{
  return false;
}

void
LoraNetDevice::SetAddress (Address address)
{
  NS_ASSERT_MSG (0, "Tried to set MAC address with no MAC");
  m_mac->SetAddress (LoraAddress::ConvertFrom (address));
}


Address 
LoraNetDevice::GetGWAddress () const
{
        return m_gwAddress;
}


void
LoraNetDevice::SetGWAddress (Address gwAddress)
{
  m_gwAddress = gwAddress;
}



void
LoraNetDevice::SetSleepMode (bool sleep)
{
  m_phy->SetSleepMode (sleep);
}


void
LoraNetDevice::TransmitStart (void)
{
  NS_LOG_FUNCTION_NOARGS ();
/*
  
  NS_ASSERT_MSG ((m_txMachineState == READY) || (m_txMachineState == BACKOFF), 
                 "Must be READY to transmit. Tx state is: " << m_txMachineState);

  //
  // Now we have to sense the state of the medium and either start transmitting
  // if it is idle, or backoff our transmission if someone else is on the wire.
  //
  if (m_channel->GetState (m_protocolNumber) != IDLE)
    {
      //
      // The channel is busy -- backoff and rechedule TransmitStart() unless
      // we have exhausted all of our retries.
      //
      m_txMachineState = BACKOFF;

      if (m_backoff.MaxRetriesReached ())
        { 
          //
          // Too many retries, abort transmission of packet
          //
          TransmitAbort ();
        } 
      else 
        {       
          m_backoff.IncrNumRetries ();
          Time backoffTime = m_backoff.GetBackoffTime ();

          NS_LOG_LOGIC ("Channel busy, backing off for " << backoffTime.GetSeconds () << " sec");

          Simulator::Schedule (Seconds(backoffTime), &LoraNetDevice::TransmitStart, this);
        }
    } 
  else 
    {
      // The channel is free, transmit the packet

        if (m_mac->Enqueue (m_currentPkt, m_dest, m_protocolNumber) == false)
         {
           NS_LOG_LOGIC ("Transmitting the packet.");
         }

      if (m_channel->TransmitStart (m_currentPkt, m_protocolNumber) == false)
        {
          NS_LOG_WARN ("Channel TransmitStart returns an error");
          m_currentPkt = 0;
          m_txMachineState = READY;

        } 
      else 
        {
          //
          // Transmission succeeded, reset the backoff time parameters and
          // schedule a transmit complete event.
          //
          m_backoff.ResetBackoffTime ();
          m_txMachineState = BUSY;
          Time tEvent = m_bps.CalculateBytesTxTime (13);
          NS_LOG_LOGIC ("Schedule TransmitCompleteEvent in " << tEvent.GetSeconds () << "sec");
          Simulator::Schedule (Seconds(tEvent), &LoraNetDevice::TransmitCompleteEvent, this);
        }

    }
*/

}


void
LoraNetDevice::TransmitAbort (void)
{
  //
  // When we started the process of transmitting the current packet, it was 
  // placed in m_currentPkt.  So we had better find one there.
  //

  m_currentPkt = 0;

  // 
  // We're done with that one, so reset the backoff algorithm and ready the
  // transmit state machine.
  //
//  m_backoff.ResetBackoffTime ();
  m_txMachineState = READY;
 
}


void
LoraNetDevice::TransmitCompleteEvent (void)
{

  NS_LOG_FUNCTION_NOARGS ();

  //
  // This function is called to finish the  process of transmitting a packet.
  // We need to tell the channel that we've stopped wiggling the wire and
  // schedule an event that will be executed when it's time to re-enable
  // the transmitter after the interframe gap.
  //
  m_txMachineState = GAP;

  //
  // When we started transmitting the current packet, it was placed in 
  // m_currentPkt.  So we had better find one there.
  //

  m_channel->TransmitEnd (m_protocolNumber); 

  m_currentPkt = 0;

  NS_LOG_LOGIC ("Schedule TransmitReadyEvent in " << m_tInterframeGap.GetSeconds () << "sec");

  Simulator::Schedule (Seconds(m_tInterframeGap), &LoraNetDevice::TransmitReadyEvent, this);

}


void
LoraNetDevice::TransmitReadyEvent (void)
{
 //  NS_LOG_FUNCTION_NOARGS ();

  //
  // This function is called to enable the transmitter after the interframe
  // gap has passed.  If there are pending transmissions, we use this opportunity
  // to start the next transmit.
  //

  m_txMachineState = READY;

  //
  // We expect that the packet we had been transmitting was cleared when the 
  // TransmitCompleteEvent() was executed.
  //

  //
  // Get the next packet from the queue for transmitting: Not implement yet.
  //
 
}

bool
LoraNetDevice::SendCsma (Ptr<Packet> packet,const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << dest << protocolNumber);
  return SendFromCsma (packet, dest, protocolNumber);
}

bool
LoraNetDevice::SendFromCsma (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber)
{
  NS_LOG_FUNCTION (packet << dest << protocolNumber);
  NS_LOG_LOGIC ("packet =" << packet);
  NS_LOG_LOGIC ("UID is " << packet->GetUid () << ")");

  //
  // Place the packet to be sent on the send queue.  Note that the 
  // queue may fire a drop trace, but we will too.
  //

  //
  // If the device is idle, we need to start a transmission. Otherwise,
  // the transmission will be started when the current packet finished
  // transmission (see TransmitCompleteEvent)
  //
  if (m_txMachineState == READY) 
    {
       {
        m_dest = dest;  
        m_protocolNumber = protocolNumber;
        m_currentPkt = packet;       

        TransmitStart ();
       }
    }
  return true;
}

} // namespace ns3


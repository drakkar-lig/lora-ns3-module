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

#include "ns3/object.h"
#include "ns3/packet.h"
#include "ns3/simulator.h"
#include "ns3/mobility-model.h"
#include "ns3/net-device.h"
#include "ns3/node.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/string.h"
#include "ns3/log.h"

#include "lora-channel.h"
#include "lora-phy.h"
#include "lora-prop-model.h"
#include "lora-tx-mode.h"
#include "lora-net-device.h"
#include "lora-transducer.h"
#include "lora-noise-model-default.h"
#include "lora-prop-model-ideal.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LoraChannel");

NS_OBJECT_ENSURE_REGISTERED (LoraChannel);

TypeId
LoraChannel::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::LoraChannel")
    .SetParent<Channel> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraChannel> ()
    .AddAttribute ("PropagationModel",
                   "A pointer to the propagation model.",
                   StringValue ("ns3::LoraPropModelIdeal"),
                   MakePointerAccessor (&LoraChannel::m_prop),
                   MakePointerChecker<LoraPropModel> ())
    .AddAttribute ("NoiseModel",
                   "A pointer to the model of the channel ambient noise.",
                   StringValue ("ns3::LoraNoiseModelDefault"),
                   MakePointerAccessor (&LoraChannel::m_noise),
                   MakePointerChecker<LoraNoiseModel> ())
  ;

  return tid;
}

LoraChannel::LoraChannel ()
  : Channel (),
    m_prop (0),
    m_cleared (false)
{
}

LoraChannel::~LoraChannel ()
{
}

void
LoraChannel::Clear ()
{
  if (m_cleared)
    {
      return;
    }
  m_cleared = true;
  LoraDeviceList::iterator it = m_devList.begin ();
  for (; it != m_devList.end (); it++)
    {
      if (it->first)
        {
          it->first->Clear ();
          it->first = 0;
        }
      if (it->second)
        {
          it->second->Clear ();
          it->second = 0;
        }
    }
  m_devList.clear ();
  if (m_prop)
    {
      m_prop->Clear ();
      m_prop = 0;
    }
  if (m_noise)
    {
      m_noise->Clear ();
      m_noise = 0;
    }

}

void
LoraChannel::DoDispose ()
{
  Clear ();
  Channel::DoDispose ();
}
void
LoraChannel::SetPropagationModel (Ptr<LoraPropModel> prop)
{
  NS_LOG_DEBUG ("Set Prop Model " << this);
  m_prop = prop;
}

uint32_t
LoraChannel::GetNDevices () const
{
  return m_devList.size ();
}

Ptr<NetDevice>
LoraChannel::GetDevice (uint32_t i) const
{
  return m_devList[i].first;
}

void
LoraChannel::AddDevice (Ptr<LoraNetDevice> dev, Ptr<LoraTransducer> trans)
{
  NS_LOG_DEBUG ("Adding dev/trans pair number " << m_devList.size ());
  m_devList.push_back (std::make_pair (dev, trans));
}

void
LoraChannel::TxPacket (Ptr<LoraTransducer> src, Ptr<Packet> packet,
                      double txPowerDb, LoraTxMode txMode)
{
  Ptr<MobilityModel> senderMobility = 0;

  NS_LOG_DEBUG ("Channel scheduling");
  for (LoraDeviceList::const_iterator i = m_devList.begin (); i
       != m_devList.end (); i++)
    {

      if (src == i->second)
        {
          senderMobility = i->first->GetNode ()->GetObject<MobilityModel> ();
          break;
        }
    }
  NS_ASSERT (senderMobility != 0);
  uint32_t j = 0;
  LoraDeviceList::const_iterator i = m_devList.begin ();
  for (; i != m_devList.end (); i++)
    {
      if (src != i->second)
        {
          NS_LOG_DEBUG ("Scheduling " << i->first->GetMac ()->GetAddress ());
          Ptr<MobilityModel> rcvrMobility = i->first->GetNode ()->GetObject<MobilityModel> ();
          Time delay = m_prop->GetDelay (senderMobility, rcvrMobility, txMode);
          LoraPdp pdp = m_prop->GetPdp (senderMobility, rcvrMobility, txMode);
          double rxPowerDb = txPowerDb - m_prop->GetPathLossDb (senderMobility,
                                                                rcvrMobility,
                                                                txMode);

          NS_LOG_DEBUG ("txPowerDb=" << txPowerDb << "dB, rxPowerDb="
                                     << rxPowerDb << "dB, distance="
                                     << senderMobility->GetDistanceFrom (rcvrMobility)
                                     << "m, delay=" << delay);

          uint32_t dstNodeId = i->first->GetNode ()->GetId ();
          Ptr<Packet> copy = packet->Copy ();
          Simulator::ScheduleWithContext (dstNodeId, delay,
                                          &LoraChannel::SendUp,
                                          this,
                                          j,
                                          copy,
                                          rxPowerDb,
                                          txMode,
                                          pdp);
        }
      j++;
    }
}

void
LoraChannel::SetNoiseModel (Ptr<LoraNoiseModel> noise)
{
  NS_ASSERT (noise);
  m_noise = noise;
}
void
LoraChannel::SendUp (uint32_t i, Ptr<Packet> packet, double rxPowerDb,
                    LoraTxMode txMode, LoraPdp pdp)
{
  NS_LOG_DEBUG ("Channel:  In sendup");
  m_devList[i].second->Receive (packet, rxPowerDb, txMode, pdp);
}

double
LoraChannel::GetNoiseDbHz (double fKhz)
{
  NS_ASSERT (m_noise);
  double noise = m_noise->GetNoiseDbHz (fKhz);
  return noise;
}



  /**
   * \param srcId The device Id of the net device that wants to
   * transmit on the channel.
   * \return True if the channel is not busy and the transmitting net
   * device is currently active.
   */
bool

LoraChannel::TransmitStart (Ptr<Packet> p, uint16_t protocolNumber)
{

  NS_LOG_INFO ("UID is " << p->GetUid () << ")");

  if (m_state[protocolNumber] != IDLE)
    {
      NS_LOG_WARN ("LoraChannel::TransmitStart(): State is not IDLE");
      return false;
    }

  NS_LOG_LOGIC ("switch to TRANSMITTING");
  m_currentPkt = p;
  m_state[protocolNumber] = TRANSMITTING;
  return true;
}


bool
LoraChannel::TransmitEnd (uint16_t protocolNumber)
{

  NS_LOG_FUNCTION (this << m_currentPkt << m_currentSrc);
  NS_LOG_INFO ("UID is " << m_currentPkt->GetUid () << ")");

  NS_ASSERT (m_state[protocolNumber] == TRANSMITTING);
  m_state[protocolNumber] = IDLE;

  bool retVal = true;

  return retVal;
}

bool
LoraChannel::IsBusy (uint16_t protocolNumber)
{
  if (m_state[protocolNumber] == IDLE) 
    {
      return false;
    } 
  else 
    {
      return true;
    }
}


DataRate
LoraChannel::GetDataRate (void)
{
  return m_bps;
}

Time
LoraChannel::GetDelay (void)
{
  return m_delay;
}

WireState
LoraChannel::GetState (uint16_t protocolNumber)
{
  return m_state[protocolNumber];
}



} // namespace ns3

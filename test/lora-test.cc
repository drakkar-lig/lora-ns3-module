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

#include "ns3/lora-net-device.h"
#include "ns3/lora-channel.h"
#include "ns3/lora-phy-gen.h"
#include "ns3/lora-transducer-hd.h"
#include "ns3/lora-prop-model-ideal.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/simulator.h"
#include "ns3/test.h"
#include "ns3/node.h"
#include "ns3/object-factory.h"
#include "ns3/pointer.h"
#include "ns3/callback.h"
#include "ns3/nstime.h"
#include "ns3/log.h"
#include "ns3/mac-lora-gw.h"

using namespace ns3;

class LoraTestAca : public TestCase
{
public:
  LoraTestAca ();

  virtual void DoRun (void);
private:
  Ptr<LoraNetDevice> CreateNode (Vector pos, Ptr<LoraChannel> chan);

  Ptr<LoraNetDevice> CreateGateway (Vector pos, Ptr<LoraChannel> chan);

  bool DoPhyTests ();
  uint32_t DoOnePhyTest (Time t1, Time t2, uint32_t r1, uint32_t r2, Ptr<LoraPropModel> prop, uint32_t mode1 = 0, uint32_t mode2 = 0);

  uint32_t DoOnePhyTest3_nNodes (Ptr<LoraPropModel> prop);

  bool RxPacket (Ptr<NetDevice> dev, Ptr<const Packet> pkt, uint16_t mode, const Address &sender);
  void SendOnePacket (Ptr<LoraNetDevice> dev, uint32_t mode);
  void SendOnePacket2GW (Ptr<LoraNetDevice> dev, uint32_t mode, Address &sender);

uint32_t RanTxTime(uint32_t fMin, uint32_t fMax);

uint32_t random_number(uint32_t min_num, uint32_t max_num);

  ObjectFactory m_phyFac;
  uint32_t m_bytesRx;
};


LoraTestAca::LoraTestAca () : TestCase ("LORA")
{

}

bool
LoraTestAca::RxPacket (Ptr<NetDevice> dev, Ptr<const Packet> pkt, uint16_t mode, const Address &sender)
{
  m_bytesRx += pkt->GetSize ();
  return true;
}
void
LoraTestAca::SendOnePacket (Ptr<LoraNetDevice> dev, uint32_t mode)
{
  Ptr<Packet> pkt = Create<Packet> (13);
  dev->Send (pkt, dev->GetBroadcast (), mode);
}

void
LoraTestAca::SendOnePacket2GW (Ptr<LoraNetDevice> dev, uint32_t mode, Address &sender)
{
  Ptr<Packet> pkt = Create<Packet> (13);
  dev->Send (pkt, sender, mode);

}

Ptr<LoraNetDevice>
LoraTestAca::CreateNode (Vector pos, Ptr<LoraChannel> chan)
{

  Ptr<LoraPhy> phy = m_phyFac.Create<LoraPhy> ();
  Ptr<Node> node = CreateObject<Node> ();
  Ptr<LoraNetDevice> dev = CreateObject<LoraNetDevice> ();
  Ptr<MacLoraAca> mac = CreateObject<MacLoraAca> ();
  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<ConstantPositionMobilityModel> ();

  Ptr<LoraTransducerHd> trans = CreateObject<LoraTransducerHd> ();

  mobility->SetPosition (pos);
  node->AggregateObject (mobility);
  mac->SetAddress (LoraAddress::Allocate ());

  dev->SetPhy (phy);
  dev->SetMac (mac);
  dev->SetChannel (chan);
  dev->SetTransducer (trans);
  node->AddDevice (dev);

  return dev;
}

Ptr<LoraNetDevice>
LoraTestAca::CreateGateway (Vector pos, Ptr<LoraChannel> chan)
{

  Ptr<LoraPhy> phy = m_phyFac.Create<LoraPhy> ();
  Ptr<Node> node = CreateObject<Node> ();
  Ptr<LoraNetDevice> dev = CreateObject<LoraNetDevice> ();

  Ptr<MacLoraAca> mac = CreateObject<MacLoraAca> ();

  Ptr<ConstantPositionMobilityModel> mobility = CreateObject<ConstantPositionMobilityModel> ();

  Ptr<LoraTransducerHd> trans = CreateObject<LoraTransducerHd> ();

  mobility->SetPosition (pos);
  node->AggregateObject (mobility);
  mac->SetAddress (LoraAddress::Allocate ());

  dev->SetPhy (phy);
  dev->SetMac (mac);
  dev->SetChannel (chan);
  dev->SetTransducer (trans);
  node->AddDevice (dev);

  return dev;
}

uint32_t
LoraTestAca::RanTxTime(uint32_t fMin, uint32_t fMax)
{

        return random_number(fMin, fMax);
}


uint32_t 
LoraTestAca::random_number(uint32_t min_num, uint32_t max_num)
{
    uint32_t  result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num)
    {
        low_num = min_num;
        hi_num = max_num + 1; 
    } else {
        low_num = max_num + 1;
        hi_num = min_num;
    }

    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}


uint32_t
LoraTestAca::DoOnePhyTest (Time txTime1,
                       Time txTime2,
                       uint32_t r1,
                       uint32_t r2,
                       Ptr<LoraPropModel> prop,
                       uint32_t mode1,
                       uint32_t mode2)
{

  Ptr<LoraChannel> channel = CreateObject<LoraChannel> ();
  channel->SetAttribute ("PropagationModel", PointerValue (prop));

                                       
  Ptr<LoraNetDevice> gw0 = CreateGateway (Vector (r1,50,50), channel);


  Ptr<LoraNetDevice> dev1 = CreateNode (Vector (0,50,50), channel);
  Ptr<LoraNetDevice> dev2 = CreateNode (Vector (r1 + r2, 50, 50), channel);


  gw0->SetReceiveCallback (MakeCallback (&LoraTestAca::RxPacket, this));

  Simulator::Schedule (txTime1, &LoraTestAca::SendOnePacket, this, dev1, mode1);
  Simulator::Schedule (txTime2, &LoraTestAca::SendOnePacket, this, dev2, mode2);

  m_bytesRx = 0;
  Simulator::Stop (Seconds (20.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return m_bytesRx;
}


uint32_t
LoraTestAca::DoOnePhyTest3_nNodes (Ptr<LoraPropModel> prop)
{
  Ptr<LoraChannel> channel = CreateObject<LoraChannel> ();
  channel->SetAttribute ("PropagationModel", PointerValue (prop));

  Ptr<LoraNetDevice> gw0 = CreateGateway (Vector (50,50,50), channel);

//Set positions to nodes
  uint32_t x = 50; uint32_t y = 50; uint32_t z = 50; 
  uint32_t n = 10; 

  Ptr<LoraNetDevice> PtrDevice[n];
  for (uint32_t i = 0; i < n; i++)
  {
      PtrDevice[i] = CreateNode (Vector (x,y,z), channel);
      x += 50; y += 50; z += 50;

      if ((x == 10000) || (y == 10000) || (z == 10000)) 
      {
         x = 50; y = 50; z = 50;
      }
              
  }

//Set gateway to receive packets from end devices node.
  gw0->SetReceiveCallback (MakeCallback (&LoraTestAca::RxPacket, this));

//Looping in Simulation Type:
  double simTime = 100.0; 
  uint32_t mode [3] = {0, 1, 2};
  uint32_t m = 0;
  double s_transmitStartTime;

  uint32_t     nTempStart = 0;
  uint32_t     nTempEnd = 100;

  uint32_t     numNodesStart = 0;
  uint32_t     numNodesEnd = 10;

  do {
     for (uint32_t j = numNodesStart; j < numNodesEnd; j++)
     { 
        s_transmitStartTime = RanTxTime(nTempStart,nTempEnd);
        PtrDevice[j]->SetChannelMode(mode[m]);
        PtrDevice[j]->SetTransmitStartTime(s_transmitStartTime);

        Simulator::Schedule (Seconds(s_transmitStartTime), &LoraTestAca::SendOnePacket, this, PtrDevice[j], mode[m]);
       
        m += 1; 
        if (m > 2) { m = 0; }
     } 
     nTempStart = nTempEnd;
     nTempEnd = nTempEnd + 100;

   } while (nTempEnd <= simTime);


  m_bytesRx = 0;
  Simulator::Stop (Seconds (120.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return m_bytesRx;
}


bool
LoraTestAca::DoPhyTests ()
{

  LoraModesList mList;

  LoraTxMode mode = LoraTxModeFactory::CreateMode (LoraTxMode::LORA, 80, 80, 10000, 4000, 2, "TestMode");

  mList.AppendMode (LoraTxMode (mode));

  Ptr<LoraPhyPerGenDefault> perDef = CreateObject<LoraPhyPerGenDefault> ();
  Ptr<LoraPhyCalcSinrDefault> sinrDef = CreateObject<LoraPhyCalcSinrDefault> ();
  m_phyFac.SetTypeId ("ns3::LoraPhyGen");
  m_phyFac.Set ("PerModel", PointerValue (perDef));
  m_phyFac.Set ("SinrModel", PointerValue (sinrDef));
  m_phyFac.Set ("SupportedModes", LoraModesListValue (mList));

  Ptr<LoraPropModelIdeal> prop = CreateObject<LoraPropModelIdeal> ();

  LoraTxMode mode00 = LoraTxModeFactory::CreateMode (LoraTxMode::LORA, 300, 120, 10000, 125, 2, "TestMode00");
  LoraTxMode mode01 = LoraTxModeFactory::CreateMode (LoraTxMode::LORA, 300, 120, 11000, 125, 2, "TestMode01");
  LoraTxMode mode02 = LoraTxModeFactory::CreateMode (LoraTxMode::LORA, 300, 120, 12000, 125, 2, "TestMode02");

  LoraModesList m0; // Mode 0
  m0.AppendMode (mode00);

  LoraModesList m1; // Mode 1
  m1.AppendMode (mode01);

  LoraModesList m2; // Mode 2
  m2.AppendMode (mode02);

  m_phyFac = ObjectFactory ();
  m_phyFac.SetTypeId ("ns3::LoraPhyDual");

  m_phyFac.Set ("SupportedModesPhy1", LoraModesListValue (m0));
  m_phyFac.Set ("SupportedModesPhy2", LoraModesListValue (m1));
  m_phyFac.Set ("SupportedModesPhy3", LoraModesListValue (m2));

   //Test n nodes
   NS_TEST_ASSERT_MSG_EQ_RETURNS_BOOL (DoOnePhyTest3_nNodes(prop), 130, "Test for n nodes");

  return false;
}

void
LoraTestAca::DoRun (void)
{
  DoPhyTests ();
}


class LoraTestAcaSuite : public TestSuite
{
public:
  LoraTestAcaSuite ();
};

LoraTestAcaSuite::LoraTestAcaSuite ()
  :  TestSuite ("lora-node", UNIT)
{
  AddTestCase (new LoraTestAca, TestCase::QUICK);
}

static LoraTestAcaSuite g_LoraTestAcaSuite;

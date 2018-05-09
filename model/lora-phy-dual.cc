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
#include "lora-phy-dual.h"
#include "lora-phy-gen.h"
#include "lora-tx-mode.h"
#include "lora-net-device.h"
#include "lora-channel.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/traced-callback.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "lora-header-common.h"

#include <cmath>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LoraPhyDual");

NS_OBJECT_ENSURE_REGISTERED (LoraPhyDual);
NS_OBJECT_ENSURE_REGISTERED (LoraPhyCalcSinrDual);

LoraPhyCalcSinrDual::LoraPhyCalcSinrDual ()
{

}
LoraPhyCalcSinrDual::~LoraPhyCalcSinrDual ()
{

}

TypeId
LoraPhyCalcSinrDual::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPhyCalcSinrDual")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraPhyCalcSinrDual> ()
  ;
  return tid;
}

double
LoraPhyCalcSinrDual::CalcSinrDb (Ptr<Packet> pkt,
                                Time arrTime,
                                double rxPowerDb,
                                double ambNoiseDb,
                                LoraTxMode mode,
                                LoraPdp pdp,
                                const LoraTransducer::ArrivalList &arrivalList) const
{

  if (mode.GetModType () != LoraTxMode::OTHER)
    {
      NS_LOG_WARN ("Calculating SINR for unsupported modulation type");
    }

  double intKp = -DbToKp (rxPowerDb); // This packet is in the arrivalList
  LoraTransducer::ArrivalList::const_iterator it = arrivalList.begin ();
  for (; it != arrivalList.end (); it++)
    {
      // Only count interference if there is overlap in incoming frequency
      if (std::abs ( (double) it->GetTxMode ().GetCenterFreqHz () - (double) mode.GetCenterFreqHz ())
          < (double)(it->GetTxMode ().GetBandwidthHz () / 2 + mode.GetBandwidthHz () / 2) - 0.5)
        {
          LoraHeaderCommon ch, ch2;
          if (pkt)
            {
              pkt->PeekHeader (ch);
            }
          it->GetPacket ()->PeekHeader (ch2);

          if (pkt)
            {
            }
          intKp += DbToKp (it->GetRxPowerDb ());
        }
    }

  double totalIntDb = KpToDb (intKp + DbToKp (ambNoiseDb));

  NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Calculating SINR:  RxPower = " << rxPowerDb << " dB.  Number of interferers = " << arrivalList.size () << "  Interference + noise power = " << totalIntDb << " dB.  SINR = " << rxPowerDb - totalIntDb << " dB.");
  return rxPowerDb - totalIntDb;
}

LoraPhyDual::LoraPhyDual ()
  :  LoraPhy ()
{

  m_phy1 = CreateObject<LoraPhyGen> ();
  m_phy2 = CreateObject<LoraPhyGen> ();

  m_phy1->SetReceiveOkCallback (m_recOkCb);
  m_phy2->SetReceiveOkCallback (m_recOkCb);

  m_phy1->SetReceiveErrorCallback (m_recErrCb);
  m_phy2->SetReceiveErrorCallback (m_recErrCb);


  m_phy3 = CreateObject<LoraPhyGen> ();
  m_phy4 = CreateObject<LoraPhyGen> ();

  m_phy3->SetReceiveOkCallback (m_recOkCb);
  m_phy4->SetReceiveOkCallback (m_recOkCb);

  m_phy3->SetReceiveErrorCallback (m_recErrCb);
  m_phy4->SetReceiveErrorCallback (m_recErrCb);


  m_phy5 = CreateObject<LoraPhyGen> ();
  m_phy6 = CreateObject<LoraPhyGen> ();

  m_phy5->SetReceiveOkCallback (m_recOkCb);
  m_phy6->SetReceiveOkCallback (m_recOkCb);

  m_phy5->SetReceiveErrorCallback (m_recErrCb);
  m_phy6->SetReceiveErrorCallback (m_recErrCb);
  
  m_phy7 = CreateObject<LoraPhyGen> ();
  m_phy8 = CreateObject<LoraPhyGen> ();

  m_phy7->SetReceiveOkCallback (m_recOkCb);
  m_phy8->SetReceiveOkCallback (m_recOkCb);

  m_phy7->SetReceiveErrorCallback (m_recErrCb);
  m_phy8->SetReceiveErrorCallback (m_recErrCb);

  m_phy9 = CreateObject<LoraPhyGen> ();
  m_phy10 = CreateObject<LoraPhyGen> ();

  m_phy9->SetReceiveOkCallback (m_recOkCb);
  m_phy10->SetReceiveOkCallback (m_recOkCb);

  m_phy9->SetReceiveErrorCallback (m_recErrCb);
  m_phy10->SetReceiveErrorCallback (m_recErrCb);
  
  m_phy11 = CreateObject<LoraPhyGen> ();
  m_phy12 = CreateObject<LoraPhyGen> ();

  m_phy11->SetReceiveOkCallback (m_recOkCb);
  m_phy12->SetReceiveOkCallback (m_recOkCb);

  m_phy11->SetReceiveErrorCallback (m_recErrCb);
  m_phy12->SetReceiveErrorCallback (m_recErrCb);

  m_phy13 = CreateObject<LoraPhyGen> ();
  m_phy14 = CreateObject<LoraPhyGen> ();

  m_phy13->SetReceiveOkCallback (m_recOkCb);
  m_phy14->SetReceiveOkCallback (m_recOkCb);

  m_phy13->SetReceiveErrorCallback (m_recErrCb);
  m_phy14->SetReceiveErrorCallback (m_recErrCb);

  m_phy15 = CreateObject<LoraPhyGen> ();
  m_phy16 = CreateObject<LoraPhyGen> ();

  m_phy15->SetReceiveOkCallback (m_recOkCb);
  m_phy16->SetReceiveOkCallback (m_recOkCb);

  m_phy15->SetReceiveErrorCallback (m_recErrCb);
  m_phy16->SetReceiveErrorCallback (m_recErrCb);

  m_phy17 = CreateObject<LoraPhyGen> ();
  m_phy18 = CreateObject<LoraPhyGen> ();

  m_phy17->SetReceiveOkCallback (m_recOkCb);
  m_phy18->SetReceiveOkCallback (m_recOkCb);

  m_phy17->SetReceiveErrorCallback (m_recErrCb);
  m_phy18->SetReceiveErrorCallback (m_recErrCb);
}

LoraPhyDual::~LoraPhyDual ()
{
}

void
LoraPhyDual::Clear ()
{
  if (m_phy1)
    {
      m_phy1->Clear ();
      m_phy1 = 0;
    }
  if (m_phy2)
    {
      m_phy2->Clear ();
      m_phy2 = 0;
    }

  if (m_phy3)
    {
      m_phy3->Clear ();
      m_phy3 = 0;
    }

  if (m_phy4)
    {
      m_phy4->Clear ();
      m_phy4 = 0;
    }

  if (m_phy5)
    {
      m_phy5->Clear ();
      m_phy5 = 0;
    }

  if (m_phy6)
    {
      m_phy6->Clear ();
      m_phy6 = 0;
    }
	

  if (m_phy7)
    {
      m_phy7->Clear ();
      m_phy7 = 0;
    }

  if (m_phy8)
    {
      m_phy8->Clear ();
      m_phy8 = 0;
    }

  if (m_phy9)
    {
      m_phy9->Clear ();
      m_phy9 = 0;
    }

  if (m_phy10)
    {
      m_phy10->Clear ();
      m_phy10 = 0;
    }
	
	
  if (m_phy11)
    {
      m_phy11->Clear ();
      m_phy11 = 0;
    }

  if (m_phy12)
    {
      m_phy12->Clear ();
      m_phy12 = 0;
    }

  if (m_phy13)
    {
      m_phy13->Clear ();
      m_phy13 = 0;
    }

  if (m_phy14)
    {
      m_phy14->Clear ();
      m_phy14 = 0;
    }
	
  if (m_phy15)
    {
      m_phy15->Clear ();
      m_phy15 = 0;
    }

  if (m_phy16)
    {
      m_phy16->Clear ();
      m_phy16 = 0;
    }

  if (m_phy17)
    {
      m_phy17->Clear ();
      m_phy17 = 0;
    }

  if (m_phy18)
    {
      m_phy18->Clear ();
      m_phy18 = 0;
    }
}
void
LoraPhyDual::DoDispose ()
{
  Clear ();
  LoraPhy::DoDispose ();
}

TypeId
LoraPhyDual::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPhyDual")
    .SetParent<LoraPhy> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraPhyDual> ()
    .AddAttribute  ("CcaThresholdPhy1",
                    "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy1.",
                    DoubleValue (10),
                    MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy1, &LoraPhyDual::SetCcaThresholdPhy1),
                    MakeDoubleChecker<double> ())
    .AddAttribute ("CcaThresholdPhy2",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy2.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy2, &LoraPhyDual::SetCcaThresholdPhy2),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("CcaThresholdPhy3",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy3.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy3, &LoraPhyDual::SetCcaThresholdPhy3),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CcaThresholdPhy4",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy4.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy4, &LoraPhyDual::SetCcaThresholdPhy4),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("CcaThresholdPhy5",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy5.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy5, &LoraPhyDual::SetCcaThresholdPhy5),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CcaThresholdPhy6",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy6.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy6, &LoraPhyDual::SetCcaThresholdPhy6),
                   MakeDoubleChecker<double> ())
				   
   .AddAttribute ("CcaThresholdPhy7",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy7.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy7, &LoraPhyDual::SetCcaThresholdPhy7),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CcaThresholdPhy8",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy8.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy8, &LoraPhyDual::SetCcaThresholdPhy8),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("CcaThresholdPhy9",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy9.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy9, &LoraPhyDual::SetCcaThresholdPhy9),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CcaThresholdPhy10",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy10.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy10, &LoraPhyDual::SetCcaThresholdPhy10),
                   MakeDoubleChecker<double> ())
				   
	.AddAttribute ("CcaThresholdPhy11",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy11.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy11, &LoraPhyDual::SetCcaThresholdPhy11),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CcaThresholdPhy12",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy12.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy12, &LoraPhyDual::SetCcaThresholdPhy12),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("CcaThresholdPhy13",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy13.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy13, &LoraPhyDual::SetCcaThresholdPhy13),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CcaThresholdPhy14",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy14.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy14, &LoraPhyDual::SetCcaThresholdPhy14),
                   MakeDoubleChecker<double> ())
				   
				      
    .AddAttribute ("CcaThresholdPhy15",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy15.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy15, &LoraPhyDual::SetCcaThresholdPhy15),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CcaThresholdPhy16",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy16.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy16, &LoraPhyDual::SetCcaThresholdPhy16),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("CcaThresholdPhy17",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy17.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy17, &LoraPhyDual::SetCcaThresholdPhy17),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("CcaThresholdPhy18",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB of Phy18.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyDual::GetCcaThresholdPhy18, &LoraPhyDual::SetCcaThresholdPhy18),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("TxPowerPhy1",
                   "Transmission output power in dB of Phy1.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy1, &LoraPhyDual::SetTxPowerDbPhy1),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerPhy2",
                   "Transmission output power in dB of Phy2.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy2, &LoraPhyDual::SetTxPowerDbPhy2),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("TxPowerPhy3",
                   "Transmission output power in dB of Phy3.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy3, &LoraPhyDual::SetTxPowerDbPhy3),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerPhy4",
                   "Transmission output power in dB of Phy4.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy4, &LoraPhyDual::SetTxPowerDbPhy4),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("TxPowerPhy5",
                   "Transmission output power in dB of Phy5.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy5, &LoraPhyDual::SetTxPowerDbPhy5),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerPhy6",
                   "Transmission output power in dB of Phy6.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy6, &LoraPhyDual::SetTxPowerDbPhy6),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("TxPowerPhy7",
                   "Transmission output power in dB of Phy7.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy7, &LoraPhyDual::SetTxPowerDbPhy7),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerPhy8",
                   "Transmission output power in dB of Phy8.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy8, &LoraPhyDual::SetTxPowerDbPhy8),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("TxPowerPhy9",
                   "Transmission output power in dB of Phy9.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy9, &LoraPhyDual::SetTxPowerDbPhy9),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerPhy10",
                   "Transmission output power in dB of Phy10.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy10, &LoraPhyDual::SetTxPowerDbPhy10),
                   MakeDoubleChecker<double> ())
				   
    .AddAttribute ("TxPowerPhy11",
                   "Transmission output power in dB of Phy11.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy11, &LoraPhyDual::SetTxPowerDbPhy11),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerPhy12",
                   "Transmission output power in dB of Phy12.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy12, &LoraPhyDual::SetTxPowerDbPhy12),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("TxPowerPhy13",
                   "Transmission output power in dB of Phy13.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy13, &LoraPhyDual::SetTxPowerDbPhy13),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerPhy14",
                   "Transmission output power in dB of Phy14.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy14, &LoraPhyDual::SetTxPowerDbPhy14),
                   MakeDoubleChecker<double> ())
				   
    .AddAttribute ("TxPowerPhy15",
                   "Transmission output power in dB of Phy15.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy15, &LoraPhyDual::SetTxPowerDbPhy15),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerPhy16",
                   "Transmission output power in dB of Phy16.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy16, &LoraPhyDual::SetTxPowerDbPhy16),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("TxPowerPhy17",
                   "Transmission output power in dB of Phy17.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy17, &LoraPhyDual::SetTxPowerDbPhy17),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPowerPhy18",
                   "Transmission output power in dB of Phy18.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyDual::GetTxPowerDbPhy18, &LoraPhyDual::SetTxPowerDbPhy18),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("RxGainPhy1",
                   "Gain added to incoming signal at receiver of Phy1.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy1, &LoraPhyDual::SetRxGainDbPhy1),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGainPhy2",
                   "Gain added to incoming signal at receiver of Phy2.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy2, &LoraPhyDual::SetRxGainDbPhy2),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("RxGainPhy3",
                   "Gain added to incoming signal at receiver of Phy3.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy3, &LoraPhyDual::SetRxGainDbPhy3),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGainPhy4",
                   "Gain added to incoming signal at receiver of Phy4.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy4, &LoraPhyDual::SetRxGainDbPhy4),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("RxGainPhy5",
                   "Gain added to incoming signal at receiver of Phy5.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy5, &LoraPhyDual::SetRxGainDbPhy5),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGainPhy6",
                   "Gain added to incoming signal at receiver of Phy6.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy6, &LoraPhyDual::SetRxGainDbPhy6),
                   MakeDoubleChecker<double> ())
				   
    .AddAttribute ("RxGainPhy7",
                   "Gain added to incoming signal at receiver of Phy7.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy7, &LoraPhyDual::SetRxGainDbPhy7),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGainPhy8",
                   "Gain added to incoming signal at receiver of Phy8.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy8, &LoraPhyDual::SetRxGainDbPhy8),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("RxGainPhy9",
                   "Gain added to incoming signal at receiver of Phy9.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy9, &LoraPhyDual::SetRxGainDbPhy9),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGainPhy10",
                   "Gain added to incoming signal at receiver of Phy10.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy10, &LoraPhyDual::SetRxGainDbPhy10),
                   MakeDoubleChecker<double> ())
				   
    .AddAttribute ("RxGainPhy11",
                   "Gain added to incoming signal at receiver of Phy11.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy11, &LoraPhyDual::SetRxGainDbPhy11),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGainPhy12",
                   "Gain added to incoming signal at receiver of Phy12.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy12, &LoraPhyDual::SetRxGainDbPhy12),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("RxGainPhy13",
                   "Gain added to incoming signal at receiver of Phy13.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy13, &LoraPhyDual::SetRxGainDbPhy13),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGainPhy14",
                   "Gain added to incoming signal at receiver of Phy14.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy14, &LoraPhyDual::SetRxGainDbPhy14),
                   MakeDoubleChecker<double> ())
				   
    .AddAttribute ("RxGainPhy15",
                   "Gain added to incoming signal at receiver of Phy15.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy15, &LoraPhyDual::SetRxGainDbPhy15),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGainPhy16",
                   "Gain added to incoming signal at receiver of Phy16.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy16, &LoraPhyDual::SetRxGainDbPhy16),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("RxGainPhy17",
                   "Gain added to incoming signal at receiver of Phy17.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy17, &LoraPhyDual::SetRxGainDbPhy17),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGainPhy18",
                   "Gain added to incoming signal at receiver of Phy18.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyDual::GetRxGainDbPhy18, &LoraPhyDual::SetRxGainDbPhy18),
                   MakeDoubleChecker<double> ())

    .AddAttribute ("SupportedModesPhy1",
                   "List of modes supported by Phy1.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy1, &LoraPhyDual::SetModesPhy1),
                   MakeLoraModesListChecker () )
    .AddAttribute ("SupportedModesPhy2",
                   "List of modes supported by Phy2.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy2, &LoraPhyDual::SetModesPhy2),
                   MakeLoraModesListChecker () )

    .AddAttribute ("SupportedModesPhy3",
                   "List of modes supported by Phy3.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy3, &LoraPhyDual::SetModesPhy3),
                   MakeLoraModesListChecker () )
    .AddAttribute ("SupportedModesPhy4",
                   "List of modes supported by Phy4.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy4, &LoraPhyDual::SetModesPhy4),
                   MakeLoraModesListChecker () )

    .AddAttribute ("SupportedModesPhy5",
                   "List of modes supported by Phy5.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy5, &LoraPhyDual::SetModesPhy5),
                   MakeLoraModesListChecker () )
    .AddAttribute ("SupportedModesPhy6",
                   "List of modes supported by Phy6.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy6, &LoraPhyDual::SetModesPhy6),
                   MakeLoraModesListChecker () )
				   
    .AddAttribute ("SupportedModesPhy7",
                   "List of modes supported by Phy7.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy7, &LoraPhyDual::SetModesPhy7),
                   MakeLoraModesListChecker () )
    .AddAttribute ("SupportedModesPhy8",
                   "List of modes supported by Phy8.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy8, &LoraPhyDual::SetModesPhy8),
                   MakeLoraModesListChecker () )

    .AddAttribute ("SupportedModesPhy9",
                   "List of modes supported by Phy9.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy9, &LoraPhyDual::SetModesPhy9),
                   MakeLoraModesListChecker () )
    .AddAttribute ("SupportedModesPhy10",
                   "List of modes supported by Phy10.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy10, &LoraPhyDual::SetModesPhy10),
                   MakeLoraModesListChecker () )
				   
    .AddAttribute ("SupportedModesPhy11",
                   "List of modes supported by Phy11.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy11, &LoraPhyDual::SetModesPhy11),
                   MakeLoraModesListChecker () )
    .AddAttribute ("SupportedModesPhy12",
                   "List of modes supported by Phy12.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy12, &LoraPhyDual::SetModesPhy12),
                   MakeLoraModesListChecker () )

    .AddAttribute ("SupportedModesPhy13",
                   "List of modes supported by Phy13.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy13, &LoraPhyDual::SetModesPhy13),
                   MakeLoraModesListChecker () )
    .AddAttribute ("SupportedModesPhy14",
                   "List of modes supported by Phy14.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy14, &LoraPhyDual::SetModesPhy14),
                   MakeLoraModesListChecker () )
				   
    .AddAttribute ("SupportedModesPhy15",
                   "List of modes supported by Phy15.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy15, &LoraPhyDual::SetModesPhy15),
                   MakeLoraModesListChecker () )
    .AddAttribute ("SupportedModesPhy16",
                   "List of modes supported by Phy16.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy16, &LoraPhyDual::SetModesPhy16),
                   MakeLoraModesListChecker () )

    .AddAttribute ("SupportedModesPhy17",
                   "List of modes supported by Phy17.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy17, &LoraPhyDual::SetModesPhy17),
                   MakeLoraModesListChecker () )
    .AddAttribute ("SupportedModesPhy18",
                   "List of modes supported by Phy18.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyDual::GetModesPhy18, &LoraPhyDual::SetModesPhy18),
                   MakeLoraModesListChecker () )

    .AddAttribute ("PerModelPhy1",
                   "Functor to calculate PER based on SINR and TxMode for Phy1.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy1, &LoraPhyDual::SetPerModelPhy1),
                   MakePointerChecker<LoraPhyPer> ())
    .AddAttribute ("PerModelPhy2",
                   "Functor to calculate PER based on SINR and TxMode for Phy2.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy2, &LoraPhyDual::SetPerModelPhy2),
                   MakePointerChecker<LoraPhyPer> ())

    .AddAttribute ("PerModelPhy3",
                   "Functor to calculate PER based on SINR and TxMode for Phy3.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy3, &LoraPhyDual::SetPerModelPhy3),
                   MakePointerChecker<LoraPhyPer> ())
    .AddAttribute ("PerModelPhy4",
                   "Functor to calculate PER based on SINR and TxMode for Phy4.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy4, &LoraPhyDual::SetPerModelPhy4),
                   MakePointerChecker<LoraPhyPer> ())

    .AddAttribute ("PerModelPhy5",
                   "Functor to calculate PER based on SINR and TxMode for Phy5.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy5, &LoraPhyDual::SetPerModelPhy5),
                   MakePointerChecker<LoraPhyPer> ())
    .AddAttribute ("PerModelPhy6",
                   "Functor to calculate PER based on SINR and TxMode for Phy6.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy6, &LoraPhyDual::SetPerModelPhy6),
                   MakePointerChecker<LoraPhyPer> ())
				   
    .AddAttribute ("PerModelPhy7",
                   "Functor to calculate PER based on SINR and TxMode for Phy7.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy7, &LoraPhyDual::SetPerModelPhy7),
                   MakePointerChecker<LoraPhyPer> ())
    .AddAttribute ("PerModelPhy8",
                   "Functor to calculate PER based on SINR and TxMode for Phy8.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy8, &LoraPhyDual::SetPerModelPhy8),
                   MakePointerChecker<LoraPhyPer> ())

    .AddAttribute ("PerModelPhy9",
                   "Functor to calculate PER based on SINR and TxMode for Phy9.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy9, &LoraPhyDual::SetPerModelPhy9),
                   MakePointerChecker<LoraPhyPer> ())
    .AddAttribute ("PerModelPhy10",
                   "Functor to calculate PER based on SINR and TxMode for Phy10.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy10, &LoraPhyDual::SetPerModelPhy10),
                   MakePointerChecker<LoraPhyPer> ())
				   
    .AddAttribute ("PerModelPhy11",
                   "Functor to calculate PER based on SINR and TxMode for Phy11.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy11, &LoraPhyDual::SetPerModelPhy11),
                   MakePointerChecker<LoraPhyPer> ())
    .AddAttribute ("PerModelPhy12",
                   "Functor to calculate PER based on SINR and TxMode for Phy12.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy12, &LoraPhyDual::SetPerModelPhy12),
                   MakePointerChecker<LoraPhyPer> ())

    .AddAttribute ("PerModelPhy13",
                   "Functor to calculate PER based on SINR and TxMode for Phy13.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy13, &LoraPhyDual::SetPerModelPhy13),
                   MakePointerChecker<LoraPhyPer> ())
    .AddAttribute ("PerModelPhy14",
                   "Functor to calculate PER based on SINR and TxMode for Phy14.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy14, &LoraPhyDual::SetPerModelPhy14),
                   MakePointerChecker<LoraPhyPer> ())
				   
    .AddAttribute ("PerModelPhy15",
                   "Functor to calculate PER based on SINR and TxMode for Phy15.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy15, &LoraPhyDual::SetPerModelPhy15),
                   MakePointerChecker<LoraPhyPer> ())
    .AddAttribute ("PerModelPhy16",
                   "Functor to calculate PER based on SINR and TxMode for Phy16.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy16, &LoraPhyDual::SetPerModelPhy16),
                   MakePointerChecker<LoraPhyPer> ())

    .AddAttribute ("PerModelPhy17",
                   "Functor to calculate PER based on SINR and TxMode for Phy17.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy17, &LoraPhyDual::SetPerModelPhy17),
                   MakePointerChecker<LoraPhyPer> ())
    .AddAttribute ("PerModelPhy18",
                   "Functor to calculate PER based on SINR and TxMode for Phy18.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyDual::GetPerModelPhy18, &LoraPhyDual::SetPerModelPhy18),
                   MakePointerChecker<LoraPhyPer> ())

    .AddAttribute ("SinrModelPhy1",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy1.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy1, &LoraPhyDual::SetSinrModelPhy1),
                   MakePointerChecker<LoraPhyCalcSinr> ())
    .AddAttribute ("SinrModelPhy2",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy2.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy2, &LoraPhyDual::SetSinrModelPhy2),
                   MakePointerChecker<LoraPhyCalcSinr> ())

    .AddAttribute ("SinrModelPhy3",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy3.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy3, &LoraPhyDual::SetSinrModelPhy3),
                   MakePointerChecker<LoraPhyCalcSinr> ())
    .AddAttribute ("SinrModelPhy4",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy4.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy4, &LoraPhyDual::SetSinrModelPhy4),
                   MakePointerChecker<LoraPhyCalcSinr> ())

    .AddAttribute ("SinrModelPhy5",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy5.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy5, &LoraPhyDual::SetSinrModelPhy5),
                   MakePointerChecker<LoraPhyCalcSinr> ())
    .AddAttribute ("SinrModelPhy6",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy6.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy6, &LoraPhyDual::SetSinrModelPhy6),
                   MakePointerChecker<LoraPhyCalcSinr> ())
				   
    .AddAttribute ("SinrModelPhy7",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy7.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy7, &LoraPhyDual::SetSinrModelPhy7),
                   MakePointerChecker<LoraPhyCalcSinr> ())
    .AddAttribute ("SinrModelPhy8",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy8.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy8, &LoraPhyDual::SetSinrModelPhy8),
                   MakePointerChecker<LoraPhyCalcSinr> ())

    .AddAttribute ("SinrModelPhy9",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy9.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy9, &LoraPhyDual::SetSinrModelPhy9),
                   MakePointerChecker<LoraPhyCalcSinr> ())
    .AddAttribute ("SinrModelPhy10",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy10.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy10, &LoraPhyDual::SetSinrModelPhy10),
                   MakePointerChecker<LoraPhyCalcSinr> ())
				   
    .AddAttribute ("SinrModelPhy11",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy11.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy11, &LoraPhyDual::SetSinrModelPhy11),
                   MakePointerChecker<LoraPhyCalcSinr> ())
    .AddAttribute ("SinrModelPhy12",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy12.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy12, &LoraPhyDual::SetSinrModelPhy12),
                   MakePointerChecker<LoraPhyCalcSinr> ())

    .AddAttribute ("SinrModelPhy13",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy13.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy13, &LoraPhyDual::SetSinrModelPhy13),
                   MakePointerChecker<LoraPhyCalcSinr> ())
    .AddAttribute ("SinrModelPhy14",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy14.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy14, &LoraPhyDual::SetSinrModelPhy14),
                   MakePointerChecker<LoraPhyCalcSinr> ())
				   
    .AddAttribute ("SinrModelPhy15",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy15.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy15, &LoraPhyDual::SetSinrModelPhy15),
                   MakePointerChecker<LoraPhyCalcSinr> ())
    .AddAttribute ("SinrModelPhy16",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy16.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy16, &LoraPhyDual::SetSinrModelPhy16),
                   MakePointerChecker<LoraPhyCalcSinr> ())

    .AddAttribute ("SinrModelPhy17",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy17.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy17, &LoraPhyDual::SetSinrModelPhy17),
                   MakePointerChecker<LoraPhyCalcSinr> ())
    .AddAttribute ("SinrModelPhy18",
                   "Functor to calculate SINR based on pkt arrivals and modes for Phy18.",
                   StringValue ("ns3::LoraPhyCalcSinrDual"),
                   MakePointerAccessor (&LoraPhyDual::GetSinrModelPhy18, &LoraPhyDual::SetSinrModelPhy18),
                   MakePointerChecker<LoraPhyCalcSinr> ())

    .AddTraceSource ("RxOk",
                     "A packet was received successfully.",
                     MakeTraceSourceAccessor (&LoraPhyDual::m_rxOkLogger),
                     "ns3::LoraPhy::TracedCallback")
    .AddTraceSource ("RxError",
                     "A packet was received unsuccessfully.",
                     MakeTraceSourceAccessor (&LoraPhyDual::m_rxErrLogger),
                     "ns3::LoraPhy::TracedCallback")
    .AddTraceSource ("Tx",
                     "Packet transmission beginning.",
                     MakeTraceSourceAccessor (&LoraPhyDual::m_txLogger),
                     "ns3::LoraPhy::TracedCallback")

  ;

  return tid;
}

void
LoraPhyDual::SetEnergyModelCallback (DeviceEnergyModel::ChangeStateCallback callback)
{
  NS_LOG_DEBUG ("Not Implemented");
}

void
LoraPhyDual::EnergyDepletionHandler ()
{
  NS_LOG_DEBUG ("Not Implemented");
}

void
LoraPhyDual::SendPacket (Ptr<Packet> pkt, uint32_t modeNum)
{
  if (modeNum <= m_phy1->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy1 with mode number " << modeNum);
      m_txLogger (pkt, m_phy1->GetTxPowerDb (), m_phy1->GetMode (modeNum));
      m_phy1->SendPacket (pkt, modeNum);
    }

  else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy2 with mode number " << modeNum - m_phy1->GetNModes ());
      m_txLogger (pkt, m_phy2->GetTxPowerDb (), m_phy2->GetMode (modeNum - m_phy1->GetNModes ()));
      m_phy2->SendPacket (pkt, modeNum - m_phy1->GetNModes ());
    }
  else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy3 with mode number " << modeNum - 
                                                       (m_phy1->GetNModes () + m_phy2->GetNModes ()));
      m_txLogger (pkt, m_phy3->GetTxPowerDb (), m_phy3->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes ())));
      m_phy3->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () ));
    }
  else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy4 with mode number " << modeNum - 
                                                        (m_phy1->GetNModes () + m_phy2->GetNModes ()  + m_phy3->GetNModes () ));
      m_txLogger (pkt, m_phy4->GetTxPowerDb (), m_phy4->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
                                                       + m_phy3->GetNModes () )));
      m_phy4->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () ));
    } 

  else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy5 with mode number " << modeNum - 
                                      (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () ));
      m_txLogger (pkt, m_phy5->GetTxPowerDb (), m_phy5->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
                                                                + m_phy3->GetNModes () + m_phy4->GetNModes () )));
      m_phy5->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () ));
    } 

  else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy6 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () ));
      m_txLogger (pkt, m_phy6->GetTxPowerDb (), m_phy6->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
                                                 + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () )));
      m_phy6->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
                                                               + m_phy4->GetNModes () + m_phy5->GetNModes () ));
    } 
	
  else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy7 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () ));
      m_txLogger (pkt, m_phy7->GetTxPowerDb (), m_phy7->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () )));
      m_phy7->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () ));
    } 
	
	else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy8 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () ));
      m_txLogger (pkt, m_phy8->GetTxPowerDb (), m_phy8->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () )));
      m_phy8->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () ));
    } 
	
	else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
					  + m_phy9->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy9 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes () ));
      m_txLogger (pkt, m_phy9->GetTxPowerDb (), m_phy9->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () + m_phy8->GetNModes () )));
      m_phy9->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes ()
            + m_phy8->GetNModes () ));
    } 
	
	else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
					  + m_phy9->GetNModes () + m_phy10->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy10 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
            + m_phy9->GetNModes () ));
      m_txLogger (pkt, m_phy10->GetTxPowerDb (), m_phy10->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () + m_phy8->GetNModes () + m_phy9->GetNModes () )));
      m_phy10->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes ()
            + m_phy8->GetNModes () + m_phy9->GetNModes () ));
    } 
	
	else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
					  + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy11 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
            + m_phy9->GetNModes () + m_phy10->GetNModes () ));
      m_txLogger (pkt, m_phy11->GetTxPowerDb (), m_phy11->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes () )));
      m_phy11->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes ()
            + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes () ));
    } 
	
	else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
					  + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
                      + m_phy12->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy12 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
            + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () ));
      m_txLogger (pkt, m_phy12->GetTxPowerDb (), m_phy12->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes ()
            + m_phy11->GetNModes ()	)));
      m_phy12->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes ()
            + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () ));
    } 
	
	else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
					  + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
                      + m_phy12->GetNModes () + m_phy13->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy13 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
            + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes () ));
      m_txLogger (pkt, m_phy13->GetTxPowerDb (), m_phy13->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes ()
            + m_phy11->GetNModes () + m_phy12->GetNModes ()	)));
      m_phy13->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes ()
            + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
            + m_phy12->GetNModes ()	));
    } 
	
	else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
					  + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
                      + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy14 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
            + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
            + m_phy13->GetNModes () ));
      m_txLogger (pkt, m_phy14->GetTxPowerDb (), m_phy14->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes ()
            + m_phy11->GetNModes () + m_phy12->GetNModes () + m_phy13->GetNModes ()	)));
      m_phy14->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes ()
            + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
            + m_phy12->GetNModes () + m_phy13->GetNModes ()	));
    }

    else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
					  + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
                      + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()
					  + m_phy15->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy15 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
            + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
            + m_phy13->GetNModes () + m_phy14->GetNModes () ));
      m_txLogger (pkt, m_phy15->GetTxPowerDb (), m_phy15->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes ()
            + m_phy11->GetNModes () + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()	)));
      m_phy15->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes ()
            + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
            + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()	));
    }

    else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
					  + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
                      + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()
					  + m_phy15->GetNModes () + m_phy16->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy16 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
            + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
            + m_phy13->GetNModes () + m_phy14->GetNModes () + m_phy15->GetNModes () ));
      m_txLogger (pkt, m_phy16->GetTxPowerDb (), m_phy16->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes ()
            + m_phy11->GetNModes () + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()
            + m_phy15->GetNModes () )));
      m_phy16->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes ()
            + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
            + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()	+ m_phy15->GetNModes () ));
    }	

   else if (modeNum <= m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
                      + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
					  + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
                      + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()
					  + m_phy15->GetNModes () + m_phy16->GetNModes () + m_phy17->GetNModes () - 1)
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy17 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
            + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
            + m_phy13->GetNModes () + m_phy14->GetNModes () + m_phy15->GetNModes () + m_phy16->GetNModes () ));
      m_txLogger (pkt, m_phy17->GetTxPowerDb (), m_phy17->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes ()
            + m_phy11->GetNModes () + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()
            + m_phy15->GetNModes () + m_phy16->GetNModes () )));
      m_phy17->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes ()
            + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
            + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()	+ m_phy15->GetNModes ()
            + m_phy16->GetNModes ()	));
    }		
	
	
   else
    {
      NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Sending packet on Phy18 with mode number " << modeNum - 
            (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () 
			+ m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
            + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
            + m_phy13->GetNModes () + m_phy14->GetNModes () + m_phy15->GetNModes () + m_phy16->GetNModes ()
		    + m_phy17->GetNModes () ));
      m_txLogger (pkt, m_phy18->GetTxPowerDb (), m_phy18->GetMode (modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () 
            + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes ()
			+ m_phy7->GetNModes () + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes ()
            + m_phy11->GetNModes () + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()
            + m_phy15->GetNModes () + m_phy16->GetNModes () + m_phy17->GetNModes () )));
      m_phy18->SendPacket (pkt, modeNum - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () 
            + m_phy4->GetNModes () + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes ()
            + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes ()
            + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes ()	+ m_phy15->GetNModes ()
            + m_phy16->GetNModes () + m_phy17->GetNModes ()	));
    } 
	
}
void
LoraPhyDual::RegisterListener (LoraPhyListener *listener)
{
  m_phy1->RegisterListener (listener);
  m_phy2->RegisterListener (listener);

  m_phy3->RegisterListener (listener);
  m_phy4->RegisterListener (listener);

  m_phy5->RegisterListener (listener);
  m_phy6->RegisterListener (listener);
  
  m_phy7->RegisterListener (listener);
  m_phy8->RegisterListener (listener);

  m_phy9->RegisterListener (listener);
  m_phy10->RegisterListener (listener);
  
  m_phy11->RegisterListener (listener);
  m_phy12->RegisterListener (listener);

  m_phy13->RegisterListener (listener);
  m_phy14->RegisterListener (listener);
  
  m_phy15->RegisterListener (listener);
  m_phy16->RegisterListener (listener);

  m_phy17->RegisterListener (listener);
  m_phy18->RegisterListener (listener);
}

void
LoraPhyDual::StartRxPacket (Ptr<Packet> pkt, double rxPowerDb, LoraTxMode txMode, LoraPdp pdp)
{
  // Not called.  StartRxPacket in m_phy* are called directly from Transducer.
}

void
LoraPhyDual::SetReceiveOkCallback (RxOkCallback cb)
{
  m_phy1->SetReceiveOkCallback (cb);
  m_phy2->SetReceiveOkCallback (cb);

  m_phy3->SetReceiveOkCallback (cb);
  m_phy4->SetReceiveOkCallback (cb);

  m_phy5->SetReceiveOkCallback (cb);
  m_phy6->SetReceiveOkCallback (cb);
  
  m_phy7->SetReceiveOkCallback (cb);
  m_phy8->SetReceiveOkCallback (cb);

  m_phy9->SetReceiveOkCallback (cb);
  m_phy10->SetReceiveOkCallback (cb);
  
  m_phy11->SetReceiveOkCallback (cb);
  m_phy12->SetReceiveOkCallback (cb);

  m_phy13->SetReceiveOkCallback (cb);
  m_phy14->SetReceiveOkCallback (cb);
  
  m_phy15->SetReceiveOkCallback (cb);
  m_phy16->SetReceiveOkCallback (cb);

  m_phy17->SetReceiveOkCallback (cb);
  m_phy18->SetReceiveOkCallback (cb);
}

void
LoraPhyDual::SetReceiveErrorCallback (RxErrCallback cb)
{
  m_phy1->SetReceiveErrorCallback (cb);
  m_phy2->SetReceiveErrorCallback (cb);

  m_phy3->SetReceiveErrorCallback (cb);
  m_phy4->SetReceiveErrorCallback (cb);

  m_phy5->SetReceiveErrorCallback (cb);
  m_phy6->SetReceiveErrorCallback (cb);
  
  m_phy7->SetReceiveErrorCallback (cb);
  m_phy8->SetReceiveErrorCallback (cb);

  m_phy9->SetReceiveErrorCallback (cb);
  m_phy10->SetReceiveErrorCallback (cb);
  
  m_phy11->SetReceiveErrorCallback (cb);
  m_phy12->SetReceiveErrorCallback (cb);

  m_phy13->SetReceiveErrorCallback (cb);
  m_phy14->SetReceiveErrorCallback (cb);
  
  m_phy15->SetReceiveErrorCallback (cb);
  m_phy16->SetReceiveErrorCallback (cb);

  m_phy17->SetReceiveErrorCallback (cb);
  m_phy18->SetReceiveErrorCallback (cb);
}

void
LoraPhyDual::SetRxGainDb (double gain)
{
  m_phy1->SetRxGainDb (gain);
  m_phy2->SetRxGainDb (gain);

  m_phy3->SetRxGainDb (gain);
  m_phy4->SetRxGainDb (gain);

  m_phy5->SetRxGainDb (gain);
  m_phy6->SetRxGainDb (gain);
  
  m_phy7->SetRxGainDb (gain);
  m_phy8->SetRxGainDb (gain);

  m_phy9->SetRxGainDb (gain);
  m_phy10->SetRxGainDb (gain);
  
  m_phy11->SetRxGainDb (gain);
  m_phy12->SetRxGainDb (gain);

  m_phy13->SetRxGainDb (gain);
  m_phy14->SetRxGainDb (gain);
  
  m_phy15->SetRxGainDb (gain);
  m_phy16->SetRxGainDb (gain);

  m_phy17->SetRxGainDb (gain);
  m_phy18->SetRxGainDb (gain);
}
void
LoraPhyDual::SetRxGainDbPhy1 (double gain)
{
  m_phy1->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy2 (double gain)
{
  m_phy2->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy3 (double gain)
{
  m_phy3->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy4 (double gain)
{
  m_phy4->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy5 (double gain)
{
  m_phy5->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy6 (double gain)
{
  m_phy6->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy7 (double gain)
{
  m_phy7->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy8 (double gain)
{
  m_phy8->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy9 (double gain)
{
  m_phy9->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy10 (double gain)
{
  m_phy10->SetRxGainDb (gain);
}


void
LoraPhyDual::SetRxGainDbPhy11 (double gain)
{
  m_phy11->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy12 (double gain)
{
  m_phy12->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy13 (double gain)
{
  m_phy13->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy14 (double gain)
{
  m_phy14->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy15 (double gain)
{
  m_phy15->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy16 (double gain)
{
  m_phy16->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy17 (double gain)
{
  m_phy17->SetRxGainDb (gain);
}

void
LoraPhyDual::SetRxGainDbPhy18 (double gain)
{
  m_phy18->SetRxGainDb (gain);
}

void
LoraPhyDual::SetTxPowerDb (double txpwr)
{
  m_phy1->SetTxPowerDb (txpwr);
  m_phy2->SetTxPowerDb (txpwr);

  m_phy3->SetTxPowerDb (txpwr);
  m_phy4->SetTxPowerDb (txpwr);

  m_phy5->SetTxPowerDb (txpwr);
  m_phy6->SetTxPowerDb (txpwr);
  
  m_phy7->SetTxPowerDb (txpwr);
  m_phy8->SetTxPowerDb (txpwr);

  m_phy9->SetTxPowerDb (txpwr);
  m_phy10->SetTxPowerDb (txpwr);
  
  m_phy11->SetTxPowerDb (txpwr);
  m_phy12->SetTxPowerDb (txpwr);

  m_phy13->SetTxPowerDb (txpwr);
  m_phy14->SetTxPowerDb (txpwr);
  
  m_phy15->SetTxPowerDb (txpwr);
  m_phy16->SetTxPowerDb (txpwr);

  m_phy17->SetTxPowerDb (txpwr);
  m_phy18->SetTxPowerDb (txpwr);
}

void
LoraPhyDual::SetTxPowerDbPhy1 (double txpwr)
{
  m_phy1->SetTxPowerDb (txpwr);
}
void
LoraPhyDual::SetTxPowerDbPhy2 (double txpwr)
{
  m_phy2->SetTxPowerDb (txpwr);
}

void
LoraPhyDual::SetTxPowerDbPhy3 (double txpwr)
{
  m_phy3->SetTxPowerDb (txpwr);
}
void
LoraPhyDual::SetTxPowerDbPhy4 (double txpwr)
{
  m_phy4->SetTxPowerDb (txpwr);
}

void
LoraPhyDual::SetTxPowerDbPhy5 (double txpwr)
{
  m_phy5->SetTxPowerDb (txpwr);
}
void
LoraPhyDual::SetTxPowerDbPhy6 (double txpwr)
{
  m_phy6->SetTxPowerDb (txpwr);
}


void
LoraPhyDual::SetTxPowerDbPhy7 (double txpwr)
{
  m_phy7->SetTxPowerDb (txpwr);
}
void
LoraPhyDual::SetTxPowerDbPhy8 (double txpwr)
{
  m_phy8->SetTxPowerDb (txpwr);
}

void
LoraPhyDual::SetTxPowerDbPhy9 (double txpwr)
{
  m_phy9->SetTxPowerDb (txpwr);
}
void
LoraPhyDual::SetTxPowerDbPhy10 (double txpwr)
{
  m_phy10->SetTxPowerDb (txpwr);
}


void
LoraPhyDual::SetTxPowerDbPhy11 (double txpwr)
{
  m_phy11->SetTxPowerDb (txpwr);
}
void
LoraPhyDual::SetTxPowerDbPhy12 (double txpwr)
{
  m_phy12->SetTxPowerDb (txpwr);
}

void
LoraPhyDual::SetTxPowerDbPhy13 (double txpwr)
{
  m_phy13->SetTxPowerDb (txpwr);
}
void
LoraPhyDual::SetTxPowerDbPhy14 (double txpwr)
{
  m_phy14->SetTxPowerDb (txpwr);
}

void
LoraPhyDual::SetTxPowerDbPhy15 (double txpwr)
{
  m_phy15->SetTxPowerDb (txpwr);
}
void
LoraPhyDual::SetTxPowerDbPhy16 (double txpwr)
{
  m_phy16->SetTxPowerDb (txpwr);
}

void
LoraPhyDual::SetTxPowerDbPhy17 (double txpwr)
{
  m_phy17->SetTxPowerDb (txpwr);
}
void
LoraPhyDual::SetTxPowerDbPhy18 (double txpwr)
{
  m_phy18->SetTxPowerDb (txpwr);
}

void
LoraPhyDual::SetRxThresholdDb (double thresh)
{
  NS_LOG_WARN ("SetRxThresholdDb is deprecated and has no effect.  Look at PER Functor attribute");
  m_phy1->SetRxThresholdDb (thresh);
  m_phy2->SetRxThresholdDb (thresh);

  m_phy3->SetRxThresholdDb (thresh);
  m_phy4->SetRxThresholdDb (thresh);

  m_phy5->SetRxThresholdDb (thresh);
  m_phy6->SetRxThresholdDb (thresh);
  
  m_phy7->SetRxThresholdDb (thresh);
  m_phy8->SetRxThresholdDb (thresh);

  m_phy9->SetRxThresholdDb (thresh);
  m_phy10->SetRxThresholdDb (thresh);
  
  m_phy11->SetRxThresholdDb (thresh);
  m_phy12->SetRxThresholdDb (thresh);

  m_phy13->SetRxThresholdDb (thresh);
  m_phy14->SetRxThresholdDb (thresh);
  
  m_phy15->SetRxThresholdDb (thresh);
  m_phy16->SetRxThresholdDb (thresh);

  m_phy17->SetRxThresholdDb (thresh);
  m_phy18->SetRxThresholdDb (thresh);
}

void
LoraPhyDual::SetCcaThresholdDb (double thresh)
{
  m_phy1->SetCcaThresholdDb (thresh);
  m_phy2->SetCcaThresholdDb (thresh);

  m_phy3->SetCcaThresholdDb (thresh);
  m_phy4->SetCcaThresholdDb (thresh);

  m_phy5->SetCcaThresholdDb (thresh);
  m_phy6->SetCcaThresholdDb (thresh);
  
  m_phy7->SetCcaThresholdDb (thresh);
  m_phy8->SetCcaThresholdDb (thresh);

  m_phy9->SetCcaThresholdDb (thresh);
  m_phy10->SetCcaThresholdDb (thresh);
  
  m_phy11->SetCcaThresholdDb (thresh);
  m_phy12->SetCcaThresholdDb (thresh);

  m_phy13->SetCcaThresholdDb (thresh);
  m_phy14->SetCcaThresholdDb (thresh);
  
  m_phy15->SetCcaThresholdDb (thresh);
  m_phy16->SetCcaThresholdDb (thresh);

  m_phy17->SetCcaThresholdDb (thresh);
  m_phy18->SetCcaThresholdDb (thresh);
}

void
LoraPhyDual::SetCcaThresholdPhy1 (double thresh)
{
  m_phy1->SetCcaThresholdDb (thresh);
}
void
LoraPhyDual::SetCcaThresholdPhy2 (double thresh)
{
  m_phy2->SetCcaThresholdDb (thresh);
}

void
LoraPhyDual::SetCcaThresholdPhy3 (double thresh)
{
  m_phy3->SetCcaThresholdDb (thresh);
}
void
LoraPhyDual::SetCcaThresholdPhy4 (double thresh)
{
  m_phy4->SetCcaThresholdDb (thresh);
}

void
LoraPhyDual::SetCcaThresholdPhy5 (double thresh)
{
  m_phy5->SetCcaThresholdDb (thresh);
}
void
LoraPhyDual::SetCcaThresholdPhy6 (double thresh)
{
  m_phy6->SetCcaThresholdDb (thresh);
}

void
LoraPhyDual::SetCcaThresholdPhy7 (double thresh)
{
  m_phy7->SetCcaThresholdDb (thresh);
}
void
LoraPhyDual::SetCcaThresholdPhy8 (double thresh)
{
  m_phy8->SetCcaThresholdDb (thresh);
}

void
LoraPhyDual::SetCcaThresholdPhy9 (double thresh)
{
  m_phy9->SetCcaThresholdDb (thresh);
}
void
LoraPhyDual::SetCcaThresholdPhy10 (double thresh)
{
  m_phy10->SetCcaThresholdDb (thresh);
}


void
LoraPhyDual::SetCcaThresholdPhy11 (double thresh)
{
  m_phy11->SetCcaThresholdDb (thresh);
}
void
LoraPhyDual::SetCcaThresholdPhy12 (double thresh)
{
  m_phy12->SetCcaThresholdDb (thresh);
}

void
LoraPhyDual::SetCcaThresholdPhy13 (double thresh)
{
  m_phy13->SetCcaThresholdDb (thresh);
}
void
LoraPhyDual::SetCcaThresholdPhy14 (double thresh)
{
  m_phy14->SetCcaThresholdDb (thresh);
}

void
LoraPhyDual::SetCcaThresholdPhy15 (double thresh)
{
  m_phy15->SetCcaThresholdDb (thresh);
}
void
LoraPhyDual::SetCcaThresholdPhy16 (double thresh)
{
  m_phy16->SetCcaThresholdDb (thresh);
}

void
LoraPhyDual::SetCcaThresholdPhy17 (double thresh)
{
  m_phy17->SetCcaThresholdDb (thresh);
}
void
LoraPhyDual::SetCcaThresholdPhy18 (double thresh)
{
  m_phy18->SetCcaThresholdDb (thresh);
}


double
LoraPhyDual::GetRxGainDb (void)
{
  NS_LOG_WARN ("Warning: LoraPhyDual::GetRxGainDb returns RxGain of Phy 1");
  return m_phy1->GetRxGainDb ();
}
double
LoraPhyDual::GetRxGainDbPhy1 (void) const
{
  return m_phy1->GetRxGainDb ();
}
double
LoraPhyDual::GetRxGainDbPhy2 (void) const
{
  return m_phy2->GetRxGainDb ();
}

double
LoraPhyDual::GetRxGainDbPhy3 (void) const
{
  return m_phy3->GetRxGainDb ();
}
double
LoraPhyDual::GetRxGainDbPhy4 (void) const
{
  return m_phy4->GetRxGainDb ();
}

double
LoraPhyDual::GetRxGainDbPhy5 (void) const
{
  return m_phy5->GetRxGainDb ();
}
double
LoraPhyDual::GetRxGainDbPhy6 (void) const
{
  return m_phy6->GetRxGainDb ();
}


double
LoraPhyDual::GetRxGainDbPhy7 (void) const
{
  return m_phy7->GetRxGainDb ();
}
double
LoraPhyDual::GetRxGainDbPhy8 (void) const
{
  return m_phy8->GetRxGainDb ();
}

double
LoraPhyDual::GetRxGainDbPhy9 (void) const
{
  return m_phy9->GetRxGainDb ();
}
double
LoraPhyDual::GetRxGainDbPhy10 (void) const
{
  return m_phy10->GetRxGainDb ();
}



double
LoraPhyDual::GetRxGainDbPhy11 (void) const
{
  return m_phy11->GetRxGainDb ();
}
double
LoraPhyDual::GetRxGainDbPhy12 (void) const
{
  return m_phy12->GetRxGainDb ();
}

double
LoraPhyDual::GetRxGainDbPhy13 (void) const
{
  return m_phy13->GetRxGainDb ();
}
double
LoraPhyDual::GetRxGainDbPhy14 (void) const
{
  return m_phy14->GetRxGainDb ();
}

double
LoraPhyDual::GetRxGainDbPhy15 (void) const
{
  return m_phy15->GetRxGainDb ();
}
double
LoraPhyDual::GetRxGainDbPhy16 (void) const
{
  return m_phy16->GetRxGainDb ();
}

double
LoraPhyDual::GetRxGainDbPhy17 (void) const
{
  return m_phy17->GetRxGainDb ();
}
double
LoraPhyDual::GetRxGainDbPhy18 (void) const
{
  return m_phy18->GetRxGainDb ();
}


double
LoraPhyDual::GetTxPowerDb (void)
{
  NS_LOG_WARN ("Warning: Dual Phy only returns TxPowerDb of Phy 1");
  return m_phy1->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy1 (void) const
{
  return m_phy1->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy2 (void) const
{
  return m_phy2->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy3 (void) const
{
  return m_phy3->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy4 (void) const
{
  return m_phy4->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy5 (void) const
{
  return m_phy5->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy6 (void) const
{
  return m_phy6->GetTxPowerDb ();
}


double
LoraPhyDual::GetTxPowerDbPhy7 (void) const
{
  return m_phy7->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy8 (void) const
{
  return m_phy8->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy9 (void) const
{
  return m_phy9->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy10 (void) const
{
  return m_phy10->GetTxPowerDb ();
}


double
LoraPhyDual::GetTxPowerDbPhy11 (void) const
{
  return m_phy11->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy12 (void) const
{
  return m_phy12->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy13 (void) const
{
  return m_phy13->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy14 (void) const
{
  return m_phy14->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy15 (void) const
{
  return m_phy15->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy16 (void) const
{
  return m_phy16->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy17 (void) const
{
  return m_phy17->GetTxPowerDb ();
}

double
LoraPhyDual::GetTxPowerDbPhy18 (void) const
{
  return m_phy18->GetTxPowerDb ();
}

double
LoraPhyDual::GetRxThresholdDb (void)
{
  return m_phy1->GetRxThresholdDb ();
}

double
LoraPhyDual::GetCcaThresholdDb (void)
{
  NS_LOG_WARN ("Dual Phy only returns CCAThreshold of Phy 1");
  return m_phy1->GetCcaThresholdDb ();
}
double
LoraPhyDual::GetCcaThresholdPhy1 (void) const
{
  return m_phy1->GetCcaThresholdDb ();
}
double
LoraPhyDual::GetCcaThresholdPhy2 (void) const
{
  return m_phy2->GetCcaThresholdDb ();
}

double
LoraPhyDual::GetCcaThresholdPhy3 (void) const
{
  return m_phy3->GetCcaThresholdDb ();
}
double
LoraPhyDual::GetCcaThresholdPhy4 (void) const
{
  return m_phy4->GetCcaThresholdDb ();
}

double
LoraPhyDual::GetCcaThresholdPhy5 (void) const
{
  return m_phy5->GetCcaThresholdDb ();
}
double
LoraPhyDual::GetCcaThresholdPhy6 (void) const
{
  return m_phy6->GetCcaThresholdDb ();
}


double
LoraPhyDual::GetCcaThresholdPhy7 (void) const
{
  return m_phy7->GetCcaThresholdDb ();
}
double
LoraPhyDual::GetCcaThresholdPhy8 (void) const
{
  return m_phy8->GetCcaThresholdDb ();
}

double
LoraPhyDual::GetCcaThresholdPhy9 (void) const
{
  return m_phy9->GetCcaThresholdDb ();
}
double
LoraPhyDual::GetCcaThresholdPhy10 (void) const
{
  return m_phy10->GetCcaThresholdDb ();
}


double
LoraPhyDual::GetCcaThresholdPhy11 (void) const
{
  return m_phy11->GetCcaThresholdDb ();
}
double
LoraPhyDual::GetCcaThresholdPhy12 (void) const
{
  return m_phy12->GetCcaThresholdDb ();
}

double
LoraPhyDual::GetCcaThresholdPhy13 (void) const
{
  return m_phy13->GetCcaThresholdDb ();
}
double
LoraPhyDual::GetCcaThresholdPhy14 (void) const
{
  return m_phy14->GetCcaThresholdDb ();
}


double
LoraPhyDual::GetCcaThresholdPhy15 (void) const
{
  return m_phy15->GetCcaThresholdDb ();
}
double
LoraPhyDual::GetCcaThresholdPhy16 (void) const
{
  return m_phy16->GetCcaThresholdDb ();
}

double
LoraPhyDual::GetCcaThresholdPhy17 (void) const
{
  return m_phy17->GetCcaThresholdDb ();
}
double
LoraPhyDual::GetCcaThresholdPhy18 (void) const
{
  return m_phy18->GetCcaThresholdDb ();
}

bool
LoraPhyDual::IsPhy1Idle (void)
{
  return m_phy1->IsStateIdle ();
}
bool
LoraPhyDual::IsPhy2Idle (void)
{
  return m_phy2->IsStateIdle ();
}

bool
LoraPhyDual::IsPhy1Rx (void)
{
  return m_phy1->IsStateRx ();
}

bool
LoraPhyDual::IsPhy2Rx (void)
{
  return m_phy2->IsStateRx ();
}

bool
LoraPhyDual::IsPhy1Tx (void)
{
  return m_phy1->IsStateTx ();
}

bool
LoraPhyDual::IsPhy2Tx (void)
{
  return m_phy2->IsStateTx ();
}

bool
LoraPhyDual::IsPhy3Idle (void)
{
  return m_phy3->IsStateIdle ();
}
bool
LoraPhyDual::IsPhy4Idle (void)
{
  return m_phy4->IsStateIdle ();
}

bool
LoraPhyDual::IsPhy3Rx (void)
{
  return m_phy3->IsStateRx ();
}

bool
LoraPhyDual::IsPhy4Rx (void)
{
  return m_phy4->IsStateRx ();
}

bool
LoraPhyDual::IsPhy3Tx (void)
{
  return m_phy3->IsStateTx ();
}

bool
LoraPhyDual::IsPhy4Tx (void)
{
  return m_phy4->IsStateTx ();
}

bool
LoraPhyDual::IsPhy5Idle (void)
{
  return m_phy5->IsStateIdle ();
}
bool
LoraPhyDual::IsPhy6Idle (void)
{
  return m_phy6->IsStateIdle ();
}

bool
LoraPhyDual::IsPhy5Rx (void)
{
  return m_phy5->IsStateRx ();
}

bool
LoraPhyDual::IsPhy6Rx (void)
{
  return m_phy6->IsStateRx ();
}

bool
LoraPhyDual::IsPhy5Tx (void)
{
  return m_phy5->IsStateTx ();
}

bool
LoraPhyDual::IsPhy6Tx (void)
{
  return m_phy6->IsStateTx ();
}


bool
LoraPhyDual::IsPhy7Idle (void)
{
  return m_phy7->IsStateIdle ();
}
bool
LoraPhyDual::IsPhy8Idle (void)
{
  return m_phy8->IsStateIdle ();
}

bool
LoraPhyDual::IsPhy7Rx (void)
{
  return m_phy7->IsStateRx ();
}

bool
LoraPhyDual::IsPhy8Rx (void)
{
  return m_phy8->IsStateRx ();
}

bool
LoraPhyDual::IsPhy7Tx (void)
{
  return m_phy7->IsStateTx ();
}

bool
LoraPhyDual::IsPhy8Tx (void)
{
  return m_phy8->IsStateTx ();
}


bool
LoraPhyDual::IsPhy9Idle (void)
{
  return m_phy9->IsStateIdle ();
}
bool
LoraPhyDual::IsPhy10Idle (void)
{
  return m_phy10->IsStateIdle ();
}

bool
LoraPhyDual::IsPhy9Rx (void)
{
  return m_phy9->IsStateRx ();
}

bool
LoraPhyDual::IsPhy10Rx (void)
{
  return m_phy10->IsStateRx ();
}

bool
LoraPhyDual::IsPhy9Tx (void)
{
  return m_phy9->IsStateTx ();
}

bool
LoraPhyDual::IsPhy10Tx (void)
{
  return m_phy10->IsStateTx ();
}

bool
LoraPhyDual::IsPhy11Idle (void)
{
  return m_phy11->IsStateIdle ();
}
bool
LoraPhyDual::IsPhy12Idle (void)
{
  return m_phy12->IsStateIdle ();
}

bool
LoraPhyDual::IsPhy11Rx (void)
{
  return m_phy11->IsStateRx ();
}

bool
LoraPhyDual::IsPhy12Rx (void)
{
  return m_phy12->IsStateRx ();
}

bool
LoraPhyDual::IsPhy11Tx (void)
{
  return m_phy11->IsStateTx ();
}

bool
LoraPhyDual::IsPhy12Tx (void)
{
  return m_phy12->IsStateTx ();
}


bool
LoraPhyDual::IsPhy13Idle (void)
{
  return m_phy13->IsStateIdle ();
}
bool
LoraPhyDual::IsPhy14Idle (void)
{
  return m_phy14->IsStateIdle ();
}

bool
LoraPhyDual::IsPhy13Rx (void)
{
  return m_phy13->IsStateRx ();
}

bool
LoraPhyDual::IsPhy14Rx (void)
{
  return m_phy14->IsStateRx ();
}

bool
LoraPhyDual::IsPhy13Tx (void)
{
  return m_phy13->IsStateTx ();
}

bool
LoraPhyDual::IsPhy14Tx (void)
{
  return m_phy14->IsStateTx ();
}


bool
LoraPhyDual::IsPhy15Idle (void)
{
  return m_phy15->IsStateIdle ();
}
bool
LoraPhyDual::IsPhy16Idle (void)
{
  return m_phy16->IsStateIdle ();
}

bool
LoraPhyDual::IsPhy15Rx (void)
{
  return m_phy15->IsStateRx ();
}

bool
LoraPhyDual::IsPhy16Rx (void)
{
  return m_phy16->IsStateRx ();
}

bool
LoraPhyDual::IsPhy15Tx (void)
{
  return m_phy15->IsStateTx ();
}

bool
LoraPhyDual::IsPhy16Tx (void)
{
  return m_phy16->IsStateTx ();
}


bool
LoraPhyDual::IsPhy17Idle (void)
{
  return m_phy17->IsStateIdle ();
}
bool
LoraPhyDual::IsPhy18Idle (void)
{
  return m_phy18->IsStateIdle ();
}

bool
LoraPhyDual::IsPhy17Rx (void)
{
  return m_phy17->IsStateRx ();
}

bool
LoraPhyDual::IsPhy18Rx (void)
{
  return m_phy18->IsStateRx ();
}

bool
LoraPhyDual::IsPhy17Tx (void)
{
  return m_phy17->IsStateTx ();
}

bool
LoraPhyDual::IsPhy18Tx (void)
{
  return m_phy18->IsStateTx ();
}


Ptr<Packet>
LoraPhyDual::GetPhy1PacketRx (void) const
{
  return m_phy1->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy2PacketRx (void) const
{
  return m_phy2->GetPacketRx ();
}


Ptr<Packet>
LoraPhyDual::GetPhy3PacketRx (void) const
{
  return m_phy3->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy4PacketRx (void) const
{
  return m_phy4->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy5PacketRx (void) const
{
  return m_phy5->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy6PacketRx (void) const
{
  return m_phy6->GetPacketRx ();
}


Ptr<Packet>
LoraPhyDual::GetPhy7PacketRx (void) const
{
  return m_phy7->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy8PacketRx (void) const
{
  return m_phy8->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy9PacketRx (void) const
{
  return m_phy9->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy10PacketRx (void) const
{
  return m_phy10->GetPacketRx ();
}


Ptr<Packet>
LoraPhyDual::GetPhy11PacketRx (void) const
{
  return m_phy11->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy12PacketRx (void) const
{
  return m_phy12->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy13PacketRx (void) const
{
  return m_phy13->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy14PacketRx (void) const
{
  return m_phy14->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy15PacketRx (void) const
{
  return m_phy15->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy16PacketRx (void) const
{
  return m_phy16->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy17PacketRx (void) const
{
  return m_phy17->GetPacketRx ();
}

Ptr<Packet>
LoraPhyDual::GetPhy18PacketRx (void) const
{
  return m_phy18->GetPacketRx ();
}


bool
LoraPhyDual::IsStateSleep (void)
{
  return m_phy1->IsStateSleep () && m_phy2->IsStateSleep ();
}
bool
LoraPhyDual::IsStateIdle (void)
{
  return m_phy1->IsStateIdle () && m_phy2->IsStateIdle ();
}
bool
LoraPhyDual::IsStateBusy (void)
{
  return !IsStateIdle () || !IsStateSleep ();
}
bool
LoraPhyDual::IsStateRx (void)
{
  return m_phy1->IsStateRx () || m_phy2->IsStateRx ();
}
bool
LoraPhyDual::IsStateTx (void)
{
  return m_phy1->IsStateTx () || m_phy2->IsStateTx ();
}
bool
LoraPhyDual::IsStateCcaBusy (void)
{
  return m_phy1->IsStateCcaBusy () || m_phy2->IsStateCcaBusy ();
}
Ptr<LoraChannel>
LoraPhyDual::GetChannel (void) const
{
  return m_phy1->GetChannel ();
}
Ptr<LoraNetDevice>
LoraPhyDual::GetDevice (void)
{
  return m_phy1->GetDevice ();
}
void
LoraPhyDual::SetChannel (Ptr<LoraChannel> channel)
{
  m_phy1->SetChannel (channel);
  m_phy2->SetChannel (channel);

  m_phy3->SetChannel (channel);
  m_phy4->SetChannel (channel);

  m_phy5->SetChannel (channel);
  m_phy6->SetChannel (channel);
  
  m_phy7->SetChannel (channel);
  m_phy8->SetChannel (channel);

  m_phy9->SetChannel (channel);
  m_phy10->SetChannel (channel);
  
  
  m_phy11->SetChannel (channel);
  m_phy12->SetChannel (channel);

  m_phy13->SetChannel (channel);
  m_phy14->SetChannel (channel);
  
  m_phy15->SetChannel (channel);
  m_phy16->SetChannel (channel);

  m_phy17->SetChannel (channel);
  m_phy18->SetChannel (channel);
}

void
LoraPhyDual::SetDevice (Ptr<LoraNetDevice> device)
{
  m_phy1->SetDevice (device);
  m_phy2->SetDevice (device);

  m_phy3->SetDevice (device);
  m_phy4->SetDevice (device);

  m_phy5->SetDevice (device);
  m_phy6->SetDevice (device);
  
  m_phy7->SetDevice (device);
  m_phy8->SetDevice (device);

  m_phy9->SetDevice (device);
  m_phy10->SetDevice (device);
  
  m_phy11->SetDevice (device);
  m_phy12->SetDevice (device);

  m_phy13->SetDevice (device);
  m_phy14->SetDevice (device);
  
  m_phy15->SetDevice (device);
  m_phy16->SetDevice (device);

  m_phy17->SetDevice (device);
  m_phy18->SetDevice (device);
}

void
LoraPhyDual::SetMac (Ptr<LoraMac> mac)
{
  m_phy1->SetMac (mac);
  m_phy2->SetMac (mac);

  m_phy3->SetMac (mac);
  m_phy4->SetMac (mac);

  m_phy5->SetMac (mac);
  m_phy6->SetMac (mac);
  
  m_phy7->SetMac (mac);
  m_phy8->SetMac (mac);

  m_phy9->SetMac (mac);
  m_phy10->SetMac (mac);
  
  m_phy11->SetMac (mac);
  m_phy12->SetMac (mac);

  m_phy13->SetMac (mac);
  m_phy14->SetMac (mac);
  
  m_phy15->SetMac (mac);
  m_phy16->SetMac (mac);

  m_phy17->SetMac (mac);
  m_phy18->SetMac (mac);
}

void
LoraPhyDual::NotifyTransStartTx (Ptr<Packet> packet, double txPowerDb, LoraTxMode txMode)
{

}
void
LoraPhyDual::NotifyIntChange (void)
{
  m_phy1->NotifyIntChange ();
  m_phy2->NotifyIntChange ();

  m_phy3->NotifyIntChange ();
  m_phy4->NotifyIntChange ();

  m_phy5->NotifyIntChange ();
  m_phy6->NotifyIntChange ();
  
  m_phy7->NotifyIntChange ();
  m_phy8->NotifyIntChange ();

  m_phy9->NotifyIntChange ();
  m_phy10->NotifyIntChange ();
  
  m_phy11->NotifyIntChange ();
  m_phy12->NotifyIntChange ();

  m_phy13->NotifyIntChange ();
  m_phy14->NotifyIntChange ();
  
  m_phy15->NotifyIntChange ();
  m_phy16->NotifyIntChange ();

  m_phy17->NotifyIntChange ();
  m_phy18->NotifyIntChange ();
}

void
LoraPhyDual::SetTransducer (Ptr<LoraTransducer> trans)
{
  m_phy1->SetTransducer (trans);
  m_phy2->SetTransducer (trans);

  m_phy3->SetTransducer (trans);
  m_phy4->SetTransducer (trans);

  m_phy5->SetTransducer (trans);
  m_phy6->SetTransducer (trans);
  
  m_phy7->SetTransducer (trans);
  m_phy8->SetTransducer (trans);

  m_phy9->SetTransducer (trans);
  m_phy10->SetTransducer (trans);
  
  m_phy11->SetTransducer (trans);
  m_phy12->SetTransducer (trans);

  m_phy13->SetTransducer (trans);
  m_phy14->SetTransducer (trans);
  
  m_phy15->SetTransducer (trans);
  m_phy16->SetTransducer (trans);

  m_phy17->SetTransducer (trans);
  m_phy18->SetTransducer (trans);
}

Ptr<LoraTransducer>
LoraPhyDual::GetTransducer (void)
{
  NS_LOG_WARN ("DualPhy Returning transducer of Phy1");
  return m_phy1->GetTransducer ();
}
uint32_t
LoraPhyDual::GetNModes (void)
{
  return m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes () 
        + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes () + m_phy9->GetNModes () + m_phy10->GetNModes ()
		+ m_phy11->GetNModes () + m_phy12->GetNModes () + m_phy13->GetNModes () + m_phy14->GetNModes () 
		+ m_phy15->GetNModes () + m_phy16->GetNModes () + m_phy17->GetNModes () + m_phy18->GetNModes ();
}

LoraTxMode
LoraPhyDual::GetMode (uint32_t n)
{
  if (n < m_phy1->GetNModes ())
    {
      return m_phy1->GetMode (n);
    }

  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes ())
    {
      return m_phy2->GetMode (n - m_phy1->GetNModes ());
    }
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes ())
    {
      return m_phy3->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () ));
    }
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ())
    {
      return m_phy4->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () ));
    }

  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () + m_phy5->GetNModes ())
    {
      return m_phy5->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes () ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes ())
    {
      return m_phy6->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () )
    {
      return m_phy7->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes () )
   {
      return m_phy8->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes () )
    {
      return m_phy9->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes () ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes () + m_phy10->GetNModes () )
    {
      return m_phy10->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes () ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () )
    {
      return m_phy11->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes ()  + m_phy10->GetNModes ()  ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes () )
    {
      return m_phy12->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes ()  + m_phy10->GetNModes () + m_phy11->GetNModes ()  ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
             + m_phy13->GetNModes () )
   {
      return m_phy13->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes ()  + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes () ));
   }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
             + m_phy13->GetNModes () + m_phy14->GetNModes () )
   {
      return m_phy14->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes ()  + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
             + m_phy13->GetNModes () ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
             + m_phy13->GetNModes () + m_phy14->GetNModes () + m_phy15->GetNModes ())
   {
      return m_phy15->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes ()  + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
             + m_phy13->GetNModes () + m_phy14->GetNModes () ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
             + m_phy13->GetNModes () + m_phy14->GetNModes () + m_phy15->GetNModes () + m_phy16->GetNModes () )
    {
      return m_phy16->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes ()  + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
             + m_phy13->GetNModes () + m_phy14->GetNModes () + m_phy15->GetNModes () ));
    }
	
  else if (n < m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
             + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes () + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
             + m_phy13->GetNModes () + m_phy14->GetNModes () + m_phy15->GetNModes () + m_phy16->GetNModes ()
             + m_phy17->GetNModes () )
    {
         return m_phy17->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes ()  + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
             + m_phy13->GetNModes () + m_phy14->GetNModes () + m_phy15->GetNModes () + m_phy16->GetNModes () ));
    }
	
  else 
    {
         return m_phy18->GetMode (n - (m_phy1->GetNModes () + m_phy2->GetNModes () + m_phy3->GetNModes () + m_phy4->GetNModes ()
	         + m_phy5->GetNModes () + m_phy6->GetNModes () + m_phy7->GetNModes () + m_phy8->GetNModes ()
             + m_phy9->GetNModes ()  + m_phy10->GetNModes () + m_phy11->GetNModes () + m_phy12->GetNModes ()
             + m_phy13->GetNModes () + m_phy14->GetNModes () + m_phy15->GetNModes () + m_phy16->GetNModes ()
		     + m_phy17->GetNModes () ));
    }
}


LoraModesList
LoraPhyDual::GetModesPhy1 (void) const
{

  LoraModesListValue modeValue;
  m_phy1->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy2 (void) const
{
  LoraModesListValue modeValue;
  m_phy2->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy3 (void) const
{

  LoraModesListValue modeValue;
  m_phy3->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy4 (void) const
{
  LoraModesListValue modeValue;
  m_phy4->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}


LoraModesList
LoraPhyDual::GetModesPhy5 (void) const
{

  LoraModesListValue modeValue;
  m_phy5->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy6 (void) const
{
  LoraModesListValue modeValue;
  m_phy6->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}


LoraModesList
LoraPhyDual::GetModesPhy7 (void) const
{

  LoraModesListValue modeValue;
  m_phy7->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy8 (void) const
{
  LoraModesListValue modeValue;
  m_phy8->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}


LoraModesList
LoraPhyDual::GetModesPhy9 (void) const
{

  LoraModesListValue modeValue;
  m_phy9->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy10 (void) const
{
  LoraModesListValue modeValue;
  m_phy10->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy11 (void) const
{

  LoraModesListValue modeValue;
  m_phy11->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy12 (void) const
{
  LoraModesListValue modeValue;
  m_phy12->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}


LoraModesList
LoraPhyDual::GetModesPhy13 (void) const
{

  LoraModesListValue modeValue;
  m_phy13->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy14 (void) const
{
  LoraModesListValue modeValue;
  m_phy14->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}


LoraModesList
LoraPhyDual::GetModesPhy15 (void) const
{

  LoraModesListValue modeValue;
  m_phy15->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy16 (void) const
{
  LoraModesListValue modeValue;
  m_phy16->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}


LoraModesList
LoraPhyDual::GetModesPhy17 (void) const
{

  LoraModesListValue modeValue;
  m_phy17->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}

LoraModesList
LoraPhyDual::GetModesPhy18 (void) const
{
  LoraModesListValue modeValue;
  m_phy18->GetAttribute ("SupportedModes", modeValue);
  return modeValue.Get ();
}


void
LoraPhyDual::SetModesPhy1 (LoraModesList modes)
{
  m_phy1->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}

void
LoraPhyDual::SetModesPhy2 (LoraModesList modes)
{
  m_phy2->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}


void
LoraPhyDual::SetModesPhy3 (LoraModesList modes)
{
  m_phy3->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}

void
LoraPhyDual::SetModesPhy4 (LoraModesList modes)
{
  m_phy4->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}


void
LoraPhyDual::SetModesPhy5 (LoraModesList modes)
{
  m_phy5->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}

void
LoraPhyDual::SetModesPhy6 (LoraModesList modes)
{
  m_phy6->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}


void
LoraPhyDual::SetModesPhy7 (LoraModesList modes)
{
  m_phy7->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}

void
LoraPhyDual::SetModesPhy8 (LoraModesList modes)
{
  m_phy8->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}


void
LoraPhyDual::SetModesPhy9 (LoraModesList modes)
{
  m_phy9->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}

void
LoraPhyDual::SetModesPhy10 (LoraModesList modes)
{
  m_phy10->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}


void
LoraPhyDual::SetModesPhy11 (LoraModesList modes)
{
  m_phy11->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}

void
LoraPhyDual::SetModesPhy12 (LoraModesList modes)
{
  m_phy12->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}


void
LoraPhyDual::SetModesPhy13 (LoraModesList modes)
{
  m_phy13->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}

void
LoraPhyDual::SetModesPhy14 (LoraModesList modes)
{
  m_phy14->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}


void
LoraPhyDual::SetModesPhy15 (LoraModesList modes)
{
  m_phy15->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}

void
LoraPhyDual::SetModesPhy16 (LoraModesList modes)
{
  m_phy16->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}


void
LoraPhyDual::SetModesPhy17 (LoraModesList modes)
{
  m_phy17->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}

void
LoraPhyDual::SetModesPhy18 (LoraModesList modes)
{
  m_phy18->SetAttribute ("SupportedModes", LoraModesListValue (modes));
}


Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy1 (void) const
{
  PointerValue perValue;
  m_phy1->GetAttribute ("PerModel", perValue);
  return perValue;
}

Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy2 (void) const
{
  PointerValue perValue;
  m_phy2->GetAttribute ("PerModel", perValue);
  return perValue;
}


Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy3 (void) const
{
  PointerValue perValue;
  m_phy3->GetAttribute ("PerModel", perValue);
  return perValue;
}

Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy4 (void) const
{
  PointerValue perValue;
  m_phy4->GetAttribute ("PerModel", perValue);
  return perValue;
}


Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy5 (void) const
{
  PointerValue perValue;
  m_phy5->GetAttribute ("PerModel", perValue);
  return perValue;
}

Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy6 (void) const
{
  PointerValue perValue;
  m_phy6->GetAttribute ("PerModel", perValue);
  return perValue;
}


Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy7 (void) const
{
  PointerValue perValue;
  m_phy7->GetAttribute ("PerModel", perValue);
  return perValue;
}

Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy8 (void) const
{
  PointerValue perValue;
  m_phy8->GetAttribute ("PerModel", perValue);
  return perValue;
}


Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy9 (void) const
{
  PointerValue perValue;
  m_phy9->GetAttribute ("PerModel", perValue);
  return perValue;
}

Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy10 (void) const
{
  PointerValue perValue;
  m_phy10->GetAttribute ("PerModel", perValue);
  return perValue;
}


Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy11 (void) const
{
  PointerValue perValue;
  m_phy11->GetAttribute ("PerModel", perValue);
  return perValue;
}

Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy12 (void) const
{
  PointerValue perValue;
  m_phy12->GetAttribute ("PerModel", perValue);
  return perValue;
}


Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy13 (void) const
{
  PointerValue perValue;
  m_phy13->GetAttribute ("PerModel", perValue);
  return perValue;
}

Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy14 (void) const
{
  PointerValue perValue;
  m_phy14->GetAttribute ("PerModel", perValue);
  return perValue;
}


Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy15 (void) const
{
  PointerValue perValue;
  m_phy15->GetAttribute ("PerModel", perValue);
  return perValue;
}

Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy16 (void) const
{
  PointerValue perValue;
  m_phy16->GetAttribute ("PerModel", perValue);
  return perValue;
}


Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy17 (void) const
{
  PointerValue perValue;
  m_phy17->GetAttribute ("PerModel", perValue);
  return perValue;
}

Ptr<LoraPhyPer>
LoraPhyDual::GetPerModelPhy18 (void) const
{
  PointerValue perValue;
  m_phy18->GetAttribute ("PerModel", perValue);
  return perValue;
}


void
LoraPhyDual::SetPerModelPhy1 (Ptr<LoraPhyPer> per)
{
  m_phy1->SetAttribute ("PerModel", PointerValue (per));
}

void
LoraPhyDual::SetPerModelPhy2 (Ptr<LoraPhyPer> per)
{
  m_phy2->SetAttribute ("PerModel", PointerValue (per));
}


void
LoraPhyDual::SetPerModelPhy3 (Ptr<LoraPhyPer> per)
{
  m_phy3->SetAttribute ("PerModel", PointerValue (per));
}

void
LoraPhyDual::SetPerModelPhy4 (Ptr<LoraPhyPer> per)
{
  m_phy4->SetAttribute ("PerModel", PointerValue (per));
}


void
LoraPhyDual::SetPerModelPhy5 (Ptr<LoraPhyPer> per)
{
  m_phy5->SetAttribute ("PerModel", PointerValue (per));
}

void
LoraPhyDual::SetPerModelPhy6 (Ptr<LoraPhyPer> per)
{
  m_phy6->SetAttribute ("PerModel", PointerValue (per));
}


void
LoraPhyDual::SetPerModelPhy7 (Ptr<LoraPhyPer> per)
{
  m_phy7->SetAttribute ("PerModel", PointerValue (per));
}

void
LoraPhyDual::SetPerModelPhy8 (Ptr<LoraPhyPer> per)
{
  m_phy8->SetAttribute ("PerModel", PointerValue (per));
}


void
LoraPhyDual::SetPerModelPhy9 (Ptr<LoraPhyPer> per)
{
  m_phy9->SetAttribute ("PerModel", PointerValue (per));
}

void
LoraPhyDual::SetPerModelPhy10 (Ptr<LoraPhyPer> per)
{
  m_phy10->SetAttribute ("PerModel", PointerValue (per));
}


void
LoraPhyDual::SetPerModelPhy11 (Ptr<LoraPhyPer> per)
{
  m_phy11->SetAttribute ("PerModel", PointerValue (per));
}

void
LoraPhyDual::SetPerModelPhy12 (Ptr<LoraPhyPer> per)
{
  m_phy12->SetAttribute ("PerModel", PointerValue (per));
}


void
LoraPhyDual::SetPerModelPhy13 (Ptr<LoraPhyPer> per)
{
  m_phy13->SetAttribute ("PerModel", PointerValue (per));
}

void
LoraPhyDual::SetPerModelPhy14 (Ptr<LoraPhyPer> per)
{
  m_phy14->SetAttribute ("PerModel", PointerValue (per));
}


void
LoraPhyDual::SetPerModelPhy15 (Ptr<LoraPhyPer> per)
{
  m_phy15->SetAttribute ("PerModel", PointerValue (per));
}

void
LoraPhyDual::SetPerModelPhy16 (Ptr<LoraPhyPer> per)
{
  m_phy16->SetAttribute ("PerModel", PointerValue (per));
}


void
LoraPhyDual::SetPerModelPhy17 (Ptr<LoraPhyPer> per)
{
  m_phy17->SetAttribute ("PerModel", PointerValue (per));
}

void
LoraPhyDual::SetPerModelPhy18 (Ptr<LoraPhyPer> per)
{
  m_phy18->SetAttribute ("PerModel", PointerValue (per));
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy1 (void) const
{
  PointerValue sinrValue;
  m_phy1->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy2 (void) const
{
  PointerValue sinrValue;
  m_phy2->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy3 (void) const
{
  PointerValue sinrValue;
  m_phy3->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy4 (void) const
{
  PointerValue sinrValue;
  m_phy4->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}


Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy5 (void) const
{
  PointerValue sinrValue;
  m_phy5->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy6 (void) const
{
  PointerValue sinrValue;
  m_phy6->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}


Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy7 (void) const
{
  PointerValue sinrValue;
  m_phy7->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy8 (void) const
{
  PointerValue sinrValue;
  m_phy8->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}


Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy9 (void) const
{
  PointerValue sinrValue;
  m_phy9->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy10 (void) const
{
  PointerValue sinrValue;
  m_phy10->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}



Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy11 (void) const
{
  PointerValue sinrValue;
  m_phy11->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy12 (void) const
{
  PointerValue sinrValue;
  m_phy12->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}


Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy13 (void) const
{
  PointerValue sinrValue;
  m_phy13->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy14 (void) const
{
  PointerValue sinrValue;
  m_phy14->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}


Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy15 (void) const
{
  PointerValue sinrValue;
  m_phy15->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy16 (void) const
{
  PointerValue sinrValue;
  m_phy16->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}


Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy17 (void) const
{
  PointerValue sinrValue;
  m_phy17->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

Ptr<LoraPhyCalcSinr>
LoraPhyDual::GetSinrModelPhy18 (void) const
{
  PointerValue sinrValue;
  m_phy18->GetAttribute ("SinrModel", sinrValue);
  return sinrValue;
}

void
LoraPhyDual::SetSinrModelPhy1 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy1->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::SetSinrModelPhy2 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy2->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::SetSinrModelPhy3 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy3->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::SetSinrModelPhy4 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy4->SetAttribute ("SinrModel", PointerValue (sinr));
}


void
LoraPhyDual::SetSinrModelPhy5 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy5->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::SetSinrModelPhy6 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy6->SetAttribute ("SinrModel", PointerValue (sinr));
}


void
LoraPhyDual::SetSinrModelPhy7 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy7->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::SetSinrModelPhy8 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy8->SetAttribute ("SinrModel", PointerValue (sinr));
}


void
LoraPhyDual::SetSinrModelPhy9 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy9->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::SetSinrModelPhy10 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy10->SetAttribute ("SinrModel", PointerValue (sinr));
}


void
LoraPhyDual::SetSinrModelPhy11 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy11->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::SetSinrModelPhy12 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy12->SetAttribute ("SinrModel", PointerValue (sinr));
}


void
LoraPhyDual::SetSinrModelPhy13 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy13->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::SetSinrModelPhy14 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy14->SetAttribute ("SinrModel", PointerValue (sinr));
}


void
LoraPhyDual::SetSinrModelPhy15 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy15->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::SetSinrModelPhy16 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy16->SetAttribute ("SinrModel", PointerValue (sinr));
}


void
LoraPhyDual::SetSinrModelPhy17 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy17->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::SetSinrModelPhy18 (Ptr<LoraPhyCalcSinr> sinr)
{
  m_phy18->SetAttribute ("SinrModel", PointerValue (sinr));
}

void
LoraPhyDual::RxOkFromSubPhy (Ptr<Packet> pkt, double sinr, LoraTxMode mode)
{
  NS_LOG_DEBUG (Simulator::Now ().GetSeconds () << " Received packet");
  m_recOkCb (pkt, sinr, mode);
  m_rxOkLogger (pkt, sinr, mode);
}

void
LoraPhyDual::RxErrFromSubPhy (Ptr<Packet> pkt, double sinr)
{
  m_recErrCb (pkt, sinr);
  m_rxErrLogger (pkt, sinr, m_phy1->GetMode (0));
}

Ptr<Packet>
LoraPhyDual::GetPacketRx (void) const
{
  NS_FATAL_ERROR ("GetPacketRx not valid for LoraPhyDual.  Must specify GetPhy1PacketRx or GetPhy2PacketRx");
  return Create<Packet> ();
}

int64_t
LoraPhyDual::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  return 0;
}

}

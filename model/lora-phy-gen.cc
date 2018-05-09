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
 *         Andrea Sacco <andrea.sacco85@gmail.com>
 *         To Thanh Hai <tthhai@gmail.com>
 */

#include "lora-phy-gen.h"
#include "lora-transducer.h"
#include "lora-channel.h"
#include "lora-net-device.h"
#include "ns3/simulator.h"
#include "ns3/traced-callback.h"
#include "ns3/ptr.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/lora-tx-mode.h"
#include "ns3/node.h"
#include "ns3/uinteger.h"
#include "ns3/energy-source-container.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LoraPhyGen");

NS_OBJECT_ENSURE_REGISTERED (LoraPhyGen);
NS_OBJECT_ENSURE_REGISTERED (LoraPhyPerGenDefault);
NS_OBJECT_ENSURE_REGISTERED (LoraPhyCalcSinrDefault);
NS_OBJECT_ENSURE_REGISTERED (LoraPhyCalcSinrFhFsk);
NS_OBJECT_ENSURE_REGISTERED (LoraPhyPerUmodem);


/*************** LoraPhyCalcSinrDefault definition *****************/
LoraPhyCalcSinrDefault::LoraPhyCalcSinrDefault ()
{

}
LoraPhyCalcSinrDefault::~LoraPhyCalcSinrDefault ()
{

}

TypeId
LoraPhyCalcSinrDefault::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPhyCalcSinrDefault")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraPhyCalcSinrDefault> ()
  ;
  return tid;
}

double
LoraPhyCalcSinrDefault::CalcSinrDb (Ptr<Packet> pkt,
                                   Time arrTime,
                                   double rxPowerDb,
                                   double ambNoiseDb,
                                   LoraTxMode mode,
                                   LoraPdp pdp,
                                   const LoraTransducer::ArrivalList &arrivalList) const
{
  if (mode.GetModType () == LoraTxMode::OTHER)
    {
      NS_LOG_WARN ("Calculating SINR for unsupported modulation type");
    }

  double intKp = -DbToKp (rxPowerDb); // This packet is in the arrivalList
  LoraTransducer::ArrivalList::const_iterator it = arrivalList.begin ();
  for (; it != arrivalList.end (); it++)
    {
      intKp += DbToKp (it->GetRxPowerDb ());
    }

  double totalIntDb = KpToDb (intKp + DbToKp (ambNoiseDb));

  NS_LOG_DEBUG ("Calculating SINR:  RxPower = " << rxPowerDb << " dB.  Number of interferers = " << arrivalList.size () << "  Interference + noise power = " << totalIntDb << " dB.  SINR = " << rxPowerDb - totalIntDb << " dB.");
  return rxPowerDb - totalIntDb;
}

/*************** LoraPhyCalcSinrFhFsk definition *****************/
LoraPhyCalcSinrFhFsk::LoraPhyCalcSinrFhFsk ()
{

}
LoraPhyCalcSinrFhFsk::~LoraPhyCalcSinrFhFsk ()
{

}

TypeId
LoraPhyCalcSinrFhFsk::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPhyCalcSinrFhFsk")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraPhyCalcSinrFhFsk> ()
    .AddAttribute ("NumberOfHops",
                   "Number of frequencies in hopping pattern.",
                   UintegerValue (13),
                   MakeUintegerAccessor (&LoraPhyCalcSinrFhFsk::m_hops),
                   MakeUintegerChecker<uint32_t> ())
  ;
  return tid;
}
double
LoraPhyCalcSinrFhFsk::CalcSinrDb (Ptr<Packet> pkt,
                                 Time arrTime,
                                 double rxPowerDb,
                                 double ambNoiseDb,
                                 LoraTxMode mode,
                                 LoraPdp pdp,
                                 const LoraTransducer::ArrivalList &arrivalList) const
{
  if (mode.GetModType () != LoraTxMode::FSK)
    {
      NS_LOG_WARN ("Calculating SINR for unsupported mode type");
    }



  double ts = 1.0 / mode.GetPhyRateSps ();
  double clearingTime = (m_hops - 1.0) * ts;
  double csp = pdp.SumTapsFromMaxNc (Seconds (0), Seconds (ts));

  // Get maximum arrival offset
  double maxAmp = -1;
  double maxTapDelay = 0.0;
  LoraPdp::Iterator pit = pdp.GetBegin ();
  for (; pit != pdp.GetEnd (); pit++)
    {
      if (std::abs (pit->GetAmp ()) > maxAmp)
        {
          maxAmp = std::abs (pit->GetAmp ());
          maxTapDelay = pit->GetDelay ().GetSeconds ();
        }
    }


  double effRxPowerDb = rxPowerDb + KpToDb (csp);

  double isiUpa = rxPowerDb * pdp.SumTapsFromMaxNc (Seconds (ts + clearingTime), Seconds (ts));
  LoraTransducer::ArrivalList::const_iterator it = arrivalList.begin ();
  double intKp = -DbToKp (effRxPowerDb);
  for (; it != arrivalList.end (); it++)
    {
      LoraPdp intPdp = it->GetPdp ();
      double tDelta = std::abs (arrTime.GetSeconds () + maxTapDelay - it->GetArrivalTime ().GetSeconds ());
      // We want tDelta in terms of a single symbol (i.e. if tDelta = 7.3 symbol+clearing
      // times, the offset in terms of the arriving symbol power is
      // 0.3 symbol+clearing times.

      int32_t syms = (uint32_t)( (double) tDelta / (ts + clearingTime));
      tDelta = tDelta - syms * (ts + clearingTime);

      // Align to pktRx
      if (arrTime + Seconds (maxTapDelay)  > it->GetArrivalTime ())
        {
          tDelta = ts + clearingTime - tDelta;
        }

      double intPower = 0.0;
      if (tDelta < ts)
        {
          intPower += intPdp.SumTapsNc (Seconds (0), Seconds (ts - tDelta));
          intPower += intPdp.SumTapsNc (Seconds (ts - tDelta + clearingTime),
                                        Seconds (2 * ts - tDelta + clearingTime));
        }
      else
        {
          Time start = Seconds (ts + clearingTime - tDelta);
          Time end = start + Seconds (ts);
          intPower += intPdp.SumTapsNc (start, end);

          start = start + Seconds (ts + clearingTime);
          end = start + Seconds (ts);
          intPower += intPdp.SumTapsNc (start, end);
        }
      intKp += DbToKp (it->GetRxPowerDb ()) * intPower;
    }

  double totalIntDb = KpToDb (isiUpa + intKp + DbToKp (ambNoiseDb));

  NS_LOG_DEBUG ("Calculating SINR:  RxPower = " << rxPowerDb << " dB.  Effective Rx power " << effRxPowerDb << " dB.  Number of interferers = " << arrivalList.size () << "  Interference + noise power = " << totalIntDb << " dB.  SINR = " << effRxPowerDb - totalIntDb << " dB.");
  return effRxPowerDb - totalIntDb;
}

/*************** LoraPhyPerGenDefault definition *****************/
LoraPhyPerGenDefault::LoraPhyPerGenDefault ()
{

}

LoraPhyPerGenDefault::~LoraPhyPerGenDefault ()
{

}
TypeId
LoraPhyPerGenDefault::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPhyPerGenDefault")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraPhyPerGenDefault> ()
    .AddAttribute ("Threshold", "SINR cutoff for good packet reception.",
                   DoubleValue (8),
                   MakeDoubleAccessor (&LoraPhyPerGenDefault::m_thresh),
                   MakeDoubleChecker<double> ());
  return tid;
}


// Default PER calculation simply compares SINR to a threshold which is configurable
// via an attribute.
double
LoraPhyPerGenDefault::CalcPer (Ptr<Packet> pkt, double sinrDb, LoraTxMode mode)
{
  if (sinrDb >= m_thresh)
    {
      return 0;
    }
  else
    {
      return 1;
    }
}

/*************** LoraPhyPerUmodem definition *****************/
LoraPhyPerUmodem::LoraPhyPerUmodem ()
{

}
LoraPhyPerUmodem::~LoraPhyPerUmodem ()
{

}

TypeId LoraPhyPerUmodem::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPhyPerUmodem")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraPhyPerUmodem> ()
  ;
  return tid;
}

double
LoraPhyPerUmodem::NChooseK (uint32_t n, uint32_t k)
{
  double result;

  result = 1.0;

  for (uint32_t i = std::max (k,n - k) + 1; i <= n; ++i)
    {
      result *= i;
    }

  for (uint32_t i = 2; i <= std::min (k,n - k); ++i)
    {
      result /= i;
    }

  return result;
}

double
LoraPhyPerUmodem::CalcPer (Ptr<Packet> pkt, double sinr, LoraTxMode mode)
{
  uint32_t d[] =
  { 12, 14, 16, 18, 20, 22, 24, 26, 28 };
  double Bd[] =
  {
    33, 281, 2179, 15035LLU, 105166LLU, 692330LLU, 4580007LLU, 29692894LLU,
    190453145LLU
  };

  // double Rc = 1.0 / 2.0;
  double ebno = std::pow (10.0, sinr / 10.0);
  double perror = 1.0 / (2.0 + ebno);
  double P[9];

  if (sinr >= 10)
    {
      return 0;
    }
  if (sinr <= 6)
    {
      return 1;
    }

  for (uint32_t r = 0; r < 9; r++)
    {
      double sumd = 0;
      for (uint32_t k = 0; k < d[r]; k++)
        {
          sumd = sumd + NChooseK (d[r] - 1 + k, k) * std::pow (1 - perror, (double) k);
        }
      P[r] = std::pow (perror, (double) d[r]) * sumd;

    }

  double Pb = 0;
  for (uint32_t r = 0; r < 8; r++)
    {
      Pb = Pb + Bd[r] * P[r];
    }

  // cout << "Pb = " << Pb << endl;
  uint32_t bits = pkt->GetSize () * 8;

  double Ppacket = 1;
  double temp = NChooseK (bits, 0);
  temp *= std::pow ( (1 - Pb), (double) bits);
  Ppacket -= temp;
  temp = NChooseK (288, 1) * Pb * std::pow ( (1 - Pb), bits - 1.0);
  Ppacket -= temp;

  if (Ppacket > 1)
    {
      return 1;
    }
  else
    {
      return Ppacket;
    }
}

/*************** LoraPhyGen definition *****************/
LoraPhyGen::LoraPhyGen ()
  : LoraPhy (),
    m_state (IDLE),
    m_channel (0),
    m_transducer (0),
    m_device (0),
    m_mac (0),
    m_rxGainDb (0),
    m_txPwrDb (0),
    m_rxThreshDb (0),
    m_ccaThreshDb (0),
    m_pktRx (0),
    m_cleared (false),
    m_disabled (false)
{
  m_pg = CreateObject<UniformRandomVariable> ();

  m_energyCallback.Nullify ();
}

LoraPhyGen::~LoraPhyGen ()
{

}

void
LoraPhyGen::Clear ()
{
  if (m_cleared)
    {
      return;
    }
  m_cleared = true;
  m_listeners.clear ();
  if (m_channel)
    {
      m_channel->Clear ();
      m_channel = 0;
    }
  if (m_transducer)
    {
      m_transducer->Clear ();
      m_transducer = 0;
    }
  if (m_device)
    {
      m_device->Clear ();
      m_device = 0;
    }
  if (m_mac)
    {
      m_mac->Clear ();
      m_mac = 0;
    }
  if (m_per)
    {
      m_per->Clear ();
      m_per = 0;
    }
  if (m_sinr)
    {
      m_sinr->Clear ();
      m_sinr = 0;
    }
  m_pktRx = 0;
}

void
LoraPhyGen::DoDispose ()
{
  Clear ();
  m_energyCallback.Nullify ();
  LoraPhy::DoDispose ();
}

LoraModesList
LoraPhyGen::GetDefaultModes (void)
{
  LoraModesList l;
  l.AppendMode (LoraTxModeFactory::CreateMode (LoraTxMode::FSK,80,80,22000,4000,13,"FSK"));
  l.AppendMode (LoraTxModeFactory::CreateMode (LoraTxMode::PSK,200, 200, 22000, 4000, 4, "QPSK"));
  return l;
}
TypeId
LoraPhyGen::GetTypeId (void)
{

  static TypeId tid = TypeId ("ns3::LoraPhyGen")
    .SetParent<LoraPhy> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraPhyGen> ()
    .AddAttribute ("CcaThreshold",
                   "Aggregate energy of incoming signals to move to CCA Busy state dB.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyGen::m_ccaThreshDb),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxThreshold",
                   "Required SNR for signal acquisition in dB.",
                   DoubleValue (10),
                   MakeDoubleAccessor (&LoraPhyGen::m_rxThreshDb),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("TxPower",
                   "Transmission output power in dB.",
                   DoubleValue (190),
                   MakeDoubleAccessor (&LoraPhyGen::m_txPwrDb),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("RxGain",
                   "Gain added to incoming signal at receiver.",
                   DoubleValue (0),
                   MakeDoubleAccessor (&LoraPhyGen::m_rxGainDb),
                   MakeDoubleChecker<double> ())
    .AddAttribute ("SupportedModes",
                   "List of modes supported by this PHY.",
                   LoraModesListValue (LoraPhyGen::GetDefaultModes ()),
                   MakeLoraModesListAccessor (&LoraPhyGen::m_modes),
                   MakeLoraModesListChecker () )
    .AddAttribute ("PerModel",
                   "Functor to calculate PER based on SINR and TxMode.",
                   StringValue ("ns3::LoraPhyPerGenDefault"),
                   MakePointerAccessor (&LoraPhyGen::m_per),
                   MakePointerChecker<LoraPhyPer> ())
    .AddAttribute ("SinrModel",
                   "Functor to calculate SINR based on pkt arrivals and modes.",
                   StringValue ("ns3::LoraPhyCalcSinrDefault"),
                   MakePointerAccessor (&LoraPhyGen::m_sinr),
                   MakePointerChecker<LoraPhyCalcSinr> ())
    .AddTraceSource ("RxOk",
                     "A packet was received successfully.",
                     MakeTraceSourceAccessor (&LoraPhyGen::m_rxOkLogger),
                     "ns3::LoraPhy::TracedCallback")
    .AddTraceSource ("RxError",
                     "A packet was received unsuccessfully.",
                     MakeTraceSourceAccessor (&LoraPhyGen::m_rxErrLogger),
                     "ns3::LoraPhy::TracedCallback")
    .AddTraceSource ("Tx",
                     "Packet transmission beginning.",
                     MakeTraceSourceAccessor (&LoraPhyGen::m_txLogger),
                     "ns3::LoraPhy::TracedCallback")
  ;
  return tid;

}

void
LoraPhyGen::SetEnergyModelCallback (DeviceEnergyModel::ChangeStateCallback cb)
{
  NS_LOG_FUNCTION (this);
  m_energyCallback = cb;
}

void
LoraPhyGen::UpdatePowerConsumption (const State state)
{
  NS_LOG_FUNCTION (this);

  if (!m_energyCallback.IsNull ())
    {
      m_energyCallback (state);
    }
}

void
LoraPhyGen::EnergyDepletionHandler ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_DEBUG ("Energy depleted at node " << m_device->GetNode ()->GetId () <<
                ", stopping rx/tx activities");

  m_disabled = true;
}

void
LoraPhyGen::SendPacket (Ptr<Packet> pkt, uint32_t modeNum)
{
  NS_LOG_DEBUG ("PHY " << m_mac->GetAddress () << ": Transmitting packet");
  if (m_disabled)
    {
      NS_LOG_DEBUG ("Energy depleted, node cannot transmit any packet. Dropping.");
      return;
    }

  if (m_state == TX)
    {
      NS_LOG_DEBUG ("PHY requested to TX while already Transmitting.  Dropping packet.");
      return;
    }
  else if (m_state == SLEEP)
    {
      NS_LOG_DEBUG ("PHY requested to TX while sleeping.  Dropping packet.");
      return;
    }

  LoraTxMode txMode = GetMode (modeNum);

  if (m_pktRx != 0)
    {
      m_minRxSinrDb = -1e30;
      m_pktRx = 0;
    }

  m_transducer->Transmit (Ptr<LoraPhy> (this), pkt, m_txPwrDb, txMode);
  m_state = TX;
  UpdatePowerConsumption (TX);
  double txdelay = pkt->GetSize () * 8.0 / txMode.GetDataRateBps ();
  Simulator::Schedule (Seconds (txdelay), &LoraPhyGen::TxEndEvent, this);
  NS_LOG_DEBUG ("PHY " << m_mac->GetAddress () << " notifying listeners");
  NotifyListenersTxStart (Seconds (txdelay));
  m_txLogger (pkt, m_txPwrDb, txMode);
}

void
LoraPhyGen::TxEndEvent ()
{
  if (m_state == SLEEP || m_disabled == true)
    {
      NS_LOG_DEBUG ("Transmission ended but node sleeping or dead");
      return;
    }

  NS_ASSERT (m_state == TX);
  if (GetInterferenceDb ( (Ptr<Packet>) 0) > m_ccaThreshDb)
    {
      m_state = CCABUSY;
      NotifyListenersCcaStart ();
    }
  else
    {
      m_state = IDLE;
    }
  UpdatePowerConsumption (IDLE);
}

void
LoraPhyGen::RegisterListener (LoraPhyListener *listener)
{
  m_listeners.push_back (listener);
}


void
LoraPhyGen::StartRxPacket (Ptr<Packet> pkt, double rxPowerDb, LoraTxMode txMode, LoraPdp pdp)
{
  if (m_disabled)
    {
      NS_LOG_DEBUG ("Energy depleted, node cannot receive any packet. Dropping.");
      NotifyRxDrop(pkt);    // traced source netanim
      return;
    }

  switch (m_state)
    {
    case TX:
      NotifyRxDrop(pkt);    // traced source netanim
      NS_ASSERT (false);
      break;
    case RX:
      {
        NS_ASSERT (m_pktRx);
        double newSinrDb = CalculateSinrDb (m_pktRx, m_pktRxArrTime, m_rxRecvPwrDb, m_pktRxMode, m_pktRxPdp);
        m_minRxSinrDb  =  (newSinrDb < m_minRxSinrDb) ? newSinrDb : m_minRxSinrDb;
        NS_LOG_DEBUG ("PHY " << m_mac->GetAddress () << ": Starting RX in RX mode.  SINR of pktRx = " << m_minRxSinrDb);
        NotifyRxBegin(pkt);    // traced source netanim
      }
      break;

    case CCABUSY:
    case IDLE:
      {
        NS_ASSERT (!m_pktRx);
        bool hasmode = false;
        for (uint32_t i = 0; i < GetNModes (); i++)
          {
            if (txMode.GetUid () == GetMode (i).GetUid ())
              {
                hasmode = true;
                break;
              }
          }
        if (!hasmode)
          {
            break;
          }


        double newsinr = CalculateSinrDb (pkt, Simulator::Now (), rxPowerDb, txMode, pdp);
        NS_LOG_DEBUG ("PHY " << m_mac->GetAddress () << ": Starting RX in IDLE mode.  SINR = " << newsinr);
        if (newsinr > m_rxThreshDb)
          {
            m_state = RX;
            UpdatePowerConsumption (RX);
            NotifyRxBegin(pkt);    // traced source netanim
            m_rxRecvPwrDb = rxPowerDb;
            m_minRxSinrDb = newsinr;
            m_pktRx = pkt;
            m_pktRxArrTime = Simulator::Now ();
            m_pktRxMode = txMode;
            m_pktRxPdp = pdp;
            double txdelay = pkt->GetSize () * 8.0 / txMode.GetDataRateBps ();
            Simulator::Schedule (Seconds (txdelay), &LoraPhyGen::RxEndEvent, this, pkt, rxPowerDb, txMode);
            NotifyListenersRxStart ();
          }

      }
      break;
    case SLEEP:
      NS_LOG_DEBUG ("Sleep mode. Dropping packet.");
      NotifyRxDrop(pkt);    // traced source netanim
      break;
    }

  if (m_state == IDLE && GetInterferenceDb ( (Ptr<Packet>) 0) > m_ccaThreshDb)
    {
      m_state = CCABUSY;
      NotifyListenersCcaStart ();
    }

}

void
LoraPhyGen::RxEndEvent (Ptr<Packet> pkt, double rxPowerDb, LoraTxMode txMode)
{
  if (pkt != m_pktRx)
    {
      return;
    }

  if (m_disabled || m_state == SLEEP)
    {
      NS_LOG_DEBUG ("Sleep mode or dead. Dropping packet");
      m_pktRx = 0;
      NotifyRxDrop(pkt);    // traced source netanim
      return;
    }

  NotifyRxEnd(pkt);    // traced source netanim
  if (GetInterferenceDb ( (Ptr<Packet>) 0) > m_ccaThreshDb)
    {
      m_state = CCABUSY;
      NotifyListenersCcaStart ();
    }
  else
    {
      m_state = IDLE;
      UpdatePowerConsumption (IDLE);
    }

  if (m_pg->GetValue (0, 1) > m_per->CalcPer (m_pktRx, m_minRxSinrDb, txMode))
    {
      m_rxOkLogger (pkt, m_minRxSinrDb, txMode);
      NotifyListenersRxGood ();
      if (!m_recOkCb.IsNull ())
        {
          m_recOkCb (pkt, m_minRxSinrDb, txMode);
        }

    }
  else
    {
      m_rxErrLogger (pkt, m_minRxSinrDb, txMode);
      NotifyListenersRxBad ();
      if (!m_recErrCb.IsNull ())
        {
          m_recErrCb (pkt, m_minRxSinrDb);
        }
    }

  m_pktRx = 0;
}

void
LoraPhyGen::SetReceiveOkCallback (RxOkCallback cb)
{
  m_recOkCb = cb;
}

void
LoraPhyGen::SetReceiveErrorCallback (RxErrCallback cb)
{
  m_recErrCb = cb;
}
bool
LoraPhyGen::IsStateSleep (void)
{
  return m_state == SLEEP;
}
bool
LoraPhyGen::IsStateIdle (void)
{
  return m_state == IDLE;
}
bool
LoraPhyGen::IsStateBusy (void)
{
  return !IsStateIdle () && !IsStateSleep ();
}
bool
LoraPhyGen::IsStateRx (void)
{
  return m_state == RX;
}
bool
LoraPhyGen::IsStateTx (void)
{
  return m_state == TX;
}

bool
LoraPhyGen::IsStateCcaBusy (void)
{
  return m_state == CCABUSY;
}


void
LoraPhyGen::SetRxGainDb (double gain)
{
  m_rxGainDb = gain;

}
void
LoraPhyGen::SetTxPowerDb (double txpwr)
{
  m_txPwrDb = txpwr;
}
void
LoraPhyGen::SetRxThresholdDb (double thresh)
{
  m_rxThreshDb = thresh;
}
void
LoraPhyGen::SetCcaThresholdDb (double thresh)
{
  m_ccaThreshDb = thresh;
}
double
LoraPhyGen::GetRxGainDb (void)
{
  return m_rxGainDb;
}
double
LoraPhyGen::GetTxPowerDb (void)
{
  return m_txPwrDb;

}
double
LoraPhyGen::GetRxThresholdDb (void)
{
  return m_rxThreshDb;
}
double
LoraPhyGen::GetCcaThresholdDb (void)
{
  return m_ccaThreshDb;
}

Ptr<LoraChannel>
LoraPhyGen::GetChannel (void) const
{
  return m_channel;
}

Ptr<LoraNetDevice>
LoraPhyGen::GetDevice (void)
{
  return m_device;
}

Ptr<LoraTransducer>
LoraPhyGen::GetTransducer (void)
{
  return m_transducer;
}
void
LoraPhyGen::SetChannel (Ptr<LoraChannel> channel)
{
  m_channel = channel;
}

void
LoraPhyGen::SetDevice (Ptr<LoraNetDevice> device)
{
  m_device = device;
}

void
LoraPhyGen::SetMac (Ptr<LoraMac> mac)
{
  m_mac = mac;
}

void
LoraPhyGen::SetTransducer (Ptr<LoraTransducer> trans)
{
  m_transducer = trans;
  m_transducer->AddPhy (this);
}

void
LoraPhyGen::SetSleepMode (bool sleep)
{
  if (sleep)
    {
      m_state = SLEEP;
      if (!m_energyCallback.IsNull ())
        {
          m_energyCallback (SLEEP);
        }
    }
  else if (m_state == SLEEP)
    {
      if (GetInterferenceDb ((Ptr<Packet>) 0) > m_ccaThreshDb)
        {
          m_state = CCABUSY;
          NotifyListenersCcaStart ();
        }
      else
        {
          m_state = IDLE;
        }

      if (!m_energyCallback.IsNull ())
        {
          m_energyCallback (IDLE);
        }
    }
}

int64_t
LoraPhyGen::AssignStreams (int64_t stream)
{
  NS_LOG_FUNCTION (this << stream);
  m_pg->SetStream (stream);
  return 1;
}

void
LoraPhyGen::NotifyTransStartTx (Ptr<Packet> packet, double txPowerDb, LoraTxMode txMode)
{
  if (m_pktRx)
    {
      m_minRxSinrDb = -1e30;
    }
}

void
LoraPhyGen::NotifyIntChange (void)
{
  if (m_state == CCABUSY && GetInterferenceDb (Ptr<Packet> ()) < m_ccaThreshDb)
    {
      m_state = IDLE;
      NotifyListenersCcaEnd ();
    }
}

double
LoraPhyGen::CalculateSinrDb (Ptr<Packet> pkt, Time arrTime, double rxPowerDb, LoraTxMode mode, LoraPdp pdp)
{
  double noiseDb = m_channel->GetNoiseDbHz ( (double) mode.GetCenterFreqHz () / 1000.0) + 10 * std::log10 (mode.GetBandwidthHz ());
  return m_sinr->CalcSinrDb (pkt, arrTime, rxPowerDb, noiseDb, mode, pdp, m_transducer->GetArrivalList ());
}

double
LoraPhyGen::GetInterferenceDb (Ptr<Packet> pkt)
{

  const LoraTransducer::ArrivalList &arrivalList = m_transducer->GetArrivalList ();

  LoraTransducer::ArrivalList::const_iterator it = arrivalList.begin ();

  double interfPower = 0;

  for (; it != arrivalList.end (); it++)
    {
      if (pkt != it->GetPacket ())
        {
          interfPower += DbToKp (it->GetRxPowerDb ());
        }
    }

  return KpToDb (interfPower);

}

double
LoraPhyGen::DbToKp (double db)
{
  return std::pow (10, db / 10.0);
}
double
LoraPhyGen::KpToDb (double kp)
{
  return 10 * std::log10 (kp);
}

void
LoraPhyGen::NotifyListenersRxStart (void)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyRxStart ();
    }

}
void
LoraPhyGen::NotifyListenersRxGood (void)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyRxEndOk ();
    }
}
void
LoraPhyGen::NotifyListenersRxBad (void)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyRxEndError ();
    }
}
void
LoraPhyGen::NotifyListenersCcaStart (void)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyCcaStart ();
    }
}
void
LoraPhyGen::NotifyListenersCcaEnd (void)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyCcaEnd ();
    }
}

void
LoraPhyGen::NotifyListenersTxStart (Time duration)
{
  ListenerList::const_iterator it = m_listeners.begin ();
  for (; it != m_listeners.end (); it++)
    {
      (*it)->NotifyTxStart (duration);
    }
}

uint32_t
LoraPhyGen::GetNModes (void)
{
  return m_modes.GetNModes ();
}

LoraTxMode
LoraPhyGen::GetMode (uint32_t n)
{
  NS_ASSERT (n < m_modes.GetNModes ());

  return m_modes[n];
}

Ptr<Packet>
LoraPhyGen::GetPacketRx (void) const
{
  return m_pktRx;
}


} // namespace ns3

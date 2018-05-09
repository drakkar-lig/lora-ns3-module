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
#include "lora-prop-model-thorp.h"
#include "lora-tx-mode.h"
#include "ns3/double.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LoraPropModelThorp");

NS_OBJECT_ENSURE_REGISTERED (LoraPropModelThorp);

LoraPropModelThorp::LoraPropModelThorp ()
{
}

LoraPropModelThorp::~LoraPropModelThorp ()
{
}

TypeId
LoraPropModelThorp::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPropModelThorp")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraPropModelThorp> ()
    .AddAttribute ("SpreadCoef",
                   "Spreading coefficient used in calculation of Thorp's approximation.",
                   DoubleValue (1.5),
                   MakeDoubleAccessor (&LoraPropModelThorp::m_SpreadCoef),
                   MakeDoubleChecker<double> ())
  ;
  return tid;
}

double
LoraPropModelThorp::GetPathLossDb (Ptr<MobilityModel> a, Ptr<MobilityModel> b, LoraTxMode mode)
{
  double dist = a->GetDistanceFrom (b);

  return m_SpreadCoef * 10.0 * std::log10 (dist)
         + (dist / 1000.0) * GetAttenDbKm (mode.GetCenterFreqHz () / 1000.0);
}

LoraPdp
LoraPropModelThorp::GetPdp (Ptr<MobilityModel> a, Ptr<MobilityModel> b, LoraTxMode mode)
{
  return LoraPdp::CreateImpulsePdp ();
}

Time
LoraPropModelThorp::GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b, LoraTxMode mode)
{
  return Seconds (a->GetDistanceFrom (b) / 1500.0);
}

double
LoraPropModelThorp::GetAttenDbKyd (double freqKhz)
{

  return GetAttenDbKm (freqKhz) / 1.093613298;
}

double
LoraPropModelThorp::GetAttenDbKm (double freqKhz)
{

  double fsq = freqKhz * freqKhz;
  double atten;

  if (freqKhz >= 0.4)
    {
      atten = 0.11 * fsq / (1 + fsq) + 44 * fsq / (4100 + fsq)
        + 2.75 * 0.0001 * fsq + 0.003;
    }
  else
    {
      atten = 0.002 + 0.11 * (freqKhz / (1 + freqKhz)) + 0.011 * freqKhz;
    }

  return atten;
}

} // namespace ns3

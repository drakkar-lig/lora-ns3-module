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

#include "lora-prop-model-ideal.h"
#include "lora-tx-mode.h"
#include "ns3/mobility-model.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LoraPropModelIdeal);

LoraPropModelIdeal::LoraPropModelIdeal ()
{
}

LoraPropModelIdeal::~LoraPropModelIdeal ()
{
}

TypeId
LoraPropModelIdeal::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraPropModelIdeal")
    .SetParent<LoraPropModel> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraPropModelIdeal> ()
  ;
  return tid;
}


double
LoraPropModelIdeal::GetPathLossDb (Ptr<MobilityModel> a, Ptr<MobilityModel> b, LoraTxMode mode)
{

  return 0;
}
LoraPdp
LoraPropModelIdeal::GetPdp (Ptr<MobilityModel> a, Ptr<MobilityModel> b, LoraTxMode mode)
{
  return LoraPdp::CreateImpulsePdp ();
}

Time
LoraPropModelIdeal::GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b, LoraTxMode mode)
{
  return Seconds (a->GetDistanceFrom (b) / 1500.0);
}


} // namespace ns3

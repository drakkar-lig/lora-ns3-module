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

#include "lora-header-common.h"
#include "lora-address.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LoraHeaderCommon);

LoraHeaderCommon::LoraHeaderCommon ()
{
}

LoraHeaderCommon::LoraHeaderCommon (const LoraAddress src, const LoraAddress dest, uint8_t type)
  : Header (),
    m_dest (dest),
    m_src (src),
    m_type (type)
{

}

TypeId
LoraHeaderCommon::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraHeaderCommon")
    .SetParent<Header> ()
    .SetGroupName ("Lora")
    .AddConstructor<LoraHeaderCommon> ()
  ;
  return tid;
}

TypeId
LoraHeaderCommon::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
LoraHeaderCommon::~LoraHeaderCommon ()
{
}


void
LoraHeaderCommon::SetDest (LoraAddress dest)
{
  m_dest = dest;
}
void
LoraHeaderCommon::SetSrc (LoraAddress src)
{
  m_src = src;
}

void
LoraHeaderCommon::SetType (uint8_t type)
{
  m_type = type;
}

void
LoraHeaderCommon::SetPayload (uint16_t payload)
{
  m_payload = payload;
}

void
LoraHeaderCommon::SetPreamble (uint16_t preamble)
{
  m_preamble = preamble;
}


LoraAddress
LoraHeaderCommon::GetDest (void) const
{
  return m_dest;
}
LoraAddress
LoraHeaderCommon::GetSrc (void) const
{
  return m_src;
}
uint8_t
LoraHeaderCommon::GetType (void) const
{
  return m_type;
}

// Inherrited methods

uint32_t
LoraHeaderCommon::GetSerializedSize (void) const
{
  return 1 + 1 + 1;
}

void
LoraHeaderCommon::Serialize (Buffer::Iterator start) const
{
  start.WriteU8 (m_src.GetAsInt ());
  start.WriteU8 (m_dest.GetAsInt ());
  start.WriteU8 (m_type);
}

uint32_t
LoraHeaderCommon::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator rbuf = start;

  m_src = LoraAddress (rbuf.ReadU8 ());
  m_dest = LoraAddress (rbuf.ReadU8 ());
  m_type = rbuf.ReadU8 ();

  return rbuf.GetDistanceFrom (start);
}

void
LoraHeaderCommon::Print (std::ostream &os) const
{
  os << "LORA src=" << m_src << " dest=" << m_dest << " type=" << (uint32_t) m_type;
}




} // namespace ns3

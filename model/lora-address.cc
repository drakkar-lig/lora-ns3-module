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

#include "lora-address.h"
#include "ns3/address.h"

namespace ns3 {

LoraAddress::LoraAddress ()
{
  m_address = 255;
}

LoraAddress::LoraAddress (uint8_t addr)
  : m_address (addr)
{
}

LoraAddress::~LoraAddress ()
{
}

uint8_t
LoraAddress::GetType (void)
{
  static uint8_t type = Address::Register ();
  return type;
}

Address
LoraAddress::ConvertTo (void) const
{
  return Address (GetType (), &m_address, 1);
}

LoraAddress
LoraAddress::ConvertFrom (const Address &address)
{
  NS_ASSERT (IsMatchingType (address));
  LoraAddress uAddr;
  address.CopyTo (&uAddr.m_address);
  return uAddr;
}

uint8_t
LoraAddress::GetAsInt (void) const
{
  return m_address;
}
bool
LoraAddress::IsMatchingType (const Address &address)
{
  return address.CheckCompatible (GetType (), 1);
}

LoraAddress::operator Address () const
{
  return ConvertTo ();
}

void
LoraAddress::CopyFrom (const uint8_t *pBuffer)
{
  m_address = *pBuffer;
}

void
LoraAddress::CopyTo (uint8_t *pBuffer)
{
  *pBuffer = m_address;

}

LoraAddress
LoraAddress::GetBroadcast ()
{
  return LoraAddress (255);
}
LoraAddress
LoraAddress::Allocate ()
{
  static uint8_t nextAllocated = 0;

  uint32_t address = nextAllocated++;
  if (nextAllocated == 255)
    {
      nextAllocated = 0;
    }

  return LoraAddress (address);
}

bool
operator < (const LoraAddress &a, const LoraAddress &b)
{
  return a.m_address < b.m_address;
}

bool
operator == (const LoraAddress &a, const LoraAddress &b)
{
  return a.m_address == b.m_address;
}

bool
operator != (const LoraAddress &a, const LoraAddress &b)
{
  return !(a == b);
}

std::ostream&
operator<< (std::ostream& os, const LoraAddress & address)
{
  os << (int) address.m_address;
  return os;
}
std::istream&
operator>> (std::istream& is, LoraAddress & address)
{
  int x;
  is >> x;
  NS_ASSERT (0 <= x);
  NS_ASSERT (x <= 255);
  address.m_address = x;
  return is;
}

} // namespace ns3

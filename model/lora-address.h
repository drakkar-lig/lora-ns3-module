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

#ifndef LORA_ADDRESS_H
#define LORA_ADDRESS_H

#include "ns3/address.h"
#include <iostream>

namespace ns3 {

/**
 *
 * A class used for addressing LORA MAC's.
 *
 */
class LoraAddress
{
public:
  /** Constructor */
  LoraAddress ();
  /**
   * Create LoraAddress object with address addr.
   *
   * \param addr Byte address to assign to this address.
   */
  LoraAddress (uint8_t addr);
  /** Destructor */
  virtual ~LoraAddress ();

  /**
   * Convert a generic address to a LoraAddress.
   *
   */
  static LoraAddress ConvertFrom (const Address &address);

  /**
   * Check that a generic Address is compatible with LoraAddress.
   *
   * \param address  Address to test.
   * \return True if address given is consistant with LoraAddress.
   */
  static bool IsMatchingType  (const Address &address);

  /**
   * Create a generic Address.
   *
   * \return The Address.
   */
  operator Address () const;

  /**
   * Sets address to address stored in parameter.
   *
   * \param pBuffer Buffer to extract address from.
   */
  void CopyFrom (const uint8_t *pBuffer);

  /**
   * Writes address to buffer parameter.
   *
   * \param pBuffer
   */
  void CopyTo (uint8_t *pBuffer);

  /**
   * Convert to integer.
   *
   * \return 8 bit integer version of address.
   */
  uint8_t GetAsInt (void) const;

  /**
   * Get the broadcast address (255).
   *
   * \return Broadcast address.
   */
  static LoraAddress GetBroadcast (void);

  /**
   * Allocates LoraAddress from 0-254
   *
   * Will wrap back to 0 if more than 254 are allocated.
   * Excludes the broadcast address.
   *
   * \return The next sequential LoraAddress.
   */
  static LoraAddress Allocate ();


private:
  uint8_t m_address;  //!< The address.

  /**
   * Get the LoraAddress type.
   *
   * \return The type value.
   */
  static uint8_t GetType (void);
  /**
   * Convert to a generic Address.
   *
   * \return The Address value.
   */
  Address ConvertTo (void) const;

  friend bool operator <  (const LoraAddress &a, const LoraAddress &b);
  friend bool operator == (const LoraAddress &a, const LoraAddress &b);
  friend bool operator != (const LoraAddress &a, const LoraAddress &b);
  friend std::ostream& operator<< (std::ostream& os, const LoraAddress & address);
  friend std::istream& operator>> (std::istream& is, LoraAddress & address);

};  // class LoraAddress


/**
 * Address comparison, less than.
 *
 * \param a First address to compare.
 * \param b Second address to compare.
 * \return True if a < b.
 */
bool operator < (const LoraAddress &a, const LoraAddress &b);

/**
 * Address comparison, equalit.
 *
 * \param a First address to compare.
 * \param b Second address to compare.
 * \return True if a == b.
 */
bool operator == (const LoraAddress &a, const LoraAddress &b);

/**
 * Address comparison, unequal.
 *
 * \param a First address to compare.
 * \param b Second address to compare.
 * \return True if a != b.
 */
bool operator != (const LoraAddress &a, const LoraAddress &b);

/**
 * Write \pname{address} to stream \pname{os} as 8 bit integer.
 *
 * \param os The output stream.
 * \param address The address
 * \return The output stream.
 */
std::ostream& operator<< (std::ostream& os, const LoraAddress & address);

/**
 * Read \pname{address} from stream \pname{is} as 8 bit integer.
 *
 * \param is The input stream.
 * \param address The address variable to set.
 * \return The input stream.
 */
std::istream& operator>> (std::istream& is, LoraAddress & address);

} // namespace ns3

#endif /* LORA_ADDRESS_H */

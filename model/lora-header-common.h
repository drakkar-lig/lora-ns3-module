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

#ifndef LORA_HEADER_COMMON_H
#define LORA_HEADER_COMMON_H

#include "ns3/header.h"
#include "ns3/nstime.h"
#include "ns3/simulator.h"
#include "lora-address.h"

namespace ns3 {

/**
 *
 * Common packet header fields.
 *
 * Includes 1 byte src address, 1 byte dest address,
 * and a 1 byte type field.
 *
 * The type field is protocol specific; see the relevant MAC protocol.
 */
class LoraHeaderCommon : public Header
{
public:
  /** Default constructor */
  LoraHeaderCommon ();
  /**
   * Create LoraHeaderCommon object with given source and destination
   * address and header type
   *
   * \param src Source address defined in header.
   * \param dest Destination address defined in header.
   * \param type Header type.
   */
  LoraHeaderCommon (const LoraAddress src, const LoraAddress dest, uint8_t type);
  /** Destructor */
  virtual ~LoraHeaderCommon ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);

  /**
   * Set the destination address.
   *
   * \param dest Address of destination node.
   */
  void SetDest (LoraAddress dest);
  /**
   * Set the source address.
   *
   * \param src Address of packet source node.
   */
  void SetSrc (LoraAddress src);
  /**
   * Set the header type.
   *
   * Use of this value is protocol specific.
   * \param type The type value.
   */
  void SetType (uint8_t type);

 /**
   * Set the payload.
   *
   * Use of this value is protocol specific.
   * \param type The type value.
   */
  void SetPayload (uint16_t payload);

 /**
   * Set the preamble.
   *
   * Use of this value is protocol specific.
   * \param type The type value.
   */
  void SetPreamble (uint16_t preamble);


  /**
   * Get the destination address.
   *
   * \return LoraAddress in destination field.
   */
  LoraAddress GetDest (void) const;
  /**
   * Get the source address
   *
   * \return LoraAddress in source field.
   */
  LoraAddress GetSrc (void) const;
  /**
   * Get the header type value.
   *
   * \return value of type field.
   */
  uint8_t GetType (void) const;


  // Inherited methods
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);
  virtual void Print (std::ostream &os) const;
  virtual TypeId GetInstanceTypeId (void) const;
private:
  LoraAddress m_dest;  //!< The destination address.
  LoraAddress m_src;   //!< The source address.
  uint8_t m_type;     //!< The type field.
  uint16_t m_payload;     //!< The payload field.
  uint16_t m_preamble;     //!< The preamble field.


};  // class LoraHeaderCommon

} // namespace ns3

#endif /* LORA_HEADER_COMMON_H */

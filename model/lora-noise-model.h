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

#ifndef LORA_NOISE_MODEL_H
#define LORA_NOISE_MODEL_H

#include "ns3/object.h"

namespace ns3 {

/**
 *
 * LORA Noise Model base class.
 */
class LoraNoiseModel : public Object
{
public:
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);

  /**
   * Compute the noise power at a given frequency.
   *
   * \param fKhz Frequency in kHz.
   * \return Noise power in dB re 1uPa/Hz.
   */
  virtual double GetNoiseDbHz (double fKhz) const = 0;

  /** Clear all pointer references. */
  virtual void Clear (void);

  virtual void DoDispose (void);

};  // class LoraNoiseModel

} // namespace ns3

#endif /* LORA_NOISE_MODEL_H */

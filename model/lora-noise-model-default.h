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

#ifndef LORA_NOISE_MODEL_DEFAULT_H
#define LORA_NOISE_MODEL_DEFAULT_H

#include "ns3/lora-noise-model.h"
#include "ns3/attribute.h"
#include "ns3/object.h"

namespace ns3 {

/**
 *
 * Standard ambient acoustic noise model.
 *
 */
class LoraNoiseModelDefault : public LoraNoiseModel
{
public:
  LoraNoiseModelDefault ();           //!< Default constructor.
  virtual ~LoraNoiseModelDefault ();  //!< Dummy destructor, DoDispose.

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);

  // Inherited methods
  virtual double GetNoiseDbHz (double fKhz) const;

private:
  double m_wind;      //!< Wind speed in m/s.
  double m_shipping;  //!< Shipping contribution to noise between 0 and 1.

};  // class LoraNoiseModelDefault

} // namespace ns3

#endif /* LORA_NOISE_MODEL_DEFAULT_H */

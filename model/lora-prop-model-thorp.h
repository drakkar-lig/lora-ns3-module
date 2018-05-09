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

#ifndef LORA_PROP_MODEL_THORP_H
#define LORA_PROP_MODEL_THORP_H

#include "lora-prop-model.h"

namespace ns3 {

class LoraTxMode;

/**
 *
 * Uses Thorp's approximation to compute pathloss.  Assumes implulse PDP.
 */
class LoraPropModelThorp : public LoraPropModel
{
public:
  /** Default constructor. */
  LoraPropModelThorp ();
  /** Destructor */
  virtual ~LoraPropModelThorp ();

  /**
   * Register this type.
   * \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  // Inherited methods
  virtual double GetPathLossDb (Ptr<MobilityModel> a, Ptr<MobilityModel> b, LoraTxMode mode);
  virtual LoraPdp GetPdp (Ptr<MobilityModel> a, Ptr<MobilityModel> b, LoraTxMode mode);
  virtual Time GetDelay (Ptr<MobilityModel> a, Ptr<MobilityModel> b, LoraTxMode mode);

private:
  /**
   * Get the attenuation in dB / 1000 yards.
   * \param freqKhz The channel center frequency, in kHz.
   * \return The attenuation, in dB / 1000 yards.
   */
  double GetAttenDbKyd (double freqKhz);
  /**
   * Get the attenuation in dB / km.
   * \param freqKhz The channel center frequency, in kHz.
   * \return The attenuation, in dB/km.
   */
  double GetAttenDbKm (double freqKhz);

  double m_SpreadCoef;  //!< Spreading coefficient used in calculation of Thorp's approximation.

};  // class LoraPropModelThorp

} // namespace ns3

#endif /* LORA_PROP_MODEL_THORP_H */

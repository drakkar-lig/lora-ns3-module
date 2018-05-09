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

#ifndef LORA_CHANNEL_H
#define LORA_CHANNEL_H

#include "ns3/net-device.h"
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/lora-prop-model.h"
#include "ns3/lora-noise-model.h"
#include "ns3/nstime.h"
#include "ns3/data-rate.h"
#include "ns3/ptr.h"

#include <list>
#include <vector>

namespace ns3 {

class LoraNetDevice;
class LoraPhy;
class LoraTransducer;
class LoraTxMode;


/**
 * Current state of the channel
 */ 
enum WireState
{
  IDLE,            /**< Channel is IDLE, no packet is being transmitted */
  TRANSMITTING,    /**< Channel is BUSY, a packet is being written by a net device */
  PROPAGATING      /**< Channel is BUSY, packet is propagating to all attached net devices */
};



/**
 *
 * Channel class used by LORA devices.
 */
class LoraChannel : public Channel
{
public:
  /**
   * LoraDeviceList is a standard template vector of pairs
   * (LoraNetDevice, LoraTransducer)
   */
  typedef std::vector<std::pair<Ptr<LoraNetDevice>, Ptr<LoraTransducer> > > LoraDeviceList;

  LoraChannel ();           //!< Constructor
  virtual ~LoraChannel ();  //!< Dummy destructor, see DoDispose.

  /**
   * Register this type.
   * \return The object TypeId.
   */
  static TypeId GetTypeId (void);

  // Inherited methods
  virtual uint32_t GetNDevices (void) const;
  virtual Ptr<NetDevice> GetDevice (uint32_t i) const;

  /**
   * Send a packet out on the channel.
   *
   * \param src Transducer transmitting packet.
   * \param packet Packet to be transmitted.
   * \param txPowerDb Transmission power in dB.
   * \param txmode LoraTxMode defining modulation of transmitted packet.
   */
  void TxPacket  (Ptr<LoraTransducer> src, Ptr<Packet> packet, double txPowerDb,
                  LoraTxMode txmode);

  /**
   * Adds device to receiver list for this channel.
   *
   * \param dev Net Device of node.
   * \param trans Transducer of net device attached to this channel.
   */
  void AddDevice (Ptr<LoraNetDevice> dev, Ptr<LoraTransducer> trans);

  /**
   * Set the propagation model this channel will use
   * for path loss/propagation delay.
   *
   * \param prop The propagation model.
   */
  void SetPropagationModel (Ptr<LoraPropModel> prop);

  /**
   * Set the noise model this channel will use
   * to determine ambient channel noise.
   *
   * \param noise The noise model.
   */
  void SetNoiseModel  (Ptr<LoraNoiseModel> noise);

  /**
   * Get the noise level on the channel.
   *
   * \param fKhz Frequency in kHz.
   * \return Ambient noise in dB/Hz on channel at a frequency.
   */
  double GetNoiseDbHz (double fKhz);

  /**
   * Clear all pointer references. */
  void Clear (void);



 /**
   * \brief Start transmitting a packet over the channel
   *
   * If the srcId belongs to a net device that is connected to the
   * channel, packet transmission begins, and the channel becomes busy
   * until the packet has completely reached all destinations.
   *
   * \param p A reference to the packet that will be transmitted over
   * the channel
   * \param srcId The device Id of the net device that wants to
   * transmit on the channel.
   * \return True if the channel is not busy and the transmitting net
   * device is currently active.
   * bool TransmitStart (Ptr<Packet> p, uint32_t srcId);
   */
  
  bool TransmitStart (Ptr<Packet> p, uint16_t protocolNumber);

  /**
   * \brief Indicates that the net device has finished transmitting
   * the packet over the channel
   *
   * The channel will stay busy until the packet has completely
   * propagated to all net devices attached to the channel. The
   * TransmitEnd function schedules the PropagationCompleteEvent which
   * will free the channel for further transmissions. Stores the
   * packet p as the m_currentPkt, the packet being currently
   * transmitting.
   *
   * \return Returns true unless the source was detached before it
   * completed its transmission.
   * bool TransmitEnd (Ptr<LoraTransducer> src, Ptr<Packet> packet,
   *                  double txPowerDb, LoraTxMode txMode);
   */
  bool TransmitEnd (uint16_t protocolNumber);

  /**
   * \brief Indicates that the channel has finished propagating the
   * current packet. The channel is released and becomes free.
   *
   * Calls the receive function of every active net device that is
   * attached to the channel.
   */

 /**
   * \return Returns the state of the channel (IDLE -- free,
   * TRANSMITTING -- busy, PROPAGATING - busy )
   */
  WireState GetState (uint16_t protocolNumber);

  /**
   * \brief Indicates if the channel is busy. The channel will only
   * accept new packets for transmission if it is not busy.
   *
   * \return Returns true if the channel is busy and false if it is
   * free.
   */
  bool IsBusy (uint16_t protocolNumber);

  /**
   * \brief Indicates if a net device is currently attached or
   * detached from the channel.
   *
   * \param deviceId The ID that was assigned to the net device when
   * it was attached to the channel.
   * \return Returns true if the net device is attached to the
   * channel, false otherwise.
   */

  /**
   * Get the assigned data rate of the channel
   *
   * \return Returns the DataRate to be used by device transmitters.
   * with deviceId i.
   */
  DataRate GetDataRate (void);

  /**
   * Get the assigned speed-of-light delay of the channel
   *
   * \return Returns the delay used by the channel.
   */
  Time GetDelay (void);


private:
  LoraDeviceList m_devList;     //!< The list of devices on this channel.
  Ptr<LoraPropModel> m_prop;    //!< The propagation model.
  Ptr<LoraNoiseModel> m_noise;  //!< The noise model.
  /** Has Clear ever been called on the channel. */
  bool m_cleared;              

  /**
   * Send a packet up to the receiving LoraTransducer.
   *
   * \param i Device number.
   * \param packet The received packet.
   * \param rxPowerDb Signal power in dB of arriving packet.
   * \param txMode Mode arriving packet is using.
   * \param pdp PDP of arriving signal.
   */
  void SendUp (uint32_t i, Ptr<Packet> packet, double rxPowerDb, LoraTxMode txMode, LoraPdp pdp);



  /**
   * The assigned data rate of the channel
   */
  DataRate      m_bps;

  /**
   * The assigned speed-of-light delay of the channel
   */
  Time          m_delay;

  /**
   * The Packet that is currently being transmitted on the channel (or last
   * packet to have been transmitted on the channel if the channel is
   * free.)
   */
  Ptr<Packet> m_currentPkt;

  /**
   * Device Id of the source that is currently transmitting on the
   * channel. Or last source to have transmitted a packet on the
   * channel, if the channel is currently not busy.
   */
  uint32_t  m_currentSrc;

  /**
   * Current state of the channel
   */
  WireState          m_state[3];

  
protected:
  virtual void DoDispose (void);

};  // class LoraChannel

} // namespace ns3

#endif /* LORA_CHANNEL_H */

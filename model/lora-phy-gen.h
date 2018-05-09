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
 *         Andrea Sacco <andrea.sacco85@gmail.com>
 *         To Thanh Hai <tthhai@gmail.com>
 */

#ifndef LORA_PHY_GEN_H
#define LORA_PHY_GEN_H


#include "lora-phy.h"
#include "ns3/traced-callback.h"
#include "ns3/nstime.h"
#include "ns3/device-energy-model.h"
#include "ns3/random-variable-stream.h"
#include <list>

namespace ns3 {

/**
 *
 * Default Packet Error Rate calculator for LoraPhyGen
 *
 */
class LoraPhyPerGenDefault : public LoraPhyPer
{
public:
  /** Constructor */
  LoraPhyPerGenDefault ();
  /** Destructor */
  virtual ~LoraPhyPerGenDefault ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  
  virtual double CalcPer (Ptr<Packet> pkt, double sinrDb, LoraTxMode mode);
private:
  
  double m_thresh;  //!< SINR threshold.

};  // class LoraPhyPerGenDefault

  
/**
 *
 * Packet error rate calculation assuming WHOI Micromodem-like PHY.
 *
 */
class LoraPhyPerUmodem : public LoraPhyPer
{
public:
  /** Constructor */
  LoraPhyPerUmodem ();
  /** Destructor */
  virtual ~LoraPhyPerUmodem ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);

  /**
   * Calculate the packet error probability based on
   * SINR at the receiver and a tx mode.
   *
   * \param pkt Packet which is under consideration.
   * \param sinrDb SINR at receiver.
   * \param mode TX mode used to transmit packet.
   * \return Probability of packet error.
   */
  virtual double CalcPer (Ptr<Packet> pkt, double sinrDb, LoraTxMode mode);

private:
  /**
   * Binomial coefficient
   *
   * \param n Pool size.
   * \param k Number of draws.
   * \return Binomial coefficient n choose k.
   */
  double NChooseK (uint32_t n, uint32_t k);

};  // class LoraPhyPerUmodem


/**
 *
 * Default SINR calculator for LoraPhyGen.
 *
 */
class LoraPhyCalcSinrDefault : public LoraPhyCalcSinr
{

public:
  /** Constructor */
  LoraPhyCalcSinrDefault ();
  /** Destructor */
  virtual ~LoraPhyCalcSinrDefault ();
  
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);
  
  /**
   * Calculate the SINR value for a packet.
   *
   * This implementation simply adds all arriving signal power
   * and assumes it acts identically to additional noise.
   *
   * \param pkt Packet to calculate SINR for.
   * \param arrTime Arrival time of pkt.
   * \param rxPowerDb The received signal strength of the packet in dB re 1 uPa.
   * \param ambNoiseDb Ambient channel noise in dB re 1 uPa.
   * \param mode TX Mode of pkt.
   * \param pdp  Power delay profile of pkt.
   * \param arrivalList  List of interfering arrivals given from Transducer.
   * \return The SINR in dB re 1 uPa.
   */
  virtual double CalcSinrDb (Ptr<Packet> pkt,
                             Time arrTime,
                             double rxPowerDb,
                             double ambNoiseDb,
                             LoraTxMode mode,
                             LoraPdp pdp,
                             const LoraTransducer::ArrivalList &arrivalList
                             ) const;

};  // class LoraPhyCalcSinrDefault


class LoraPhyCalcSinrFhFsk : public LoraPhyCalcSinr
{

public:
  /** Constructor */
  LoraPhyCalcSinrFhFsk ();
  /** Destructor */
  virtual ~LoraPhyCalcSinrFhFsk ();
  
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);

  virtual double CalcSinrDb (Ptr<Packet> pkt,
                             Time arrTime,
                             double rxPowerDb,
                             double ambNoiseDb,
                             LoraTxMode mode,
                             LoraPdp pdp,
                             const LoraTransducer::ArrivalList &arrivalList
                             ) const;
private:
  uint32_t m_hops;  //!< Number of hops.

};  // class LoraPhyCalcSinrFhFsk


/**
 *
 * Generic PHY model.
 *
 */
class LoraPhyGen : public LoraPhy
{
public:
  /** Constructor */
  LoraPhyGen ();
  /** Dummy destructor, see DoDispose */
  virtual ~LoraPhyGen ();
  /**
   * Get the default transmission modes.
   *
   * \return The default mode list.
   */
  static LoraModesList GetDefaultModes (void);

  
  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId (void);

  // Inherited methods
  virtual void SetEnergyModelCallback (DeviceEnergyModel::ChangeStateCallback cb);
  virtual void EnergyDepletionHandler (void);
  virtual void SendPacket (Ptr<Packet> pkt, uint32_t modeNum);
  virtual void RegisterListener (LoraPhyListener *listener);
  virtual void StartRxPacket (Ptr<Packet> pkt, double rxPowerDb, LoraTxMode txMode, LoraPdp pdp);
  virtual void SetReceiveOkCallback (RxOkCallback cb);
  virtual void SetReceiveErrorCallback (RxErrCallback cb);
  virtual bool IsStateSleep (void);
  virtual bool IsStateIdle (void);
  virtual bool IsStateBusy (void);
  virtual bool IsStateRx (void);
  virtual bool IsStateTx (void);
  virtual bool IsStateCcaBusy (void);
  virtual void SetRxGainDb (double gain);
  virtual void SetTxPowerDb (double txpwr);
  virtual void SetRxThresholdDb (double thresh);
  virtual void SetCcaThresholdDb (double thresh);
  virtual double GetRxGainDb (void);
  virtual double GetTxPowerDb (void);
  virtual double GetRxThresholdDb (void);
  virtual double GetCcaThresholdDb (void);
  virtual Ptr<LoraChannel> GetChannel (void) const;
  virtual Ptr<LoraNetDevice> GetDevice (void);
  virtual Ptr<LoraTransducer> GetTransducer (void);
  virtual void SetChannel (Ptr<LoraChannel> channel);
  virtual void SetDevice (Ptr<LoraNetDevice> device);
  virtual void SetMac (Ptr<LoraMac> mac);
  virtual void SetTransducer (Ptr<LoraTransducer> trans);
  virtual void NotifyTransStartTx (Ptr<Packet> packet, double txPowerDb, LoraTxMode txMode);
  virtual void NotifyIntChange (void);
  virtual uint32_t GetNModes (void);
  virtual LoraTxMode GetMode (uint32_t n);
  virtual Ptr<Packet> GetPacketRx (void) const;
  virtual void Clear (void);
  virtual void SetSleepMode (bool sleep);
  int64_t AssignStreams (int64_t stream);

private:
  /** List of Phy Listeners. */
  typedef std::list<LoraPhyListener *> ListenerList;

  LoraModesList m_modes;             //!< List of modes supported by this PHY.

  State m_state;                    //!< Phy state.
  ListenerList m_listeners;         //!< List of listeners.
  RxOkCallback m_recOkCb;           //!< Callback for packets received without error.
  RxErrCallback m_recErrCb;         //!< Callback for packets received with errors.
  Ptr<LoraChannel> m_channel;        //!< Attached channel.
  Ptr<LoraTransducer> m_transducer;  //!< Associated transducer.
  Ptr<LoraNetDevice> m_device;       //!< Device hosting this Phy.
  Ptr<LoraMac> m_mac;                //!< MAC layer.
  Ptr<LoraPhyPer> m_per;             //!< Error model.
  Ptr<LoraPhyCalcSinr> m_sinr;       //!< SINR calculator.

  double m_rxGainDb;                //!< Receive gain.
  double m_txPwrDb;                 //!< Transmit power.
  double m_rxThreshDb;              //!< Receive SINR threshold.
  double m_ccaThreshDb;             //!< CCA busy threshold.

  Ptr<Packet> m_pktRx;              //!< Received packet.
  double m_minRxSinrDb;             //!< Minimum receive SINR during packet reception.
  double m_rxRecvPwrDb;             //!< Receiver power.
  Time m_pktRxArrTime;              //!< Packet arrival time.
  LoraPdp m_pktRxPdp;                //!< Power delay profile of pakket.
  LoraTxMode m_pktRxMode;            //!< Packet transmission mode at receiver.

  bool m_cleared;                   //!< Flag when we've been cleared.
  bool m_disabled;                  //!< Energy depleted. 

  /** Provides uniform random variables. */
  Ptr<UniformRandomVariable> m_pg;

  /** Energy model callback. */
  DeviceEnergyModel::ChangeStateCallback m_energyCallback;
  /** A packet destined for this Phy was received without error. */
  ns3::TracedCallback<Ptr<const Packet>, double, LoraTxMode > m_rxOkLogger;
  /** A packet destined for this Phy was received with error. */
  ns3::TracedCallback<Ptr<const Packet>, double, LoraTxMode > m_rxErrLogger;
  /** A packet was sent from this Phy. */
  ns3::TracedCallback<Ptr<const Packet>, double, LoraTxMode > m_txLogger;

  /**
   * Calculate the SINR value for a packet.
   *
   * \param pkt Packet to calculate SINR for.
   * \param arrTime Arrival time of pkt.
   * \param rxPowerDb The received signal strength of the packet in dB re 1 uPa.
   * \param mode TX Mode of pkt.
   * \param pdp  Power delay profile of pkt.
   * \return The SINR in dB re 1 uPa.
   */
  double CalculateSinrDb (Ptr<Packet> pkt, Time arrTime, double rxPowerDb,
                          LoraTxMode mode, LoraPdp pdp);

  /**
   * Calculate interference power from overlapping packet arrivals, in dB.
   *
   * The "signal" packet power is excluded.  Use
   * GetInterferenceDb ( (Ptr<Packet>) 0) to treat all signals as
   * interference, for instance in calculating the CCA busy.
   *
   * \param pkt The arriving (signal) packet.
   * \return The total interference power, in dB.
   */
  double GetInterferenceDb (Ptr<Packet> pkt);
  /**
   * Convert dB to kilopascals.
   *
   *   \f[{\rm{kPa}} = {10^{\frac{{{\rm{dB}}}}{{10}}}}\f]
   *
   * \param db Signal level in dB.
   * \return Sound pressure in kPa.
   */
  double DbToKp (double db);
  /**
   * Convert kilopascals to dB.
   *
   *   \f[{\rm{dB}} = 10{\log _{10}}{\rm{kPa}}\f]
   *
   * \param kp Sound pressure in kPa.
   * \return Signal level in dB.
   */
  double KpToDb (double kp);
  /**
   * Event to process end of packet reception.
   *
   * \param pkt The packet.
   * \param rxPowerDb Received signal power.
   * \param txMode Transmission mode.
   */
  void RxEndEvent (Ptr<Packet> pkt, double rxPowerDb, LoraTxMode txMode);
  /** Event to process end of packet transmission. */
  void TxEndEvent ();
  /**
   * Update energy source with new state.
   *
   * \param state The new Phy state.
   */
  void UpdatePowerConsumption (const State state);


  /** Call LoraListener::NotifyRxStart on all listeners. */
  void NotifyListenersRxStart (void);  
  /** Call LoraListener::NotifyRxEndOk on all listeners. */
  void NotifyListenersRxGood (void);
  /** Call LoraListener::NotifyRxEndError on all listeners. */
  void NotifyListenersRxBad (void);
  /** Call LoraListener::NotifyCcaStart on all listeners. */
  void NotifyListenersCcaStart (void);
  /** Call LoraListener::NotifyCcaEnd on all listeners. */
  void NotifyListenersCcaEnd (void);
  /**
   * Call LoraListener::NotifyTxStart on all listeners.
   *
   * \param duration Duration of transmission.
   */
  void NotifyListenersTxStart (Time duration);

protected:
  virtual void DoDispose ();

};  // class LoraPhyGen

} // namespace ns3

#endif /* LORA_PHY_GEN_H */

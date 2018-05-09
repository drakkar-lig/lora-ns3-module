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

#ifndef LORA_PHY_DUAL_H
#define LORA_PHY_DUAL_H

#include "ns3/lora-phy.h"



namespace ns3 {

class LoraTxMode;
class LoraModesList;


/**
 * Default SINR model for LoraPhyDual
 *
 */
class LoraPhyCalcSinrDual : public LoraPhyCalcSinr
{
public:
  /** Constructor */
  LoraPhyCalcSinrDual ();
  /** Destructor */
  virtual ~LoraPhyCalcSinrDual ();
  
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

};  // class LoraPhyCalcSinrDual

class LoraPhyDual : public LoraPhy
{
public:
  /** Constructor */
  LoraPhyDual ();
  /** Dummy destructor \see DoDispose */
  virtual ~LoraPhyDual ();

  /**
   * Register this type.
   * \return The TypeId.
   */
  static TypeId GetTypeId ();

  // Inherited methods:
  virtual void SetEnergyModelCallback (DeviceEnergyModel::ChangeStateCallback callback);
  virtual void EnergyDepletionHandler (void);
  virtual void SendPacket (Ptr<Packet> pkt, uint32_t modeNum);
  virtual void RegisterListener (LoraPhyListener *listener);
  virtual void StartRxPacket (Ptr<Packet> pkt, double rxPowerDb, LoraTxMode txMode, LoraPdp pdp);
  virtual void SetReceiveOkCallback (RxOkCallback cb);
  virtual void SetReceiveErrorCallback (RxErrCallback cb);
  virtual void SetRxGainDb (double gain);
  virtual void SetTxPowerDb (double txpwr);
  virtual void SetRxThresholdDb (double thresh) NS_DEPRECATED;
  virtual void SetCcaThresholdDb (double thresh);
  virtual double GetRxGainDb (void);
  virtual double GetTxPowerDb (void);
  virtual double GetRxThresholdDb (void);
  virtual double GetCcaThresholdDb (void);
  virtual bool IsStateSleep (void);
  virtual bool IsStateIdle (void);
  virtual bool IsStateBusy (void);
  virtual bool IsStateRx (void);
  virtual bool IsStateTx (void);
  virtual bool IsStateCcaBusy (void);
  virtual Ptr<LoraChannel> GetChannel (void) const;
  virtual Ptr<LoraNetDevice> GetDevice (void);
  virtual void SetChannel (Ptr<LoraChannel> channel);
  virtual void SetDevice (Ptr<LoraNetDevice> device);
  virtual void SetMac (Ptr<LoraMac> mac);
  virtual void NotifyTransStartTx (Ptr<Packet> packet, double txPowerDb, LoraTxMode txMode);
  virtual void NotifyIntChange (void);
  virtual void SetTransducer (Ptr<LoraTransducer> trans);
  virtual Ptr<LoraTransducer> GetTransducer (void);
  virtual uint32_t GetNModes (void);
  virtual LoraTxMode GetMode (uint32_t n);
  virtual void Clear (void);
  virtual void SetSleepMode (bool sleep)
  {
    /// \todo This method has to be implemented
  }
  int64_t AssignStreams (int64_t stream);
  Ptr<Packet> GetPacketRx (void) const;
  

  /** \copydoc LoraPhy::IsStateIdle */
  bool IsPhy1Idle (void);
  /** \copydoc LoraPhy::IsStateIdle */
  bool IsPhy2Idle (void);
  /** \copydoc LoraPhy::IsStateRx */
  bool IsPhy1Rx (void);

  bool IsPhy2Rx (void);

  bool IsPhy1Tx (void);

  bool IsPhy2Tx (void);

  bool IsPhy3Idle (void);
  bool IsPhy4Idle (void);

  bool IsPhy3Rx (void);
  bool IsPhy4Rx (void);

  bool IsPhy3Tx (void);
  bool IsPhy4Tx (void);

  bool IsPhy5Idle (void);
  bool IsPhy6Idle (void);

  bool IsPhy5Rx (void);
  bool IsPhy6Rx (void);

  bool IsPhy5Tx (void);
  bool IsPhy6Tx (void);
  
  bool IsPhy7Idle (void);
  bool IsPhy8Idle (void);

  bool IsPhy7Rx (void);
  bool IsPhy8Rx (void);

  bool IsPhy7Tx (void);
  bool IsPhy8Tx (void);

  bool IsPhy9Idle (void);
  bool IsPhy10Idle (void);

  bool IsPhy9Rx (void);
  bool IsPhy10Rx (void);

  bool IsPhy9Tx (void);
  bool IsPhy10Tx (void);
  
  bool IsPhy11Idle (void);
  bool IsPhy12Idle (void);

  bool IsPhy11Rx (void);
  bool IsPhy12Rx (void);

  bool IsPhy11Tx (void);
  bool IsPhy12Tx (void);

  bool IsPhy13Idle (void);
  bool IsPhy14Idle (void);

  bool IsPhy13Rx (void);
  bool IsPhy14Rx (void);

  bool IsPhy13Tx (void);
  bool IsPhy14Tx (void);
  
  bool IsPhy15Idle (void);
  bool IsPhy16Idle (void);

  bool IsPhy15Rx (void);
  bool IsPhy16Rx (void);

  bool IsPhy15Tx (void);
  bool IsPhy16Tx (void);

  bool IsPhy17Idle (void);
  bool IsPhy18Idle (void);

  bool IsPhy17Rx (void);
  bool IsPhy18Rx (void);

  bool IsPhy17Tx (void);
  bool IsPhy18Tx (void);


  // Attribute getters and setters
  /** \copydoc ns3::LoraPhy::GetCcaThresholdDb() */
  double GetCcaThresholdPhy1 (void) const;
  /** \copydoc LoraPhy::GetCcaThresholdDb() */
  double GetCcaThresholdPhy2 (void) const;

  void SetCcaThresholdPhy1 (double thresh);
  void SetCcaThresholdPhy2 (double thresh);

  double GetCcaThresholdPhy3 (void) const;
  double GetCcaThresholdPhy4 (void) const;
  void SetCcaThresholdPhy3 (double thresh);
  void SetCcaThresholdPhy4 (double thresh);

  double GetCcaThresholdPhy5 (void) const;
  double GetCcaThresholdPhy6 (void) const;
  void SetCcaThresholdPhy5 (double thresh);
  void SetCcaThresholdPhy6 (double thresh);
  
  double GetCcaThresholdPhy7 (void) const;
  double GetCcaThresholdPhy8 (void) const;
  void SetCcaThresholdPhy7 (double thresh);
  void SetCcaThresholdPhy8 (double thresh);

  double GetCcaThresholdPhy9 (void) const;
  double GetCcaThresholdPhy10 (void) const;
  void SetCcaThresholdPhy9 (double thresh);
  void SetCcaThresholdPhy10 (double thresh);
  
  double GetCcaThresholdPhy11 (void) const;
  double GetCcaThresholdPhy12 (void) const;
  void SetCcaThresholdPhy11 (double thresh);
  void SetCcaThresholdPhy12 (double thresh);

  double GetCcaThresholdPhy13 (void) const;
  double GetCcaThresholdPhy14 (void) const;
  void SetCcaThresholdPhy13 (double thresh);
  void SetCcaThresholdPhy14 (double thresh);
  
  double GetCcaThresholdPhy15 (void) const;
  double GetCcaThresholdPhy16 (void) const;
  void SetCcaThresholdPhy15 (double thresh);
  void SetCcaThresholdPhy16 (double thresh);

  double GetCcaThresholdPhy17 (void) const;
  double GetCcaThresholdPhy18 (void) const;
  void SetCcaThresholdPhy17 (double thresh);
  void SetCcaThresholdPhy18 (double thresh);

  /** \copydoc LoraPhy::GetTxPowerDb */
  double GetTxPowerDbPhy1 (void) const;
  /** \copydoc LoraPhy::GetTxPowerDb */
  double GetTxPowerDbPhy2 (void) const;
  void SetTxPowerDbPhy1 (double txpwr);
  void SetTxPowerDbPhy2 (double txpwr);

  double GetTxPowerDbPhy3 (void) const;
  double GetTxPowerDbPhy4 (void) const;
  void SetTxPowerDbPhy3 (double txpwr);
  void SetTxPowerDbPhy4 (double txpwr);

  double GetTxPowerDbPhy5 (void) const;
  double GetTxPowerDbPhy6 (void) const;
  void SetTxPowerDbPhy5 (double txpwr);
  void SetTxPowerDbPhy6 (double txpwr);
 
  double GetTxPowerDbPhy7 (void) const;
  double GetTxPowerDbPhy8 (void) const;
  void SetTxPowerDbPhy7 (double txpwr);
  void SetTxPowerDbPhy8 (double txpwr);

  double GetTxPowerDbPhy9 (void) const;
  double GetTxPowerDbPhy10 (void) const;
  void SetTxPowerDbPhy9 (double txpwr);
  void SetTxPowerDbPhy10 (double txpwr);
  
  double GetTxPowerDbPhy11 (void) const;
  double GetTxPowerDbPhy12 (void) const;
  void SetTxPowerDbPhy11 (double txpwr);
  void SetTxPowerDbPhy12 (double txpwr);

  double GetTxPowerDbPhy13 (void) const;
  double GetTxPowerDbPhy14 (void) const;
  void SetTxPowerDbPhy13 (double txpwr);
  void SetTxPowerDbPhy14 (double txpwr);
  
  double GetTxPowerDbPhy15 (void) const;
  double GetTxPowerDbPhy16 (void) const;
  void SetTxPowerDbPhy15 (double txpwr);
  void SetTxPowerDbPhy16 (double txpwr);

  double GetTxPowerDbPhy17 (void) const;
  double GetTxPowerDbPhy18 (void) const;
  void SetTxPowerDbPhy17 (double txpwr);
  void SetTxPowerDbPhy18 (double txpwr);

  /** \copydoc LoraPhy::GetRxGainDb */
  double GetRxGainDbPhy1 (void) const;
  /** \copydoc LoraPhy::GetRxGainDb */
  double GetRxGainDbPhy2 (void) const;

  void SetRxGainDbPhy1 (double gain);
  void SetRxGainDbPhy2 (double gain);

  double GetRxGainDbPhy3 (void) const;
  double GetRxGainDbPhy4 (void) const;
  void SetRxGainDbPhy3 (double gain);
  void SetRxGainDbPhy4 (double gain);

  double GetRxGainDbPhy5 (void) const;
  double GetRxGainDbPhy6 (void) const;
  void SetRxGainDbPhy5 (double gain);
  void SetRxGainDbPhy6 (double gain);
  
  double GetRxGainDbPhy7 (void) const;
  double GetRxGainDbPhy8 (void) const;
  void SetRxGainDbPhy7 (double gain);
  void SetRxGainDbPhy8 (double gain);

  double GetRxGainDbPhy9 (void) const;
  double GetRxGainDbPhy10 (void) const;
  void SetRxGainDbPhy9 (double gain);
  void SetRxGainDbPhy10 (double gain);

  double GetRxGainDbPhy11 (void) const;
  double GetRxGainDbPhy12 (void) const;
  void SetRxGainDbPhy11 (double gain);
  void SetRxGainDbPhy12 (double gain);

  double GetRxGainDbPhy13 (void) const;
  double GetRxGainDbPhy14 (void) const;
  void SetRxGainDbPhy13 (double gain);
  void SetRxGainDbPhy14 (double gain);  
  
  double GetRxGainDbPhy15 (void) const;
  double GetRxGainDbPhy16 (void) const;
  void SetRxGainDbPhy15 (double gain);
  void SetRxGainDbPhy16 (double gain);

  double GetRxGainDbPhy17 (void) const;
  double GetRxGainDbPhy18 (void) const;
  void SetRxGainDbPhy17 (double gain);
  void SetRxGainDbPhy18 (double gain);  

  /**
   * Get the list of available modes.
   *
   * \return The mode list.
   */
  LoraModesList GetModesPhy1 (void) const;
  /** \copydoc GetModesPhy1 */
  LoraModesList GetModesPhy2 (void) const;

  LoraModesList GetModesPhy3 (void) const;
  LoraModesList GetModesPhy4 (void) const;

  LoraModesList GetModesPhy5 (void) const;
  LoraModesList GetModesPhy6 (void) const;
  
  LoraModesList GetModesPhy7 (void) const;
  LoraModesList GetModesPhy8 (void) const;

  LoraModesList GetModesPhy9 (void) const;
  LoraModesList GetModesPhy10 (void) const;
  
  
  LoraModesList GetModesPhy11 (void) const;
  LoraModesList GetModesPhy12 (void) const;

  LoraModesList GetModesPhy13 (void) const;
  LoraModesList GetModesPhy14 (void) const;
  
  LoraModesList GetModesPhy15 (void) const;
  LoraModesList GetModesPhy16 (void) const;

  LoraModesList GetModesPhy17 (void) const;
  LoraModesList GetModesPhy18 (void) const;

  /**
   * Set the available modes.
   * 
   * \param modes List of modes.
   */
  void SetModesPhy1 (LoraModesList modes);
  /** \copydoc SetModesPhy1 */
  void SetModesPhy2 (LoraModesList modes);

  void SetModesPhy3 (LoraModesList modes);
  void SetModesPhy4 (LoraModesList modes);

  void SetModesPhy5 (LoraModesList modes);
  void SetModesPhy6 (LoraModesList modes);
  
  
  void SetModesPhy7 (LoraModesList modes);
  void SetModesPhy8 (LoraModesList modes);

  void SetModesPhy9 (LoraModesList modes);
  void SetModesPhy10 (LoraModesList modes);
  
  void SetModesPhy11 (LoraModesList modes);
  void SetModesPhy12 (LoraModesList modes);

  void SetModesPhy13 (LoraModesList modes);
  void SetModesPhy14 (LoraModesList modes);
  
  
  void SetModesPhy15 (LoraModesList modes);
  void SetModesPhy16 (LoraModesList modes);

  void SetModesPhy17 (LoraModesList modes);
  void SetModesPhy18 (LoraModesList modes);

  /**
   * Get the error probability model.
   *
   * \return The error model.
   */
  Ptr<LoraPhyPer> GetPerModelPhy1 (void) const;
  /** \copydoc GetPerModelPhy1() */
  Ptr<LoraPhyPer> GetPerModelPhy2 (void) const;

  Ptr<LoraPhyPer> GetPerModelPhy3 (void) const;
  Ptr<LoraPhyPer> GetPerModelPhy4 (void) const;

  Ptr<LoraPhyPer> GetPerModelPhy5 (void) const;
  Ptr<LoraPhyPer> GetPerModelPhy6 (void) const;
  
  Ptr<LoraPhyPer> GetPerModelPhy7 (void) const;
  Ptr<LoraPhyPer> GetPerModelPhy8 (void) const;

  Ptr<LoraPhyPer> GetPerModelPhy9 (void) const;
  Ptr<LoraPhyPer> GetPerModelPhy10 (void) const;
  
  Ptr<LoraPhyPer> GetPerModelPhy11 (void) const;
  Ptr<LoraPhyPer> GetPerModelPhy12 (void) const;

  Ptr<LoraPhyPer> GetPerModelPhy13 (void) const;
  Ptr<LoraPhyPer> GetPerModelPhy14 (void) const;
  
  Ptr<LoraPhyPer> GetPerModelPhy15 (void) const;
  Ptr<LoraPhyPer> GetPerModelPhy16 (void) const;

  Ptr<LoraPhyPer> GetPerModelPhy17 (void) const;
  Ptr<LoraPhyPer> GetPerModelPhy18 (void) const;

  /**
   * Set the error probability model.
   *
   * \param per The error model.
   */
  void SetPerModelPhy1 (Ptr<LoraPhyPer> per);
  /** \copydoc SetPerModelPhy1 */
  void SetPerModelPhy2 (Ptr<LoraPhyPer> per);

  void SetPerModelPhy3 (Ptr<LoraPhyPer> per);
  void SetPerModelPhy4 (Ptr<LoraPhyPer> per);

  void SetPerModelPhy5 (Ptr<LoraPhyPer> per);
  void SetPerModelPhy6 (Ptr<LoraPhyPer> per);
  
  void SetPerModelPhy7 (Ptr<LoraPhyPer> per);
  void SetPerModelPhy8 (Ptr<LoraPhyPer> per);

  void SetPerModelPhy9 (Ptr<LoraPhyPer> per);
  void SetPerModelPhy10 (Ptr<LoraPhyPer> per);
  
  void SetPerModelPhy11 (Ptr<LoraPhyPer> per);
  void SetPerModelPhy12 (Ptr<LoraPhyPer> per);

  void SetPerModelPhy13 (Ptr<LoraPhyPer> per);
  void SetPerModelPhy14 (Ptr<LoraPhyPer> per);
  
  void SetPerModelPhy15 (Ptr<LoraPhyPer> per);
  void SetPerModelPhy16 (Ptr<LoraPhyPer> per);

  void SetPerModelPhy17 (Ptr<LoraPhyPer> per);
  void SetPerModelPhy18 (Ptr<LoraPhyPer> per);

  /**
   * Get the SINR calculator.
   *
   * \return The SINR calculator.
   */
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy1 (void) const;
  /** \copydoc GetSinrModelPhy1 */
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy2 (void) const;

  Ptr<LoraPhyCalcSinr> GetSinrModelPhy3 (void) const;
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy4 (void) const;

  Ptr<LoraPhyCalcSinr> GetSinrModelPhy5 (void) const;
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy6 (void) const;
  
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy7 (void) const;
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy8 (void) const;

  Ptr<LoraPhyCalcSinr> GetSinrModelPhy9 (void) const;
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy10 (void) const;
  
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy11 (void) const;
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy12 (void) const;

  Ptr<LoraPhyCalcSinr> GetSinrModelPhy13 (void) const;
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy14 (void) const;
  
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy15 (void) const;
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy16 (void) const;

  Ptr<LoraPhyCalcSinr> GetSinrModelPhy17 (void) const;
  Ptr<LoraPhyCalcSinr> GetSinrModelPhy18 (void) const;
  
  /**
   * Set the SINR calculator.
   *
   * \param calcSinr The SINR calculator.
   */
  void SetSinrModelPhy1 (Ptr<LoraPhyCalcSinr> calcSinr);
  /** \copydoc SetSinrModelPhy1 */
  void SetSinrModelPhy2 (Ptr<LoraPhyCalcSinr> calcSinr);

  void SetSinrModelPhy3 (Ptr<LoraPhyCalcSinr> calcSinr);
  void SetSinrModelPhy4 (Ptr<LoraPhyCalcSinr> calcSinr);

  void SetSinrModelPhy5 (Ptr<LoraPhyCalcSinr> calcSinr);
  void SetSinrModelPhy6 (Ptr<LoraPhyCalcSinr> calcSinr);
  
  void SetSinrModelPhy7 (Ptr<LoraPhyCalcSinr> calcSinr);
  void SetSinrModelPhy8 (Ptr<LoraPhyCalcSinr> calcSinr);

  void SetSinrModelPhy9 (Ptr<LoraPhyCalcSinr> calcSinr);
  void SetSinrModelPhy10 (Ptr<LoraPhyCalcSinr> calcSinr);
  
  void SetSinrModelPhy11 (Ptr<LoraPhyCalcSinr> calcSinr);
  void SetSinrModelPhy12 (Ptr<LoraPhyCalcSinr> calcSinr);

  void SetSinrModelPhy13 (Ptr<LoraPhyCalcSinr> calcSinr);
  void SetSinrModelPhy14 (Ptr<LoraPhyCalcSinr> calcSinr);
  
  void SetSinrModelPhy15 (Ptr<LoraPhyCalcSinr> calcSinr);
  void SetSinrModelPhy16 (Ptr<LoraPhyCalcSinr> calcSinr);

  void SetSinrModelPhy17 (Ptr<LoraPhyCalcSinr> calcSinr);
  void SetSinrModelPhy18 (Ptr<LoraPhyCalcSinr> calcSinr);

  
  /** \copydoc LoraPhy::GetPacketRx */
  Ptr<Packet> GetPhy1PacketRx (void) const;
  Ptr<Packet> GetPhy2PacketRx (void) const;

  Ptr<Packet> GetPhy3PacketRx (void) const;
  Ptr<Packet> GetPhy4PacketRx (void) const;

  Ptr<Packet> GetPhy5PacketRx (void) const;
  Ptr<Packet> GetPhy6PacketRx (void) const;
  
  Ptr<Packet> GetPhy7PacketRx (void) const;
  Ptr<Packet> GetPhy8PacketRx (void) const;

  Ptr<Packet> GetPhy9PacketRx (void) const;
  Ptr<Packet> GetPhy10PacketRx (void) const;
  
  Ptr<Packet> GetPhy11PacketRx (void) const;
  Ptr<Packet> GetPhy12PacketRx (void) const;

  Ptr<Packet> GetPhy13PacketRx (void) const;
  Ptr<Packet> GetPhy14PacketRx (void) const;
  
  Ptr<Packet> GetPhy15PacketRx (void) const;
  Ptr<Packet> GetPhy16PacketRx (void) const;

  Ptr<Packet> GetPhy17PacketRx (void) const;
  Ptr<Packet> GetPhy18PacketRx (void) const;


private:
  /** First Phy layer. */
  Ptr<LoraPhy> m_phy1;
  Ptr<LoraPhy> m_phy2;

  Ptr<LoraPhy> m_phy3;
  Ptr<LoraPhy> m_phy4;

  Ptr<LoraPhy> m_phy5;
  Ptr<LoraPhy> m_phy6;
  
  Ptr<LoraPhy> m_phy7;
  Ptr<LoraPhy> m_phy8;

  Ptr<LoraPhy> m_phy9;
  Ptr<LoraPhy> m_phy10;
  
  Ptr<LoraPhy> m_phy11;
  Ptr<LoraPhy> m_phy12;

  Ptr<LoraPhy> m_phy13;
  Ptr<LoraPhy> m_phy14;
  
  Ptr<LoraPhy> m_phy15;
  Ptr<LoraPhy> m_phy16;

  Ptr<LoraPhy> m_phy17;
  Ptr<LoraPhy> m_phy18;

  /** A packet was received successfully. */
  ns3::TracedCallback<Ptr<const Packet>, double, LoraTxMode > m_rxOkLogger;
  /** A packet was received unsuccessfully. */
  ns3::TracedCallback<Ptr<const Packet>, double, LoraTxMode > m_rxErrLogger;
  /** A packet was sent from this Phy. */
  ns3::TracedCallback<Ptr<const Packet>, double, LoraTxMode > m_txLogger;
  /** Callback when packet received without errors. */
  RxOkCallback m_recOkCb;
  /** Callback when packet received with errors. */
  RxErrCallback m_recErrCb;


  /**
   * Handle callback and logger for packets received without error.
   *
   * \param pkt The packet.
   * \param sinr The SINR.
   * \param mode The channel mode.
   */
  void RxOkFromSubPhy (Ptr<Packet> pkt, double sinr, LoraTxMode mode);
  /**
   * Handle callback and logger for packets received with error.
   *
   * \param pkt The packet.
   * \param sinr The SINR.
   */
  void RxErrFromSubPhy (Ptr<Packet> pkt, double sinr);
  
protected:
  virtual void DoDispose ();

};  // class LoraPhyDual

} // namespace ns3

#endif /* LORA_PHY_DUAL_H */

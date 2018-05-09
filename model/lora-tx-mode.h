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

#ifndef LORA_TX_MODE_H
#define LORA_TX_MODE_H

#include "ns3/object.h"
#include <map>

namespace ns3 {

class LoraTxModeFactory;
class LoraTxMode;

/**
 *
 * Abstraction of packet modulation information.
 *
 */
class LoraTxMode
{
public:
  LoraTxMode ();   //!< Constructor.
  ~LoraTxMode ();  //!< Destructor.

  /**
   * Modulation type.
   */
  typedef enum {
    PSK,   //!< Phase shift keying.
    QAM,   //!< Quadrature amplitude modulation.
    FSK,   //!< Frequency shift keying.
    LORA,  //!< LORA Modulation.
    OTHER  //!< Unspecified/undefined.
  } ModulationType;
  /**
   * Get the modulation type of the mode.
   *
   * \return The modulation type.
   */
  ModulationType GetModType (void) const;
  /**
   * Get the data rate of the transmit mode.
   *
   * \return Data rate of the TX mode, in bits per second.
   */
  uint32_t GetDataRateBps (void) const;
  /**
   * Get the physical signaling rate.
   *
   * \return PHY rate in symbols per second.
   */
  uint32_t GetPhyRateSps (void) const;
  /**
   * Get the transmission center frequency.
   *
   * \return Center frequency, in Hz.
   */
  uint32_t GetCenterFreqHz (void) const;
  /**
   * Get the transmission signal bandwidth.
   *
   * \return Bandwidth in Hz.
   */
  uint32_t GetBandwidthHz (void) const;
  /**
   * Get the number of constellation points in the modulation scheme.
   *
   * \return Number of constellation points.
   */
  uint32_t GetConstellationSize (void) const;
  /**
   * Get the mode name.
   *
   * \return Name
   */
  std::string GetName (void) const;
  /**
   * Get a unique id for the mode.
   *
   * \return Unique ID.
   */
  uint32_t GetUid (void) const;

private:
  friend class LoraTxModeFactory;
  friend std::ostream &operator<< (std::ostream & os, const LoraTxMode &mode);
  friend std::istream &operator>> (std::istream & is, LoraTxMode &mode);


  uint32_t m_uid;  //!< Mode id

};  // class LoraTxMode

  
/**
 * Writes tx mode entry to stream os.
 *
 * \param os The output stream.
 * \param mode The mode.
 * \return The stream.
 */
std::ostream & operator << (std::ostream & os, const LoraTxMode &mode);
/**
 * Reads tx mode entry from stream is
 *
 * \param is The input stream.
 * \param mode The mode.
 * \return The stream.
 */
std::istream & operator >> (std::istream & is, LoraTxMode &mode);

/**
 *
 * Global database of LoraTxMode objects, retrievable by id or name.
 */
class LoraTxModeFactory
{
public:
  LoraTxModeFactory ();   //!< Constructor.
  ~LoraTxModeFactory ();  //!< Destructor.

  /**
   *
   * \param type Modulation type.
   * \param dataRateBps Data rate in BPS.
   * \param phyRateSps  Symbol rate in symbols per second.
   * \param cfHz Center frequency in Hz.
   * \param bwHz Bandwidth in Hz.
   * \param constSize Modulation constellation size (2 for BPSK, 4 for QPSK).
   * \param name Unique string name for this transmission mode.
   *
   * \return the transmit mode object
   */
  static LoraTxMode CreateMode (LoraTxMode::ModulationType type,
                               uint32_t dataRateBps,
                               uint32_t phyRateSps,
                               uint32_t cfHz,
                               uint32_t bwHz,
                               uint32_t constSize,
                               std::string name);

  /**
   * Get a mode by name.
   *
   * \param name String name of mode.
   * \return Mode with given name.
   */
  static LoraTxMode GetMode (std::string name);
  /**
   * Get a mode by id.
   *
   * \param uid Unique ID of mode.
   * \return The mode with given uid.
   */
  static LoraTxMode GetMode (uint32_t uid);

private:
  friend class LoraTxMode;
  uint32_t m_nextUid;                  //!< next id number

  /**
   *
   * Container for the LoraTxMode properties.
   */
  struct LoraTxModeItem
  {
    LoraTxMode::ModulationType m_type;  //!< Modulation type.
    uint32_t m_cfHz;                   //!< Center frequency in Hz.
    uint32_t m_bwHz;                   //!< Bandwidth in Hz.
    uint32_t m_dataRateBps;            //!< Data rate in BPS.
    uint32_t m_phyRateSps;             //!< Symbol rate in symbols per second.
    uint32_t m_constSize;              //!< Modulation constellation size (2 for BPSK, 4 for QPSK).
    uint32_t m_uid;                    //!< Unique id.
    std::string m_name;                //!< Unique string name for this transmission mode.
  };

  /**
   * Container for modes
   *
   */
  std::map<uint32_t, LoraTxModeItem> m_modes;

  /**
   * Check if the mode \pname{name} already exists.
   *
   * \param name The mode name to test.
   * \return True if \pname{name} exists.
   */
  bool NameUsed (std::string name);

  /**
   * Construct and get the static global factory instance.
   *
   * \return The global instance.
   */
  static LoraTxModeFactory &GetFactory (void);

  /**
   * Get a mode by id.
   *
   * \param uid The unique id to find.
   * \return The corresponding mode.
   */
  LoraTxModeItem &GetModeItem (uint32_t uid);

  /**
   * Get a mode by name.
   * \param name The mode name to find.
   * \return The corresponding mode.
   */
  LoraTxModeItem &GetModeItem (std::string name);

  /**
   * Create a public LoraTxMode from an internal LoraTxModeItem.
   *
   * \param item The LoraTxModeItem to reference.
   * \return A public LoraTxMode.
   */
  LoraTxMode MakeModeFromItem (const LoraTxModeItem &item);

};  // class LoraTxModeFactory

/**
 *
 * Container for LoraTxModes.
 *
 * \see attribute_LoraModesList
 */
class LoraModesList
{
public:
  LoraModesList ();           //!< Constructor
  virtual ~LoraModesList ();  //!< Destructor

  /**
   * Add mode to this list.
   * \param mode The mode to add.
   */
  void AppendMode (LoraTxMode mode);
  /**
   * Delete the mode at given index.
   * \param num Index of mode to delete.
   */
  void DeleteMode (uint32_t num);
  /**
   * Retrieve a mode by index.
   *
   * \param index Mode index.
   * \return Mode at given index.
   */
  LoraTxMode operator[] (uint32_t index) const;
  /**
   * Get the number of modes in this list.
   *
   * \return Number of modes.
   */
  uint32_t GetNModes (void) const;


private:
  /** The vector of modes in this list. */
  std::vector<LoraTxMode> m_modes;
  
  friend std::ostream &operator << (std::ostream &os, const LoraModesList &ml);
  friend std::istream &operator >> (std::istream &is, LoraModesList &ml);

};  // class LoraModesList

/**
 * Write LoraModesList to stream os
 *
 * \param os The output stream.
 * \param ml The mode list.
 * \return The stream.
 */
std::ostream &operator << (std::ostream &os, const LoraModesList &ml);
/**
 * Read LoraModesList from stream is.
 *
 * \param is The input stream.
 * \param ml The mode list to fill.
 * \return The stream.
 */
std::istream &operator >> (std::istream &is, LoraModesList &ml);

ATTRIBUTE_HELPER_HEADER (LoraModesList);

} // namespace ns3

#endif /* LORA_TX_MODE_H */

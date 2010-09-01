/* -*- Mode: c++ -*-
 *  DTVChannel
 *  Copyright (c) 2005,2006 by Daniel Kristjansson
 *  Contains base class for digital channels.
 */

#ifndef _DTVCHANNEL_H_
#define _DTVCHANNEL_H_

// POSIX headers
#include <stdint.h>

// C++ headers
#include <vector>
using namespace std;

// Qt headers
#include <QMutex>
#include <QString>

// MythTV headers
#include "dtvconfparserhelpers.h" // for DTVTunerType
#include "channelbase.h"
#include "channelutil.h" // for pid_cache_t

class TVRec;

/** \class DTVChannel
 *  \brief Class providing a generic interface to digital tuning hardware.
 */
class DTVChannel : public ChannelBase
{
  public:
    DTVChannel(TVRec *parent);
    virtual ~DTVChannel();

    // Commands

    /// \brief To be used by the channel scanner and possibly the EIT scanner.
    virtual bool TuneMultiplex(uint mplexid, QString inputname) = 0;
    /// \brief To be used by the channel scanner and possibly the EIT scanner.
    virtual bool Tune(const DTVMultiplex &tuning, QString inputname) = 0;
    /// \brief Enters power saving mode if the card supports it
    virtual bool EnterPowerSavingMode(void) { return true; }

    // Gets

    /// \brief Returns program number in PAT, -1 if unknown.
    int GetProgramNumber(void) const
        { return currentProgramNum; };

    /// \brief Returns major channel, 0 if unknown.
    uint GetMajorChannel(void) const
        { return currentATSCMajorChannel; };

    /// \brief Returns minor channel, 0 if unknown.
    uint GetMinorChannel(void) const
        { return currentATSCMinorChannel; };

    /// \brief Returns DVB original_network_id, 0 if unknown.
    uint GetOriginalNetworkID(void) const
        { return currentOriginalNetworkID; };

    /// \brief Returns DVB transport_stream_id, 0 if unknown.
    uint GetTransportID(void) const
        { return currentTransportID; };

    /// \brief Returns PSIP table standard: MPEG, DVB, ATSC, or OpenCable
    QString GetSIStandard(void) const;

    /// \brief Returns suggested tuning mode: "mpeg", "dvb", or "atsc"
    QString GetSuggestedTuningMode(bool is_live_tv) const;

    /// \brief Returns tuning mode last set by SetTuningMode().
    QString GetTuningMode(void) const;

    /// \brief Returns a vector of supported tuning types.
    virtual vector<DTVTunerType> GetTunerTypes(void) const;

    void GetCachedPids(pid_cache_t &pid_cache) const;

    DTVChannel *GetMaster(const QString &videodevice);
    const DTVChannel *GetMaster(const QString &videodevice) const;

    /// \brief Returns true if this is the first of a number of multi-rec devs
    virtual bool IsMaster(void) const { return false; }

    // Sets

    /// \brief Sets tuning mode: "mpeg", "dvb", "atsc", etc.
    void SetTuningMode(const QString &tuningmode);

    void SaveCachedPids(const pid_cache_t &pid_cache) const;

  protected:
    /// \brief Sets PSIP table standard: MPEG, DVB, ATSC, or OpenCable
    void SetSIStandard(const QString&);
    void SetDTVInfo(uint atsc_major, uint atsc_minor,
                    uint dvb_orig_netid,
                    uint mpeg_tsid, int mpeg_pnum);
    void ClearDTVInfo(void) { SetDTVInfo(0, 0, 0, 0, -1); }

  protected:
    mutable QMutex dtvinfo_lock;

    DTVTunerType tunerType;
    QString sistandard; ///< PSIP table standard: MPEG, DVB, ATSC, OpenCable
    QString tuningMode;
    int     currentProgramNum;
    uint    currentATSCMajorChannel;
    uint    currentATSCMinorChannel;
    uint    currentTransportID;
    uint    currentOriginalNetworkID;

    static QMutex                    master_map_lock;
    static QMap<QString,DTVChannel*> master_map;
};

#endif // _DTVCHANNEL_H_

/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#ifndef _D_REQUEST_GROUP_MAN_H_
#define _D_REQUEST_GROUP_MAN_H_

#include "common.h"
#include "SharedHandle.h"
#include "TransferStat.h"
#include <string>
#include <deque>
#include <iosfwd>

namespace aria2 {

class DownloadEngine;
class RequestGroup;
class Command;
class Logger;
class DownloadResult;

class RequestGroupMan {
private:
  std::deque<SharedHandle<RequestGroup> > _requestGroups;
  std::deque<SharedHandle<RequestGroup> > _reservedGroups;
  std::deque<SharedHandle<DownloadResult> > _downloadResults;
  const Logger* _logger;
  unsigned int _maxSimultaneousDownloads;
  int32_t _gidCounter;

  std::string
  formatDownloadResult(const std::string& status,
		       const SharedHandle<DownloadResult>& downloadResult) const;

public:
  RequestGroupMan(const std::deque<SharedHandle<RequestGroup> >& requestGroups,
		  unsigned int maxSimultaneousDownloads = 1);

  bool downloadFinished();

  void save();

  void closeFile();
  
  void halt();

  void forceHalt();

  std::deque<Command*> getInitialCommands(DownloadEngine* e);

  void removeStoppedGroup();

  void fillRequestGroupFromReserver(DownloadEngine* e);

  void addRequestGroup(const SharedHandle<RequestGroup>& group);

  void addReservedGroup(const std::deque<SharedHandle<RequestGroup> >& groups);

  void addReservedGroup(const SharedHandle<RequestGroup>& group);

  size_t countRequestGroup() const;
		  
  SharedHandle<RequestGroup> getRequestGroup(size_t index) const;
		  
  void showDownloadResults(std::ostream& o) const;

  bool isSameFileBeingDownloaded(RequestGroup* requestGroup) const;

  TransferStat calculateStat();

  class DownloadStat {
  private:
    size_t _completed;
    size_t _error;
    size_t _inProgress;
    size_t _waiting;
  public:
    DownloadStat():_completed(0), _error(0), _inProgress(0), _waiting(0) {}

    void setCompleted(size_t c) { _completed = c; }
    void setError(size_t c) { _error = c; }
    void setInProgress(size_t c) { _inProgress = c; }
    void setWaiting(size_t c) { _waiting = c; }

    bool allCompleted() const
    {
      return _error == 0 && _inProgress == 0 && _waiting == 0;
    }
  };

  DownloadStat getDownloadStat() const;

  const std::deque<SharedHandle<DownloadResult> >&
  getDownloadResults() const;

};

typedef SharedHandle<RequestGroupMan> RequestGroupManHandle;

} // namespace aria2

#endif // _D_REQUEST_GROUP_MAN_H_

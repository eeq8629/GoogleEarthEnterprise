/*
 * Copyright 2017 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __Reservation_h
#define __Reservation_h


#include <khRefCounter.h>
#include <autoingest/khFusionURI.h>


class ReservationImpl : public khRefCounter
{
 protected:
  bool released;
 public:
  ReservationImpl(void) : released(false) { }
  virtual ~ReservationImpl(void) { }
  virtual void Release(void) { released = true; }
  virtual bool BindFilename(const std::string &assetpath, std::string &bound)
  {
    return false;
  }
};


typedef khRefGuard<ReservationImpl> Reservation;


class CPUReservationImpl : public ReservationImpl
{
  std::string host;
  uint num_;
  CPUReservationImpl(const std::string &host_, uint n);
 protected:
  virtual ~CPUReservationImpl(void) {
    if (!released) {
      Release();
    }
  }
 public:
  uint num(void) const { return num_; }
  virtual void Release(void);
  static Reservation Make(const std::string &host, uint num) {
    return Reservation(khRefGuardFromNew(new CPUReservationImpl(host, num)));
  }
};


class VolumeReservationImpl : public ReservationImpl
{
  std::string assetpath;
  khFusionURI uri;
  VolumeReservationImpl(const std::string &assetpath_,
                        const khFusionURI &uri_);
 protected:
  virtual ~VolumeReservationImpl(void);
 public:
  virtual void Release(void);
  static Reservation Make(const std::string &assetpath,
                          const khFusionURI &uri);
  virtual bool BindFilename(const std::string &assetpath,std::string &bound);
};



#endif /* __Reservation_h */

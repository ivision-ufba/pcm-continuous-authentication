/**
 * @file TemporalIntegration.hpp
 * @author Matheus Santos
 * @brief Temporal Integration from Pamplona et. al. method
 * @version 0.1
 * @date 2019-10-01
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef TEMPORAL_INTEGRATION
#define TEMPORAL_INTEGRATION

#include <bits/stdc++.h>

#define K_DROP 15000

class TemporalIntegrator
{
private:
  double pSafe;
  double pNotSafe;
  long now;
  long lastObservation;
  double lastKsafe;

public:
  TemporalIntegrator()
  {
    pSafe = 1.0;
    pNotSafe = 0.0;
    now = 0;
    lastObservation = 0;
    lastKsafe = 1.0;
  };
  /**
   * @brief Calculate the probability of the system is safe or not save
   * 
   * @param ztSafe probability of the system is safe given a historic Zt
   * @param ztNotSafe probability of the system is not safe given a historic Zt
   * @return double 
   */
  double calculatePsafe(const float &ztSafe, const float &ztNotSafe);
  /**
   * @brief Returns the Psafe value at time
   * 
   * @return double 
   */
  double updatePSafe();
  // Get and sets
  inline const double getPSafe() { return pSafe; }
  inline const double getPNotSafe() { return pNotSafe; }
  inline void setPSafe(const double &newPSafe) { pSafe = newPSafe; };
  inline void setPNotSafe(const double &newPNotSafe) { pNotSafe = newPNotSafe; };
  inline void setNow(const long &andNow) { now = andNow; };
  inline void setLastObservation(const long &last) { lastObservation = last; };
};
#endif
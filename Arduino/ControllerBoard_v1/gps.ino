void getGPS() {
  // Output longitude and latitude
  // Output GPS time, and update RTC
  // Output current speed
  // Output the distance and direction to next waypoint

  if (gps.location.isUpdated()) {
    longitude = gps.location.lng();
    latitude = gps.location.lat();
    satellites = gps.satellites.value();

    // Attach to the XRF object
    xrf.gps.setLatitude(latitude);
    xrf.gps.setLongitude(longitude);
    xrf.gps.setSatellites(satellites);
  }

  fix_age = gps.location.age();

  if (gps.date.isUpdated() && gps.time.isUpdated() && fix_age < 5000) {
    // May as well update the RTC

    // Due to the way the Teensy RTC interacts with the now() variable, we have to first
    // create a timestamp, adjust it, then set both the Teensy clock and then the 
    // current time
    tmElements_t new_time;
    new_time.Second = gps.time.second();
    new_time.Minute = gps.time.minute();
    new_time.Hour = gps.time.hour();
    new_time.Day = gps.date.day();
    new_time.Month = gps.date.month();
    new_time.Year = gps.date.year() - 1970;

    // Make a timestamp from the GPS data
    time_t new_time_t = makeTime(new_time);

    // Adjust for daylight savings
    new_time_t += 3600;

    // Set the Teensy RTC and the current time
    Teensy3Clock.set(new_time_t);
    setTime(new_time_t);
  }

  if (gps.speed.isUpdated()) {
    speed_mph = gps.speed.mph();

    // Attach to the XRF object
    xrf.gps.setSpeed(speed_mph);
  }

  // Now we need to decide as to whether we have a fix or not
  // Check to see if the fix age is less than 10 seconds old, if so, we have a fix
  if (fix_age < 10000 && fix_age != 0) {
    has_fix = true;
  } else {
    has_fix = false;
    xrf.gps.setSpeed(0);
    xrf.gps.setLatitude(0);
    xrf.gps.setLongitude(0);
    xrf.gps.setSatellites(0);
  }
  
  // Update the radio module as to whether we have a fix  
  xrf.gps.setHasFix(has_fix);
}


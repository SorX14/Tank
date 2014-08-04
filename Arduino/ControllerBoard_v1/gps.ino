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
  }
  
  fix_age = gps.location.age();

  if (gps.date.isUpdated() && gps.time.isUpdated()) {
    // May as well update the RTC
    setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
    adjustTime(3600);
  }

  if (gps.speed.isUpdated()) {
    speed_mph = gps.speed.mph();
  }

  // Now we need to decide as to whether we have a fix or not
  // Check to see if the fix age is less than 10 seconds old, if so, we have a fix
  if (fix_age < 10000 && fix_age != 0) {
    has_fix = true;
  } else {
    has_fix = false;
  }
}

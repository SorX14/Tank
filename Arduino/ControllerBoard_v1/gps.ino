void getGPS() {
	// Output longitude and latitude
	// Output GPS time, and update RTC
	// Output current speed
	// Output the distance and direction to next waypoint
	
	if (gps.location.isUpdated()) {
		longitude = gps.location.lng();
		latitude = gps.location.lat();
		fix_age = gps.location.age();
	}
	
	if (gps.time.isUpdated()) {
		//gps.time.value();
	}
		
		
  }
}
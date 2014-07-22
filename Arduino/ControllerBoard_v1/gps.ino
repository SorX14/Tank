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
	
	if (gps.date.isUpdated() && gps.time.isUpdated()) {
		// May as well update the RTC
		setTime(gps.time.hours(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
	}
	
	if (gps.speed.isUpdated()) {
		speed_mph = gps.speed.mph();
	}
}

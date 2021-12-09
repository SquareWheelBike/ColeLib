# ColeLib is a C library for arduino, that includes a series of functions that are typically used in controls and other applications.

## These functions are designed to be used in a variety of applications.

 - TON is an asynchronous timer that can be used to schedule events in the future. It needs to be called every scan cycle. Returns true when timer complete. Starts timing on a positive parameter trigger.
 - oneshot allows for a single-scan pulse true condition on a true parameter, ie press a button and will only send one true pulse per positive edge trigger. 
 - debounce is a simple way to filter out noise from a sensor. It will only return true when the sensor is in a stable state for a certain number of scans.
1. Background info
------------------

We are making software for smart electricity meter.

The software consists of several components:
 - metering_hw - providing RAW metering data (eg. reading it from some dedicated IC and passing it to userspace)
 - meter_srv - "logic-level" metering - computing some data based on RAW readings (eg. max/min/avg, etc.)
 - meter_app - application logic - responsible for capturing the data from meter_srv at constant time intervals,
   communication with outside world, etc.

2. The task
-----------

Your task is to design and implement communication protocol between meter_srv and meter_app with sample meter_srv
implementation and commandline client for it.

Bear in mind that:
 - the solution should be written in C language
 - the solution should work on POSIX-compliant OS (like linux)
 - we're making embedded software, so memory and CPU usage optimisation is important
 - (because of above) try to make single-threaded application
 - the main use case is to have one client for meter_srv, but there has to be support for multiple clients
   (eg. while testing the meter_app we would like to get simultaneously data using other channels)
 - we would like to run meter_srv on one (embedded) device and commandline client on another machine
   (eg. while running metering test cases)

3. metering_hw
--------------

We're providing sample metering_hw implementation. It is badly written on purpose - feel free to fix it.

In real environment this part would be a kernel module accessed by device file, but for ease of development we're
using userspace daemon + named pipes for communication.

4. The protocol
---------------

The protocol needs to support below use cases:
 - get meter number (8 digits) and meter_srv version
 - get instantaneous per-phase values
 - get per-phase time integral A+ and A- (sum of all phases)
 - get angles
 - get min/max per-phase voltage and current since last reset
 - reset min/max voltages and currents
 - set under-voltage threshold for x mV
 - set over-voltage threshold for x mV
 - receive event notification - under-voltage/over-voltage begin/end

5. metering_srv
---------------

Your implementation should:
 - periodically get data from metering_hw
 - process data to support all above use cases
 - store time integrals (A+, A-) between device reboots (metering_hw computes A+/A- only since power up)
 - support multiple clients (max. 5)
 - NOTE: all variables are global (eg. the under-voltage/over-voltage thresholds are shared between clients)


6. Deliverables
---------------

The solution should consist of:
 - meter_srv sample implementation (as described above)
 - header file with protocol specification (to be used in meter_app)
 - sample commandline client for meter_srv
 - fixed metering_hw (if applicable)
 - a way to build executables on POSIX-compliant OS (eg. linux)


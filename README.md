# Cache-Covert-Channel

An inter-process messaging chat bot realized using a prime and probe cache side-channel attack. This projects demonstrates the possibility for covert communication between isolated processes, as long as they share the same LLC cache. This project was done on a modern multicore Intel CPU running Ubuntu Server OS.

Note: This software only works on the processor it is programmed for. A few parameters need to be changed for it to work on other processors. In addition, communication rate is unoptimized as this is a proof-of-concept design.

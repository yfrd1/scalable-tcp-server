# TODO

## Connection Management
- [ ] Replace active_connections counter logic with compare_exchange_weak/strong to enforce max_connections atomically.

## Message Handling
- [ ] Improve async read buffer logic to correctly handle maximum message size across multiple reads.

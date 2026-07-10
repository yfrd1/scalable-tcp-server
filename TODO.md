# TODO

## Core Features
- Add server file
- Add chat room
- Add video streaming

## Implementation Details

## Connection Management
- [ ] Replace active_connections counter logic with compare_exchange_weak/strong to enforce max_connections atomically.

## Message Handling
- [ ] Improve async read buffer logic to correctly handle maximum message size (config::max_message_size_bytes) across multiple reads.

## Periodic Log Flushing
- [ ] Implement periodic log flushing based on flush_interval_seconds.


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

## Packet encryption/decryption
- [ ] Implement Packet payload compression and encryption/decryption

## Packet sequence ID
- [ ] Implement Packet sequence ID validation and ordering system

## Localization layer for server
- [ ] Add localization layer for server responses, errors, and messages

## Header parsers
- [ ] Add header parsers for each packet type and validate that the parsed header size exactly matches `header_size_`.

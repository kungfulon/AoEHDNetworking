# AoEHDNetworking

This library makes AoE HD to use newer `SteamNetworkingMessages` API over `SteamNetworking` API, and force use of relay connection (TURN) instead of NAT traversal (STUN).

The main reason for this to exist: Sometimes when I was playing with my friends, we noticed that direct P2P connection was unstable, while relayed connection was OK.

This library might be extended to choose between relay connection and NAT traversal based on ping.

## License

This project is licensed under MIT License.

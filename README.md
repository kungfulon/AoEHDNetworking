# AoEHDNetworking

This library makes AoE HD to use newer `SteamNetworkingMessages` API over `SteamNetworking` API, and force use of relay connection (TURN) instead of NAT traversal (STUN).

The main reason for this to exist: Sometimes when I was playing with my friends, we noticed that direct P2P connection was unstable, while relayed connection was OK.

This library might be extended to choose between relay connection and NAT traversal based on ping.

## Building

Use Visual Studio 2019 to build the project.

## Installation

- Rename `steam_api.dll` in your AoE HD folder to `valve_api.dll`
- Copy `steam_api.dll` that was built before to your AoE HD folder.

Note that you cannot connect to other players that are not using this library.

## License

This project is licensed under MIT License.

This project uses Steamworks SDK. The license is available at https://partner.steamgames.com/documentation/sdk_access_agreement. 

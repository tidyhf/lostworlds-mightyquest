# Credits

This project stands on the shoulders of prior work:

- **MQELOffline** and the **Ayria / Localnetworking** framework by **Convery / Hedgehogscience**
  ([MQELOffline_cpp](https://github.com/Hedgehogscience/MQELOffline_cpp),
  [Localnetworking_cpp](https://github.com/Hedgehogscience/Localnetworking_cpp),
  [Platformwrapper_cpp](https://github.com/Convery/Platformwrapper_cpp)) — MIT.
  The `.hqs` game-server logic, the OpenSSL TLS server, and the libCURL certificate-bypass
  byte-patterns all come from MQELOffline. This repo **vendors that frontend** (under
  `src/mqeloffline/`, lightly patched to build on modern MSVC) and runs it as a **standalone
  server** over a real socket, instead of an in-process Localnetworking plugin.
- **nlohmann/json**, **miniz / miniz_cpp**, and **http_parser** — vendored inside MQELOffline.
- **OpenSSL** (Apache-2.0) — TLS.

### What this repo adds
- `src/mqelserver/main.cpp` — a real loopback/LAN TCP+TLS listener that drives MQELOffline's
  `Gameserver` (replacing the Localnetworking transport), so no game injection is needed for
  the *server*.
- `src/certshim/` — a tiny cert-patch DLL (the same two-function bypass MQELOffline uses) plus
  a same-user injector, so the unmodified retail client trusts the local server's certificate.
- Hosting/setup scripts and packaging.

### Legal
"The Mighty Quest for Epic Loot" is © Ubisoft. This is a **non-commercial preservation /
hobby project** and ships **no game assets or game code** — you provide your own client from
Steam. It is unaffiliated with and unendorsed by Ubisoft.

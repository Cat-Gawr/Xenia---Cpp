group("third_party")
project("discord-rpc")
  uuid("012f6131-efc0-4abd-852d-a33640732d4c")
  kind("StaticLib")
  language("C++")
  links({
  })
  defines({
    "_LIB",
  })
  includedirs({
    "discord-rpc/include",
    "rapidjson/include"
  })
  files({
    "discord-rpc/src/connection.h",
    "discord-rpc/src/discord_rpc.cpp",
    "discord-rpc/src/msg_queue.h",
    "discord-rpc/src/rpc_connection.cpp",
    "discord-rpc/src/rpc_connection.h",
    "discord-rpc/src/serialization.cpp",
    "discord-rpc/src/serialization.h"
  })
  filter("platforms:Linux")
    files({
      "discord-rpc/src/connection_unix.cpp",
      "discord-rpc/src/discord_register_linux.cpp"
    })
  filter("platforms:Mac")
    files({
      "discord-rpc/src/discord_register_osx.m"
    })
  filter("platforms:Windows")
    files({
      "discord-rpc/src/connection_win.cpp",
      "discord-rpc/src/discord_register_win.cpp"
    })

## questo file potrebbe essere difetosso( non si usa più linux  )

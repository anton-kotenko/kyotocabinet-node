{
  "target_defaults": {
    "default_configuration": "Release",
    "configurations": {
      "Debug": {
        "cflags": ["-g", "-O0"],
        "ccflags": ["-g", "-O0"]
      },
      "Release": {
      }
    }
  },
  "targets": [{
      "target_name": "kyotonode",
      "sources":  ["main.cc", "kyotocabinet"],
      "cflags_cc!": ["-fno-exceptions"],
      "cflags!": ["-fno-exceptions"],
      "libraries": [
        "-l kyotocabinet"
      ]

  }]
}

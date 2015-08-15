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
      "sources":  ["src/main.cc", "kyotocabinet"],
      "cflags_cc!": ["-fno-exceptions"],
      "cflags!": ["-fno-exceptions"],
      "include_dirs" : [
        "<!(node -e \"require('nan')\")"
      ],
      "libraries": [
        "-l kyotocabinet"
      ]

  }]
}

{
  "targets": [
    {
      "target_name": "kyotonode",
      "sources":  ["main.cc", "kyotocabinet"],
      "cflags_cc!": ["-fno-exceptions"],
      "cflags!": ["-fno-exceptions"],
      "libraries": [
        "-l kyotocabinet"
      ]
    },
  ]
}

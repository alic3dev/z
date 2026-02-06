#include <z_words.h>

#include <clic3_bytes.h>
#include <clic3_char_arrays.h>
#include <clic3_memory.h>

const char* z_words[
  z_words_length
] = {
  "iridescent",
  "sparkling",
  "lost",
  "shining",
  "dream",
  "cavern",
  "cove",
  "forgotten",
  "imaginative",
  "shimmering",
  "embers",
  "emergent",
  "thinly",
  "fluid",
  "translucent",
  "refletive",
  "immersion",
  "lively",
  "flying",
  "blue",
  "hues",
  "shades",
  "light",
  "shadows",
  "edges",
  "glimpses",
  "infinite",
  "eternal",
  "wandering",
  "thoughts",
  "eyes",
  "lies",
  "before",
  "truth",
  "dies",
  "slumbering",
  "forever",
  "never",
  "awoken",
  "spoken",
  "gone",
  "hope",
  "tomorrow",
  "another",
  "night",
  "turns",
  "spins",
  "twisting",
  "snarling",
  "echoing",
  "sounds",
  "patterned",
  "vibrations",
  "mistaken",
  "cost",
  "lower",
  "level",
  "heights",
  "waves",
  "blowing",
  "reverberates",
  "systemic",
  "design",
  "hollowed",
  "shadowed",
  "plainly",
  "seen",
  "heard",
  "heart",
  "beating",
  "slept",
  "kept",
  "falling",
  "whispers",
  "time",
  "cosmic",
  "radiance"
};

void z_words_construct(
  char** words_constructed,
  char* words
) {
  unsigned int length_name = 0;
  unsigned int length_name_next = 0;

  unsigned int offset_name = 0;
  unsigned int offset_name_next = 0;

  for (
    unsigned char index_name = 0;
    words[
      index_name
    ] != '\0';
    ++index_name
  ) {
    unsigned int index_word = (
      words[
        index_name
      ] %
      z_words_length
    );

    char* char_array_word = (
      (char*)
      z_words[
        index_word
      ]
    );

    unsigned int length_word = (
      clic3_char_array_length(
        char_array_word
      )
    );

    length_name = (
      length_name +
      (
        index_name > 0
        ? 1
        : 0
      ) +
      length_word
    );

    clic3_memory_reallocate_raw(
      words_constructed,
      length_name
    );

    if (
      index_name > 0
    ) {
      (*words_constructed)[
        offset_name
      ] = (
        ' '
      );

      offset_name = (
        offset_name +
        1
      );
    }

    clic3_bytes_copy(
      (
        (*words_constructed) +
        offset_name
      ),
      char_array_word,
      length_word
    );

    offset_name = (
      length_name
    );
  }

  clic3_memory_reallocate_raw(
    words_constructed,
    (
      length_name +
      1
    )
  );

  (*words_constructed)[
    length_name
  ] = (
    '\0'
  );
}

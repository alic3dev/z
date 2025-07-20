#include <z.h>
#include <io_proc.h>
#include <io_proc_data.h>

#include <cer0.h>
#include <interrupt_handler.h>

#include <CoreAudio/CoreAudio.h>

#include <time.h>

int main() {
  srand(time((void*)0));

  struct io_proc_data io_proc_data;

  io_proc_data_initialize(
    &io_proc_data
  );

  struct cer0_audio_output audio_output;
  cer0_audio_output_initialize(
    &audio_output,
    io_proc,
    &io_proc_data
  );

  io_proc_data_initialize_synthesizer(
    &io_proc_data,
    audio_output.sample_rate
  );

  interrupt_handler_initialize();

  printf("[press_enter_to_exit] ");
  getc(stdin);

  cer0_audio_output_destroy(
    &audio_output
  );

  io_proc_data_destroy(
    &io_proc_data
  );

  return 0;
}

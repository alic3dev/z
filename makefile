name=z

directory_include=include
directory_objects=objects
directory_output=output
directory_sources=sources

directory_cero=../cer0
directory_cero_include=${directory_cero}/include
directory_cero_library=${directory_cero}/library/macos/release

directory_clic3=../clic3
directory_clic3_include=${directory_clic3}/include
directory_clic3_library=${directory_clic3}/library/macos/release

directory_interrupt_handler=../interrupt_handler
directory_interrupt_handler_include=${directory_interrupt_handler}/include
directory_interrupt_handler_library=${directory_interrupt_handler}/library/macos/release

directory_math_c=../math_c
directory_math_c_include=${directory_math_c}/include
directory_math_c_library=${directory_math_c}/library/macos/release

directory_rand=../rand
directory_rand_include=${directory_rand}/include
directory_rand_library=${directory_rand}/library/macos/release

file_output=${directory_output}/${name}

file_cero_library=${directory_cero_library}/cer0.0.dylib
file_clic3_library=${directory_clic3_library}/clic3.0.dylib
file_interrupt_handler_library=${directory_interrupt_handler_library}/interrupt_handler.0.dylib
file_math_c_library=${directory_math_c_library}/math_c.0.dylib
file_rand_library=${directory_rand_library}/rand.0.dylib

files_sources=${wildcard ${directory_sources}/*.c}
files_objects=${patsubst ${directory_sources}/%.c,${directory_objects}/%.o,${files_sources}}
files_libraries=${file_cero_library} ${file_clic3_library} ${file_interrupt_handler_library} ${file_math_c_library} ${file_rand_library}

ifndef target_device_version
	target_device_version=26.1
endif

target_platform=arm64-apple-macos${target_device_version}

directory_sdk=${shell xcrun --sdk macosx${target_device_version} --show-sdk-path}

cc=clang
c_flags_platform=-target ${target_platform} -isysroot ${directory_sdk}
c_includes=${addprefix -I,${directory_include} ${directory_cero_include} ${directory_clic3_include} ${directory_interrupt_handler_include} ${directory_math_c_include} ${directory_rand_include}}
c_flags=-O3 ${c_flags_platform} ${c_includes} -Daudio_enabled=1
c_flags_output=-framework CoreAudio

${name}: ${file_output}

run:
	./${file_output}

${file_output}: ${files_objects} ${directory_output}
	${cc} ${c_flags} ${c_flags_output} ${files_libraries} ${files_objects} -o ${file_output}
	-rm ${directory_output}/${shell basename ${file_cero_library}}
	-rm ${directory_output}/${shell basename ${file_clic3_library}}
	-rm ${directory_output}/${shell basename ${file_interrupt_handler_library}}
	-rm ${directory_output}/${shell basename ${file_math_c_library}}
	-rm ${directory_output}/${shell basename ${file_rand_library}}
	ln -s ../${file_cero_library} ${directory_output}/${shell basename ${file_cero_library}}
	ln -s ../${file_clic3_library} ${directory_output}/${shell basename ${file_clic3_library}}
	ln -s ../${file_interrupt_handler_library} ${directory_output}/${shell basename ${file_interrupt_handler_library}}
	ln -s ../${file_math_c_library} ${directory_output}/${shell basename ${file_math_c_library}}
	ln -s ../${file_rand_library} ${directory_output}/${shell basename ${file_rand_library}}

${directory_objects}/%.o: ${directory_sources}/%.c ${directory_objects}
	${cc} ${c_flags} -c $< -o $@

${directory_objects}:
	mkdir ${directory_objects}

${directory_output}:
	mkdir ${directory_output}

clean: clean_objects clean_output

clean_objects:
	-rm -r ${directory_objects}

clean_output:
	-rm -r ${directory_output}

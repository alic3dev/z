name=z

directory_include=include
directory_objects=objects
directory_output=output
directory_sources=sources

directory_cero=../cer0
directory_cero_include=${directory_cero}/include
directory_cero_library=${directory_cero}/library

directory_clic3=../clic3
directory_clic3_library=${directory_clic3}/library

directory_interrupt_handler=../interrupt_handler
directory_interrupt_handler_include=${directory_interrupt_handler}/include
directory_interrupt_handler_library=${directory_interrupt_handler}/library

directory_math_c=../math_c
directory_math_c_include=${directory_math_c}/include
directory_math_c_library=${directory_math_c}/library

file_output=${directory_output}/${name}

file_cero_library=${directory_cero_library}/cer0.o
file_clic3_library=${directory_clic3_library}/clic3.o
file_interrupt_handler_library=${directory_interrupt_handler_library}/interrupt_handler.o
file_math_c_library=${directory_math_c_library}/math_c.o

files_sources=${wildcard ${directory_sources}/*.c}
files_objects=${patsubst ${directory_sources}/%.c,${directory_objects}/%.o,${files_sources}}
files_libraries=${file_cero_library} ${file_clic3_library} ${file_interrupt_handler_library} ${file_math_c_library}

cc=clang
c_includes=${addprefix -I,${directory_include} ${directory_cero_include} ${directory_interrupt_handler_include} ${directory_math_c_include}}
c_flags=-O3 ${c_includes}
c_flags_output=-framework CoreAudio

${name}: ${file_output}

run: .always
	./${file_output}

${file_output}: ${files_objects} ${directory_output}
	${cc} ${c_flags} ${c_flags_output} ${files_libraries} ${files_objects} -o ${file_output}

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

.always:

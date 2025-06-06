name=z

directory_include=include
directory_objects=objects
directory_output=output
directory_sources=sources

directory_cero=../cer0
directory_cero_include=${directory_cero}/include
directory_cero_library=${directory_cero}/library

file_output=${directory_output}/${name}

file_cero_library=${directory_cero_library}/cer0.o

files_sources=${wildcard ${directory_sources}/*.c}
files_objects=${patsubst ${directory_sources}/%.c,${directory_objects}/%.o,${files_sources}}

cc=clang
c_flags=-O3 -I${directory_include} -I${directory_cero_include}
c_flags_output=-framework CoreAudio

${name}: ${file_output}

${file_output}: ${files_objects} ${directory_output}
	${cc} ${c_flags} ${c_flags_output} ${file_cero_library} ${files_objects} -o ${file_output}

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

name=z

ifndef target_device
target_device=mac
endif

ifeq (${target_device},mac)
target_os=macos
endif

ifeq (${target_device},iphone)
target_os=ios
name:=${name}_${target_os}
endif

version_major=0
version_minor=0
version_patch=0
version_major_minor=${version_major}.${version_minor}
version=${version_major}.${version_minor}.${version_patch}

directory_library_base=library
directory_objects_base=objects
directory_output_base=output

ifeq (${debug}, 1)
name:=${name}_debug
directory_library=${directory_library_base}/${target_os}/debug
directory_objects=${directory_objects_base}/${target_os}/debug
directory_output=${directory_output_base}/${target_os}/debug
else
directory_library=${directory_library_base}/${target_os}/release
directory_objects=${directory_objects_base}/${target_os}/release
directory_output=${directory_output_base}/${target_os}/release
endif

directory_include=include

directory_sources=sources

directory_cero=../cer0
directory_cero_include=${directory_cero}/include
directory_cero_library=${directory_cero}/library/${target_os}/release

directory_clic3=../clic3
directory_clic3_include=${directory_clic3}/include
directory_clic3_library=${directory_clic3}/library/${target_os}/release

directory_interrupt_handler=../interrupt_handler
directory_interrupt_handler_include=${directory_interrupt_handler}/include
directory_interrupt_handler_library=${directory_interrupt_handler}/library/${target_os}/release

directory_math_c=../math_c
directory_math_c_include=${directory_math_c}/include
directory_math_c_library=${directory_math_c}/library/${target_os}/release

directory_rand=../rand
directory_rand_include=${directory_rand}/include
directory_rand_library=${directory_rand}/library/${target_os}/release

file_library_object=${directory_library}/${name}.o

name_library_dylib_major=${name}.${version_major}.dylib
file_library_dylib=${directory_library}/${name}.dylib
file_library_dylib_major=${directory_library}/${name_library_dylib_major}

file_install=${directory_install}/OS${name}.dylib
file_install_major=${directory_install}/OS${name_library_dylib_major}

name_library_dynamic_major=${name}.${version_major}.so
file_library_dynamic=${directory_library}/${name}.so
file_library_dynamic_major=${directory_library}/${name_library_dynamic_major}

file_library_static=${directory_library}/${name}.a

file_output=${directory_output}/${name}

ifeq (${target_os},ios)
file_cero_library=${directory_cero_library}/cer0_ios.0.dylib
file_clic3_library=${directory_clic3_library}/clic3_ios.0.dylib
file_interrupt_handler_library=${directory_interrupt_handler_library}/interrupt_handler_ios.0.dylib
file_math_c_library=${directory_math_c_library}/math_c_ios.0.dylib
file_rand_library=${directory_rand_library}/rand_ios.0.dylib
else
file_cero_library=${directory_cero_library}/cer0.0.dylib
file_clic3_library=${directory_clic3_library}/clic3.0.dylib
file_interrupt_handler_library=${directory_interrupt_handler_library}/interrupt_handler.0.dylib
file_math_c_library=${directory_math_c_library}/math_c.0.dylib
file_rand_library=${directory_rand_library}/rand.0.dylib
endif

file_cero_library:=${shell realpath "${file_cero_library}"}
file_clic3_library:=${shell realpath "${file_clic3_library}"}
file_interrupt_handler_library:=${shell realpath "${file_interrupt_handler_library}"}
file_math_c_library:=${shell realpath "${file_math_c_library}"}
file_rand_library:=${shell realpath "${file_rand_library}"}

files_sources=${wildcard ${directory_sources}/*.c}
ifeq (${target_os},ios)
files_sources:=${filter-out ${directory_sources}/z.c,${files_sources}}
endif
files_objects=${patsubst ${directory_sources}/%.c,${directory_objects}/%.o,${files_sources}}
files_objects_library:=${patsubst ${directory_objects}/%.o,${directory_objects}/%_library.o,${files_objects}}
files_objects_library:=${filter-out ${directory_objects}/z_library.o,${files_objects_library}}
files_objects_library:=${filter-out ${directory_objects}/z_display_library.o,${files_objects_library}}
files_libraries=${file_cero_library} ${file_clic3_library} ${file_interrupt_handler_library} ${file_math_c_library} ${file_rand_library}

ifndef target_device_version
	target_device_version=26.1
endif

ifeq (${target_os},macos)
target_platform=arm64-apple-macos${target_device_version}

directory_sdk=${shell xcrun --sdk macosx${target_device_version} --show-sdk-path}
endif

ifeq (${target_os},ios)
files_objects:=${patsubst ${directory_objects}/%.o,${directory_objects}/%_${target_os}.o,${files_objects}}
files_objects_library:=${patsubst ${directory_objects}/%.o,${directory_objects}/%_library.o,${files_objects}}

target_platform=arm64-apple-ios${target_device_version}

directory_sdk=${shell xcrun --sdk iphoneos${target_device_version} --show-sdk-path}
endif

cc=clang
c_flags_platform=-target ${target_platform} -isysroot ${directory_sdk}
c_includes=${addprefix -I,${directory_include} ${directory_cero_include} ${directory_clic3_include} ${directory_interrupt_handler_include} ${directory_math_c_include} ${directory_rand_include}}
c_flags=${c_flags_platform} ${c_includes}

ifeq (${target_os},ios)
c_flags:=${c_flags} -Dtarget_os_ios
endif

ifeq (${debug}, 1)
c_flags_debug=-O0 -g -v

c_flags:=${c_flags} ${c_flags_debug}
else
c_flags:=${c_flags} -O3
endif

c_flags_output=${c_flags} -framework CoreAudio
c_flags_objects=${c_flags}

ifeq (${debug},1)
c_flags_output:=${c_flags_output} -dA
c_flags_objects:=${c_flags_objects} -Qy
else
c_flags_objects:=${c_flags_objects} -Qn
endif

c_flags_library=${c_flags_objects}

ar=ar
ar_flags=cqS

ld=ld
ld_flags=

strip=strip
strip_flags=-x

ifeq (${target_os},ios)
${name}: libraries
else
${name}: libraries ${file_output}
endif

${name}_dylib: ${file_library_dylib}
${name}_dynamic: ${file_library_dynamic}
${name}_object: ${file_library_object}
${name}_static: ${file_library_static}

libraries: ${name}_dylib ${name}_dynamic ${name}_object ${name}_static

run:
	./${file_output}

ifeq (${target_os},macos)
${file_output}: ${files_objects}
	mkdir -p ${directory_output}
	${cc} ${c_flags_output} ${files_libraries} ${files_objects} -o ${file_output}
	if [[ ! -f "${directory_output}/${shell basename ${file_cero_library}}" ]]; then ln -s ${shell realpath "${file_cero_library}"} "${directory_output}/${shell basename ${file_cero_library}}"; fi
	if [[ ! -f "${directory_output}/${shell basename ${file_clic3_library}}" ]]; then ln -s ${shell realpath "${file_clic3_library}"} "${directory_output}/${shell basename ${file_clic3_library}}"; fi
	if [[ ! -f "${directory_output}/${shell basename ${file_interrupt_handler_library}}" ]]; then ln -s ${shell realpath "${file_interrupt_handler_library}"} "${directory_output}/${shell basename ${file_interrupt_handler_library}}"; fi
	if [[ ! -f "${directory_output}/${shell basename ${file_math_c_library}}" ]]; then ln -s ${shell realpath "${file_math_c_library}"} "${directory_output}/${shell basename ${file_math_c_library}}"; fi
	if [[ ! -f "${directory_output}/${shell basename ${file_rand_library}}" ]]; then ln -s ${shell realpath "${file_rand_library}"} "${directory_output}/${shell basename ${file_rand_library}}"; fi
endif

${file_library_dylib}: ${files_objects_library}
	mkdir -p ${directory_library}
	${cc} -dynamiclib ${c_flags_platform} -install_name ${name_library_dylib_major} -current_version ${version} -compatibility_version ${version_major_minor} ${files_libraries} ${files_objects_library} -o ${file_library_dylib_major}
ifneq (${debug}, 1)
	${strip} ${strip_flags} ${file_library_dylib_major}
endif
	if [[ -f "${file_library_dylib}" ]]; then rm "${file_library_dylib}"; fi
	ln -s ${name_library_dylib_major} ${file_library_dylib}

${file_library_dynamic}: ${files_objects_library}
	mkdir -p ${directory_library}
	${cc} -shared ${c_flags_platform} -install_name ${name_library_dynamic_major} -current_version ${version} -compatibility_version ${version_major_minor} ${files_libraries} ${files_objects_library} -o ${file_library_dynamic_major}
ifneq (${debug}, 1)
	${strip} ${strip_flags} ${file_library_dynamic_major}
endif
	if [[ -f "${file_library_dynamic}" ]]; then rm "${file_library_dynamic}"; fi
	ln -s ${name_library_dynamic_major} ${file_library_dynamic}

${file_library_object}: ${files_objects_library}
	mkdir -p ${directory_library}
	${ld} ${ld_flags} -r ${files_objects_library} -o ${file_library_object}
ifneq (${debug}, 1)
	${strip} ${strip_flags} ${file_library_object}
endif

${file_library_static}: ${files_objects_library}
	mkdir -p ${directory_library}
	${ar} ${ar_flags} ${file_library_static} ${files_objects_library}

${directory_objects}/%.o: ${directory_sources}/%.c
	mkdir -p ${directory_objects}
	${cc} ${c_flags_objects} -c $< -o $@

${directory_objects}/%_ios_library.o: ${directory_sources}/%.c
	mkdir -p ${directory_objects}
	${cc} ${c_flags_library} -c $< -o $@

${directory_objects}/%_library.o: ${directory_sources}/%.c
	mkdir -p ${directory_objects}
	${cc} ${c_flags_library} -c $< -o $@

${directory_objects}/%_ios.o: ${directory_sources}/%.c
	mkdir -p ${directory_objects}
	${cc} ${c_flags_objects} -c $< -o $@

clean: clean_library clean_objects clean_output

clean_library:
	-rm -r ${directory_library_base}

clean_objects:
	-rm -r ${directory_objects_base}

clean_output:
	-rm -r ${directory_output_base}

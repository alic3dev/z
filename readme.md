# z

z:a_radio:sewer_cavern_time

<img width="876" height="266" alt="z" src="https://github.com/user-attachments/assets/e0d406bf-dbf9-4a18-9037-222859c7ce5e" />

## settings

### location

`${HOME}/.config/z`

### options

- `volume`:`float`

#### format

`name->{value};`

### example

```
volume->{0.5};
```

// 00 26 a3 46

## library_usage

### preprocessor

- `target_os_ios`:switches_conditionals_to_target_ios

## development

### requirements

- [`alic3dev`](https://github.com/alic3dev)
- - [`cer0`](https://github.com/alic3dev/cer0)
- - [`clic3`](https://github.com/alic3dev/clic3)
- - [`interrupt_handler`](https://github.com/alic3dev/interrupt_handler)
- - [`math_c`](https://github.com/alic3dev/math_c)
- - [`rand`](https://github.com/alic3dev/rand)

### `make`:targets

```zsh
# executable|libraries
make
# library
make libraries
```

### `make`:flags

- `debug=1`:adds->{`debugging_symbols`}:disables->{`optimizations`};
- `target_device`:sets_the_target_device_platform->{values::[`mac`|`iphone`]}
- `target_device_version`:sets_the_target_version.for->{`macos`|`metal`};

### clean

```zsh
make clean
```

## running

```zsh
./output/z
```

or

```zsh
make run
```

## copyleft|copyright

> ©️ [copyleft|copyright]->{alic3dev(2025)}:[all_rights_reserved|all_lefts_reserved]

# z

z:pond

<img width="401" height="84" alt="z" src="https://github.com/user-attachments/assets/fda0bf76-e7fa-45ce-817b-4cff475f3ecb" />

## settings

### location

`${HOME}/.config/z`

### options

- `volume`:`float`

#### format

`name->{value};`

### example

```
volume->{1.0};
```

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
- - [`wave`](https://github.com/alic3dev/wave)

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

### wave_file_outputs

```zsh
./output/z output_path.wav
```

## copyright|copyleft

> © [copyleft|copyright]->{alic3dev(2025|2026)}:[all_rights_reserved|all_lefts_reserved]

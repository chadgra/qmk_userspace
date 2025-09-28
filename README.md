# QMK Userspace

This repo was made from a template repository which allows for an external set
of QMK keymaps to be defined and compiled. This is useful for users who want
to maintain their own keymaps without having to fork the main QMK repository.

This repo did add a personal fork of the qmk_firmware repository as a submodule,
since the main qmk_firmware repository does not fully support the sofle/keyhive
keyboard.

## Howto setup this repo

1. Run the normal `qmk setup` procedure if you haven't already done so --
see [QMK Docs](https://docs.qmk.fm/#/newbs) for details.
1. Clone this repo to your local machine
    - Clone recursively to also get the qmk_firmware submodule
1. Enable userspace in QMK config using
`qmk config user.overlay_dir="$(realpath qmk_userspace)"`
1. Add this keyboard to the QMK config using
`qmk config user.keyboard=sofle/keyhive`
1. Add this keymap to the QMK config using
`qmk config user.keymap=chadgra`

## Howto build locally

1. `cd` into the repository's qmk_firmware directory
1. Compile normally: `qmk compile -kb sofle/keyhive -km chadgra` or
just `qmk compile` since the keyboard and keymap were already configured in
the QMK config file

## Howto flash keyboard

1. The main (left) half of the keyboard should be connected to the computer
1. Run `qmk flash` from the qmk_firmware directory
1. When the following message appears, press the reset button on the side of
the keyboard to put it in bootloader mode:

```console
Flashing for bootloader: atmel-dfu
Bootloader not found. Make sure the board is in bootloader mode. See https://docs.qmk.fm/#/newbs_flashing
 Trying again every 0.5s (Ctrl+C to cancel)...................
```

1. If flashing the right keyboard half, make sure to connect it directly to the
computer and follow the previous step again, using this command to flash
`qmk flash -bl dfu-split-right`
